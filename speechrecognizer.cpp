#include "speechrecognizer.h"
#include "minimal_sapi.h"
#include <QDebug>
#include <QTimer>

SpeechRecognizer::SpeechRecognizer(QObject *parent) : QObject(parent)
{
    qDebug() << "Initializing speech recognition...";

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        emit errorOccurred("COM initialization failed");
        return;
    }

    m_sapiDll = LoadLibrary(L"sapi.dll");
    if (!m_sapiDll) {
        DWORD err = GetLastError();
        QString msg = QString("Failed to load sapi.dll (Error: 0x%1)").arg(err, 8, 16, QLatin1Char('0'));

        if (err == ERROR_MOD_NOT_FOUND) {
            msg += "\nSAPI.DLL not found. Ensure:\n"
                   "- You're on a 32-bit system (or using 32-bit SAPI)\n"
                   "- Speech Recognition is installed (check Windows Features)";
        }
        emit errorOccurred(msg);
        return;
    }
    qDebug() << "sapi.dll loaded successfully at:" << m_sapiDll;

    // 2. Get function pointers
    auto SpCreateRecognizer = (SpCreateRecognizerFunc)GetProcAddress(m_sapiDll, "SpCreateRecognizer");
    auto SpFindBestToken = (SpFindBestTokenFunc)GetProcAddress(m_sapiDll, "SpFindBestToken");

    if (!SpCreateRecognizer || !SpFindBestToken) {
        QString missingFuncs;
        if (!SpCreateRecognizer) missingFuncs += "SpCreateRecognizer ";
        if (!SpFindBestToken) missingFuncs += "SpFindBestToken";

        emit errorOccurred("Missing SAPI functions: " + missingFuncs +
                         "\nTry: regsvr32 sapi.dll");
        return;
    }
    qDebug() << "Critical functions found";

     hr = SpCreateRecognizer(&m_recognizer);
    if (FAILED(hr)) {
        QString error = QString("Recognizer creation failed (HRESULT: 0x%1)").arg(hr, 8, 16, QLatin1Char('0'));

        if (hr == REGDB_E_CLASSNOTREG) {
            error += "\nSpeech recognition not installed.\n"
                     "1. Install Windows Speech Recognition\n"
                     "2. Enable in Windows Features\n"
                     "3. Run 'speechUX' in Windows Run dialog";
        }
        emit errorOccurred(error);
        return;
    }
    qDebug() << "Recognizer created successfully";

    // 4. Find language token
    ISpObjectToken* cpToken = nullptr;
    hr = SpFindBestToken(SPCAT_RECOGNIZERS, L"Language=40C", nullptr, &cpToken); // French
    if (FAILED(hr)) {
        qDebug() << "French recognizer not found (HRESULT: 0x" << QString::number(hr, 16) << "), trying English...";
        hr = SpFindBestToken(SPCAT_RECOGNIZERS, L"Language=409", nullptr, &cpToken); // English
        if (FAILED(hr)) {
            emit errorOccurred("No recognizer found for French (40C) or English (409)\n"
                             "Install speech language packs in Windows Settings");
            return;
        }
    }

    // Debug token info
    LPWSTR tokenId = nullptr;
    if (SUCCEEDED(cpToken->GetId(&tokenId))) {
        qDebug() << "Using recognizer:" << QString::fromWCharArray(tokenId);
        CoTaskMemFree(tokenId);
    }

    // 5. Set recognizer
    hr = m_recognizer->SetRecognizer(cpToken);
    safeRelease(&cpToken);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to set recognizer token");
        return;
    }
    qDebug() << "Recognizer token set";

    // 6. Create recognition context
    hr = m_recognizer->CreateRecoContext(&m_recoContext);
    if (FAILED(hr)) {
        emit errorOccurred(QString("Failed to create reco context (0x%1)").arg(hr, 8, 16, QLatin1Char('0')));
        return;
    }
    qDebug() << "RecoContext created";

    // 7. Set event interest
    hr = m_recoContext->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
    if (FAILED(hr)) {
        emit errorOccurred("Failed to set event interest");
        return;
    }
    qDebug() << "Event interest set";

    // 8. Create grammar
    hr = m_recoContext->CreateGrammar(0, &m_grammar);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to create grammar");
        return;
    }
    qDebug() << "Grammar created";

    // 9. Load dictation
    hr = m_grammar->LoadDictation(nullptr, SPLO_STATIC);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to load dictation grammar");
        return;
    }
    qDebug() << "Dictation loaded";

    // 10. Set up event notification
    hr = m_recoContext->SetNotifyWin32Event();
    if (FAILED(hr)) {
        emit errorOccurred("Failed to set notification");
        return;
    }

    m_eventHandle = m_recoContext->GetNotifyEventHandle();
    if (m_eventHandle == INVALID_HANDLE_VALUE) {
        emit errorOccurred("Invalid event handle");
        return;
    }
    qDebug() << "Event handle:" << m_eventHandle;

    m_initialized = true;
    qDebug() << "Initialization COMPLETE";
}

SpeechRecognizer::~SpeechRecognizer()
{
    if (m_sapiDll) {
            FreeLibrary(m_sapiDll);
        }
    stopListening();
    safeRelease(&m_grammar);
    safeRelease(&m_recoContext);
    safeRelease(&m_recognizer);
    if (m_eventHandle) {
        CloseHandle(m_eventHandle);
    }
    CoUninitialize();
}

void SpeechRecognizer::startListening()
{

    if (!m_initialized) return;

    HRESULT hr = m_grammar->SetDictationState(SPRS_ACTIVE);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to activate dictation");
        return;
    }

    hr = m_recognizer->SetRecoState(SPRST_ACTIVE);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to activate recognition");
    }
}

void SpeechRecognizer::stopListening()
{
    if (!m_initialized) return;

    m_grammar->SetDictationState(SPRS_INACTIVE);
    m_recognizer->SetRecoState(SPRST_INACTIVE);
}

void SpeechRecognizer::processEvents()
{

    if (!m_initialized) {
        qDebug() << "Not initialized - skipping events";
        return;
    }

    DWORD waitResult = WaitForSingleObject(m_eventHandle, 0);
    qDebug() << "Event wait result:" << waitResult;

    if (waitResult == WAIT_OBJECT_0) {
        SPEVENT event;
        ULONG fetched;
        while (SUCCEEDED(m_recoContext->GetEvents(1, &event, &fetched)) && fetched) {
            qDebug() << "Processing event type:" << event.eEventId;

            if (event.eEventId == SPEI_RECOGNITION) {
                ISpRecoResult* result = reinterpret_cast<ISpRecoResult*>(event.lParam);
                WCHAR* text = nullptr;
                if (SUCCEEDED(result->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &text, nullptr))) {
                    QString recognizedText = QString::fromWCharArray(text);
                    qDebug() << "Raw recognized text:" << recognizedText;
                    CoTaskMemFree(text);
                    emit recognized(recognizedText);
                }
                result->Release();
            }
        }
    }
}

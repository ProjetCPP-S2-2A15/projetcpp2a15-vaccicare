#include "speechrecognizer.h"
#include "minimal_sapi.h"
#include <QDebug>
#include <QTimer>

SpeechRecognizer::SpeechRecognizer(QObject *parent) : QObject(parent)
{
    // Initialize COM
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        emit errorOccurred("COM initialization failed");
        return;
    }

    // Create recognizer instance
    hr = CoCreateInstance(CLSID_SpSharedRecognizer, nullptr,
                        CLSCTX_ALL, IID_ISpRecognizer,
                        (void**)&m_recognizer);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to create speech recognizer");
        return;
    }

    // Create recognition context
    hr = m_recognizer->
            CreateRecoContext(&m_recoContext);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to create recognition context");
        return;
    }

    // Set which events we're interested in
    hr = m_recoContext->SetInterest(SPFEI(SPEI_RECOGNITION), SPFEI(SPEI_RECOGNITION));
    if (FAILED(hr)) {
        emit errorOccurred("Failed to set event interest");
        return;
    }

    // Create grammar
    hr = m_recoContext->CreateGrammar(0, &m_grammar);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to create grammar");
        return;
    }

    // Load dictation grammar
    hr = m_grammar->LoadDictation(nullptr, SPLO_STATIC);
    if (FAILED(hr)) {
        emit errorOccurred("Failed to load dictation grammar");
        return;
    }

    // Set up event notification
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

    m_initialized = true;
}

SpeechRecognizer::~SpeechRecognizer()
{
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
    if (!m_initialized || !m_eventHandle || m_eventHandle == INVALID_HANDLE_VALUE) return;

    if (WaitForSingleObject(m_eventHandle, 0) == WAIT_OBJECT_0) {
        SPEVENT event;
        ULONG fetched;
        while (SUCCEEDED(m_recoContext->GetEvents(1, &event, &fetched)) && fetched) {
            if (event.eEventId == SPEI_RECOGNITION) {
                ISpRecoResult* result = reinterpret_cast<ISpRecoResult*>(event.lParam);
                WCHAR* text = nullptr;
                if (SUCCEEDED(result->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, TRUE, &text, nullptr))) {
                    QString recognizedText = QString::fromWCharArray(text);
                    CoTaskMemFree(text);
                    emit recognized(recognizedText);
                }
                result->Release();
            }
        }
    }
}

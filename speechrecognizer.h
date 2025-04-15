#ifndef SPEECHRECOGNIZER_H
#define SPEECHRECOGNIZER_H

#include <QObject>
#include <windows.h>
#include "minimal_sapi.h"


class SpeechRecognizer : public QObject
{
    Q_OBJECT
public:
    explicit SpeechRecognizer(QObject *parent = nullptr);
    ~SpeechRecognizer();

    bool isAvailable() const { return m_initialized; }
    void startListening();
    void stopListening();
    void processEvents();

signals:
    void recognized(const QString &text);
    void errorOccurred(const QString &message);

private:
    ISpRecognizer* m_recognizer = nullptr;
    ISpRecoContext* m_recoContext = nullptr;
    ISpRecoGrammar* m_grammar = nullptr;
    HANDLE m_eventHandle = nullptr;
    bool m_initialized = false;

    template <class T> void safeRelease(T** ppT) {
        if (*ppT) {
            (*ppT)->Release();
            *ppT = nullptr;
        }
    }
};

#endif // SPEECHRECOGNIZER_H

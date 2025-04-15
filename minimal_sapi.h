// minimal_sapi.h
#ifndef MINIMAL_SAPI_H
#define MINIMAL_SAPI_H

#include <windows.h>
#include <objbase.h>
#include "sapi_guids.h"
#include "qt_win32_compat.h"

// Basic constants
#define SPFEI(x) (1 << (x))
#define SPEI_RECOGNITION 1
#define SPRS_ACTIVE 1
#define SPRS_INACTIVE 0
#define SPRST_ACTIVE 1
#define SPRST_INACTIVE 0
#define SPLO_STATIC 0
#define SP_GETWHOLEPHRASE TRUE

// Forward declarations
struct ISpRecognizer;
struct ISpRecoContext;
struct ISpRecoGrammar;
struct ISpRecoResult;

// SPEVENT structure
typedef struct SPEVENT {
    WORD        eEventId;
    WORD        elParamType;
    ULONG       ulStreamNum;
    ULONGLONG   ullAudioStreamOffset;
    WPARAM      wParam;
    LPARAM      lParam;
} SPEVENT;

// COM interface declarations (simplified)
struct ISpRecognizer {
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void**) = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
    virtual ULONG STDMETHODCALLTYPE Release() = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateRecoContext(ISpRecoContext**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRecoState(ULONG) = 0;
};

struct ISpRecoContext {
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void**) = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
    virtual ULONG STDMETHODCALLTYPE Release() = 0;
    virtual HRESULT STDMETHODCALLTYPE SetInterest(ULONGLONG, ULONGLONG) = 0;
    virtual HRESULT STDMETHODCALLTYPE CreateGrammar(ULONGLONG, ISpRecoGrammar**) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetNotifyWin32Event() = 0;
    virtual HANDLE STDMETHODCALLTYPE GetNotifyEventHandle() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetEvents(ULONG, SPEVENT*, ULONG*) = 0;
};

struct ISpRecoGrammar {
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void**) = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
    virtual ULONG STDMETHODCALLTYPE Release() = 0;
    virtual HRESULT STDMETHODCALLTYPE LoadDictation(LPCWSTR, ULONG) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetDictationState(ULONG) = 0;
};

struct ISpRecoResult {
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void**) = 0;
    virtual ULONG STDMETHODCALLTYPE AddRef() = 0;
    virtual ULONG STDMETHODCALLTYPE Release() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetText(ULONG, ULONG, BOOL, WCHAR**, BYTE*) = 0;
};

// GUID definitions
extern "C" const GUID CLSID_SpSharedRecognizer;
extern "C" const GUID IID_ISpRecognizer;

// Windows API functions
extern "C" {
    __declspec(dllimport) BOOL WINAPI CloseHandle(HANDLE hObject);
    __declspec(dllimport) void WINAPI CoTaskMemFree(LPVOID pv);
}

#endif // MINIMAL_SAPI_H

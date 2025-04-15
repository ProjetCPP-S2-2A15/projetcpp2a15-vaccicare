// qt_win32_compat.h
#pragma once

#include <windows.h>
#include <objbase.h>

// Basic type definitions
typedef unsigned long ULONG;
typedef unsigned short WORD;
typedef unsigned long long ULONGLONG;
typedef long LONG;
typedef unsigned long DWORD;
typedef long HRESULT;
typedef void* HANDLE;
typedef wchar_t WCHAR;
typedef WCHAR* LPWSTR;
typedef const WCHAR* LPCWSTR;
typedef void* LPVOID;

// COM macros
#define STDMETHODCALLTYPE __stdcall
#define STDMETHOD(method) virtual HRESULT STDMETHODCALLTYPE method
#define STDMETHOD_(type,method) virtual type STDMETHODCALLTYPE method
#define REFIID const IID&

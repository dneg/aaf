//+---------------------------------------------------------------------------
//
//  Microsoft Windows
//  Copyright (C) Microsoft Corporation, 1992 - 1996.
//
//  File:	ref.hxx
//
//  Contents:	Reference implementation headers
//
//----------------------------------------------------------------------------

#ifndef __REF_HXX__
#define __REF_HXX__

#include <stdio.h>

#ifdef _WIN32
#include <malloc.h>
#include <crtdbg.h>
#endif

#define FARSTRUCT
#define interface struct
#define DECLARE_INTERFACE(iface) interface iface
#define DECLARE_INTERFACE_(iface, baseiface) interface iface: public baseiface

typedef int SCODE;
typedef int HRESULT;

#define NOERROR 0

#ifdef __cplusplus
    #define EXTERN_C    extern "C"
#else
    #define EXTERN_C    extern
#endif

#define PURE = 0

#ifdef _MSC_VER

#define STDCALL __stdcall
#define STDMETHODCALLTYPE __stdcall
#define EXPORTDLL _declspec(dllexport)

#else // _MSC_VER

#define STDCALL 
#define EXPORTDLL
#define STDMETHODCALLTYPE 

#endif // _MSC_VER

#define STDMETHOD(method)        virtual HRESULT STDCALL method
#define STDMETHOD_(type, method) virtual type STDCALL method
#define STDAPI HRESULT EXPORTDLL STDCALL
#define STDAPI_(type)  type EXPORTDLL STDCALL

#define THIS_
#define THIS void
#define FAR

typedef int BOOL, *LPBOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef int INT;
typedef unsigned int DWORD;
typedef short SHORT;
typedef unsigned short USHORT;
typedef DWORD ULONG;
typedef void VOID;
typedef WORD WCHAR;

typedef void *LPVOID;
typedef char *LPSTR;
typedef const char *LPCSTR;

#define TRUE 1
#define FALSE 0

typedef struct _ULARGE_INTEGER {
    DWORD LowPart;
    DWORD HighPart;
} ULARGE_INTEGER;

typedef struct _LARGE_INTEGER {
        DWORD LowPart;
        INT   HighPart;
} LARGE_INTEGER;

typedef struct tagGUID
{
    DWORD Data1;
    WORD  Data2;
    WORD  Data3;
    BYTE  Data4[8];
} GUID;

typedef GUID CLSID;
typedef GUID IID;

#define REFGUID             const GUID &
#define REFIID              const IID &
#define REFCLSID            const CLSID &

DECLARE_INTERFACE(IUnknown)
{
    STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
    STDMETHOD_(ULONG,Release) (THIS) PURE;
};

#define MAKE_SCODE(sev,fac,code) \
    ((SCODE) (((unsigned int)(sev)<<31) | ((unsigned int)(fac)<<16) | \
              ((unsigned int)(code))) )

#define SEVERITY_SUCCESS    0
#define SEVERITY_ERROR      1
#define FACILITY_STORAGE    0x0003 // storage errors (STG_E_*)
#define FACILITY_NULL 0x0000

#define E_NOINTERFACE       MAKE_SCODE(SEVERITY_ERROR,   FACILITY_NULL, 4)
#define E_FAIL              MAKE_SCODE(SEVERITY_ERROR,   FACILITY_NULL, 8)
#define SUCCEEDED(Status) ((SCODE)(Status) >= 0)
#define FAILED(Status) ((SCODE)(Status)<0)


#endif // #ifndef __REF_HXX__

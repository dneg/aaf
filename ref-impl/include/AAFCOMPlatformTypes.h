#ifndef __AAFCOMPlatformTypes_h__
#define __AAFCOMPlatformTypes_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/
// Files that contain standard types for windows/com interfaces.
// Files that contain DECLARE_INTERFACE macros, IUnknown and IClassFactory.

// NOTE: This file may have to be modified to support other COM platforms.

// AAF platform definitions and data types
#ifndef __AAFPlatform_h__
#include "AAFPlatform.h"
#endif

#if defined( OS_MACOS )
  // Include file form ActiveX SDK for the Macintosh. Otherwise the 
  // OLE (2.08, 2.24) interfaces cannot be used since they use standard
  // window's types and defines...
  #include <wintypes.h>


#elif defined( OS_WINDOWS )
  #include <wtypes.h>


#elif defined( OS_UNIX )

  // Bobt: for some reason, gcc on FreeBSD needs this in order to  
  // properly do rtti stuff.
# if defined ( OS_FREEBSD )
#  include <g++/std/typeinfo.h>
# endif

  // Note! Much of the stuff in this section copied from
  // ss-impl/.../ref.hxx

  // BobT hack! Should have used aafUIntXX types from AAFTypes.h to
  // defined BYTE et al, but due to nested includes we had to defined
  // them in terms of primitives...

  typedef long SCODE;
  typedef long HRESULT;

  typedef int BOOL;
  typedef unsigned char BYTE;
  typedef BYTE * LPBYTE;
  typedef unsigned short WORD;
  typedef unsigned long DWORD;
  typedef DWORD ULONG;
  typedef void *LPVOID;
  typedef char *LPSTR;
  typedef const char *LPCSTR;
  typedef unsigned short WCHAR;
  typedef WCHAR TCHAR;
  typedef WCHAR *LPWSTR;

# define _MAX_PATH 1024
# define MAX_PATH  _MAX_PATH

  typedef struct tagGUID
  {
      DWORD Data1;
      WORD  Data2;
      WORD  Data3;
      BYTE  Data4[8];
  } GUID;

# ifndef CLSID_DEFINED
# define CLSID_DEFINED
    typedef GUID CLSID;
# endif /* ! CLSID_DEFINED */

# ifndef __IID_DEFINED__
# define __IID_DEFINED__
    typedef GUID IID;
# endif /* ! __IID_DEFINED__ */

# define REFGUID             const GUID &
# define REFIID              const IID &
# define REFCLSID            const CLSID &

# define __RPC_FAR

  typedef void __RPC_FAR *HINSTANCE;

# define TRUE 1
# define FALSE 0

typedef char      OLECHAR;
typedef LPSTR     LPOLESTR;
typedef LPCSTR    LPCOLESTR;
#define OLESTR(str) str

# define STDCALL 
# define EXPORTDLL
# define STDMETHODCALLTYPE 

# define STDMETHODIMP HRESULT STDCALL
# define STDMETHOD(method)        virtual HRESULT STDCALL method
# define STDMETHOD_(type, method) virtual type STDCALL method

# define PURE = 0
# define THIS_
# define THIS void
# define FAR

# define STDAPI EXTERN_C HRESULT EXPORTDLL STDCALL
# define STDAPI_(type)  EXTERN_C type EXPORTDLL STDCALL
# define STDAPICALLTYPE STDCALL

# define interface struct
# define DECLARE_INTERFACE(iface) interface iface
# define DECLARE_INTERFACE_(iface, baseiface) interface iface: public baseiface

# ifdef __cplusplus
#  define EXTERN_C    extern "C"
# else
#   define EXTERN_C    extern
# endif

  DECLARE_INTERFACE(IUnknown)
  {
      STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
      STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
      STDMETHOD_(ULONG,Release) (THIS) PURE;
  };

# ifndef IID_ICLASSFACTORY
  const IID IID_IClassFactory =  
  {0x1,0x0,0x0,{0xc0,0x0,0x0,0x0,0x0,0x0,0x0,0x46}};

# define IID_ICLASSFACTORY
# endif

const GUID IID_IUnknown = {0x0,0x0,0x0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};;


  interface IClassFactory : public IUnknown
    {
    public:
	  virtual /* [local] */ HRESULT STDMETHODCALLTYPE CreateInstance( 
            /* [unique][in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject) = 0;
        
	  virtual /* [local] */ HRESULT STDMETHODCALLTYPE LockServer( 
            /* [in] */ BOOL fLock) = 0;
   };


  typedef /* [unique] */ IUnknown __RPC_FAR *LPUNKNOWN;

  typedef void __RPC_FAR *HMODULE;

  typedef enum tagCLSCTX
    {
	  CLSCTX_INPROC_SERVER	= 0x1,
	  CLSCTX_INPROC_HANDLER	= 0x2,
	  CLSCTX_LOCAL_SERVER	= 0x4,
	  CLSCTX_INPROC_SERVER16	= 0x8,
	  CLSCTX_REMOTE_SERVER	= 0x10,
	  CLSCTX_INPROC_HANDLER16	= 0x20,
	  CLSCTX_INPROC_SERVERX86	= 0x40,
	  CLSCTX_INPROC_HANDLERX86	= 0x80,
	  CLSCTX_ESERVER_HANDLER	= 0x100
	} CLSCTX;

  // BobT meta-hack! These really don't belong in this hack! (s.b. in
  // AAFComPlatform.h)
# define FACILITY_NULL 0x0000
# define SEVERITY_SUCCESS    0
# define SEVERITY_ERROR      1

# define MAKE_SCODE(sev,fac,code) \
      ((SCODE) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | \
                ((unsigned long)(code))) )

# define E_OUTOFMEMORY       MAKE_SCODE(SEVERITY_ERROR,   FACILITY_NULL, 2)
# define E_INVALIDARG        MAKE_SCODE(SEVERITY_ERROR,   FACILITY_NULL, 3)   
# define E_NOINTERFACE       MAKE_SCODE(SEVERITY_ERROR,   FACILITY_NULL, 4)
# define E_FAIL              MAKE_SCODE(SEVERITY_ERROR,   FACILITY_NULL, 8)
# define S_OK                0L
# define S_FALSE             MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_NULL, 1)

# define SUCCEEDED(Status) ((SCODE)(Status) >= 0)
# define FAILED(Status) ((SCODE)(Status)<0)

  // Stolen from winerror.h (from vc++6.0)
# define _HRESULT_TYPEDEF_(_sc)     _sc
# define CLASS_E_CLASSNOTAVAILABLE  _HRESULT_TYPEDEF_(0x80040111L)
# define E_UNEXPECTED               _HRESULT_TYPEDEF_(0x8000FFFFL)
  typedef void __RPC_FAR *HKEY;

  // stolen from winreg.h (from vc++6.0)
#define HKEY_CLASSES_ROOT           (( HKEY ) 0x80000000 )

#else
#error Unknown operating system

#endif


#endif // __AAFCOMPlatformTypes_h__


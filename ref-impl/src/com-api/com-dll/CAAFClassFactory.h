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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

#ifndef __CAAFClassFactory_h__
#define __CAAFClassFactory_h__

#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif

// We need the following definition for a bool type since
// the Win32 used BOOL as an int and ActiveX SDK, MacOLE use
// unsigned long for OLEBOOL.
// NOTE: We may have to move this definition to AAFTypes.h.
#if defined(_MAC) || defined(_MAC_) || defined(macintosh)
#define AAFBOOL OLEBOOL
#else
#define AAFBOOL BOOL
#endif

// Define the object creation callback function that should be
// implemented as a static method for every concrete AAF object.
// 
typedef HRESULT (*AAFCreateComObjectProc)(IUnknown *, void **); 

class CAAFClassFactory : 
	public IClassFactory,
	public CAAFUnknown
{
public:
	CAAFClassFactory(AAFCreateComObjectProc );

    // IUnknown methods  
	AAF_DECLARE_STANDARD_UNKNOWN()
	
protected:
	// CAAFUnknown override
    STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppv);

  // Wrapper to call the private _pfnCreate proc.
  HRESULT CallClassFactoryFunction(IUnknown *, void **);

public:
    // IClassFactory methods 
    STDMETHOD(CreateInstance)(IUnknown * pUnkOuter, REFIID riid, void ** ppv); 
    STDMETHOD(LockServer)(AAFBOOL fLock);     

private:
	AAFCreateComObjectProc _pfnCreate;
};


#endif // __CAAFClassFactory_h__

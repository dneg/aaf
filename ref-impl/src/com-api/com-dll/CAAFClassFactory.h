#ifndef __CAAFClassFactory_h__
#define __CAAFClassFactory_h__

#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif

// We meed the following definition for a bool type since
// the Win32 used BOOL as an int and ActiveX SDK, MacOLE use
// unsigned long for OLEBOOL.
// NOTE: We may have to move this definition to AAFTypes.h.
#if defined(_WIN32) || defined(WIN32)
#define AAFBOOL BOOL
#else
#define AAFBOOL OLEBOOL
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
    virtual HRESULT InternalQueryInterface(REFIID riid, void **ppv);

public:
    // IClassFactory methods 
    STDMETHOD(CreateInstance)(IUnknown * punkOuter, REFIID riid, void ** ppv); 
    STDMETHOD(LockServer)(AAFBOOL fLock);     
     

private:
	AAFCreateComObjectProc _pfnCreate;
};


#endif // __CAAFClassFactory_h__

#ifndef __CAAFClassFactory_h__
#define __CAAFClassFactory_h__

#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
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
    STDMETHOD(LockServer)(BOOL fLock);     
     

private:
	AAFCreateComObjectProc _pfnCreate;
};


#endif // __CAAFClassFactory_h__

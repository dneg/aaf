#ifndef __CAAFModuleTest_h__
#define __CAAFModuleTest_h__

#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif


#ifndef __AAFModuleTest_h__
#include "AAFModuleTest.h"
#endif

// Forward declaration for test method
class ostream;

class CAAFModuleTest : 
	public IAAFModuleTest,
	public CAAFUnknown
{
protected:
	CAAFModuleTest(IUnknown *pUnkOuter);
	virtual ~CAAFModuleTest();
    
public:
	// Declare the factory for this class.
	AAF_DECLARE_CONCRETE();

	// Declare the module test method. The implementation of the will be be
	// in /test/CAAFObjectTest.cpp.
	static HRESULT test(ostream& stream);


	// IAAFModuleTest methods
	//
    STDMETHOD(Test)(unsigned char *pClassName);

protected:
	// CAAFUnknown override
	//
    virtual HRESULT InternalQueryInterface(REFIID riid, void **ppv);

private:
};


// Redefine the end object map macro to include the module test
// object.
#ifdef AAF_END_OBJECT_MAP
#undef AAF_END_OBJECT_MAP
#define AAF_END_OBJECT_MAP() AAF_OBJECT_ENTRY(AAFModuleTest) { NULL, NULL, NULL } };
#endif


#endif // __CAAFModuleTest_h__

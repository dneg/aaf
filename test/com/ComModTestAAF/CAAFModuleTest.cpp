/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/
//
// File: CAAFModuleTest.cpp
// 
// Implementation for the AAFClassFactory class.
//


#ifndef __CAAFModuleTest_h__
#include "CAAFModuleTest.h"
#endif

#include <iostream.h>
#include <string.h>

//
// Define the AAF_BEGIN_OBJECT_XXXX macros so that we can reuse the
// AAFObjectMap and build a table for object test methods.
//

// Define the object creation callback function that should be
// implemented as a static method for every concrete AAF object.
// 
typedef HRESULT (*AAFModuleTestProc)(); 


typedef struct tagAAFObjectTestInfo
{
	const CLSID* pCLSID;
	LPCSTR pClassName;
	AAFModuleTestProc pfnTestProc;
} AAFObjectTestInfo_t;


#ifdef AAF_END_OBJECT_MAP
#undef AAF_END_OBJECT_MAP
#endif

#define AAF_BEGIN_OBJECT_MAP(x) static AAFObjectTestInfo_t x[] = {
#define AAF_OBJECT_ENTRY(xclass) { &CLSID_##xclass, #xclass, &C##xclass##::test },
#define AAF_END_OBJECT_MAP() { NULL, NULL, NULL } };



// Include the table the associates all of the CLSID's with class names and test methods.
#include "AAFObjectTable_i.cpp"



// CLSID for AAFObject 
// {B1A213AE-1A7D-11D2-BF78-00104BC9156D}
const CLSID CLSID_AAFModuleTest = { 0xB1A213AE, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };


// Default Interface for AAFObject 
// {B1A213AD-1A7D-11D2-BF78-00104BC9156D}
const IID IID_IAAFModuleTest = { 0xB1A213AD, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };


// Implementation
CAAFModuleTest::CAAFModuleTest(IUnknown *pUnkOuter)
	: CAAFUnknown(pUnkOuter)
{
}

CAAFModuleTest::~CAAFModuleTest()
{
}

// Implement the class factory for AAFModuleTest objects.
AAF_DEFINE_CONCRETE(AAFModuleTest)


HRESULT CAAFModuleTest::InternalQueryInterface 
(
	REFIID riid,
	void **ppvObj)
{
	HRESULT hr = S_OK;

	if (NULL == ppvObj)
		return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (riid == IID_IAAFModuleTest) 
    { 
        *ppvObj = (IAAFModuleTest *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
		return S_OK;
    }

	// Always delegate back to base implementation.
	return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}


STDMETHODIMP CAAFModuleTest::Test
(
	unsigned char *pClassName
)
{
	HRESULT hr = S_OK;


	// Search the object table for the given class id.
	long int index = 0;

	if (NULL != pClassName)
	{
		cout << "Looking for " << pClassName << " module test...";

		while (NULL != AAFObjectMap[index].pCLSID)
		{
			if (0 == strcmp(reinterpret_cast<char *>(pClassName), AAFObjectMap[index].pClassName))
			{
				cout << "Found." << endl;
				cout << "Running " << AAFObjectMap[index].pClassName << " module test...";
				hr = AAFObjectMap[index].pfnTestProc();
				if (FAILED(hr))
					cout << "FAILED!" << endl;
				else if (AAFRESULT_NOT_IMPLEMENTED == hr)
					cout << "NOT IMPLEMENTED!" << endl;
				else
					cout << "SUCCEEDED." << endl;

				return S_OK;
			}
			++index;
		}
		
		cout<< "NOT FOUND IN MODULE TABLE!" << endl;
	}
	else
	{
		while (NULL != AAFObjectMap[index].pCLSID)
		{
			cout << "Running " << AAFObjectMap[index].pClassName << " module test...";
			hr = AAFObjectMap[index].pfnTestProc();
			if (FAILED(hr))
				cout << "FAILED!" << endl;
			else if (AAFRESULT_NOT_IMPLEMENTED == hr)
				cout << "NOT IMPLEMENTED!" << endl;
			else
				cout << "SUCCEEDED." << endl;

			++index;
		}
	}
	return hr;
}

HRESULT CAAFModuleTest::test()
{
	return S_OK;
}




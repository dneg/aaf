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
#include <iomanip.h>
#include <string.h>
#include "AAFResult.h"


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

#define MAX_TEST_COUNT 1000

STDMETHODIMP CAAFModuleTest::Test
(
	unsigned char *pClassName
)
{
	HRESULT		hr = S_OK;
	HRESULT		testResults[MAX_TEST_COUNT];	/* table of HR's for all test */

	aafInt16 	testCount = 0;	/* total number of Mod test objects attemped to find */
	aafInt16	passCount = 0;	/* number of tests that succeeded */
	aafInt16	failCount = 0;	/* number of tests that failed */
	aafInt16	nImplCount = 0;	/* number of tests not implemented */
	aafInt32	index = 0;		/* General purpose index counter */ 
	aafInt16	partialSuccessCount = 0;	/*number of tests that partially succeeded */

	testResults[0] = NULL;

	// Search the object table for the given class id.

	if (NULL != pClassName)
	{
		while (NULL != AAFObjectMap[index].pCLSID)
		{
			if (0 == strcmp(reinterpret_cast<char *>(pClassName), AAFObjectMap[index].pClassName))
			{
				cout << "Testing " << AAFObjectMap[index].pClassName << " ...." << endl;

				hr = AAFObjectMap[index].pfnTestProc();

				cout << "Module test for " << setiosflags(ios::left) << setw(38) << AAFObjectMap[index].pClassName;

				if ( AAFRESULT_SUCCESS == hr )
					cout << "SUCCEEDED.\n" << endl;
				else if ( AAFRESULT_TEST_PARTIAL_SUCCESS == hr )
					cout<< "PARTIAL SUCCESS\n" << endl;
				else if (AAFRESULT_NOT_IMPLEMENTED == hr)
					cout << "NOT IMPLEMENTED!\n" << endl;
				else
					cout << "FAILED!\n" << endl;
			
					

				return S_OK;
			}
		++index;	
		}
		
		cout<< setiosflags(ios::left) << setw(54) << pClassName << "NOT FOUND IN MODULE TABLE!" << endl;
	}
	else
	{
		cout<< "Running Module tests .... \n"<< endl;

		while (NULL != AAFObjectMap[testCount].pCLSID && MAX_TEST_COUNT > testCount)
		{
			cout<< "  "<< AAFObjectMap[testCount].pClassName << endl;
			testResults[testCount] = AAFObjectMap[testCount].pfnTestProc();

			++testCount;
			if ( MAX_TEST_COUNT <= testCount ) 
				cout << "\n\nMAX_TEST_COUNT has been reached\n";
		}
		cout<< "\n******************************";
		cout<< "\n*  COM Module Tests Results: *";
		cout<< "\n******************************\n"<< endl;

		for ( index = 0; index < testCount; ++index )
			if ( AAFRESULT_SUCCESS == testResults[index] )
			{
				cout<< setiosflags(ios::left)<< setw(4)<< ++passCount;  
				cout<< setw(30) << AAFObjectMap[index].pClassName;
				cout<< "SUCCEEDED." << endl;
			}

		for ( index = 0; index < testCount; ++index )
			if ( AAFRESULT_TEST_PARTIAL_SUCCESS == testResults[index] )
			{
				cout<< setiosflags(ios::left)<< setw(4)<< ++partialSuccessCount + passCount;  
				cout<< setw(30) << AAFObjectMap[index].pClassName;
				cout<< "Partial Success." << endl;
			}

		for ( index = 0; index < testCount; ++index )
			if ( AAFRESULT_SUCCESS != testResults[index] &&
				 AAFRESULT_NOT_IMPLEMENTED != testResults[index] &&
				 AAFRESULT_TEST_PARTIAL_SUCCESS != testResults[index] )
			{
				cout<< setiosflags(ios::left)<< setw(4)<< ++failCount + partialSuccessCount + passCount;  
				cout<< setw(30) << AAFObjectMap[index].pClassName;
				cout<< "FAILED" << endl;
			}

		for ( index = 0; index < testCount; ++index )
			if ( AAFRESULT_NOT_IMPLEMENTED == testResults[index])
			{
				cout<< setiosflags(ios::left)<< setw(4)<< ++nImplCount + failCount + partialSuccessCount + passCount;  
				cout<< setw(30) << AAFObjectMap[index].pClassName;
				cout<< "Not Implemented!" << endl;
			}


		cout<< "\n\n";
		cout<< setw(20)<< "  Tests Run:"<< testCount << endl;
		cout<< setw(20)<< "  Passed:"<< passCount << endl;
		cout<< setw(20)<< "  Failed:"<< failCount << endl;
		cout<< setw(20)<< "  Not Implemented:"<< nImplCount<< endl;

		if (partialSuccessCount > 0 )
		{
			cout<< setw(20)<< "  Partial Success:"<< partialSuccessCount<< endl;
			cout<< "\n  Note: Partial Success means:"<< endl;
			cout<< "       All currently implemented tests succeed"<< endl;
			cout<< "       More tests need to be implemented\n"<<endl;
		}

		
	}

	return hr;
}

HRESULT CAAFModuleTest::test()
{
	return S_OK;
}

//
// Return private implementation pointer for delegation.
// NOTE: This is NOT the pointer to the COM object's implementation
// object!
//
HRESULT STDMETHODCALLTYPE
    CAAFModuleTest::GetImplRep(void **ppRep)
{
#ifdef BOB_TEST
    if (!ppRep)
        return E_INVALIDARG;
//	*ppRep = static_cast<void *>(GetRepObject());
//	return S_OK;
	*ppRep = static_cast<void *>(NULL);
    return AAFRESULT_NOT_IMPLEMENTED;
#else
    return AAFRESULT_NOT_IMPLEMENTED;
#endif
}





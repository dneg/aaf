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
#include <time.h>


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

	struct STAT_t
	{
		aafInt16 	num_run;	/* total number of tests run */
		aafInt16	num_failed;	/* number of tests that failed */
		aafInt32	result[1000];	/* and the result of the test */
	} stats = { 0, 0, NULL };

	/* Print Header */

	cout << endl; \
	cout<< "*******************" << endl; \
	cout<< " AAFCOMTST Results" << endl; \
	cout<< "*******************" << endl<< endl;	


	/* Get and print start time */

	time_t s_time;
	time(&s_time);
	cout<<"AAF COM Interface tests started: " <<ctime(&s_time) <<endl <<endl;


	// Search the object table for the given class id.

	if (NULL != pClassName)
	{
		cout << "Looking for " << pClassName << " module test...";

		while (NULL != AAFObjectMap[stats.num_run].pCLSID)
		{
			if (0 == strcmp(reinterpret_cast<char *>(pClassName), AAFObjectMap[stats.num_run].pClassName))
			{
				cout << "Found." << endl;
				cout << "Running " << AAFObjectMap[stats.num_run].pClassName << " module test...";
				hr = AAFObjectMap[stats.num_run].pfnTestProc();
				if (FAILED(hr))
				{
					cout << "FAILED!" << endl;
					++stats.num_failed;
				}
				else if (AAFRESULT_NOT_IMPLEMENTED == hr)
				{
					cout << "NOT IMPLEMENTED!" << endl;
					++stats.num_failed;
				}
				else
					cout << "SUCCEEDED." << endl;

				return S_OK;
			}
			stats.result[stats.num_run] = hr;
			++stats.num_run;
		}
		
		cout<< "NOT FOUND IN MODULE TABLE!" << endl;
	}
	else
	{
		while (NULL != AAFObjectMap[stats.num_run].pCLSID)
		{
			cout << "Running " << AAFObjectMap[stats.num_run].pClassName << " module test...";
			hr = AAFObjectMap[stats.num_run].pfnTestProc();
			if (FAILED(hr))
			{
				cout << "FAILED!" << endl;
				++stats.num_failed;
			}
			else if (AAFRESULT_NOT_IMPLEMENTED == hr)
			{
				cout << "NOT IMPLEMENTED!" << endl;
				++stats.num_failed;
			}
			else
				cout << "SUCCEEDED." << endl;

			stats.result[stats.num_run] = hr;
			++stats.num_run;
		}
	}


	/* Get and Print finish time	*/
	time_t e_time;
	time(&e_time);
	cout<< endl<< "AAF COM Interface tests finished: " <<ctime(&e_time) <<endl;


	/* Determine and print elapsed time */
	double elapsed_time = difftime( e_time, s_time );
	cout<< "AAF COM Interface tests completed in ";
	cout<< ((long)elapsed_time/3600) <<":"; /* hours */
	cout<< (((long)elapsed_time%3600)/60) <<":"; /* minutes */
	cout<< (((long)elapsed_time%3600)%60) <<endl <<endl; /* seconds */

	cout<< stats.num_run<< " Test(s) Run"<< endl;
	cout<< stats.num_failed<< " Failed"<< endl<< endl;

	/*	This is being commented out because until implementation, all tests fail
	/* list of all tests that failed */
/*	if ( stats.num_failed > 0 )
	{
		cout << endl << "List of tests that failed" << endl;
		for ( index = 0; index < stats.num_run; ++index )
			switch (stats.result[index])
		{
			case 0:
				break;
			case -1:
				cout<< AAFObjectMap[index].pClassName << "	"<< "FAILED!"<< endl;
				break;
			case 1:
				cout<< AAFObjectMap[index].pClassName << "	"<< "NOT IMPLEMENTED"<< endl;
				break;
			case -2:
				cout<< AAFObjectMap[index].pClassName << "	"<< "Not found in table"<< endl;
				break;
			default:
				cout<< AAFObjectMap[index].pClassName << "	"<< "UNKOWN RESULT"<< endl;
		}
	}
*/
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




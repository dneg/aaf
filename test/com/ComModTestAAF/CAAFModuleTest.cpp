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
// Implementation for the CAAFModuleTest class.
//

#include "CAAFModuleTest.h"

#include <iostream.h>
#include <iomanip.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"

//
// Use the x-macros in AAFObjectTable.h to declare the function
// prototypes for all of the module tests.
//
#define AAF_OBJECT_ENTRY(xclass) extern "C" HRESULT C##xclass##_test();

#include "AAFObjectTable.h"

#undef AAF_BEGIN_OBJECT_MAP
#undef AAF_OBJECT_ENTRY
#undef AAF_END_OBJECT_MAP


//
// Redefine the AAF_XXXX macros so that we can reuse the
// AAFObjectMap and build a table for object test methods.
//

// Define the object creation callback function that should be
// implemented as a static method for every concrete AAF object.
// 
typedef HRESULT (*AAFModuleTestProc)(); 


typedef struct tagAAFObjectTestInfo
{
	LPCSTR pClassName;
	AAFModuleTestProc pfnTestProc;
} AAFObjectTestInfo_t;



#define AAF_BEGIN_OBJECT_MAP(x) static AAFObjectTestInfo_t x[] = {
#define AAF_OBJECT_ENTRY(xclass) { #xclass, &C##xclass##_test },
#define AAF_END_OBJECT_MAP() { NULL, NULL } };


// Include the table the associates all of the CLSID's with class names and test methods.
#include "AAFObjectTable.h"





// Implementation
CAAFModuleTest::CAAFModuleTest()
{
}

CAAFModuleTest::~CAAFModuleTest()
{
}




#define MAX_TEST_COUNT 1000

HRESULT CAAFModuleTest::Test
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
		while (NULL != AAFObjectMap[index].pClassName)
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

		while (NULL != AAFObjectMap[testCount].pClassName && MAX_TEST_COUNT > testCount)
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

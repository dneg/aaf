/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
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
//
// File: CAAFModuleTest.cpp
// 
// Implementation for the CAAFModuleTest class.
//

#include "CAAFModuleTest.h"
#include "ModuleTest.h"

#include <iostream.h>
#include <iomanip.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"

//
// Use the x-macros in AAFObjectTable.h to declare the function
// prototypes for all of the module tests.
//
#define AAF_OBJECT_ENTRY(xclass) extern "C" HRESULT C##xclass##_test(testMode_t);

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
typedef HRESULT (*AAFModuleTestProc)(testMode_t mode); 


struct AAFObjectTestInfo
{
	LPCSTR pClassName;
	AAFModuleTestProc pfnTestProc;

  // Encapsulate the test proc so that we can trap exceptions
  // in one place.
  HRESULT CallTestProc(testMode_t mode) const;
};



#define AAF_BEGIN_OBJECT_MAP(x) static AAFObjectTestInfo x[] = {
#define AAF_OBJECT_ENTRY(xclass) { #xclass, &C##xclass##_test },
#define AAF_END_OBJECT_MAP() { NULL, NULL } };


// Include the table the associates all of the CLSID's with class names and test methods.
#include "AAFObjectTable.h"


// Encapsulate the test proc so that we can trap exceptions
// in one place.
HRESULT AAFObjectTestInfo::CallTestProc(testMode_t mode) const
{
  HRESULT result = S_OK;
  try
  {
    // Call the module test.
    result = (*pfnTestProc)(mode);
  }
  catch (...)
  {
    result = AAFRESULT_UNEXPECTED_EXCEPTION;
  }
  
  return (result);
}


// Implementation
CAAFModuleTest::CAAFModuleTest()
{
}

CAAFModuleTest::~CAAFModuleTest()
{
}

void CAAFModuleTest::List(void)
{
  aafUInt32 index = 0;
  while (NULL != AAFObjectMap[index].pClassName)
  {
    cout << AAFObjectMap[index].pClassName << endl;
    ++index;
  }

}

#define MAX_TEST_COUNT 1000

HRESULT CAAFModuleTest::Test
(
	testMode_t	mode,
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
				if(mode == kAAFUnitTestReadOnly)
					cout << "Testing " << AAFObjectMap[index].pClassName << " (Read Only) ...." << endl;
				else
					cout << "Testing " << AAFObjectMap[index].pClassName << " ...." << endl;

				hr = AAFObjectMap[index].CallTestProc(mode);

				cout << "Module test for " << setiosflags(ios::left) << setw(38) << AAFObjectMap[index].pClassName;
				if(mode == kAAFUnitTestReadOnly)
					cout << " (Read Only) ";

				if ( AAFRESULT_SUCCESS == hr )
					cout << "SUCCEEDED.\n" << endl;
				else if ( AAFRESULT_TEST_PARTIAL_SUCCESS == hr )
					cout<< "PARTIAL SUCCESS\n" << endl;
				else if (AAFRESULT_NOT_IMPLEMENTED == hr)
					cout << "NOT IMPLEMENTED!\n" << endl;
				else if (AAFRESULT_NOT_IN_CURRENT_VERSION == hr)
					cout << "SUCCEEDED.  One or more methods not implemented in the current SDK.\n" << endl;				
				else if (AAFRESULT_UNEXPECTED_EXCEPTION == hr)
					cout << "FAILED WITH UNEXPECTED EXCEPTION!\n" << endl;
				else
					cout << "FAILED!\n" << endl;
					
			
					

				return hr;
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
			testResults[testCount] = AAFObjectMap[testCount].CallTestProc(mode);

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
				if(mode == kAAFUnitTestReadOnly)
					cout << " (Read Only) ";
				cout<< "SUCCEEDED." << endl;
			}

		for ( index = 0; index < testCount; ++index )
			if ( AAFRESULT_NOT_IN_CURRENT_VERSION == testResults[index] )
			{
				cout<< setiosflags(ios::left)<< setw(4)<< ++passCount;  
				cout<< setw(30) << AAFObjectMap[index].pClassName;
				if(mode == kAAFUnitTestReadOnly)
					cout << " (Read Only) ";
				cout << "SUCCEEDED.  One or more methods will be implemented in a future SDK." << endl;				
			}

			for ( index = 0; index < testCount; ++index )
			if ( AAFRESULT_TEST_PARTIAL_SUCCESS == testResults[index] )
			{
				cout<< setiosflags(ios::left)<< setw(4)<< ++partialSuccessCount + passCount;  
				cout<< setw(30) << AAFObjectMap[index].pClassName;
				if(mode == kAAFUnitTestReadOnly)
					cout << " (Read Only) ";
				cout<< "Partial Success." << endl;
			}

		for ( index = 0; index < testCount; ++index )
			if ( AAFRESULT_SUCCESS != testResults[index] &&
				 AAFRESULT_NOT_IMPLEMENTED != testResults[index] &&
				 AAFRESULT_NOT_IN_CURRENT_VERSION != testResults[index] &&
				 AAFRESULT_TEST_PARTIAL_SUCCESS != testResults[index] )
			{
				cout<< setiosflags(ios::left)<< setw(4)<< ++failCount + partialSuccessCount + passCount;  
				cout<< setw(30) << AAFObjectMap[index].pClassName;
				if(mode == kAAFUnitTestReadOnly)
					cout << " (Read Only) ";
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

		if ( 0 < failCount)
		{
			hr = AAFRESULT_TEST_FAILED;
		}
	}


	return hr;
}


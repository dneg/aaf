//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=
//
// File: CAAFModuleTest.cpp
//
// Implementation for the CAAFModuleTest class.
//

#include "AAFWideString.h"
#include "CAAFModuleTest.h"
#include "ModuleTest.h"

#include <string>
#include <iostream>
#include <iomanip>

#include <time.h>
#include <stdlib.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFSmartPointer.h"

//
// Use the x-macros in AAFObjectTable.h to declare the function
// prototypes for all of the module tests.
//
#define AAF_OBJECT_ENTRY(xclass) extern "C" HRESULT C##xclass##_test(testMode_t,\
                                                                     aafUID_t,\
                                                                     testRawStorageType_t,\
                                                                     aafProductIdentification_t);

#include "AAFObjectTable.h"

#undef AAF_BEGIN_OBJECT_MAP
#undef AAF_OBJECT_ENTRY
#undef AAF_END_OBJECT_MAP

// Define the object creation callback function that should be
// implemented as a static method for every concrete AAF object.
//
typedef HRESULT (*AAFModuleTestProc)(testMode_t mode,
                                     aafUID_t fileKind,
                                     testRawStorageType_t rawStorageType,
                                     aafProductIdentification_t productID);

struct AAFObjectTestInfo
{
  wchar_t* pClassName;
  AAFModuleTestProc pfnTestProc;

  // Encapsulate the test proc so that we can trap exceptions
  // in one place.
  HRESULT CallTestProc(testMode_t mode,
                       aafUID_t fileKind,
                       testRawStorageType_t rawStorageType) const;
};

// Redefine the AAF_XXXX macros so that we can reuse the
// AAFObjectMap and build a table for object test methods.
//
#define AAF_BEGIN_OBJECT_MAP(x) static AAFObjectTestInfo x[] = {
#define AAF_OBJECT_ENTRY(xclass) {L###xclass, &C##xclass##_test},
#define AAF_END_OBJECT_MAP() { NULL, NULL } };

// Include the table the associates all of the CLSID's with class names and test methods.
#include "AAFObjectTable.h"

// Compute the maximum size of the result array.
const aafUInt32 MAX_TEST_COUNT = sizeof(AAFObjectMap)/sizeof(AAFObjectTestInfo) - 1;

// Encapsulate the test proc so that we can trap exceptions
// in one place.
HRESULT AAFObjectTestInfo::CallTestProc(testMode_t mode,
                                        aafUID_t fileKind,
                                        testRawStorageType_t rawStorageType) const
{
	HRESULT result = S_OK;

	// Create default module test product ID.
	aafProductIdentification_t productID = MakeProductID();

	//
	// In module test product ID replace product name
	// with the module test's name.
	//
	const aafCharacter* pProductNameSuffix = L"Test";
	// Changing the default value of the productID.productName pointer
	// will not cause a memory leak because MakeProductID() sets
	// productID.productName to point to a static string.
	productID.productName = new aafCharacter[ wcslen(pClassName) + wcslen(pProductNameSuffix) + 1 ];
	wcscpy( productID.productName, pClassName );
	wcscat( productID.productName, pProductNameSuffix );

	try
	{
		// Call the module test.
		result = (*pfnTestProc)(mode, fileKind, rawStorageType, productID);
	}
	catch (...)
	{
		result = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	delete [] productID.productName;

	return (result);
}

// convenient error handler
inline void
checkResult(AAFRESULT ar)
{
	if (FAILED(ar)) {
		throw ar;
	}
}

// Utility to find the test entry in the static table.
static int
findObjectTestInfo(const wchar_t* pClassName)
{
    for (int index = 0; NULL != AAFObjectMap[index].pClassName; index++)
    {
      if (0 == wcscmp(pClassName, AAFObjectMap[index].pClassName))
      {
        return index;
      }
    }
    
    return -1;
}
 
// Utility to find the given string in the filter array.
static bool
filterName(const wchar_t *pClassName, const wchar_t* const* filterArray)
{
	for (unsigned int i=0; filterArray[i]; ++i) {
		if (wcscmp(pClassName, filterArray[i])==0) {
			return true;
		}
    }
    return false;
} 

static const wchar_t*
getResultString(HRESULT hr)
{
	if (AAFRESULT_SUCCESS == hr) {
		return L"SUCCEEDED";
	} else if (AAFRESULT_TEST_PARTIAL_SUCCESS == hr) {
		return L"PARTIAL SUCCESS";
	} else if (AAFRESULT_NOT_IMPLEMENTED == hr) {
		return L"NOT IMPLEMENTED";
	} else if (AAFRESULT_NOT_IN_CURRENT_VERSION == hr) {
		return L"SUCCEEDED (But one or more methods not implemented in the current SDK)";        
	} else if (AAFRESULT_UNEXPECTED_EXCEPTION == hr) {
		return L"FAILED WITH UNEXPECTED EXCEPTION";
	} else if (-1 == hr) {
		return L"Skipped";
	}
	return L"FAILED";
}

static void
printName(const wchar_t* pClassName, bool eol=false)
{
	if (eol) {
		std::wcout << L"  " << pClassName << std::endl;
	} else {
		std::wcout << L"  " << std::left << std::setw(30) << pClassName;
	}
}

static void
printResult(testMode_t  mode, HRESULT hr)
{
	if (mode==kAAFUnitTestReadOnly) {
		std::wcout << L" (Read Only) - ";
	}
	std::wcout << getResultString(hr) << std::endl;
}

static void
printNotSuccess(HRESULT hr)
{
	std::wcout << L"^^^ NOT SUCCESS ^^^ -- Return: " << getResultString(hr)
		<< L" [" << std::hex << std::showbase << std::setw(10) << hr
		<< std::dec << std::noshowbase << L']'
		<< std::endl;
}

static void
printNotFound()
{
	std::wcout << L"^^^ NOT FOUND ^^^ -- Test missing from module table!" << std::endl;
}

// Implementation
CAAFModuleTest::CAAFModuleTest()
{
}

CAAFModuleTest::~CAAFModuleTest()
{
}

void
CAAFModuleTest::ListObjectMap() const
{
	std::wcout << std::endl << L"ComModTestAAF - List of all avail;able tests" << std::endl << std::endl;
	for (unsigned int i=0; AAFObjectMap[i].pClassName; i++) {
		std::wcout << L"  " << AAFObjectMap[i].pClassName << std::endl;
	}
}

AAFRESULT
CAAFModuleTest::ListEncodings() const
{
	AAFRESULT ar=AAFRESULT_SUCCESS;

	std::wcout << std::endl << L"ComModTestAAF - List of all available encodings" << std::endl << std::endl;
    try {
        IAAFSmartPointer<IEnumAAFFileEncodings> enumFileEncodings;
        AAFRESULT ar=AAFGetFileEncodings(&enumFileEncodings);

        if (ar==AAFRESULT_DLL_SYMBOL_NOT_FOUND) {
 			std::wcout << L"Old version of DLL - no alternate file encodings permitted" << std::endl << std::endl;
		} else if (ar==AAFRESULT_SUCCESS) {
            IAAFSmartPointer<IAAFFileEncoding> fileEncoding;
			while (enumFileEncodings->NextOne(&fileEncoding)==AAFRESULT_SUCCESS) {
				aafUInt32 bufferLen;
				std::wstring::size_type offset;

				checkResult(fileEncoding->GetNameBufLen(&bufferLen));
				std::wstring encodingName((bufferLen+sizeof(wchar_t))/sizeof(wchar_t)+1, L'\0');
				checkResult(fileEncoding->GetName(&encodingName[0], bufferLen));
				offset=encodingName.find(L'\0');
				if (offset!=std::wstring::npos) {
					encodingName.resize(offset);
				}

				checkResult(fileEncoding->GetDescriptionBufLen(&bufferLen));
				std::wstring encodingDescription((bufferLen+sizeof(wchar_t))/sizeof(wchar_t)+1, L'\0');
				checkResult(fileEncoding->GetDescription(&encodingDescription[0], bufferLen));
				offset=encodingDescription.find(L'\0');
				if (offset!=std::wstring::npos) {
					encodingDescription.resize(offset);
				}
				std::wcout << L"  " << std::left << std::setw(16) << encodingName << L" - " << encodingDescription << std::endl;
			}
		} else {
			throw ar;
		}
    } catch (AAFRESULT ar) {
		std::wcout << L"^^^ NOT SUCCESS ^^^ -- Return: "
			<< std::hex << std::showbase << std::setw(10) << ar
			<< std::dec << std::noshowbase << std::endl << std::endl;
    } catch (...) {
		std::wcout << L"INTERNAL ERROR" << std::endl << std::endl;
        ar=AAFRESULT_INTERNAL_ERROR;
    }
	return ar;
}

HRESULT CAAFModuleTest::Test
(
  testMode_t  mode,
  aafUID_t fileKind,
  testRawStorageType_t rawStorageType,
  unsigned int count,
  const wchar_t* const* pClassName,
  bool filter,
  bool terse
)
{
	HRESULT hr = AAFRESULT_SUCCESS;
	HRESULT testResults[MAX_TEST_COUNT];  /* table of HR's for all test */

	aafInt16  testCount = 0;  /* total number of Mod test objects attemped to find */
	aafInt16  passCount = 0;  /* number of tests that succeeded */
	aafInt16  failCount = 0;  /* number of tests that failed */
	aafInt16  nImplCount = 0;  /* number of tests not implemented */
	aafInt16  notInCurrentCount = 0;
	aafUInt32 index = 0;    /* General purpose index counter */
	aafInt32  totalTestCount = 0; 
	aafInt16  partialSuccessCount = 0;  /*number of tests that partially succeeded */
	aafInt16  skippedCount = 0;

	for (index = 0; index < MAX_TEST_COUNT; index++)
		testResults[index] = -1;

	if (count && !filter && terse) {
		for (unsigned int i=0; i<count; ++i) {
			printName(pClassName[i]);
			if (mode==kAAFUnitTestReadOnly) {
				std::wcout << L" (Read Only)";
			}
			std::wcout << std::endl;
			index = findObjectTestInfo(pClassName[i]);
			if (index!=static_cast<aafUInt32>(-1)) {
				testResults[index] = AAFObjectMap[index].CallTestProc(mode, fileKind, rawStorageType);
				if (testResults[index]!=AAFRESULT_SUCCESS) {
					printNotSuccess(testResults[index]);
					if (hr==AAFRESULT_SUCCESS) {
						hr=testResults[index];
					}
				}
			} else {
				printNotFound();
				hr=AAFRESULT_TEST_FAILED;
			}
		}
	} else {
		/* Print Header */
		std::wcout << std::endl
			<< L"***************************" << std::endl
			<< L"*     COMMODTESTAAF       *" << std::endl
			<< L"*   AAF COM Module Test   *" << std::endl
			<< L"***************************" << std::endl
			<< std::endl;  

		/* Get and print start time */
		time_t s_time;
		time(&s_time);
		
		std::wcout << ctime(&s_time) << std::endl
			<< L"Running Module Tests ..." << std::endl << std::endl;
    
		if (count && !filter)
		{
			for (unsigned int i=0; i<count; ++i)
			{
				printName(pClassName[i], true);
				index = findObjectTestInfo(pClassName[i]);
				if (index!=static_cast<aafUInt32>(-1))
				{
					++testCount;
					testResults[index] = AAFObjectMap[index].CallTestProc(mode, fileKind, rawStorageType);
					if (testResults[index]!=AAFRESULT_SUCCESS && testResults[index]!=AAFRESULT_NOT_IN_CURRENT_VERSION) {
						printNotSuccess(testResults[index]);
					}
				}
				else
				{  
					printNotFound();
				}
			}
		}
		else
		{
			for (unsigned int i=0; i<MAX_TEST_COUNT; ++i)
			{
				const wchar_t* const className=AAFObjectMap[i].pClassName;
				if (className && (!filter || !filterName(className, pClassName))) {
					++testCount;
					printName(className, true);
					testResults[i] = AAFObjectMap[i].CallTestProc(mode, fileKind, rawStorageType);
					if (testResults[i]!=AAFRESULT_SUCCESS && testResults[i]!=AAFRESULT_NOT_IN_CURRENT_VERSION) {
						printNotSuccess(testResults[i]);
					}
				}
			}
		}
    
		std::wcout << std::endl
			<< L"*********************************" << std::endl
			<< L"*  COM Module AAF Test Results: *" << std::endl
			<< L"*********************************" << std::endl << std::endl;

		passCount = 0;
		for ( index = 0; index < MAX_TEST_COUNT; index++ ) {
			if ( AAFRESULT_SUCCESS == testResults[index] )
			{
				++passCount;
				std::wcout << std::left << std::setw(3) << ++totalTestCount;
				printName(AAFObjectMap[index].pClassName);
				printResult(mode, testResults[index]);
			}
		}

		for ( index = 0; index < MAX_TEST_COUNT; index++ ) {
			if ( AAFRESULT_NOT_IN_CURRENT_VERSION == testResults[index] )
			{
				++notInCurrentCount;
				std::wcout << std::left << std::setw(3) << ++totalTestCount;
				printName( AAFObjectMap[index].pClassName);
				printResult(mode, testResults[index]);
			}
		}

		for ( index = 0; index < MAX_TEST_COUNT; index++ ) {
			if ( AAFRESULT_TEST_PARTIAL_SUCCESS == testResults[index] )
			{
				++partialSuccessCount;    
				std::wcout << std::left << std::setw(3) << ++totalTestCount;
				printName(AAFObjectMap[index].pClassName);
				printResult(mode, testResults[index]);
			}
		}

		for ( index = 0; index < MAX_TEST_COUNT; index++ ) {
			if (AAFRESULT_SUCCESS != testResults[index]
				&& AAFRESULT_NOT_IMPLEMENTED != testResults[index]
				&& AAFRESULT_NOT_IN_CURRENT_VERSION != testResults[index]
				&& AAFRESULT_TEST_PARTIAL_SUCCESS != testResults[index]
				&& -1 !=  testResults[index])
			{
				++failCount;
				std::wcout << std::left << std::setw(3) << ++totalTestCount;
				printName(AAFObjectMap[index].pClassName);
				printResult(mode, testResults[index]);
			}
		}

		for ( index = 0; index < MAX_TEST_COUNT; index++ ) {
			if ( AAFRESULT_NOT_IMPLEMENTED == testResults[index])
			{
				++nImplCount;
				std::wcout << std::left << std::setw(3) << ++totalTestCount;
				printName(AAFObjectMap[index].pClassName);
				printResult(mode, testResults[index]);
			}
		}

		for ( index = 0; index < MAX_TEST_COUNT; index++ )
		{
			if ( -1  == testResults[index])
			{
				++skippedCount;
				std::wcout << std::left << std::setw(3) << ++totalTestCount;
				printName(AAFObjectMap[index].pClassName);
				printResult(mode, testResults[index]);
			}
		}

		std::wcout << std::endl << std::endl
			<< std::left << std::setw(20) << L"  Tests Run:"<< testCount << std::endl
			<< std::left << std::setw(20) << L"  Passed:"<< passCount + notInCurrentCount << std::endl
			<< std::left << std::setw(20) << L"  Failed:"<< failCount << std::endl
			<< std::left << std::setw(20) << L"  Not Implemented:"<< nImplCount << std::endl;

		if (partialSuccessCount > 0 )
		{
			std::wcout << std::left << std::setw(20) << L"  Partial Success:" << partialSuccessCount << std::endl << std::endl
				<< L"  Note: Partial Success means:"<< std::endl
				<< L"       All currently implemented tests succeeded"<< std::endl
				<< L"       More tests need to be implemented" << std::endl << std::endl;
		}

		if (skippedCount > 0 )
		{
			std::wcout << std::left << std::setw(20)<< L"  Skipped:" << skippedCount << std::endl << std::endl
				<< L"  Note: Tests are usually skipped for regression"<< std::endl
				<< L"        testing with older versions of the toolkit" << std::endl << std::endl;
		}

		if ( 0 < failCount)
		{
			hr = AAFRESULT_TEST_FAILED;
		}


		/* Get and Print finish time  */
		time_t e_time;
		time(&e_time);
		std::wcout << ctime(&e_time) << std::endl;

		/* Determine and print elapsed time */
		const int elapsed_time = static_cast<int>(difftime(e_time, s_time));
		std::wcout << L"ComModTestAAF completed in "
			<< (elapsed_time/3600) << L":" /* hours */
			<< ((elapsed_time%3600)/60) << L":" /* minutes */
			<< ((elapsed_time%3600)%60) /* seconds */
			<< std::endl << std::endl; /* seconds */
	}

	return hr;
}

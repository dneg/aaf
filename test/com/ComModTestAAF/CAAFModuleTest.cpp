//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
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

#include "CAAFModuleTest.h"
#include "ModuleTest.h"

#include <iostream.h>
#include <iomanip.h>
#include <time.h>
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
#define AAF_OBJECT_ENTRY(xclass) { #xclass, &C##xclass##_test},
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
    index++;
  }

}

// Compute the maximum size of the result array.
const aafUInt32 MAX_TEST_COUNT = sizeof(AAFObjectMap)/sizeof(AAFObjectTestInfo) - 1;


// Utility to find the test entry in the static table.
static int findObjectTestInfo(const char *pClassName)
{
    for (int index = 0; NULL != AAFObjectMap[index].pClassName; index++)
    {
      if (0 == strcmp(pClassName, AAFObjectMap[index].pClassName))
      {
        return index;
      }
    }
    
    return -1;
}
 
// Utility to find the given string in the filter array.
static bool filterName(const char *pClassName, int count, const char **filterArray)
{
    for (int index = 0; index < count; index++)
    {
      if (0 == strcmp(pClassName, filterArray[index]))
      {
        return true;
      }
    }
    
    return false;
} 

static void printName(const char * pClassName)
{
  cout << setiosflags(ios::left) << setw(30) << pClassName;
}

static void printResult(testMode_t  mode, HRESULT hr)
{
  if(mode == kAAFUnitTestReadOnly)
    cout << " (Read Only) ";

  if ( AAFRESULT_SUCCESS == hr )
    cout << "SUCCEEDED." << endl;
  else if ( AAFRESULT_TEST_PARTIAL_SUCCESS == hr )
    cout<< "PARTIAL SUCCESS" << endl;
  else if (AAFRESULT_NOT_IMPLEMENTED == hr)
    cout << "NOT IMPLEMENTED!" << endl;
  else if (AAFRESULT_NOT_IN_CURRENT_VERSION == hr)
    cout << "SUCCEEDED.  One or more methods not implemented in the current SDK." << endl;        
  else if (AAFRESULT_UNEXPECTED_EXCEPTION == hr)
    cout << "FAILED WITH UNEXPECTED EXCEPTION!" << endl;
  else if (-1 == hr)
    cout << "Skipped." << endl;
  else
    cout << "FAILED!" << endl;
}

static void printNotFound(void)
{
  cout << "# WARNING: NOT FOUND IN MODULE TABLE!" << endl;
}

HRESULT CAAFModuleTest::Test
(
  testMode_t  mode,
  bool filter,
  int count,
  const char *pClassName[]
)
{
  HRESULT    hr = S_OK;
  HRESULT    testResults[MAX_TEST_COUNT];  /* table of HR's for all test */

  aafInt16   testCount = 0;  /* total number of Mod test objects attemped to find */
  aafInt16  passCount = 0;  /* number of tests that succeeded */
  aafInt16  failCount = 0;  /* number of tests that failed */
  aafInt16  nImplCount = 0;  /* number of tests not implemented */
  aafInt16  notInCurrentCount = 0;
  aafInt32  index = 0;    /* General purpose index counter */
  aafInt32  totalTestCount = 0; 
  aafInt16  partialSuccessCount = 0;  /*number of tests that partially succeeded */
  aafInt16  skippedCount = 0;

  for (index = 0; index < MAX_TEST_COUNT; index++)
    testResults[index] = -1;

  // Search the object table for the given class id.

  if (1 == count && NULL != pClassName && !filter)
  {
    printName(pClassName[0]);
    
    index = findObjectTestInfo(pClassName[0]);
    if (0 <= index)
    {
      hr = AAFObjectMap[index].CallTestProc(mode);
      printResult(mode, hr);  
      return hr;
    }
    else
    {
      printNotFound();
      return AAFRESULT_TEST_FAILED;
    }    
  }
  else
  {
    /* Print Header */
    cout<< "\n\n"<< endl;
    cout<< "***************************\n";
    cout<< "*       COMMODAAF         *\n";
    cout<< "*   AAF COM Module Test   *\n";
    cout<< "***************************\n"<< endl;  


    /* Get and print start time */
    time_t s_time;
    time(&s_time);
    cout<< ctime(&s_time)<< endl<< endl;

    cout<< "Running Module tests .... \n"<< endl;
    
    if (1 < count && NULL != pClassName && !filter)
    {
      for (int arg = 0; arg < count; ++arg)
      {
        index = findObjectTestInfo(pClassName[arg]);
        cout << "  ";
        if (0 <= index)
        {
          cout << AAFObjectMap[index].pClassName << endl;
          testResults[index] = AAFObjectMap[index].CallTestProc(mode);
          ++testCount;
        }
        else
        {  
          printName(pClassName[arg]);
          printNotFound();
        }
      }
    }
    else
    {
      for (index = 0; NULL != AAFObjectMap[index].pClassName && MAX_TEST_COUNT > index; ++index)
      {
        if (filter && filterName(AAFObjectMap[index].pClassName, count, pClassName))
          continue;
          
        cout << "  " << AAFObjectMap[index].pClassName << endl;
        testResults[index] = AAFObjectMap[index].CallTestProc(mode);
        ++testCount;
      }
    }
    
    
    cout<< "\n******************************";
    cout<< "\n*  COM Module Tests Results: *";
    cout<< "\n******************************\n"<< endl;
    passCount = 0;
    for ( index = 0; index < MAX_TEST_COUNT; index++ )
      if ( AAFRESULT_SUCCESS == testResults[index] )
      {
        ++passCount;
        cout<< setiosflags(ios::left)<< setw(4)<< ++totalTestCount;
        cout<< setw(30) << AAFObjectMap[index].pClassName;
        printResult(mode, testResults[index]);
      }

    for ( index = 0; index < MAX_TEST_COUNT; index++ )
      if ( AAFRESULT_NOT_IN_CURRENT_VERSION == testResults[index] )
      {
        ++notInCurrentCount;
        cout<< setiosflags(ios::left)<< setw(4)<< ++totalTestCount;
        cout<< setw(30) << AAFObjectMap[index].pClassName;
        printResult(mode, testResults[index]);
      }

      for ( index = 0; index < MAX_TEST_COUNT; index++ )
      if ( AAFRESULT_TEST_PARTIAL_SUCCESS == testResults[index] )
      {
        ++partialSuccessCount    ;    
        cout<< setiosflags(ios::left)<< setw(4)<< ++totalTestCount;
        cout<< setw(30) << AAFObjectMap[index].pClassName;
        printResult(mode, testResults[index]);
      }

    for ( index = 0; index < MAX_TEST_COUNT; index++ )
      if ( AAFRESULT_SUCCESS != testResults[index] &&
         AAFRESULT_NOT_IMPLEMENTED != testResults[index] &&
         AAFRESULT_NOT_IN_CURRENT_VERSION != testResults[index] &&
         AAFRESULT_TEST_PARTIAL_SUCCESS != testResults[index] &&
         -1 !=  testResults[index])
      {
        ++failCount;
        cout<< setiosflags(ios::left)<< setw(4)<< ++totalTestCount;
        cout<< setw(30) << AAFObjectMap[index].pClassName;
        printResult(mode, testResults[index]);
      }

    for ( index = 0; index < MAX_TEST_COUNT; index++ )
      if ( AAFRESULT_NOT_IMPLEMENTED == testResults[index])
      {
        ++nImplCount;
        cout<< setiosflags(ios::left)<< setw(4)<< ++totalTestCount;
        cout<< setw(30) << AAFObjectMap[index].pClassName;
        printResult(mode, testResults[index]);
      }

    for ( index = 0; index < MAX_TEST_COUNT; index++ )
    {
      if ( -1  == testResults[index])
      {
        ++skippedCount;
        cout<< setiosflags(ios::left)<< setw(4)<< ++totalTestCount;
        cout<< setw(30) << AAFObjectMap[index].pClassName;
        printResult(mode, testResults[index]);
      }
    }

    cout<< "\n\n";
    cout<< setw(20)<< "  Tests Run:"<< testCount << endl;
    cout<< setw(20)<< "  Passed:"<< passCount + notInCurrentCount << endl;
    cout<< setw(20)<< "  Failed:"<< failCount << endl;
    cout<< setw(20)<< "  Not Implemented:"<< nImplCount<< endl;

    if (partialSuccessCount > 0 )
    {
      cout<< setw(20)<< "  Partial Success:"<< partialSuccessCount<< endl;
      cout<< "\n  Note: Partial Success means:"<< endl;
      cout<< "       All currently implemented tests succeed"<< endl;
      cout<< "       More tests need to be implemented\n"<<endl;
    }

    if (skippedCount > 0 )
    {
      cout<< setw(20)<< "  Skipped:"<< skippedCount<< endl;
      cout<< "\n  Note: Tests are usually skipped for regression"<< endl;
      cout<< "       testing with older versions of the toolkit\n"<<endl;
    }

    if ( 0 < failCount)
    {
      hr = AAFRESULT_TEST_FAILED;
    }


    /* Get and Print finish time  */
    time_t e_time;
    time(&e_time);
    cout<< endl<< ctime(&e_time)<< endl;

    /* Determine and print elapsed time */
    double elapsed_time = difftime( e_time, s_time );
    cout<< "COMMODAAF completed in ";
    cout<< ((long)elapsed_time/3600) <<":"; /* hours */
    cout<< (((long)elapsed_time%3600)/60) <<":"; /* minutes */
    cout<< (((long)elapsed_time%3600)%60) <<"\n" <<endl; /* seconds */
  }


  return hr;
}


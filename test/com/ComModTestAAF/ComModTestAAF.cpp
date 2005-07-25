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
// An example program that calls the test AAF COM interfaces.
//

//
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

using namespace std;


#ifndef __AAF_h__
#include "AAF.h"
#endif

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#include "AAFResult.h"
#include "AAFFileKinds.h"
#include "AAFWideString.h"

#if defined( OS_WINDOWS )
#include <winbase.h>
#include <unknwn.h>
#include <objbase.h>
#else
#undef _WINDEF_
#endif

#include "CAAFModuleTest.h"
#include "ModuleTest.h"



// routine copied from Tim Bingham's test program...
static void formatError(DWORD errorCode)
{
  cerr << "RESULT = " << (long)errorCode << " (0x" << hex << errorCode << dec << ")" << endl;

#if defined( OS_WINDOWS )
  CHAR buffer[256];

  int status = FormatMessageA(
    FORMAT_MESSAGE_FROM_SYSTEM,
    NULL,
    errorCode,
    LANG_SYSTEM_DEFAULT,
    buffer, sizeof(buffer)/sizeof(buffer[0]),
    NULL);

  if (status != 0) {
    int length = strlen(buffer);
    if (length >= 1) {
      buffer[length - 1] = '\0';
    }
    cerr << buffer << endl;
  }
#endif // OS_WINDOWS

  cerr << endl;
}

static void throwIfError(HRESULT hr)
{
  if (FAILED(hr))
  {
    formatError(hr);
    throw hr;
  }
}

static void dumpLibInfo(ostream & os)
{
	// print library path name
	aafUInt32 bufSize;
	HRESULT hr = AAFGetLibraryPathNameBufLen(&bufSize);
	if (hr != AAFRESULT_DLL_SYMBOL_NOT_FOUND)
	{
		if (AAFRESULT_SUCCEEDED(hr))
		{
			aafCharacter* buffer = (aafCharacter*)new aafUInt8[bufSize];
			hr = AAFGetLibraryPathName (buffer, bufSize);
			if (AAFRESULT_SUCCEEDED(hr))
			{
				os << "AAF DLL path name: ";
#if 1
				size_t length = (bufSize / sizeof(aafCharacter)) - 1;
				char* result = new char[length + 1];
				wcstombs(result, buffer, length + 1);
				os << result;
				delete [] result;
#else
				//printAAFString(buffer, os);
#endif
				os << endl;
			}
			delete [] buffer;
		}
	}
	
	// print library version
	aafProductVersion_t vers;
	hr = AAFGetLibraryVersion(&vers);
	if (hr != AAFRESULT_DLL_SYMBOL_NOT_FOUND)
	{
		if (AAFRESULT_SUCCEEDED(hr))
		{
			os << "AAF DLL version  : "
				<< vers.major << "."
				<< vers.minor << "."
				<< vers.tertiary << " (" << vers.patchLevel << ")" << endl;
		}
	}
}

// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
  CAAFInitialize(const char *dllname = NULL)
  {
    cout << "Attempting to load the AAF dll...";
    cout.flush();
    HRESULT hr = AAFLoad(dllname);
    if (S_OK != hr)
    {
      cerr << "FAILED! ";
      throwIfError(hr);
    }
    cout << "DONE" << endl;
    dumpLibInfo(cout);
  }

  ~CAAFInitialize()
  {
    AAFUnload();
  }
};


// simple helper class to initialize and cleanup AAF library.
class CAAFInitializePlugins
{
public:
  CAAFInitializePlugins() :
    pPluginMgr(NULL)
  {
    throwIfError(AAFGetPluginManager(&pPluginMgr));
    throwIfError(pPluginMgr->RegisterSharedPlugins());
    pPluginMgr->Release();
    pPluginMgr = NULL;
  }

  ~CAAFInitializePlugins()
  {
    if (pPluginMgr)
      pPluginMgr->Release();
  }

  // cached for error cleanup.
  IAAFPluginManager *pPluginMgr;
};


// forward declarations.




//
// If there are no arguments then perform the module test for all modules.
// Otherwise the arguments are assumed to be valid AAF object names such as
// AAFObject or AAFComponent.
//


int main(int argc, char* argv[])
{
  int result = AAFRESULT_SUCCESS;
  int  startArg = 1;
  testMode_t  testMode = kAAFUnitTestReadWrite;
  bool skipTests = false;
  const size_t maxEncodingNameLength = 64;
  aafCharacter encodingName[ maxEncodingNameLength+1 ] = L"";
  bool isEncodingNameSpecified = false;


  // Create the module test object.
  CAAFModuleTest AAFModuleTest;
  try
  {
    HRESULT hr = S_OK;


    for (startArg = 1; (startArg < argc) && ('-' == argv[startArg][0]); startArg++)
    {
      /* List the AAF class names, one per line, in the order that the tests will be run. */
      /* This can be used for more selective automated testing... */
      if (0 == strcmp(argv[startArg],"-l") || 0 == strcmp(argv[startArg],"--list"))
      {
        AAFModuleTest.List();
        return(0);
      }
      else if (0 == strcmp(argv[startArg],"-r") || 0 == strcmp(argv[startArg],"--readonly"))
      {
        testMode = kAAFUnitTestReadOnly;
       }
      else if (0 == strcmp(argv[startArg],"-s") || 0 == strcmp(argv[startArg],"--skip"))
      {
        skipTests = true;
      }
      else if (0 == strcmp(argv[startArg],"-e") || 0 == strcmp(argv[startArg],"--encoding"))
      {
        if ( startArg < (argc-1))
        {
          startArg++;
          mbstowcs( encodingName, argv[startArg], strlen(argv[startArg])+1 );
          encodingName[ maxEncodingNameLength ] = L'\0';
          isEncodingNameSpecified = true;
        }
        else
        {
          cerr << "ERROR: The " << argv[startArg]
               << " option requires an argument" << endl;
          return EXIT_FAILURE;
        }
      }
      /* Check arguments to see if help was requested */
      else
      {
        cout<< "\nCOMMODAAF [-l] [-r] [-s] [-e file_encoding] [test1 test2 etc] ***********************\n"; 
        cout<< " No arguments --> To run all tests\n";
        cout<< " Otherwise any AAF object class name can be typed\n";
        cout<< " and that objects test method will be executed.\n";
        cout<< "ex AAFSegment AAFTransition etc\n\n";
        cout<< " -l : print list of all tests to stdout\n";
        cout<< " -r : run read/only tests for regression and cross-platform testing.\n";
        cout<< " -s : Use with -r to skip tests that were not supported in earlier releases.\n";
        cout<< " -e : Use to test with file encoding other than MS Structured Storage encoding.\n";
        cout<< " Note -e option not implemented yet!\n";
        cout<< endl;        

        return EXIT_SUCCESS;
      }
    }


    // Make sure the dll can be loaded and initialized.
    CAAFInitialize aafInit;

    // Make sure the shared plugins can be loaded and registered.
     CAAFInitializePlugins aafInitPlugins;


    // If encoding name is specified change the
    // default value of file encoding to be used
    // by module tests.
    aafUID_t fileKind = testFileKindDefault;
    testRawStorageType_t rawStorageType = kAAFNamedFile;

    if (startArg >= argc)
    {    
      // Call all Module tests...
      // WARNING: old style module tests only require/specify first paarmeter -- C calling conventions hanld this
      hr = AAFModuleTest.Test(testMode, fileKind, rawStorageType);
    }
    else
    {
      // Call only the modules that are named in the command line.
      // WARNING: old style module tests only require/specify first paarmeter -- C calling conventions hanld this
      hr = AAFModuleTest.Test(testMode, fileKind, rawStorageType, skipTests, argc - startArg, const_cast<const char **>(&argv[startArg]));
    }

    result = (int)hr;
  }
  catch (HRESULT& rhr)
  {
    result = rhr;
  }
  catch (...)
  {
    result = AAFRESULT_TEST_FAILED;
  }
  

  // Translate AAFRESULTs into an exit status.
  // Only the 8 least significant bits are available to a waiting parent
  // (see IEEE Std 1003.1) and by convention the 8th bit (values over 127)
  // is reserved to indicate core dumped (GNU C Library manual 25.6.2).

  if (result == AAFRESULT_SUCCESS)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;		// Test failed or test could not run
}

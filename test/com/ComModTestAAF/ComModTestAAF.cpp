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
// An example program that calls the test AAF COM interfaces.
//

//
#include <iostream.h>
#include <fstream.h>
#include <stdlib.h>
#include <string.h>


#ifndef __AAF_h__
#include "AAF.h"
#endif

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif


#if defined( OS_WINDOWS )
#include <winbase.h>
#include <unknwn.h>
#include <objbase.h>
#endif

#if defined( OS_MACOS )
#define _MAC

// Make sure we have defined IID_IUnknown and IID_IClassFactory.
#include <initguid.h>
#include <coguid.h>  
#include "DataInput.h"

#if !defined(CDECL) && defined(_MSC_VER)
#define CDECL  _cdecl
#endif // CDECL

#if !defined(FAR)
#define FAR
#endif

#include <ole2.h>
#include "dispatch.h"
#include "wintypes.h"
#include <olectl.h>

#endif  // OS_MACOS


#include "CAAFModuleTest.h"
#include "ModuleTest.h"


typedef AAFRESULT (*AAFModuleTestProc)();


#define SUCCESS (0)
#define FAILURE (-1)

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
  int result = SUCCESS;
  int  startArg = 1;
  testMode_t  testMode = kAAFUnitTestReadWrite;
  bool skipTests = false;


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
      /* Check arguments to see if help was requested */
      else
      {
        cout<< "\nCOMMODAAF [-l] [-r] [-s] [test1 test2 etc] ***********************\n"; 
        cout<< " No arguments --> To run all tests\n";
        cout<< " Otherwise any AAF object class name can be typed\n";
        cout<< " and that objects test method will be executed.\n";
        cout<< "ex AAFSegment AAFTransition etc\n\n";
        cout<< " -l : print list of all tests to stdout\n";
        cout<< " -r : run read/only tests for regression and cross-platform testing.\n";
        cout<< " -s : Use with -r to skip tests that were not supported in earlier releases.\n";
        cout<< endl;        

        return(0);
      }
    }


    // Make sure the dll can be loaded and initialized.
    CAAFInitialize aafInit;

    // Make sure the shared plugins can be loaded and registered.
     CAAFInitializePlugins aafInitPlugins;

    if (startArg >= argc)
    {    
      // Call all Module tests...
      hr = AAFModuleTest.Test(testMode);
    }
    else
    {
      // Call only the modules that are named in the command line.
      hr = AAFModuleTest.Test(testMode, skipTests, argc - startArg, const_cast<const char **>(&argv[startArg]));
    }

    result = (int)hr;
  }
  catch (HRESULT& rhr)
  {
    result = rhr;
  }
  catch (...)
  {
    result = FAILURE;
  }
  

  return result;
}



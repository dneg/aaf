/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
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
#include <time.h>


#ifdef WIN32
#include <unknwn.h>
#include <objbase.h>
#endif

#if defined(macintosh)
#define _MAC

// Make sure we have defined IID_IUnknown and IID_IClassFactory.
#include <initguid.h>
#include <coguid.h>	
#include "DataInput.h"

#if !defined(CDECL) && defined(_MSC_VER)
#define CDECL	_cdecl
#endif // CDECL

#if !defined(FAR)
#define FAR
#endif

#include <ole2.h>
#include "dispatch.h"
#include "wintypes.h"
#include <olectl.h>
#endif


#ifndef __AAF_h__
#include "AAF.h"
#endif


#include "CAAFModuleTest.h"

#ifdef WIN32
#include <winbase.h>
//#include <tchar.h>
#endif

typedef AAFRESULT (*AAFModuleTestProc)();


#define SUCCESS (0)
#define FAILURE (-1)

// routine copied from Tim Bingham's test program...
static void formatError(DWORD errorCode)
{
  cerr << "RESULT = " << (long)errorCode << " (0x" << hex << errorCode << dec << ")" << endl;

#ifdef WIN32
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
#endif

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

// helper class
struct CComInitialize
{
	CComInitialize() { CoInitialize(NULL); }
	~CComInitialize() { CoUninitialize(); }
};


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
	aafInt16	count = 0, 
				i = 0;
	char 		**myargv = NULL;


	// Initialize com library for this process.
	CComInitialize comInit;

	// Create the module test object.
	CAAFModuleTest AAFModuleTest;
	try
	{
		HRESULT hr = S_OK;

		// Make sure the dll can be loaded and initialized.
		CAAFInitialize aafInit;

		// Make sure the shared plugins can be loaded and registered.
   		CAAFInitializePlugins aafInitPlugins;


		#ifdef _MAC
			char dataFile[] = "COMMODAAF (PPC).inp";
			getInputData(&argc, argv, dataFile);
		#endif

		/* Check arguments to see if help was requested */

		if ( argc > 1 &&
			(0 == strncmp(argv[1],"-h",2) ||
			 0 == strncmp(argv[1],"-H",2) )	)
		{
			cout<< "\nCOMMODAAF***********************\n"; 
			cout<< "No arguments --> To run all tests\n";
			cout<< "Otherwise any AAF object class name can be typed\n";
			cout<< "and that objects test method will be executed.\n";
			cout<< "ex AAFSegment AAFTransition etc\n\n";
			cout<< "Input can also be read in from a text.\n";
			cout<< "Just name it \"COMMODAAF (PPC)\"" << endl;

			return(0);
		}

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

		if (1 >= argc)
			// Call all Module tests...
			hr = AAFModuleTest.Test(NULL);
		else
		{
			// Call only the modules that are named in the command line.
			int module;

			for (module = 1; module < argc; module++)
				hr = AAFModuleTest.Test(reinterpret_cast<unsigned char *>(argv[module]));
		}
		
		#ifdef _MAC
			cleanUpInputData(argc, argv);
		#endif

		/* Get and Print finish time	*/
		time_t e_time;
		time(&e_time);
		cout<< endl<< ctime(&e_time)<< endl;

		/* Determine and print elapsed time */
		double elapsed_time = difftime( e_time, s_time );
		cout<< "COMMODAAF completed in ";
		cout<< ((long)elapsed_time/3600) <<":"; /* hours */
		cout<< (((long)elapsed_time%3600)/60) <<":"; /* minutes */
		cout<< (((long)elapsed_time%3600)%60) <<"\n" <<endl; /* seconds */


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



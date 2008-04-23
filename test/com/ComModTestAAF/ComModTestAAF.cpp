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

#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

#if defined(OS_WINDOWS) && !defined(NDEBUG)
#include <windows.h>
#define CHECKLEAKS 1
#if defined(CHECKLEAKS)
#include <crtdbg.h>
#endif
#endif

#include "CAAFModuleTest.h"
#include "ModuleTest.h"


static void formatError(HRESULT errorCode)
{
	std::wcerr << L"RESULT = " << errorCode
		<< L'(' << std::hex << std::showbase << errorCode << std::dec << std::noshowbase << L')'
		<< std::endl;

#if defined( OS_WINDOWS )
	wchar_t buffer[256] = L"";

	int status = FormatMessageW(
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		LANG_SYSTEM_DEFAULT,
		buffer,
		sizeof(buffer)/sizeof(buffer[0]),
		NULL);

	if (status) {
		unsigned int length = wcslen(buffer);
		if (length >= 1) {
			buffer[length - 1] = L'\0';
	}
	std::wcerr << buffer << std::endl;
	}
#endif // OS_WINDOWS

	std::wcerr << std::endl;
}

static void
throwIfError(HRESULT hr)
{
	if (FAILED(hr)) {
		formatError(hr);
		throw hr;
	}
}

static void
dumpLibInfo(std::wostream &wos)
{
	// print library path name
	aafUInt32 bufSize;
	HRESULT hr = AAFGetLibraryPathNameBufLen(&bufSize);
	if (hr != AAFRESULT_DLL_SYMBOL_NOT_FOUND)
	{
		if (AAFRESULT_SUCCEEDED(hr))
		{
			aafCharacter* buffer = (aafCharacter*) new aafUInt8[bufSize];
			hr = AAFGetLibraryPathName (buffer, bufSize);
			wos << L"AAF DLL path name: ";
			if (AAFRESULT_SUCCEEDED(hr)) {
				wos << L"AAF DLL path name: " << buffer << std::endl;
			} else {
				wos << L"<unknown>" << std::endl;
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
			wos << L"AAF DLL version  : "
				<< static_cast<unsigned int>(vers.major) << L"."
				<< static_cast<unsigned int>(vers.minor) << L"."
				<< static_cast<unsigned int>(vers.tertiary) << L" ("
				<< static_cast<unsigned int>(vers.patchLevel) << L')' << std::endl;
		}
	}
}

static void
listFileEncodings()
{
	IEnumAAFFileEncodings *pFileEncodings = 0;
	if (AAFGetFileEncodings (&pFileEncodings) != AAFRESULT_SUCCESS) {
		return;
	}

	static bool saw_first_one=false;
	const unsigned int namelen = 32;
	aafCharacter pEncodingName[namelen];
	IAAFFileEncoding*  pEncoding = 0;

	std::wcout << L"      Available file encodings: ";
	while ( pFileEncodings->NextOne( &pEncoding ) == AAFRESULT_SUCCESS ) {
		if( AAFRESULT_SUCCESS == pEncoding->GetName( pEncodingName, namelen)) {
			if (saw_first_one) {
				std::wcout << L", ";
			} else {
				saw_first_one = true;
			}

			std::wcout << pEncodingName;

			if (pEncoding != 0) {
				pEncoding->Release();
				pEncoding = 0;
			}
		}
	}

	if (pFileEncodings != 0) {
		pFileEncodings->Release();
		pEncoding = 0;
	}

	std::cout << std::endl;
}

// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
	CAAFInitialize(const char *dllname = NULL) {
		std::wcout << "Attempting to load the AAF dll... " << std::flush;
		HRESULT hr = AAFLoad(dllname);
		if (S_OK != hr)
		{
			std::wcout << L"FAILED!" << std::endl;;
			throwIfError(hr);
		}
		std::wcout << L"DONE" << std::endl;
		dumpLibInfo(std::wcout);
	}

	~CAAFInitialize() {
		AAFUnload();
	}
};


// simple helper class to initialize and cleanup AAF library.
class CAAFInitializePlugins
{
	#if defined(CHECKLEAKS)
	_CrtMemState	_memoryState;
#endif

public:
  CAAFInitializePlugins() :
    pPluginMgr(NULL)
  {
#if defined(CHECKLEAKS)
	// Send	all	reports	to STDOUT
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE	);
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
	_CrtMemCheckpoint( &_memoryState );
#endif

    throwIfError(AAFGetPluginManager(&pPluginMgr));
    throwIfError(pPluginMgr->RegisterSharedPlugins());
    pPluginMgr->Release();
    pPluginMgr = NULL;
  }

  ~CAAFInitializePlugins()
  {
    if (pPluginMgr)
      pPluginMgr->Release();
#if defined(CHECKLEAKS)
		//_CrtMemDumpAllObjectsSince( &_memoryState );
		_CrtMemState memNow;
		_CrtMemCheckpoint(&memNow);
		_CrtMemState memDiff;
		std::string leakMsg = (_CrtMemDifference(&memDiff, &_memoryState, &memNow)==0)?"No leak.":"Leak!";
		std::wcout << "_CrtMemDifference says " << leakMsg.c_str() << std::endl;
		_CrtMemDumpStatistics( &memDiff );
#endif
  }

  // cached for error cleanup.
  IAAFPluginManager *pPluginMgr;
};


//
// If there are no arguments then perform the module test for all modules.
// Otherwise the arguments are assumed to be valid AAF object names such as
// AAFObject or AAFComponent.
//

int
main(int argc, char* argv[])
{
	int result = AAFRESULT_SUCCESS;
	int osReturn = EXIT_SUCCESS;
	testMode_t testMode = kAAFUnitTestReadWrite;
	bool help = false;
	bool list = false;
	bool filter = false;
	bool terse = false;
	bool isEncodingNameSpecified = false;
	std::wstring encodingName;

	wchar_t** wargv=new wchar_t*[argc+1];
	wargv[argc]=0;
	for (int i=0; i<argc; ++i) {
		size_t argSize=strlen(argv[i])+1;
		wargv[i]=new wchar_t[argSize];
		if (mbstowcs(wargv[i], argv[i], argSize)==static_cast<size_t>(-1)) {
			std::wcerr << L"ERROR: Cannot convert arguments to wide characters." << std::endl;
			return EXIT_FAILURE;
		}
	}

	// Create the module test object.
	CAAFModuleTest AAFModuleTest;
	try
	{
		HRESULT hr = S_OK;

		int startArg=1;
		for ( ; startArg<argc && wargv[startArg][0]==L'-'; ++startArg) {
			std::wstring options(wargv[startArg]+1);
			if (options==L"--") {
				break;
			}
			if (options.empty()) {
				std::wcerr << L"ERROR: The - option specifier has no option" << std::endl;
				osReturn=EXIT_FAILURE;
				continue;
			} else if (options==L"--help") {
				options='h';
			} else if (options==L"--list") {
				options='l';
			} else if (options==L"--read") {
				options='r';
			} else if (options==L"--skip") {
				options='s';
			} else if (options==L"--terse") {
				options='s';
			} else if (options==L"--encoding") {
				options='e';
			}
			for (std::wstring::const_iterator cit(options.begin()), ecit(options.end())
				; cit<ecit
				; ++cit)
			{
				switch (*cit) {
					case L'h':
						help = true;
						break;
					case L'l':
						// List the AAF class names, one per line, in the order that the tests will be run.
						// This can be used for more selective automated testing...
						list = true;
						break;
					case L'r':
						testMode = kAAFUnitTestReadOnly;
						break;
					case L's':
						filter = true;
						break;
					case L't':
						terse = true;
						break;
					case L'e':
						if (startArg < (argc-1)) {
							encodingName=wargv[++startArg];
							isEncodingNameSpecified = true;
						} else {
							std::wcerr << L"ERROR: The -e option requires an option argument" << std::endl;
							osReturn=EXIT_FAILURE;
						}
						break;
					default:
						std::wcerr << L"ERROR: The -" << *cit << L" option is not valid" << std::endl;
						osReturn=EXIT_FAILURE;
				}
			}
		}
		if (filter && startArg>=argc) {
			std::wcerr << L"ERROR: The -s (skip tests) option requires one or more tests to be specified" << std::endl;
			osReturn=EXIT_FAILURE;
		}
		if (terse && startArg>=argc) {
			std::wcerr << L"ERROR: The -t (terse) option requires one or more tests to be specified" << std::endl;
			osReturn=EXIT_FAILURE;
		}
		if (help || osReturn!=EXIT_SUCCESS) {
			std::wcout << std::endl
				<< L"ComModTestAAF [-hlrs] [-e file_encoding] [test1 test2 ...]" << std::endl
				<< L" No arguments --> To run all tests" << std::endl
				<< L" Otherwise any AAF object class name can be typed" << std::endl
				<< L" and that objects test method will be executed." << std::endl
				<< L" example: AAFSegment AAFTransition etc" << std::endl << std::endl
				<< L" -h : Produce this help" << std::endl
				<< L" -l : Print list of all tests and available encodings" << std::endl
				<< L" -r : Run read/only tests for regression and cross-platform testing" << std::endl
				<< L" -s : Skip specified test(s) - use with -r for tests not supported in earlier releases" << std::endl
				<< L" -t : Terse output for specified test(s)" << std::endl
				<< L" -e : Use to test with file encoding other than the default Structured Storage encoding" << std::endl;
			listFileEncodings();
			std::wcout << std::endl
				<< L" --help, --list, --read, --skip, --terse, and --encoding are also supported" << std::endl
				<< std::endl;
			return osReturn;
		}
		if (list) {
			AAFModuleTest.ListObjectMap();
			AAFRESULT ar=AAFModuleTest.ListEncodings();
			if (ar!=AAFRESULT_SUCCESS) {
				throw ar;
			}
			return EXIT_SUCCESS;
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
		if (!encodingName.empty()) {
			if (FindFileEncodingByName(encodingName.c_str(), &fileKind)!= AAFRESULT_SUCCESS)
			{
				std::wcerr << L"ERROR: Failed to find the specified file encoding -- " << encodingName << std::endl;
				return EXIT_FAILURE;
			}
            if (fileKind != aafFileKindAafXmlText)
            {
                // Always use disk raw storage if file encoding is specified and is 
                // not the XML file encoding
                // The XML file encoding is a multi-file format and does not support the
                // "single file" raw storage type
                rawStorageType = kAAFDiskRawStorage;
            }
		}

		if (startArg >= argc)
		{
			// Call all Module tests...
			hr = AAFModuleTest.Test(testMode
				, fileKind
				, rawStorageType
			);
		}
		else
		{
			// Call only the modules that are named in the command line.
			hr = AAFModuleTest.Test(testMode
				, fileKind
				, rawStorageType
				, argc-startArg
				, &wargv[startArg]
				, filter
				, terse
			);
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

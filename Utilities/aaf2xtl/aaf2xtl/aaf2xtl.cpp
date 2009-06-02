//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Siemens SBS Media.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <Aif2XtlXmlGen.h>

#include <AxUtil.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

// For registry access.
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#define STRINGIFY( EXP ) #EXP

namespace {

class Aaf2XtlUsageEx : public exception {
public:
	Aaf2XtlUsageEx()
	{}
	virtual ~Aaf2XtlUsageEx()
	{}

	virtual const char* what() const;
private:
	mutable std::string _msg;
};

const char* Aaf2XtlUsageEx::what() const
{
	using namespace std;

	stringstream msg;

	msg << "Usage: aaf2xtl [file_name] [-dump] [-file file_name] [-extdir directory_name] [-cutsonly] [-extess] ";
#if AAF2XTL_MODIFY_FILE
	msg << "[-stripess] ";
#endif
	msg << "[-version]" << endl;
	
	_msg = msg.str();

	return _msg.c_str();
}


const char* GetAAFComLibPath()
{
	const TCHAR* szRegistryKey = TEXT("Software\\BBC Technology\\AAF Windows Media Player Plugin");

	HKEY hkey;

	LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szRegistryKey, 0, KEY_READ, &hkey );
	if ( ERROR_SUCCESS != result ) {
		return 0;
	}

	DWORD bufSize = 0;

	char *pAafDllPath = 0;

	result = RegQueryValueEx(hkey, TEXT("AAFLoadPath"), 0, 0, 0, &bufSize );
	if ( ERROR_SUCCESS == result ) {
		pAafDllPath = new char [ bufSize ];
		if ( !pAafDllPath ) {
			return 0;
		}
		result = RegQueryValueEx(hkey, TEXT("AAFLoadPath"), 0, 0,
								 reinterpret_cast<LPBYTE>(pAafDllPath), &bufSize );
		if ( ERROR_SUCCESS != result ) {
			delete [] pAafDllPath;
			pAafDllPath = 0;
			return 0;
		}
	}

	return pAafDllPath;
}

} // end of namespace

int main( int argc, char** argv )
{
	using namespace std;

	try {

		AxCmdLineArgs args( argc, argv );

		// Dump version info in requested.
		pair<bool,int> verOp = args.get( "-version" );
		if ( verOp.first ) {
			cout << "aaf2xtl build: ";
			cout << STRINGIFY( $Name$ );
			cout << endl;
			return 0;
		}

		// Dump the aif2xtllib's internal data structure.
		pair<bool,int> dumpOp = args.get( "-dump" );
		
		// Process all transitions as cuts.
		pair<bool,int> cutsOnlyOp = args.get( "-cutsonly" );
		
		// Put the external essence in this directory.
		pair<bool,int> extractDirOp = args.get( "-extdir" );
		pair<bool,const char*> extractDirArg(true, "c:/tmp/aaf2xtl_essence" );
		if ( extractDirOp.first ) {
			extractDirArg = args.get( extractDirOp.second + 1 );
		}
		if ( !extractDirArg.first ) {
			throw Aaf2XtlUsageEx();
		}
		
		// This is the AAF file to convert.
		pair<bool,int> fileOp = args.get( "-file" );
		pair<bool,const char*> fileArg(false,"");
		if ( fileOp.first ) {
			fileArg = args.get( fileOp.second + 1 );
		}
		if ( !fileArg.first ) {

			// Try the second argument on the command line.
			fileArg = args.get( 1 );

			if ( !fileArg.first ) {	
				throw Aaf2XtlUsageEx();
			}
		}

		// Extract essence if this is set.
		pair<bool,int> extEssOp = args.get( "-extess" );

#if AAF2XTL_MODIFY_FILE
		// Extract essence if this is set.
		pair<bool,int> stripEssOp = args.get( "-stripess" );
#endif

		std::ostream* pUnownedOutputStream = 0;
		std::auto_ptr<std::ofstream> apOwnedOutputStream;
		Aif2XtlXmlGen* pXtlGenerator = 0;

		if ( true ) {
		  // Don't store this in the auto pointer because we don't own cout.
		  pUnownedOutputStream = &cout;
		}
		else {
		  // We do own this one, so it goes in the owned auto_ptr.
		  char* file = "c:\\tmp\\aaf2xtl.xtl";
		  std::auto_ptr<std::ofstream> apTmp( new ofstream( "c:/tmp/aaf2xtl.xtl" ) );
		  apOwnedOutputStream = apTmp;
		  pUnownedOutputStream = apOwnedOutputStream.get();
		}

		const char* comLibPath = GetAAFComLibPath();
		cerr << "AAF Library: " << comLibPath << endl;
		CHECK_HRESULT( Aif2XtlXmlGen::CreateInstance( fileArg.second,
													  comLibPath,
													  &pXtlGenerator ) );
		
		// Pass ownership of pXtlGenerator to auto_ptr to
		// ensure exception safeness should CHECK_HRESULT
		// throw an exception.
		std::auto_ptr<Aif2XtlXmlGen> apXtlGenerator( pXtlGenerator );
		try {
 			pXtlGenerator = 0;
			CHECK_HRESULT( apXtlGenerator->Execute( *pUnownedOutputStream, std::wcerr ) );
		}
		catch (...) {
		  if ( dumpOp.first )	{
		    apXtlGenerator->Dump( wcout );
		  }
		}
	}
	catch ( const AxEx& ex ) {
		wcout << ex.what() << endl;
	}
	catch ( const Aaf2XtlUsageEx& ex ) {
		cout << ex.what() << endl;
	}
	catch ( const exception& ex ) {
		cout << ex.what() << endl;
	}
	catch ( ... ) {
		wcout << L"unknown exception" << endl;
	}
	
	return 0;
}

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
//=---------------------------------------------------------------------=

#include <MultiGenTest.h>

#include "MultiGenCommon.h"

#include <AAFDefUIDs.h>
#include <AAFFileKinds.h>

#include <map>
#include <string>
#include <memory>
#include <iostream>
#include <sstream>

#include <stdlib.h>
#include <assert.h>

namespace {

class KindMap {
public:
  KindMap()
  {
    #define ADD_KIND( X ) \
    _kindMap[ string( #X ) ] = aafFileKindAaf##X;

    ADD_KIND( MSSBinary );
    ADD_KIND( SSSBinary );
    ADD_KIND( M4KBinary );
    ADD_KIND( S4KBinary );
  }

  ~KindMap()
  {}


  aafUID_t GetKind( const char* kindName )
  {
    std::map<std::string, aafUID_t >::const_iterator iter;
    iter = _kindMap.find( kindName );

    if ( iter == _kindMap.end() ) {
      std::stringstream what;
      what << "File kind unknown: " << kindName;
      throw UsageEx( what.str().c_str() );
    }

    return iter->second;
  }

private:
  std::map< std::string, aafUID_t > _kindMap;
};


class FileOp : public MultiGenTest
{ 
public:
  FileOp()
    : MultiGenTest()
  {}

  virtual ~FileOp()
  {}

  virtual void RunTest( CmdState& state, int argc, char** argv );
};

IAAFSmartPointer<IAAFFile> CreateFileOfKind( const std::string& fileName,
					     aafFileExistence_e existance,
					     aafFileAccess_e access,
					     const aafUID_t& fileKind,
					     const aafProductIdentification_t& prodId )
{
  std::basic_string<wchar_t> wfileName( ToWideString( fileName.c_str() ) );
  
  IAAFSmartPointer<IAAFRawStorage> spRawStorage;
  CHECK_HRESULT( AAFCreateRawStorageDisk( wfileName.c_str(),
					  existance,
					  access,
					  &spRawStorage) );
  
  IAAFSmartPointer<IAAFFile> spFile;
  CHECK_HRESULT( AAFCreateAAFFileOnRawStorage( spRawStorage,
					       existance,
					       access,
					       &fileKind,
					       0,
					       &prodId,
					       &spFile));

  return spFile;
}

void FileOp::RunTest( CmdState& state, int argc, char** argv )
{
  using namespace std;
	
  IAAFSmartPointer<IAAFFile> iaafFile;

  aafProductVersion_t v;
  aafProductIdentification_t	productInfo;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  productInfo.companyName = L"AAF Developers Desk";
  productInfo.productName = L"AAF MultiGenTest";
  productInfo.productVersion = &v;
  productInfo.productVersionString = NULL;
  productInfo.productID = UnitTestProductID;
  productInfo.platform = NULL;

  if ( argc < 2 ) {
    throw UsageEx("FileOp requires least one argument.");
  }

  string which(argv[1]);

  if ( which == "write" || which == "read" || which == "modify" ) {

    if ( argc != 4 ) {
      throw UsageEx("FileOp write|read|modify expects 3 arguments.");
    }

    KindMap kmap;
    aafUID_t fileKind = kmap.GetKind( argv[2] );
    std::string fileName( argv[3] );

    aafFileExistence_e existance;
    aafFileAccess_e access;

    if ( which == "write" ) {
      existance = kAAFFileExistence_new;
      access = kAAFFileAccess_modify;
    }
    else if ( which == "read" ) {
      existance = kAAFFileExistence_existing;
      access = kAAFFileAccess_read;
    }
    else if ( which == "modify" ) {
      existance = kAAFFileExistence_existing;
      access = kAAFFileAccess_modify;
    }
    else {
      assert(0);
    }
    
    IAAFSmartPointer<IAAFFile> spFile =
      CreateFileOfKind( fileName,
			existance,
			access,
			fileKind,
			productInfo );

    CHECK_HRESULT( spFile->Open() );
    state.SetFile( spFile );
  }
  else if ( which == "save" ) {
    CHECK_HRESULT( state.GetFile()->Save() );
  }
  else if ( which == "close" ) {
    CHECK_HRESULT( state.GetFile()->Close() );
  }
  else if ( which == "save_and_close" ) {
    CHECK_HRESULT( state.GetFile()->Save() );
    CHECK_HRESULT( state.GetFile()->Close() );
  }
  else {
    string msg("FileOp option unrecognized: " + which);
    throw UsageEx( msg );
  }
}

StandardFactory<FileOp> factory(
  "FileOp",
  "File Operations",
  "{ (write|read|modify MSSBinary|SSSBinary|M4KBinary|S4KBinary filename} | save | close | save_and_close }",
  "",
  2, 4
  );

} // end of namespace

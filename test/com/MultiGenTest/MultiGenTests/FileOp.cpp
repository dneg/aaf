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

#include <string>
#include <memory>
#include <iostream>

#include <stdlib.h>

namespace {

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
  productInfo.productName = L"AAF Enumerator Test";
  productInfo.productVersion = &v;
  productInfo.productVersionString = NULL;
  productInfo.productID = UnitTestProductID;
  productInfo.platform = NULL;

  if ( argc < 2 ) {
    throw UsageEx("FileOp requires least one argument.");
  }

  string which(argv[1]);

  auto_ptr<wchar_t> fileName(0);

  if ( argc >= 3 ) {
    auto_ptr<wchar_t> tmp( ToWideString( argv[2] ) );
    fileName = tmp;
  }

  if ( which == "write" ) {

    if ( !fileName.get() ) {
      throw UsageEx("FileOp write expected filename");
    }

    IAAFSmartPointer<IAAFFile> pFile;
    CHECK_HRESULT( AAFFileOpenNewModify( fileName.get(), 0, &productInfo,
					 &iaafFile ) );

    state.SetFile( iaafFile );
  }
  else if ( which == "read" ) {

    if ( !fileName.get() ) {
      throw UsageEx("FileOp read expected filename");
    }

    CHECK_HRESULT( AAFFileOpenExistingRead( fileName.get(),
					    0,
					    &iaafFile ) );
    state.SetFile( iaafFile );
  }
  else if ( which == "modify" ) {

    if ( !fileName.get() ) {
      throw UsageEx("FileOp modify expected filename");
    }

    CHECK_HRESULT( AAFFileOpenExistingModify( fileName.get(),
					      0,
					      &productInfo,
					      &iaafFile ) );

    state.SetFile( iaafFile );
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
  "{ {write|read|modify filename} | save | close | save_and_close }",
  "",
  2, 3
  );

} // end of namespace

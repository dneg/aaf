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
#include <AAFStoredObjectIDs.h>

#include <memory>

namespace {

class AddMasterMobs : public MultiGenTest
{ 
public:
  AddMasterMobs( const char* name,
	const char* desc,
	const char* usage,
	const char* notes )
    : MultiGenTest( name, desc, usage, notes )
  {}

  virtual ~AddMasterMobs()
  {}

  virtual void RunTest( CmdState& state, int argc, char** argv );
};

void AddMasterMobs::RunTest( CmdState& state, int argc, char** argv )
{
  // expect at least one argument (beyond argv[0])
  if ( argc < 2 ) {
    throw UsageEx( "AddMasterMob expects one more mob names as arguments." );
  }

  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  checkResult( file->GetHeader( &header ) );
  
  IAAFSmartPointer<IAAFDictionary> dictionary;
  checkResult( file->GetDictionary( &dictionary ) );

  int i;
  for( i = 1; i < argc; i++ ) {

    IAAFSmartPointer<IAAFMasterMob> masmob;
    checkResult( dictionary->CreateInstance( AUID_AAFMasterMob,
					   IID_IAAFMasterMob,
					   ToIUnknown( &masmob ) ) );
    IAAFSmartPointer<IAAFMob> mob;
    checkResult( masmob->QueryInterface( IID_IAAFMob, ToVoid(&mob) ) );
    checkResult( masmob->Initialize() );

    auto_ptr<wchar_t> mobName( ToWideString( argv[i] ) );
    
    checkResult( mob->SetName( mobName.get() ) );

    checkResult( header->AddMob( mob ) );
  }
}

MULTIGEN_TEST_FACTORY( AddMasterMobFactory,
		       AddMasterMobs,
		       "Add one or more MasterMob's to a file.",
		       "[mob_name mob_name mob_name ...]",
		       "" );

// Global static instance of factory.  Ctor will register this
// instance with the MultiGenTestRegistry.
AddMasterMobFactory factory;

} // end of namespace

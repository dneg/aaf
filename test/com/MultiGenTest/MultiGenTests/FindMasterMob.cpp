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

#include <AAFStoredObjectIDs.h>

#include <memory>

namespace {

class FindMasterMobs : public MultiGenTest
{ 
public:
  FindMasterMobs()
    : MultiGenTest()
  {}

  virtual ~FindMasterMobs()
  {}

  virtual void RunTest( CmdState& state, int argc, char** argv );
};

void FindMasterMobs::RunTest( CmdState& state, int argc, char** argv )
{
  // expect at least one argument (beyond argv[0])
  if ( argc < 2 ) {
    throw UsageEx( "FindMasterMob expects one more mob names as arguments." );
  }
  
  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  CHECK_HRESULT( file->GetHeader( &header ) );

  aafSearchCrit_t	criteria;
  criteria.searchTag = kAAFByMobKind;
  criteria.tags.mobKind = kAAFMasterMob;
  IAAFSmartPointer<IEnumAAFMobs> mobsEnumerator;
  CHECK_HRESULT( header->GetMobs( &criteria, &mobsEnumerator ) );

  IAAFSmartPointer<IAAFMob> nextMob;
  HRESULT hr;

  // This will be simpler when it can be done with a wide char map
  // and basic_string<wchar_t>.

  vector<bool> found;
  int i;
  for( i = 0; i < argc; i++ ) {
    found.push_back(false);
  }

  int mobCount;
  for( hr = mobsEnumerator->NextOne( &nextMob ), mobCount = 0;
       hr != AAFRESULT_NO_MORE_OBJECTS;
       hr = mobsEnumerator->NextOne( &nextMob ), mobCount++ ) {

    aafUInt32 bufSize;
    CHECK_HRESULT( nextMob->GetNameBufLen( &bufSize ) );
    // Convert bufSize from bytes to wide char;
    bufSize = (bufSize+1)/2;
    auto_ptr<wchar_t> buf( new wchar_t [ bufSize ] );
    CHECK_HRESULT( nextMob->GetName( buf.get(), sizeof(wchar_t)*bufSize ) );

    for( i = 1; i < argc; i++ ) {
      if ( found[i] ) {
	continue;
      }

      auto_ptr<wchar_t> wideName( ToWideString( argv[i] ) );
      if ( wstrcmp( wideName.get(), buf.get() ) ) {
	found[i] = true;
      }
    }
  }
  if ( hr != AAFRESULT_NO_MORE_OBJECTS ) {
    CHECK_HRESULT( hr );
  }

  // See if any mobs were not found.
  string notFound( "Mobs not found: " );
  bool throwIt = false;
  for( i = 1; i < argc; i++ ) {
    if ( !found[i] ) {
      notFound += argv[i];
      notFound += " ";
      throwIt = true;
    }
  }    

  if (throwIt) {
    throw notFound;
  }
  
  if ( strcmp( argv[0], "FindMasterMobsExclusive" ) == 0  &&
       mobCount != argc-1 ) {
    throw "Additional mobs exist.";
  }

}

#if 0
MULTIGEN_TEST_FACTORY( FindMasterMobsFactory,
		       FindMasterMobs,
		       "Verify that named mobs exist. Other master mobs may also exist.",
		       "[mob_name mob_name mob_name ...]",
		       "" );

MULTIGEN_TEST_FACTORY_LONG( FindMasterMobsExclusiveFactory,
			    FindMasterMobsExclusive,
			    FindMasterMobs,
			    "Verify that named mobs are the only master mobs in the file.",
			    "[mob_name mob_name mob_name ...]",
			    "This is just a different way to execute FindMasterMobs." );

// Global static instance of factory.  Ctor will register this
// instance with the MultiGenTestRegistry.
FindMasterMobsFactory factoryA;
FindMasterMobsExclusiveFactory factoryB;
#endif

StandardFactory<FindMasterMobs> factoryA (
  "FindMasterMobs",
  "Verify that named mobs exist. Other master mobs may also exist.",
  "[mob_name mob_name mob_name ... ]",
  "Depricated.  Do not use in new test configurations.",
  1, -1
);

StandardFactory<FindMasterMobs> factoryB (
  "FindMasterMobsExclusive",
  "Verify that named mobs are the only master mobs in the file.",
  "[mob_name mob_name mob_name ...]",
  "Depricated. Do not use in new test configurations.",
  1, -1
);

} // end of namespace

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

#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxMob.h>

#include <axFileGen.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  CompositionMob,           
  L"CompositionMob",
  L"Create a composition mob.",
  L"FileName CompMobName mob_name",
  L"CompMobName is your reference to the object.  mob_name is the persistent mob name",
  4,
  4 ) 

CompositionMob::~CompositionMob()
{}

void CompositionMob::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString mobName  = argv[2];
	AxString name     = argv[3];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );

	IAAFCompositionMobSP spCompMob;
	AxCreateInstance( axDict, spCompMob );
	AxCompositionMob axCompMob( spCompMob );

	axCompMob.Initialize( name );
 
	AxHeader( HeaderFromFileOp( fileName ) ).AddMob( axCompMob );

	SetCOM( spCompMob );
	RegisterInstance( mobName );
}

} // end of namespace



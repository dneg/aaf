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

#include <axFileGen.h>

#include <AxDictionary.h>
#include <AxMob.h>
#include <AxHeader.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  SourceMob,           
  L"SourceMob",
  L"Create a source mob and add it to the file header.",
  L"file_ref name essence_desc_ref",
  L"Initialized with a nill source reference.",
  4,
  4 ) 

SourceMob::~SourceMob()
{}

void SourceMob::Execute( const std::vector<AxString>& argv )
{
	AxString fileName        = argv[1];
	AxString sourceMobName   = argv[2];
	AxString essenceDescName = argv[3];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );
	IAAFSourceMobSP spSourceMob;
	AxCreateInstance( axDict, spSourceMob );
	AxSourceMob axSrcMob( spSourceMob );

	IAAFEssenceDescriptorSP spDesc;
	GetInstance( essenceDescName ).GetCOM( spDesc );
	axSrcMob.SetEssenceDescriptor( spDesc );

	AxHeader axHeader( HeaderFromFileOp( fileName) );
	axHeader.AddMob( axSrcMob );

	SetCOM( spSourceMob );
	RegisterInstance( sourceMobName );
}

} // end of namespace



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
#include <AxComponent.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  Filler,           
  L"Filler",
  L"Create filler.",
  L"FileName FillerName DataDefName length",
  L"",
  5,
  5 ) 

Filler::~Filler()
{}

void Filler::Execute( const std::vector<AxString>& argv )
{
	AxString fileName     = argv[1];
	AxString fillerName   = argv[2];
	AxString dataDefName  = argv[3];
	AxString length       = argv[4];

	IAAFFillerSP spFiller;
	AxCreateInstance( DictionaryFromFileOp( fileName ), spFiller );
	AxFiller axFiller( spFiller );

	IAAFDataDefSP spDataDef;
	GetInstance( dataDefName ).GetCOM( spDataDef );

	axFiller.Initialize( spDataDef,	AxStringUtil::strtol( length ) );

	SetCOM( spFiller );
	RegisterInstance( fillerName );
}

} // end of namespace



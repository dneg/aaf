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

#include "ParamMaps.h"

#include <AxFile.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxEssence.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  TapeDescriptor,           
  L"TapeDescriptor",
  L"A Tape EssenceDescriptor.",
  L"file_ref name manufacturer tape_model tape_case signal tape_format length",
  L"case, signal, and format names match their typedefs.",
  9,
  9 )

TapeDescriptor::~TapeDescriptor()
{}

void TapeDescriptor::Execute( const std::vector<AxString>& argv )
{
	AxString fileOpName    = argv[1];
	AxString tapeOpName    = argv[2];
	AxString manufacturer  = argv[3];
	AxString model         = argv[4];
	AxString caseType      = argv[5];
	AxString signalType    = argv[6];
	AxString tapeFormat    = argv[7];
	AxString lengthString  = argv[8];
	
	AxDictionary axDictionary( DictionaryFromFileOp(fileOpName) );
	IAAFTapeDescriptorSP spTapeDesc;
	AxCreateInstance( axDictionary, spTapeDesc );

	AxTapeDescriptor axTapeDesc( spTapeDesc );
	axTapeDesc.Initialize();
	axTapeDesc.SetTapeManufacturer( manufacturer );
	axTapeDesc.SetTapeModel( model );
	axTapeDesc.SetTapeFormFactor( TapeCaseParams::GetInstance().Find( *this, caseType ) );
	axTapeDesc.SetSignalType( SignalTypeParams::GetInstance().Find( *this, signalType ) );
	axTapeDesc.SetTapeFormat( TapeFormatParams::GetInstance().Find( *this, tapeFormat ) );
	axTapeDesc.SetTapeLength( AxStringUtil::strtol(lengthString) );


	SetCOM( spTapeDesc );
	RegisterInstance( tapeOpName );
}

} // end of namespace



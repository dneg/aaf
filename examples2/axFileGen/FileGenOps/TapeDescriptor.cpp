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
  L"file_ref name length [manufacturer tape_model tape_case signal tape_format]",
  L"case, signal, and format names match their typedefs.",
  4,
  9 )

TapeDescriptor::~TapeDescriptor()
{}

void TapeDescriptor::Execute( const std::vector<AxString>& argv )
{
	AxString fileOpName    = argv[1];
	AxString tapeOpName    = argv[2];
	AxString lengthString  = argv[3];       

	pair<bool,AxString> manufacturer( false, L"" );
	pair<bool,AxString> model( false, L"" );
	pair<bool,AxString> caseType( false, L"" );
	pair<bool,AxString> signalType( false, L"" );
	pair<bool,AxString> tapeFormat( false, L"" );


	// FIXME - This should be generalized... can't expect every operation
	// that has optional parameters to go through this.
	if ( argv.size() == 9 ) {
		manufacturer.first = true;
		manufacturer.second = argv[4];

		model.first = true;
		manufacturer.second = argv[5];

		caseType.first = true;
		caseType.second = argv[6];
		
		signalType.first = true;
		signalType.second = argv[7];
		
		tapeFormat.first = true;
		tapeFormat.second = argv[8];
	}

	AxDictionary axDictionary( DictionaryFromFileOp(fileOpName) );
	IAAFTapeDescriptorSP spTapeDesc;
	AxCreateInstance( axDictionary, spTapeDesc );

	AxTapeDescriptor axTapeDesc( spTapeDesc );
	axTapeDesc.Initialize();

	axTapeDesc.SetTapeLength( AxStringUtil::strtol(lengthString) );

	if ( manufacturer.first ) {
		axTapeDesc.SetTapeManufacturer( manufacturer.second );
	}

	if ( model.first ) {
		axTapeDesc.SetTapeModel( model.second );
	}

	if ( caseType.first ) {
		axTapeDesc.SetTapeFormFactor( TapeCaseParams::GetInstance().Find( *this, caseType.second ) );
	}

	if ( signalType.first ) {
		axTapeDesc.SetSignalType( SignalTypeParams::GetInstance().Find( *this, signalType.second ) );
	}

	if ( tapeFormat.first ) {
		axTapeDesc.SetTapeFormat( TapeFormatParams::GetInstance().Find( *this, tapeFormat.second ) );
	}


	SetCOM( spTapeDesc );
	RegisterInstance( tapeOpName );
}

} // end of namespace

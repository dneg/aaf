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
#include <AxMob.h>

namespace {

// Let the compiler zero this for us to avoid memset pollution.
aafSourceRef_t _nullSourceRef;

//=---------------------------------------------------------------------=

AXFG_OP(
  SourceClip,           
  L"SourceClip",
  L"Create a new SourceClip.",
  L"FileName SourceClipName DataDefName length [MobName slot_id start_time]",
  L"The id of MobName is used as the source reference - null if absent.",
  5,
  8 ) 

SourceClip::~SourceClip()
{}

void SourceClip::Execute( const std::vector<AxString>& argv )
{
	AxString fileName       = argv[1];
	AxString sourceClipName = argv[2];
	AxString dataDefName    = argv[3];
	AxString length         = argv[4];
	// **** more args below ****

	IAAFSourceClipSP spSourceClip;
	AxCreateInstance( DictionaryFromFileOp( fileName ), spSourceClip );	
	AxSourceClip axSourceClip( spSourceClip );

	IAAFDataDefSP spDataDef;
	GetInstance( dataDefName ).GetCOM( spDataDef );
		
	aafLength_t lengthVal = AxStringUtil::strtol( length );
		
	if ( argv.size() == GetMinArgC() ) {

		axSourceClip.Initialize( spDataDef,
								 lengthVal,
								 _nullSourceRef );

	}
	else if ( argv.size() == GetMaxArgC() ) {

		// **** more args here ****
		AxString mobName   = argv[5];
		AxString slotId    = argv[6];
		AxString startTime = argv[7];

		IAAFMobSP spMob;
		GetInstance( mobName ).GetCOM( spMob );
		AxMob axMob( spMob );
		
		aafSourceRef_t srcRef;

		srcRef.sourceID    = axMob.GetMobID();
		srcRef.sourceSlotID = AxStringUtil::strtol( slotId );
		srcRef.startTime    = AxStringUtil::strtol( startTime );

		axSourceClip.Initialize( spDataDef,
								 lengthVal,
								 srcRef );

	}
	else {
		throw AxFGOpUsageEx( *this );
	}


	SetCOM( spSourceClip );
	RegisterInstance( sourceClipName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  SetSourceRef,
  L"SetSourceRef",
  L"Set a soure clip's source reference in order to build a mob chain.",
  L"SourceClipName MobName slot_id start_time",
  L"",
  5,
  5 )

SetSourceRef::~SetSourceRef()
{}

void SetSourceRef::Execute( const std::vector<AxString>& argv )
{
	AxString srcClipName = argv[1];
	AxString mobName     = argv[2];
	AxString slotId      = argv[3];
	AxString startTime   = argv[4];

	IAAFSourceClipSP spSrcClip;
	GetInstance( srcClipName ).GetCOM( spSrcClip );
	AxSourceClip axSrcClip( spSrcClip );

	IAAFMobSP spMob;
	GetInstance( mobName ).GetCOM( spMob );
	AxMob axMob( spMob );
		
	aafSourceRef_t srcRef;

	srcRef.sourceID     = axMob.GetMobID();
	srcRef.sourceSlotID = AxStringUtil::strtol( slotId );
	srcRef.startTime    = AxStringUtil::strtol( startTime );

	axSrcClip.SetSourceReference( srcRef );
}

} // end of namespace



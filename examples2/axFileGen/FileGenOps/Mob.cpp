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

#include "Rate.h"

#include <axFileGen.h>

#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxDictionary.h>
#include <AxHeader.h>
#include <AxContentStorage.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  AppendTimelineMobSlot,
  L"AppendTimelineMobSlot",
  L"Create a new mob slot and add to the mob at the specified slot..",
  L"MobName SegmentName MobSlotRefName slot_id slot_name origin edit_rate_num edit_rate_den",
  L"The new MobSlot is referenced MobSlotRefName.  slot_name is the name stored in the file.",
  8,
  8 )

AppendTimelineMobSlot::~AppendTimelineMobSlot()
{
}

void AppendTimelineMobSlot::Execute( const std::vector<AxString>& argv )
{
	AxString mobName      = argv[1];
	AxString segName      = argv[2];
	AxString slotRefName  = argv[3];
	AxString slotId       = argv[4];
	AxString slotName     = argv[5];
	AxString origin       = argv[6];
	AxString rateName     = argv[7];

	IAAFMobSP spMob;
	GetInstance( mobName ).GetCOM( spMob );
	AxMob axMob( spMob );

	IAAFSegmentSP spSeg;
	GetInstance( segName ).GetCOM( spSeg );

	aafRational_t editRateVal = RateOpToRational( GetInstance( rateName ) );

	IAAFTimelineMobSlotSP spSlot;
	spSlot = axMob.AppendNewTimelineSlot( editRateVal,
									spSeg,
									AxStringUtil::strtol( slotId ),
									slotName,
									AxStringUtil::strtol( origin ) );

	SetCOM( spSlot );
	RegisterInstance( slotRefName );	
}

//=---------------------------------------------------------------------=

AXFG_OP(
  LookupSourceID,
  L"LookupSourceID",
  L"Get the the source ID from a source clip, and lookup the identified mob.",
  L"FileName SourceClipName MobName",
  L"The identified, new, mob is referenced by MobName",
  4,
  4 )

LookupSourceID::~LookupSourceID()
{}

void LookupSourceID::Execute( const std::vector<AxString>& argv )
{
	AxString fileName       = argv[1];
	AxString sourceClipName = argv[2];
	AxString mobName        = argv[3];

	AxHeader axHeader( HeaderFromFileOp( fileName ) );
	AxContentStorage axContent( axHeader.GetContentStorage() );

	IAAFSourceClipSP spSourceClip;
	GetInstance( sourceClipName ).GetCOM( spSourceClip );
	AxSourceClip axSourceClip( spSourceClip );


	SetCOM( axContent.LookupMob( axSourceClip.GetSourceID() ) );
	RegisterInstance( mobName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  AppendSlot,           
  L"AppendSlot",
  L"Append a mob slot to a mob.",
  L"MobName SlotName",
  L"",
  3,
  3 ) 

AppendSlot::~AppendSlot()
{}

void AppendSlot::Execute( const std::vector<AxString>& argv )
{
	AxString mobName  = argv[1];
	AxString slotName = argv[2];

	IAAFMobSP spMob;
	GetInstance( mobName ).GetCOM( spMob );
	AxMob axMob(spMob);

	IAAFMobSlotSP spSlot;
	GetInstance( slotName ).GetCOM( spSlot );
	AxMobSlot axSlot( spSlot );

	axMob.AppendSlot( spSlot );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  SetMobName,           
  L"SetMobName",
  L"Set mob name.",
  L"MobName mob_name",
  L"MobName is a reference to an axFileGen object.  mob_name is the persistent AAF name.",
  3,
  3 ) 

SetMobName::~SetMobName()
{}

void SetMobName::Execute( const std::vector<AxString>& argv )
{
	AxString mobName = argv[1];
	AxString name    = argv[2];

	IAAFMobSP spMob;
	GetInstance( mobName ).GetCOM( spMob );
	AxMob axMob( spMob );
	axMob.SetName( name );
}

//=---------------------------------------------------------------------=


} // end of namespace

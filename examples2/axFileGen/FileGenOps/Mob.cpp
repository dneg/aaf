//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

#include "Rate.h"
#include "ParamMaps.h"

#include <axFileGen.h>

#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxDictionary.h>
#include <AxHeader.h>
#include <AxContentStorage.h>
#include <AxIterator.h>
#include <AxEssence.h>
#include <AxDefObject.h>

#include <AAFResult.h>

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

AXFG_OP(
  FindNamedMob,           
  L"FindNamedMob",
  L"Find a mob by name.",
  L"FileName mob_name FoundMobName",
  L"Search for mob \"mob_name\", save reference as \"FoundMobName\".  The first occurence is returned.",
  4,
  4 ) 

FindNamedMob::~FindNamedMob()
{}

void FindNamedMob::Execute( const std::vector<AxString>& argv )
{
	AxString fileName   = argv[1];
	AxString mobName    = argv[2];
	AxString mobRefName = argv[3];

	AxHeader axHeader( HeaderFromFileOp( fileName ) );

	aafSearchCrit_t criteria;
	criteria.tags.name = const_cast<aafString_t>(mobName.c_str());
  	criteria.searchTag = kAAFByName;

	AxMobIter axMobIter( axHeader.GetMobs( criteria ) );
	
	IAAFSmartPointer2<IAAFMob> spMob;
	bool notAtEnd;
	notAtEnd = axMobIter.NextOne( spMob );
  	if ( !notAtEnd ) {
		throw AxFGEx( L"Mob not found." );
	}

	SetCOM( spMob );
	RegisterInstance( mobRefName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  FindMobByDataDef,           
  L"FindMobByDataDef",
  L"Find a mob by data def.",
  L"FileName DataDefName FoundMobName",
  L"The first occurence of a mob with a slot that matches the data def is returned.",
  4,
  4 ) 

FindMobByDataDef::~FindMobByDataDef()
{}

void FindMobByDataDef::Execute( const std::vector<AxString>& argv )
{
	AxString fileName   = argv[1];
	AxString defName    = argv[2];
	AxString mobRefName = argv[3];

	AxHeader axHeader( HeaderFromFileOp( fileName ) );

	IAAFDataDefSP spDataDef;
	GetInstance( defName ).GetCOM( spDataDef );
	AxDataDef axDataDef( spDataDef );

	aafSearchCrit_t criteria;
	criteria.tags.datadef = axDataDef.GetAUID();
  	criteria.searchTag = kAAFByDataDef;

	AxMobIter axMobIter( axHeader.GetMobs( criteria ) );
	
	IAAFSmartPointer2<IAAFMob> spMob;
	bool notAtEnd;
	notAtEnd = axMobIter.NextOne( spMob );
  	if ( !notAtEnd ) {
		throw AxFGEx( L"Mob not found." );
	}


	SetCOM( spMob );
	RegisterInstance( mobRefName );
}
	
//=---------------------------------------------------------------------=

AXFG_OP(
  CopyMob,           
  L"CopyMob",
  L"Copy a mob.",
  L"FileName SrcMobName DstMobName",
  L"The copy may be referenced by the namd DstMobName - it *not* the actual mob name.",
  4,
  4 ) 

CopyMob::~CopyMob()
{}

void CopyMob::Execute( const std::vector<AxString>& argv )
{
	AxString fileName   = argv[1];
	AxString srcMobName = argv[2];
	AxString dstMobName = argv[3];
	
	AxHeader axHeader( HeaderFromFileOp( fileName ) );

	IAAFMobSP spMob;
	GetInstance( srcMobName ).GetCOM( spMob );
	AxMob axMob( spMob );

	AxString name = axMob.GetName() + L" Copy";
	IAAFMobSP spNewMob = axMob.Copy( name.c_str() );

	axHeader.AddMob( spNewMob );

	SetCOM( spNewMob );
	RegisterInstance( dstMobName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CloneExternal,           
  L"CloneExternal",
  L"Copy a mob to another file.",
  L"SrcMobName DstFileName DstMobName include_media follow_depends",
  L"The copy may be referenced by the namd DstMobName - it *not* the actual mob name.",
  6,
  6 ) 

CloneExternal::~CloneExternal()
{}

void CloneExternal::Execute( const std::vector<AxString>& argv )
{
	AxString srcMobName    = argv[1];
	AxString dstFileName   = argv[2];
	AxString dstMobName    = argv[3];
	AxString incMedia      = argv[4];
	AxString followDepends = argv[5];

	IAAFMobSP spSrcMob;
	GetInstance( srcMobName ).GetCOM( spSrcMob );
	AxMob axSrcMob( spSrcMob );

	IAAFMobSP spNewDstMob = axSrcMob.CloneExternal( DependParams::GetInstance().Find( *this, followDepends ),
													IncMediaParams::GetInstance().Find( *this, incMedia ),
													FileFromFileOp( dstFileName ) );

	SetCOM( spNewDstMob );
	RegisterInstance( dstMobName );
}

//=---------------------------------------------------------------------=

} // end of namespace

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AxMobSlot.h"

//=---------------------------------------------------------------------=

AxMobSlot::AxMobSlot( IAAFMobSlotSP spIaafMobSlot )
:	AxObject( AxQueryInterface<IAAFMobSlot, IAAFObject>(spIaafMobSlot) ),
	_spIaafMobSlot( spIaafMobSlot )
{}

AxMobSlot::~AxMobSlot()
{}


void AxMobSlot::SetName( const AxString& name )
{
	CHECK_HRESULT( _spIaafMobSlot->SetName( name.c_str() ) );
}

void AxMobSlot::SetSegment( IAAFSegmentSP sp )
{
	CHECK_HRESULT( _spIaafMobSlot->SetSegment( sp ) );
}

void AxMobSlot::SetPhysicalNum( aafUInt32 val )
{
	CHECK_HRESULT( _spIaafMobSlot->SetPhysicalNum( val ) );
}

void AxMobSlot::SetSlotID( aafSlotID_t val )
{
	CHECK_HRESULT( _spIaafMobSlot->SetSlotID( val ) );
}

AxString AxMobSlot::GetName()
{
	return AxNameToString< IAAFMobSlot >( _spIaafMobSlot );
}

AxString AxMobSlot::GetName( const AxString& defaul )
{
	return AxNameToString< IAAFMobSlot >( _spIaafMobSlot, defaul );
}

std::pair<bool,AxString> AxMobSlot::ExistsName()
{
  return AxExistsNameToString< IAAFMobSlot >( _spIaafMobSlot );
}

aafUInt32 AxMobSlot::GetPhysicalNum()
{
	aafUInt32 num;

	CHECK_HRESULT( _spIaafMobSlot->GetPhysicalNum( &num ) );

	return num;
}

std::pair<bool,aafUInt32> AxMobSlot::ExistsPhysicalNum()
{
  std::pair<bool,aafUInt32> num(true,0);

  AAFRESULT hr = _spIaafMobSlot->GetPhysicalNum( &num.second );

  if ( hr == AAFRESULT_PROP_NOT_PRESENT )
  {
    num.first = false;
  }
  else
  {
    CHECK_HRESULT( hr );
  }

  return num;
}

aafSlotID_t AxMobSlot::GetSlotID()
{
	aafSlotID_t id;

	CHECK_HRESULT( _spIaafMobSlot->GetSlotID( &id ) );

	return id;
}

IAAFSegmentSP AxMobSlot::GetSegment()
{
	IAAFSegmentSP sp;

	CHECK_HRESULT( _spIaafMobSlot->GetSegment( &sp ) );

	return sp;
}

IAAFDataDefSP AxMobSlot::GetDataDef()
{
	IAAFDataDefSP spIaafDataDef;

	CHECK_HRESULT( _spIaafMobSlot->GetDataDef( &spIaafDataDef ) );

	return spIaafDataDef;
}

//=---------------------------------------------------------------------=

AxTimelineMobSlot::AxTimelineMobSlot( IAAFTimelineMobSlotSP spIaafTimelineMobSlot )
: AxMobSlot( AxQueryInterface<IAAFTimelineMobSlot, IAAFMobSlot>( spIaafTimelineMobSlot ) ),
    _spIaafTimelineMobSlot( AxQueryInterface<IAAFTimelineMobSlot, IAAFTimelineMobSlot2>( spIaafTimelineMobSlot ) )
{}

AxTimelineMobSlot::AxTimelineMobSlot( IAAFTimelineMobSlot2SP spIaafTimelineMobSlot )
: AxMobSlot( AxQueryInterface<IAAFTimelineMobSlot2, IAAFMobSlot>( spIaafTimelineMobSlot ) ),
    _spIaafTimelineMobSlot( spIaafTimelineMobSlot )
{}

AxTimelineMobSlot::~AxTimelineMobSlot()
{}

void AxTimelineMobSlot::Initialize()
{
	CHECK_HRESULT( _spIaafTimelineMobSlot->Initialize() );
}

void AxTimelineMobSlot::SetOrigin( aafPosition_t origin )
{
	CHECK_HRESULT( _spIaafTimelineMobSlot->SetOrigin( origin ) );
}

void AxTimelineMobSlot::SetEditRate( const aafRational_t& rate )
{
	CHECK_HRESULT( _spIaafTimelineMobSlot->SetEditRate( rate ) );
}

void AxTimelineMobSlot::SetMarkIn( aafPosition_t value )
{
    CHECK_HRESULT( _spIaafTimelineMobSlot->SetMarkIn( value ) );
}

void AxTimelineMobSlot::SetMarkOut( aafPosition_t value )
{
    CHECK_HRESULT( _spIaafTimelineMobSlot->SetMarkOut( value ) );
}

void AxTimelineMobSlot::SetUserPos ( aafPosition_t value )
{
    CHECK_HRESULT( _spIaafTimelineMobSlot->SetUserPos( value ) );
}

aafPosition_t AxTimelineMobSlot::GetOrigin()
{
	aafPosition_t pos;

	CHECK_HRESULT( _spIaafTimelineMobSlot->GetOrigin( &pos ) );

	return pos;
}

aafRational_t AxTimelineMobSlot::GetEditRate()
{
	aafRational_t rate;

	CHECK_HRESULT( _spIaafTimelineMobSlot->GetEditRate( &rate ) );

	return rate;
}

aafPosition_t AxTimelineMobSlot::GetMarkIn()
{
    aafPosition_t pos;

    CHECK_HRESULT( _spIaafTimelineMobSlot->GetMarkIn( &pos ) );

    return pos;
}

aafPosition_t AxTimelineMobSlot::GetMarkOut()
{
    aafPosition_t pos;

    CHECK_HRESULT( _spIaafTimelineMobSlot->GetMarkOut( &pos ) );

    return pos;
}


std::pair<bool,aafPosition_t> AxTimelineMobSlot::ExistsMarkIn()
{
	std::pair<bool,aafPosition_t> pos(true,0);

	AAFRESULT hr = _spIaafTimelineMobSlot->GetMarkIn( &pos.second );
	
	if ( hr == AAFRESULT_PROP_NOT_PRESENT )
	{
		pos.first = false;
	}
	else
	{
		CHECK_HRESULT( hr );
	}

	return pos;
}

std::pair<bool,aafPosition_t> AxTimelineMobSlot::ExistsMarkOut()
{
	std::pair<bool,aafPosition_t> pos(true,0);

	AAFRESULT hr = _spIaafTimelineMobSlot->GetMarkOut( &pos.second );
	
	if ( hr == AAFRESULT_PROP_NOT_PRESENT )
	{
		pos.first = false;
	}
	else
	{
		CHECK_HRESULT( hr );
	}

	return pos;
}

aafPosition_t AxTimelineMobSlot::GetUserPos()
{
    aafPosition_t pos;

    CHECK_HRESULT( _spIaafTimelineMobSlot->GetUserPos( &pos ) );

    return pos;
}

std::pair<bool,aafPosition_t> AxTimelineMobSlot::ExistsUserPos()
{
	std::pair<bool,aafPosition_t> pos(true,0);

	AAFRESULT hr = _spIaafTimelineMobSlot->GetUserPos( &pos.second );
	
	if ( hr == AAFRESULT_PROP_NOT_PRESENT )
	{
		pos.first = false;
	}
	else
	{
		CHECK_HRESULT( hr );
	}

	return pos;
}


//=---------------------------------------------------------------------=

AxEventMobSlot::AxEventMobSlot( IAAFEventMobSlotSP spIaafEventMobSlot )
:	AxMobSlot( AxQueryInterface<IAAFEventMobSlot, IAAFMobSlot>(spIaafEventMobSlot) ),
	_spIaafEventMobSlot( spIaafEventMobSlot )
{}

AxEventMobSlot::~AxEventMobSlot()
{}

void AxEventMobSlot::SetEditRate( aafRational_t rate )
{
	CHECK_HRESULT( _spIaafEventMobSlot->SetEditRate( &rate ) );
}


aafRational_t AxEventMobSlot::GetEditRate()
{
	aafRational_t rate;

	CHECK_HRESULT( _spIaafEventMobSlot->GetEditRate( &rate ) );

	return rate;
}

//=---------------------------------------------------------------------=

AxStaticMobSlot::AxStaticMobSlot( IAAFStaticMobSlotSP spIaafStaticMobSlot )
:   AxMobSlot( AxQueryInterface<IAAFStaticMobSlot, IAAFMobSlot>(spIaafStaticMobSlot) ),
    _spIaafStaticMobSlot( spIaafStaticMobSlot )
{}

AxStaticMobSlot::~AxStaticMobSlot()
{}

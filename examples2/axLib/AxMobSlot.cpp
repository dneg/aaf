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

#include "AxMobSlot.h"

//=---------------------------------------------------------------------=

AxMobSlot::AxMobSlot( IAAFMobSlotSP& spIaafMobSlot )
:	AxObject( AxQueryInterface<IAAFMobSlot, IAAFObject>(
				spIaafMobSlot, IID_IAAFObject ) ),
	_spIaafMobSlot( spIaafMobSlot )
{}

AxMobSlot::~AxMobSlot()
{}

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

AxTimelineMobSlot::AxTimelineMobSlot( IAAFTimelineMobSlotSP& spIaafTimelineMobSlot )
:	AxMobSlot( AxQueryInterface<IAAFTimelineMobSlot, IAAFMobSlot>(
			  spIaafTimelineMobSlot, IID_IAAFMobSlot ) ),
	_spIaafTimelineMobSlot( spIaafTimelineMobSlot )
{}

AxTimelineMobSlot::~AxTimelineMobSlot()
{}

aafPosition_t AxTimelineMobSlot::GetOrigin()
{
	aafPosition_t pos;

	CHECK_HRESULT( _spIaafTimelineMobSlot->GetOrigin( &pos ) );

	return pos;
}

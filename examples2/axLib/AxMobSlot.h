#ifndef __AxMobSlot_h__
#define __AxMobSlot_h__

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

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

//=---------------------------------------------------------------------=

class AxMobSlot : public AxObject {
public:
	AxMobSlot( IAAFMobSlotSP spIaafMobSlot );
	~AxMobSlot();

	aafSlotID_t GetSlotID();

	void SetName( AxString name );
	void SetSegment( IAAFSegmentSP sp );
	void SetPhysicalNum( aafUInt32 val );
	void SetSlotID( aafSlotID_t val );

	IAAFSegmentSP GetSegment();

	IAAFDataDefSP GetDataDef();

	// Allow typecast to internal type.  This seems to be needed for the
	// AxString AxNameToString( IAAFSmartPointer< Type >& sp ) in
	// AxUtil.h to work on Ax objects that don't have a get. Maybe there
	// is a better way, in which case email me jtl21@users.sourceforge.net
	inline operator IAAFMobSlotSP ()
	{ return _spIaafMobSlot; }

private:
	AxMobSlot();
	AxMobSlot( const AxMobSlot& );
	AxMobSlot& operator=( const AxMobSlot& );
	
	IAAFMobSlotSP _spIaafMobSlot;
};

//=---------------------------------------------------------------------=

class AxTimelineMobSlot : public AxMobSlot {
public:
	AxTimelineMobSlot( IAAFTimelineMobSlotSP spIaafTimelineMobSlot );
	~AxTimelineMobSlot();

	void Initialize();

	void SetOrigin( aafPosition_t origin );

	void SetEditRate( const aafRational_t& rate );

	aafPosition_t GetOrigin();
	aafRational_t GetEditRate();

	operator IAAFTimelineMobSlotSP ()
	{ return _spIaafTimelineMobSlot; }

private:
	AxTimelineMobSlot();
	AxTimelineMobSlot( const AxTimelineMobSlot& );
	AxTimelineMobSlot& operator=( const AxTimelineMobSlot& );

	IAAFTimelineMobSlotSP _spIaafTimelineMobSlot;
};

//=---------------------------------------------------------------------=

class AxEventMobSlot : public AxMobSlot {
public:
	AxEventMobSlot( IAAFEventMobSlotSP spIaafEventMobSlot );
	~AxEventMobSlot();

	void SetEditRate( aafRational_t rate );

	aafRational_t GetEditRate();

	operator IAAFEventMobSlotSP ()
	{ return _spIaafEventMobSlot; }

private:
	AxEventMobSlot();
	AxEventMobSlot( const AxEventMobSlot& );
	AxEventMobSlot& operator=( const AxEventMobSlot& );

	IAAFEventMobSlotSP _spIaafEventMobSlot;
};

//=---------------------------------------------------------------------=

#endif

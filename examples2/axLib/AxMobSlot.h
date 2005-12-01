#ifndef __AxMobSlot_h__
#define __AxMobSlot_h__

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

#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

//=---------------------------------------------------------------------=

class AxMobSlot : public AxObject {
public:
	AxMobSlot( IAAFMobSlotSP spIaafMobSlot );
	~AxMobSlot();


	void SetName( const AxString& name );
	void SetSegment( IAAFSegmentSP sp );
	void SetPhysicalNum( aafUInt32 val );
	void SetSlotID( aafSlotID_t val );

	AxString GetName();
	IAAFSegmentSP GetSegment();
	aafUInt32 GetPhysicalNum();
	aafSlotID_t GetSlotID();

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
    AxTimelineMobSlot( IAAFTimelineMobSlot2SP spIaafTimelineMobSlot );
	~AxTimelineMobSlot();

	void Initialize();

	void SetOrigin( aafPosition_t origin );
	void SetEditRate( const aafRational_t& rate );
    void SetMarkIn( aafPosition_t value );
    void SetMarkOut( aafPosition_t value );
    void SetUserPos ( aafPosition_t value );

	aafPosition_t GetOrigin();
	aafRational_t GetEditRate();
    aafPosition_t GetMarkIn();
    aafPosition_t GetMarkOut();
    aafPosition_t GetUserPos();
   
    inline operator IAAFTimelineMobSlotSP ()
    { return AxQueryInterface<IAAFTimelineMobSlot2,IAAFTimelineMobSlot>( _spIaafTimelineMobSlot ); }

    inline operator IAAFTimelineMobSlot2SP ()
    { return _spIaafTimelineMobSlot; }

private:
	AxTimelineMobSlot();
	AxTimelineMobSlot( const AxTimelineMobSlot& );
	AxTimelineMobSlot& operator=( const AxTimelineMobSlot& );

	IAAFTimelineMobSlot2SP _spIaafTimelineMobSlot;
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

class AxStaticMobSlot : public AxMobSlot {
public:
    AxStaticMobSlot( IAAFStaticMobSlotSP spIaafStaticMobSlot );
    ~AxStaticMobSlot();

    operator IAAFStaticMobSlotSP ()
    { return _spIaafStaticMobSlot; }

private:
    AxStaticMobSlot();
    AxStaticMobSlot( const AxStaticMobSlot& );
    AxStaticMobSlot& operator=( const AxStaticMobSlot& );

    IAAFStaticMobSlotSP _spIaafStaticMobSlot;
};

//=---------------------------------------------------------------------=

#endif

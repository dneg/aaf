//@doc
//@class    AAFMobSlot | Implementation class for AAFMobSlot
#ifndef __ImplAAFMobSlot_h__
#define __ImplAAFMobSlot_h__

#include "ImplAAFObject.h"
#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif
#include "OMProperty.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

////////////////////////////////////////////////////////////////////////////////
// Types required by this module:
//
// aafBool,
// aafRational_t,
// AAFSegment,
// aafPosition_t,
// aafTrackID_t,
// aafUInt32,
// AAFDataDef

const int PID_MOBSLOT_NAME				= 0;
const int PID_MOBSLOT_TRACKID			= 1;
const int PID_MOBSLOT_PHYSICAL_TRACK	= 2;
const int PID_MOBSLOT_SEGMENT			= 3;

class ImplAAFSegment;

class ImplAAFDataDef;







#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFMobSlot : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFMobSlot ();
  ~ImplAAFMobSlot ();

  OMDECLARE_STORABLE(ImplAAFMobSlot);
  //****************
  // GetSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSegment
        (ImplAAFSegment ** result);  //@parm [out,retval] Segment property value

  //****************
  // SetSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSegment
        (ImplAAFSegment *value);  //@parm [in] Segment property value


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (aafString_t *  name);  //@parm [in,out] Mob Slot Name

  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (aafString_t *  name);  //@parm [in] Mob Slot Name


  //****************
  // GetPhysicalNum()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPhysicalNum
        (aafUInt32 *  result);  //@parm [out,retval] The physical track number property value


  //****************
  // SetPhysicalNum()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPhysicalNum
        (aafUInt32  number);  //@parm [in] The physical track number property value


  //****************
  // GetDataKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDef
        (aafUID_t *result);  //@parm [out,retval] Data Definition UUID

  //***********************************************************
  // METHOD NAME: GetTrackID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetTrackID (
    // @parm [out,retval] aafTrackID_t * | result | Track id of the Mob Slot
    aafTrackID_t *  result
  );

  //***********************************************************
  // METHOD NAME: SetTrackID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  SetTrackID (
    // @parm [in] aafTrackID_t | value | Track id of the Mob Slot
    aafTrackID_t value
  );

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFMobSlotTest.cpp.
  static AAFRESULT test();

protected:
	OMStringProperty					_name;
//!!!	OMFixedSizeProperty<aafPosition_t>	_origin; Move to TimelineMobSlot
	OMFixedSizeProperty<aafUInt32>		_trackID;
	OMFixedSizeProperty<aafUInt32>		_physicalTrackNum;
	OMStrongReferenceProperty<ImplAAFSegment> _segment;
};

#endif // ! __ImplAAFMobSlot_h__


//@doc
//@class    AAFMobSlot | Implementation class for AAFMobSlot
#ifndef __ImplAAFMobSlot_h__
#define __ImplAAFMobSlot_h__

#include "ImplAAFObject.h"
#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

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
// aafSlotID_t,
// aafUInt32,
// AAFDataDef


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

  OMDECLARE_STORABLE(ImplAAFMobSlot)
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
        (aafWChar *  name,  //@parm [in] buffer provided by client to hold the Mob Slot Name
		aafInt32	bufsize);	//@parm [in] length of the buffer provided to hold the slot name

  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
		(aafInt32	*bufsize);	//@parm [in] length of the buffer provided to hold the slot name
							// including the terminator

  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (aafWChar *  name);  //@parm [in] Mob Slot Name


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
  // GetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDef
        (aafUID_t *result);  //@parm [out,retval] Data Definition UUID

  //***********************************************************
  // METHOD NAME: GetSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetSlotID (
    // @parm [out,retval] aafSlotID_t * | result | Slot id of the Mob Slot
    aafSlotID_t *  result
  );

  //***********************************************************
  // METHOD NAME: SetSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  SetSlotID (
    // @parm [in] aafSlotID_t | value | Slot id of the Mob Slot
    aafSlotID_t value
  );

public:
// Internal to the SDK, but available to other SDK internal code.
  virtual AAFRESULT FindSegment(aafPosition_t offset,
										  ImplAAFSegment **segment,
										  aafRational_t *srcRate,
										  aafPosition_t *diffPos);
  virtual AAFRESULT ConvertToEditRate(aafPosition_t tmpPos,
										aafRational_t destRate,
										aafPosition_t *convertPos);
  virtual AAFRESULT ConvertToMyRate(aafPosition_t tmpPos,
										  ImplAAFMobSlot *srcSlot,
										aafPosition_t *convertPos);
  virtual AAFRESULT ChangeContainedReferences(aafUID_t *from, aafUID_t *to);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFMobSlotTest.cpp.
  static AAFRESULT test();

protected:
	OMWideStringProperty				_name;
	OMFixedSizeProperty<aafUInt32>		_trackID;
	OMFixedSizeProperty<aafUInt32>		_physicalTrackNum;
	OMStrongReferenceProperty<ImplAAFSegment> _segment;
};

#endif // ! __ImplAAFMobSlot_h__


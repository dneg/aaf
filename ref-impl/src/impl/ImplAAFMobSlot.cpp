
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


#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif

#include <assert.h>
#include "AAFResult.h"
#include "aafErr.h"

ImplAAFMobSlot::ImplAAFMobSlot ()
: _name(			PID_MOBSLOT_NAME,			"Name"),
  _trackID(			PID_MOBSLOT_TRACKID,		"SlotID"),
  _physicalTrackNum(PID_MOBSLOT_PHYSICAL_TRACK,	"PhysicalTrack"),
  _segment(			PID_MOBSLOT_SEGMENT,		"Segment") 
{
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_trackID.address());
	_persistentProperties.put(_physicalTrackNum.address());
	_persistentProperties.put(_segment.address());
}


ImplAAFMobSlot::~ImplAAFMobSlot ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetSegment (ImplAAFSegment **result)
{
	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	*result = _segment;
	if (*result)
		(*result)->AcquireReference();

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::SetSegment (ImplAAFSegment *value)
{
	ImplAAFSegment *oldValue = _segment;
	if (oldValue)
		oldValue->ReleaseReference();

	_segment = value;

	if (value)
		value->AcquireReference();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetName
        (aafWChar *  pName,  //@parm [in] buffer provided by client to hold the Mob Slot Name
		aafInt32	size)	//@parm [in] length of the buffer provided to hold the slot name
{
	bool stat;

	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	stat = _name.copyToBuffer(pName, size);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}

  //****************
  // GetNameLength()
  //
AAFRESULT STDMETHODCALLTYPE
ImplAAFMobSlot::GetNameLength
		(aafInt32	*pSize)	//@parm [in] length of the buffer provided to hold the slot name
							// including the terminator
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pSize = _name.length()+1;

	return(AAFRESULT_SUCCESS); 
}
  
 AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::SetName (aafWChar *pName)
{
	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	_name = pName;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetPhysicalNum (aafUInt32 *result)
{
	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	*result = _physicalTrackNum;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::SetPhysicalNum (aafUInt32  number)
{
	_physicalTrackNum = number;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetDataDef (aafUID_t *result)
{
	ImplAAFSegment	*seg = _segment;
	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	aafAssert(seg != NULL, NULL, AAFRESULT_NULLOBJECT);
	return seg->GetDataDef(result);
}


AAFRESULT STDMETHODCALLTYPE
  ImplAAFMobSlot::GetSlotID (
    // @parm [out,retval] aafSlotID_t * | result | Slot id of the Mob Slot
    aafSlotID_t *  result
  )
{
	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	*result = _trackID;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
  ImplAAFMobSlot::SetSlotID (
    // @parm [in] aafSlotID_t | result | Slot id of the Mob Slot
    aafSlotID_t value
  )
{
	_trackID = value;
	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_AAFMobSlot;

OMDEFINE_STORABLE(ImplAAFMobSlot, CLSID_AAFMobSlot);


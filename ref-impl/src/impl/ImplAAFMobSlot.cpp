
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
  _trackID(			PID_MOBSLOT_TRACKID,		"TrackID"),
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
	*result = _segment;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::SetSegment (ImplAAFSegment *value)
{
	_segment = value;
	return AAFRESULT_SUCCESS;
}


static void stringPropertyToAAFString(aafString_t *aafString, OMStringProperty& stringProperty)
{
  const char* string = stringProperty;
  aafString->length = stringProperty.length();
  aafString->value = new aafWChar[aafString->length + 1];
  mbstowcs(aafString->value, string, aafString->length);
  aafString->value[aafString->length] = L'\0';
  //!!!Leaking string?
}


static void AAFStringToStringProperty(OMStringProperty& stringProperty, aafString_t *aafString)
{
	char* string;
	string = new char[aafString->length + 1];	//!!!S/b more than we need
	wcstombs(string, aafString->value, aafString->length);
	stringProperty = string;
  //!!!Leaking string?
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetName (aafString_t *name)
{
	aafAssert(name != NULL, NULL, AAFRESULT_NULL_PARAM);

	stringPropertyToAAFString(name, _name);

	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::SetName (aafString_t *name)
{
	aafAssert(name != NULL, NULL, AAFRESULT_NULL_PARAM);

	AAFStringToStringProperty(_name, name);

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetPhysicalNum (aafUInt32 *result)
{
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
	aafAssert(seg != NULL, NULL, AAFRESULT_NULLOBJECT);
	return seg->GetDataDef(result);
}


AAFRESULT STDMETHODCALLTYPE
  ImplAAFMobSlot::GetTrackID (
    // @parm [out,retval] aafTrackID_t * | result | Track id of the Mob Slot
    aafTrackID_t *  result
  )
{
	*result = _trackID;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
  ImplAAFMobSlot::SetTrackID (
    // @parm [in] aafTrackID_t | result | Track id of the Mob Slot
    aafTrackID_t value
  )
{
	_trackID = value;
	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_AAFMobSlot;

OMDEFINE_STORABLE(AAFMobSlot, CLSID_AAFMobSlot);


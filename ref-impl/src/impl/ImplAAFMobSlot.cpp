
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

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

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
  _origin(			PID_MOBSLOT_ORIGIN,			"Origin"),
  _trackID(			PID_MOBSLOT_TRACKID,		"TrackID"),
  _physicalTrackNum(PID_MOBSLOT_PHYSICAL_TRACK,	"PhysicalTrack"),
  _segment(			PID_MOBSLOT_SEGMENT,		"Segment")
{}


ImplAAFMobSlot::~ImplAAFMobSlot ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::IsATrack (aafBool *retval)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetSegment (ImplAAFSegment ** /*result*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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
    ImplAAFMobSlot::GetDataKind (ImplAAFDataDef ** /*result*/)
{
	//Call segment->GetDataKind.
  return AAFRESULT_NOT_IMPLEMENTED;
}




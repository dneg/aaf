
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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"
#include "aafCvt.h"
#include "aafErr.h"

ImplAAFMobSlot::ImplAAFMobSlot ()
: _name(			PID_MobSlot_SlotName,			L"SlotName"),
  _trackID(			PID_MobSlot_SlotID,		L"SlotID"),
  _physicalTrackNum(PID_MobSlot_PhysicalTrackNumber,	L"PhysicalTrackNumber"),
  _segment(			PID_MobSlot_Segment,		L"Segment") 
{
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_trackID.address());
	_persistentProperties.put(_physicalTrackNum.address());
	_persistentProperties.put(_segment.address());
}


ImplAAFMobSlot::~ImplAAFMobSlot ()
{
	ImplAAFSegment *segment = _segment.clearValue();
	if (segment)
	{
	  segment->ReleaseReference();
	  segment = 0;
	}
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetSegment (ImplAAFSegment **result)
{
	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(_segment.isVoid())
		return (AAFRESULT_NULLOBJECT);
	*result = _segment;
	(*result)->AcquireReference();

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::SetSegment (ImplAAFSegment *value)
{
	if( value == NULL )
	    return AAFRESULT_NULL_PARAM;

	if (_segment)
	{
	  if( _segment == value )
		return AAFRESULT_SUCCESS;
	  _segment->ReleaseReference();
	}

	if( value->attached() )
	    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_segment = value;
	_segment->AcquireReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::GetName
        (aafCharacter *  pName,  //@parm [in] buffer provided by client to hold the Mob Slot Name
		aafInt32	size)	//@parm [in] length of the buffer provided to hold the slot name
{
	bool stat;

	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_name.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	stat = _name.copyToBuffer(pName, size);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}

  //****************
  // GetNameBufLen()
  //
AAFRESULT STDMETHODCALLTYPE
ImplAAFMobSlot::GetNameBufLen
		(aafUInt32	*pSize)	//@parm [in] length of the buffer provided to hold the slot name
							// including the terminator
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_name.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pSize = _name.size();

	return(AAFRESULT_SUCCESS); 
}
  
 AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::SetName (const aafCharacter *pName)
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

	if(!_physicalTrackNum.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

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
    ImplAAFMobSlot::GetDataDef (ImplAAFDataDef ** result)
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



AAFRESULT ImplAAFMobSlot::FindSegment(aafPosition_t offset,
										  ImplAAFSegment **segment,
										  aafRational_t *srcRate,
										  aafPosition_t *diffPos)
{
	ImplAAFSegment	*tmpSegment = NULL;
	aafPosition_t begPos = CvtInt32toPosition(0, begPos);
	aafBool					foundClip = kAAFFalse;

	if(diffPos == NULL || segment == NULL || srcRate == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		/* Initialize return parameters */
		*diffPos = 0;
		srcRate->numerator = 0;
		srcRate->denominator = 1;
		*segment = NULL;
				
		CHECK(GetSegment(&tmpSegment));
				
		CHECK(tmpSegment->FindSubSegment(offset, &begPos, segment, &foundClip));
		if(!foundClip)
			RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);

		/* Calculate diffPos - difference between requested offset and
		* the beginning of clip that contains it. 
		*/
		*diffPos = offset;
		tmpSegment->ReleaseReference();
		tmpSegment = 0;
		
	} /* XPROTECT */
	XEXCEPT
	{
		if (tmpSegment)	
		  tmpSegment->ReleaseReference();
		tmpSegment = 0;
	}
	XEND;
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFMobSlot::ConvertToEditRate(aafPosition_t tmpPos,
										aafRational_t /*destRate*/,
										aafPosition_t *convertPos)
{
	if(convertPos == NULL )
		return(AAFRESULT_NULL_PARAM);
	*convertPos = tmpPos;		// if static (not time-based) slot, assume 1-1 mapping
	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFMobSlot::ConvertToMyRate(aafPosition_t tmpPos,
										  ImplAAFMobSlot * /*srcSlot*/,
										aafPosition_t *convertPos)
{
	if(convertPos == NULL )
		return(AAFRESULT_NULL_PARAM);
	*convertPos = tmpPos;		// if static (not time-based) slot, assume 1-1 mapping
	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFMobSlot::ChangeContainedReferences(aafMobID_constref from,
													aafMobID_constref to)
{
	ImplAAFSegment	*seg;
	
	seg = _segment;

	if(seg != NULL)
		seg->ChangeContainedReferences(from, to);

	return AAFRESULT_SUCCESS;
}


/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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
: _name(			PID_MobSlot_SlotName,			"SlotName"),
  _trackID(			PID_MobSlot_SlotID,		"SlotID"),
  _physicalTrackNum(PID_MobSlot_PhysicalTrackNumber,	"PhysicalTrackNumber"),
  _segment(			PID_MobSlot_Segment,		"Segment") 
{
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_trackID.address());
	_persistentProperties.put(_physicalTrackNum.address());
	_persistentProperties.put(_segment.address());
}


ImplAAFMobSlot::~ImplAAFMobSlot ()
{
	ImplAAFSegment *segment = _segment.setValue(0);
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
	*result = _segment;
	if (*result)
		(*result)->AcquireReference();
	else
		return (AAFRESULT_NULLOBJECT);

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMobSlot::SetSegment (ImplAAFSegment *value)
{
	if (_segment)
	  _segment->ReleaseReference();
	_segment = 0;

	_segment = value;

	if (value)
		value->AcquireReference();

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
	aafBool					foundClip = AAFFalse;

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
										aafRational_t destRate,
										aafPosition_t *convertPos)
{
	if(convertPos == NULL )
		return(AAFRESULT_NULL_PARAM);
	*convertPos = tmpPos;		// if static (not time-based) slot, assume 1-1 mapping
	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFMobSlot::ConvertToMyRate(aafPosition_t tmpPos,
										  ImplAAFMobSlot *srcSlot,
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

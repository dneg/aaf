/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFEssenceGroup_h__
#include "ImplAAFEssenceGroup.h"
#endif

#include <assert.h>
#include <string.h>
#include "ImplAAFSourceClip.h"
#include "aafErr.h"
#include "aafUtils.h"
#include "aafCvt.h"
#include "ImplAAFMobSlot.h"
#include "ImplAAFMob.h"
#include "ImplAAFMasterMob.h"

ImplAAFEssenceGroup::ImplAAFEssenceGroup ()
:   _choices(	PID_EssenceGroup_Choices,		"Choices"),
  _stillFrame(	PID_EssenceGroup_StillFrame,    "StillFrame")
{
	_persistentProperties.put(_choices.address());
	_persistentProperties.put(_stillFrame.address());
}


ImplAAFEssenceGroup::~ImplAAFEssenceGroup ()
{
	size_t size = _choices.getSize();
	for (size_t i = 0; i < size; i++) {
		ImplAAFSourceClip *pClip = _choices.setValueAt(0, i);

		if (pClip) {
			pClip->ReleaseReference();
		}
	}
	ImplAAFSourceClip *pClip = _stillFrame.setValue(0);
	if (pClip)
	{
		pClip->ReleaseReference();
	}
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::SetStillFrame (
      ImplAAFSourceClip *stillFrame)
{
	aafUID_t	newDataDef, groupDataDef;
	aafLength_t	oneLength, stillLength;
	
	if (stillFrame == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		/* Verify that groups's datakind converts to still's datakind */
		CHECK(stillFrame->GetDataDef(&newDataDef));
		CHECK(GetDataDef(&groupDataDef));
		if (!EqualAUID(&newDataDef, &groupDataDef))
			//!!!!stillDatakind->DoesDatakindConvertTo(datakindName, &aafError))
		{
			RAISE(AAFRESULT_INVALID_DATADEF);
		}
		
		/* Verify that length of still frame is 1 */
		CvtInt32toInt64(1, &oneLength);
		CHECK(stillFrame->GetLength (&stillLength));
		if (Int64NotEqual(oneLength, stillLength))
		{
			RAISE(AAFRESULT_STILLFRAME_BADLENGTH);
		}
		
		ImplAAFSourceClip *pOldClip = _stillFrame;
		if (pOldClip)
			pOldClip->ReleaseReference();
		
		_stillFrame = stillFrame;
		
		if (stillFrame)
			stillFrame->AcquireReference();
	}
	XEXCEPT
	{
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}

    //@comm Essence group choices should be added with the AddChoice() function.
    
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::AppendChoice (
      ImplAAFSourceClip *choice)
{
	aafUID_t	newDataDef, groupDataDef;
	aafLength_t	groupLength, newLength;
	
	if(choice == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	XPROTECT()
	{
		CHECK(choice->GetDataDef(&newDataDef));
		CHECK(GetDataDef(&groupDataDef));
		/* Verify that groups's datakind converts to still's datakind */
		if (!EqualAUID(&newDataDef, &groupDataDef))
			//!!!!stillDatakind->DoesDatakindConvertTo(datakindName, &aafError))
		{
			RAISE(AAFRESULT_INVALID_DATADEF);
		}
		
		/* Verify that length of choice matches length of group */
		CHECK(GetLength (&groupLength));
		CHECK(choice->GetLength (&newLength));
		if (Int64NotEqual(groupLength, newLength))
		{
			RAISE(AAFRESULT_BAD_LENGTH);
		}
		
		_choices.appendValue(choice);
		choice->AcquireReference();
	}
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::GetNumRepresentations (
      aafUInt32  *result)
{
	size_t	numClips;

	_choices.getSize(numClips);
	*result = numClips;

	return AAFRESULT_SUCCESS;
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::GetIndexedRepresentation (
      aafUInt32  index,
      ImplAAFSourceClip  **result)
{
	ImplAAFSourceClip	*obj;

	XPROTECT()
	{
		_choices.getValueAt(obj, index);
		if (obj)
			obj->AcquireReference();
		else
			RAISE(AAFRESULT_NO_MORE_OBJECTS); // AAFRESULT_BADINDEX !!!???
		*result = obj;
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFEssenceGroup::GetMinimumBounds(aafPosition_t rootPos, aafLength_t rootLen,
										ImplAAFMob *mob, ImplAAFMobSlot *track,
										aafMediaCriteria_t *mediaCrit,
										aafPosition_t currentObjPos,
										aafOperationChoice_t *effectChoice,
										ImplAAFComponent	*prevObject,
										ImplAAFComponent *nextObject,
										ImplAAFScopeStack *scopeStack,
										aafPosition_t *diffPos, aafLength_t *minLength,
										ImplAAFOperationGroup **effeObject, aafInt32	*nestDepth,
										ImplAAFComponent **foundObj, aafBool *foundTransition)
{
  aafMediaCriteria_t criteriaStruct, *ptrCriteriaStruct;
  ImplAAFSourceClip	*critClip = NULL;
  aafSlotID_t trackID;
  ImplAAFComponent	*tmpFound = NULL;
  aafLength_t tmpMinLen;

	XPROTECT()
	  {
	  CHECK(track->GetSlotID(&trackID));


//!!!		  criteriaStruct.proc = NULL;
		  if (mediaCrit)
			criteriaStruct = *mediaCrit; 
		  else
			criteriaStruct.type = kAAFAnyRepresentation;
		  ptrCriteriaStruct = &criteriaStruct;
		  CHECK(((ImplAAFMasterMob *)this)->GetCriteriaSourceClip(trackID,
											  ptrCriteriaStruct, &critClip));

		  if (critClip)
			{
			  CHECK(mob->MobFindLeaf(track, mediaCrit, effectChoice,
								(ImplAAFComponent *)critClip, rootPos, rootLen,
								prevObject, nextObject, 
								scopeStack, 
								currentObjPos, &tmpFound, &tmpMinLen, foundTransition,
								effeObject, nestDepth, diffPos));
			  if (tmpFound)
				{
				  *foundObj = tmpFound;
				  if (Int64Less(tmpMinLen, rootLen))
					*minLength = tmpMinLen;
				  else
					*minLength = rootLen;
				}
			  else
				{
				  RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);
				}
			}
			  }
	XEXCEPT
	  {
	  }
	XEND;

	return(AAFRESULT_SUCCESS);
}

OMDEFINE_STORABLE(ImplAAFEssenceGroup, AUID_AAFEssenceGroup);


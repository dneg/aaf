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

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include "ImplAAFObjectCreation.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFEssenceGroup_h__
#include "ImplAAFEssenceGroup.h"
#endif

#include <assert.h>
#include <string.h>
#include "ImplAAFSourceClip.h"
#include "aafErr.h"
#include "AAFUtils.h"
#include "aafCvt.h"
#include "ImplAAFMobSlot.h"
#include "ImplAAFMob.h"
#include "ImplAAFMasterMob.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFDataDef.h"
#include "ImplAAFEssenceAccess.h"
#include "ImplAAFSourceMob.h"

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef> ImplAAFDataDefSP;

extern "C" const aafClassID_t CLSID_AAFEssenceAccess;

ImplAAFEssenceGroup::ImplAAFEssenceGroup ()
:   _choices(	PID_EssenceGroup_Choices,		L"Choices"),
  _stillFrame(	PID_EssenceGroup_StillFrame,    L"StillFrame")
{
	_persistentProperties.put(_choices.address());
	_persistentProperties.put(_stillFrame.address());
}


ImplAAFEssenceGroup::~ImplAAFEssenceGroup ()
{
	size_t count = _choices.count();
	for (size_t i = 0; i < count; i++) {
		ImplAAFSegment *pClip = _choices.clearValueAt(i);

		if (pClip) {
		  pClip->ReleaseReference();
		  pClip = 0;
		}
	}
	ImplAAFSourceClip *pClip = _stillFrame.clearValue();
	if (pClip)
	{
	  pClip->ReleaseReference();
	  pClip = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::SetStillFrame (
      ImplAAFSourceClip *stillFrame)
{
	aafUID_t	newDataDef;
	aafLength_t	oneLength, stillLength;
	ImplAAFDictionary	*pDict = NULL;
	ImplAAFDataDef	*pDef = NULL;
	aafBool			willConvert;

	if (stillFrame == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		/* Verify that groups's datakind converts to still's datakind */
	    ImplAAFDataDefSP pNewDataDef;
		CHECK(stillFrame->GetDataDef(&pNewDataDef));
		CHECK(pNewDataDef->GetAUID(&newDataDef));

	    ImplAAFDataDefSP pGroupDataDef;
		CHECK(GetDataDef(&pGroupDataDef));

		CHECK(GetDictionary(&pDict));
		CHECK(pDict->LookupDataDef(newDataDef, &pDef));
		pDict->ReleaseReference();
		pDict = NULL;
		CHECK(pDef->DoesDataDefConvertTo(pGroupDataDef, &willConvert));
		pDef->ReleaseReference();
		pDef = NULL;

		if (willConvert == kAAFFalse)
			RAISE(AAFRESULT_INVALID_DATADEF);
		
		/* Verify that length of still frame is 1 */
		CvtInt32toInt64(1, &oneLength);
		CHECK(stillFrame->GetLength (&stillLength));
		if (Int64NotEqual(oneLength, stillLength))
		{
			RAISE(AAFRESULT_STILLFRAME_BADLENGTH);
		}
		if (_stillFrame.isPresent())
		{
			ImplAAFSourceClip *pOldClip = _stillFrame;
			if (pOldClip)
			{
			  if( pOldClip == stillFrame )
				RAISE( AAFRESULT_SUCCESS );

			  pOldClip->ReleaseReference();
			  pOldClip = 0;
			}
		}

		if( stillFrame->attached() )
			RAISE( AAFRESULT_OBJECT_ALREADY_ATTACHED );

		_stillFrame = stillFrame;
		
		if (stillFrame)
			stillFrame->AcquireReference();
	}
	XEXCEPT
	{
	  if (NULL != pDict)
	    pDict->ReleaseReference();
	  pDict = 0;
	  if (NULL != pDef)
	    pDef->ReleaseReference();
	  pDef = 0;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::GetStillFrame (
      ImplAAFSourceClip **stillFrame)
{
	if (stillFrame == NULL)
		return AAFRESULT_NULL_PARAM;
	
	if (!_stillFrame.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*stillFrame = _stillFrame;
		
	if (*stillFrame)
		(*stillFrame)->AcquireReference();
	
	return AAFRESULT_SUCCESS;
}

    //@comm Essence group choices should be added with the AddChoice() function.
    
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::AppendChoice (
      ImplAAFSegment *choice)
{
	if(choice == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(choice->attached())
		return(AAFRESULT_OBJECT_ALREADY_ATTACHED);

	AAFRESULT ar=ValidateChoice(choice);
	if(AAFRESULT_FAILED(ar))
		return(ar);

	_choices.appendValue(choice);
	choice->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::PrependChoice (
      ImplAAFSegment *choice)
{
	if(choice == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(choice->attached())
		return(AAFRESULT_OBJECT_ALREADY_ATTACHED);

	AAFRESULT ar=ValidateChoice(choice);
	if(AAFRESULT_FAILED(ar))
		return(ar);

	_choices.prependValue(choice);
	choice->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::InsertChoiceAt (
	  aafUInt32 index,
      ImplAAFSegment *choice)
{
  if(choice == NULL)
	return(AAFRESULT_NULL_PARAM);

  aafUInt32 count;
  AAFRESULT hr;
  hr = CountChoices (&count);
  if (AAFRESULT_FAILED (hr)) return hr;

  if (index > count)
	return(AAFRESULT_BADINDEX);

  AAFRESULT ar=ValidateChoice(choice);
  if(AAFRESULT_FAILED(ar))
	return(ar);

  if(choice->attached())
	return(AAFRESULT_OBJECT_ALREADY_ATTACHED);

  _choices.insertAt(choice,index);
  choice->AcquireReference();
	
  return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::CountChoices (
      aafUInt32  *result)
{
	size_t	numClips = _choices.count();
	*result = numClips;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::GetChoiceAt (
      aafUInt32  index,
      ImplAAFSegment  **result)
{
	ImplAAFSegment	*obj;

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


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceGroup::RemoveChoiceAt (
      aafUInt32  index)
{
  aafUInt32 count;
  AAFRESULT hr;
  ImplAAFSegment	*pSeg;
  
  hr = CountChoices (&count);
  if (AAFRESULT_FAILED (hr)) return hr;

  if (index > count)
	return AAFRESULT_BADINDEX;

	pSeg = _choices.removeAt(index);
	if(pSeg)
		pSeg->ReleaseReference();

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
  ImplAAFSegment	*critClip = NULL;
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
		  CHECK(((ImplAAFMasterMob *)this)->GetCriteriaSegment(trackID,
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

AAFRESULT ImplAAFEssenceGroup::GetCriteriaSegment(
			aafMediaCriteria_t *criteria,
			ImplAAFSegment		**retSrcClip)
{
	aafInt32				score, highestScore;
	aafUInt32				n, numReps;
	ImplAAFMob				*mob = NULL;
	ImplAAFSourceMob		*fileMob = NULL;
	ImplAAFSegment			*highestScoreSourceClip = NULL, *sourceClip = NULL;
	ImplAAFSourceClip		*sclp;
	aafSelectInfo_t			selectInfo;
	ImplAAFEssenceAccess	*access;

	aafAssert(criteria != NULL, file, AAFRESULT_NULL_PARAM);
	aafAssert(retSrcClip != NULL, file, AAFRESULT_NULL_PARAM);
	highestScore = 0;
	highestScoreSourceClip = NULL;
	*retSrcClip = NULL;
		
	XPROTECT()
	{
		CHECK(CountChoices(&numReps));
		for(n = 0; n < numReps; n++)
		{
			CHECK(GetChoiceAt(n, &sourceClip));
			if(numReps == 0)
			{
				highestScoreSourceClip = sourceClip;
				break;
			}
			sclp = dynamic_cast<ImplAAFSourceClip*>(sourceClip);
			if(sclp == 0)
				RAISE(AAFRESULT_INVALID_LINKAGE);

			CHECK(sclp->ResolveRef(&mob));
			fileMob = dynamic_cast<ImplAAFSourceMob*>(mob);
			if(fileMob == NULL)
				RAISE(AAFRESULT_INCONSISTANCY);

			access = (ImplAAFEssenceAccess *)CreateImpl (CLSID_AAFEssenceAccess);
			CHECK(access->GetSelectInfo (fileMob, &selectInfo))
				
			/* Check for locator file existance & continue if not present
			 * A file which is supposed to be an OMFI file must be opened
			 * to check for the existance of the data object, so we must
			 * open the file here.
			 */
//!!!			CHECK(LocateMediaFile(file, fileMob, &dataFile, &isOMFI));
//			if(dataFile == NULL)
//				continue;
//			if(dataFile != file)
//				omfsCloseFile(dataFile);

			score = 0;
			switch(criteria->type)
			{
			case kAAFAnyRepresentation:
				break;
				
			case kAAFFastestRepresentation:
				if(selectInfo.hwAssisted)
					score += 10;
				if(selectInfo.isNative)
					score += 10;
				break;
				
			case kAAFBestFidelityRepresentation:
				score = (100 - selectInfo.relativeLoss);
				break;
				
			case kAAFSmallestRepresentation:
				score = -1 * selectInfo.avgBitsPerSec;
				break;
			}
	
			if((score > highestScore) || (highestScoreSourceClip == NULL))
			{
				highestScore = score;
				highestScoreSourceClip = sourceClip;
			}
		}
	}
	XEXCEPT
	XEND
		
	*retSrcClip = highestScoreSourceClip;
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFEssenceGroup::ChangeContainedReferences(aafMobID_constref from,
													aafMobID_constref to)
{
	aafUInt32			n, count;
	ImplAAFSegment		*seg = NULL;
	
	XPROTECT()
	{
		CHECK(CountChoices (&count));
		for(n = 0; n < count; n++)
		{
			CHECK(GetChoiceAt (n, &seg));
			CHECK(seg->ChangeContainedReferences(from, to));
			seg->ReleaseReference();
			seg = NULL;
		}
	}
	XEXCEPT
	{
		if(seg != NULL)
		  seg->ReleaseReference();
		seg = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFEssenceGroup::ValidateChoice(
		ImplAAFSegment *choice)
{
	aafLength_t	groupLength, newLength;
	ImplAAFDictionary	*pDict = NULL;
	//ImplAAFDataDef	*pDef = NULL;
	aafBool			willConvert;

	if(choice == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
	    ImplAAFDataDefSP pNewDataDef;
		CHECK(choice->GetDataDef(&pNewDataDef));
		// CHECK(pNewDataDef->GetAUID(&newDataDef));

	    ImplAAFDataDefSP pGroupDataDef;
		CHECK(GetDataDef(&pGroupDataDef));

		/* Verify that groups's datakind converts to still's datakind */
		CHECK(GetDictionary(&pDict));
		// CHECK(pDict->LookupDataDef(newDataDef, &pDef));
		pDict->ReleaseReference();
		pDict = NULL;
		CHECK(pNewDataDef->DoesDataDefConvertTo(pGroupDataDef, &willConvert));
		// pDef->ReleaseReference();
		// pDef = NULL;

		if (willConvert == kAAFFalse)
			RAISE(AAFRESULT_INVALID_DATADEF);
		
		/* Verify that length of choice matches length of group */
		CHECK(GetLength (&groupLength));
		CHECK(choice->GetLength (&newLength));
		if (Int64NotEqual(groupLength, newLength))
		{
			RAISE(AAFRESULT_BAD_LENGTH);
		}
		
	}
	XEXCEPT
	{
		if(pDict != NULL)
		  pDict->ReleaseReference();
		pDict = 0;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

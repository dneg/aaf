/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFEssenceDescriptor_h__
#include "ImplAAFEssenceDescriptor.h"
#endif

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplAAFFindSourceInfo_h__
#include "ImplAAFFindSourceInfo.h"
#endif

#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"


#ifndef __ImplAAFSourceMob_h__
#include "ImplAAFSourceMob.h"
#endif

#include "ImplAAFDictionary.h"

#include <assert.h>
#include "ImplAAFEssenceDescriptor.h"
#include "ImplAAFSourceClip.h"
#include "aafCvt.h"
#include "aafErr.h"
#include "AAFResult.h"
#include "ImplAAFDictionary.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFUtils.h"
#include "ImplEnumAAFMobSlots.h"
#include "ImplAAFSequence.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFTimecode.h"
#include "ImplAAFFileDescriptor.h"
#include "ImplAAFFiller.h"
#include "ImplAAFEdgecode.h"
#include "ImplAAFPulldown.h"

ImplAAFSourceMob::ImplAAFSourceMob ()
: _essenceDesc(        PID_SourceMob_EssenceDescription,          "EssenceDescription")
{
  _persistentProperties.put(_essenceDesc.address());
}

ImplAAFSourceMob::~ImplAAFSourceMob ()
{
	ImplAAFEssenceDescriptor *essenceDesc = _essenceDesc.setValue(0);
	if (essenceDesc)
	{
		essenceDesc->ReleaseReference();
	}
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFSourceMob::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


//****************
// GetEssenceDescriptor()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::GetEssenceDescriptor (ImplAAFEssenceDescriptor **ppEdes)
{
	if (ppEdes == NULL)
		return AAFRESULT_NULL_PARAM;

	*ppEdes = _essenceDesc;
	// !!!Handle case where essence descriptor may not
	// exist, return  AAFRESULT_NO_ESSENCE_DESC.

  if (*ppEdes)
    (*ppEdes)->AcquireReference();

	return AAFRESULT_SUCCESS;
}



//****************
// SetEssenceDescriptor()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::SetEssenceDescriptor (ImplAAFEssenceDescriptor *pEdes)
{
	if (pEdes == NULL)
		return AAFRESULT_NULL_PARAM;

	ImplAAFEssenceDescriptor *pOldEdes = _essenceDesc;
	if (pOldEdes)
		pOldEdes->ReleaseReference();

	_essenceDesc = pEdes;
	
	if (pEdes)
		pEdes->AcquireReference();

	return AAFRESULT_SUCCESS;
}


//****************
// AddNilReference()
//
AAFRESULT STDMETHODCALLTYPE
   ImplAAFSourceMob::AddNilReference (aafSlotID_t slotID,
                           aafLength_t  length,
                           aafUID_t *dataDef,
                           aafRational_t  editRate)
{
	ImplAAFSourceClip *		sub = NULL;
 	aafPosition_t	zeroPos = CvtInt32toPosition(0, zeroPos);
	aafLength_t		zeroLen = CvtInt32toLength(0, zeroLen);
	aafSourceRef_t	sourceRef;
	ImplAAFTimelineMobSlot *	newSlot = NULL;		// Need version for non-timeline slots!!!
	ImplAAFDictionary *pDictionary = NULL;
	
	if (editRate.denominator == 0)
		return AAFRESULT_BADRATE;
	
	XPROTECT()
	{
 		sourceRef.sourceID = NilMOBID;
		sourceRef.sourceSlotID = 0;
		CvtInt32toPosition(0, sourceRef.startTime);
		CHECK(GetDictionary(&pDictionary));
		sub = (ImplAAFSourceClip *)pDictionary->CreateImplObject(AUID_AAFSourceClip);
		if(sub == NULL)
			RAISE(E_FAIL);
		pDictionary->ReleaseReference();
		pDictionary = NULL;
		CHECK(sub->Initialize (dataDef, &length, sourceRef));
		CHECK(AppendNewTimelineSlot(editRate, sub, slotID, L"Test", zeroPos, 
												&newSlot));
		newSlot->ReleaseReference();
		sub->ReleaseReference();
	}
	XEXCEPT
	{
		if(sub != NULL)
			sub->ReleaseReference();
		if(newSlot != NULL)
			newSlot->ReleaseReference();
		if(pDictionary != NULL)
			pDictionary->ReleaseReference();
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}



//****************
// AddTimecodeClip()
//
AAFRESULT STDMETHODCALLTYPE
   ImplAAFSourceMob::AppendTimecodeSlot (aafRational_t editrate,
                           aafInt32 slotID,
						   aafTimecode_t startTC,
                           aafFrameLength_t length32)
{
	ImplAAFTimecode *tccp = NULL;
	ImplAAFSequence	*aSequ = NULL;
	aafInt32         zeroL = 0L;
	aafFrameLength_t maxLength;
	aafUID_t		 timecodeKind = DDEF_Timecode;
	AAFRESULT			aafError = AAFRESULT_SUCCESS;
	aafPosition_t	zeroPos;
	aafLength_t		length, zeroLen;
	ImplAAFTimelineMobSlot *	newSlot = NULL, *mobSlot = NULL;
	aafBool			fullLength = AAFFalse;
	ImplAAFDictionary *pDictionary = NULL;

	//!!!Validate tape mobs only, return AAFRESULT_TAPE_DESC_ONLY
	if(length32 == FULL_LENGTH)
	  {
		 fullLength = AAFTrue;
		 length32 = 1;
	  }
	else
	  fullLength = AAFFalse;
	
	CvtInt32toPosition(0, zeroPos);
	CvtInt32toLength(0, zeroLen);
 	CvtInt32toLength(length32, length);

	XPROTECT()
	{
		CHECK(GetDictionary(&pDictionary));
		tccp = (ImplAAFTimecode *)pDictionary->CreateImplObject(AUID_AAFTimecode);
		if(NULL == tccp)
			RAISE(E_FAIL);

		tccp->Initialize(length, &startTC);		 
 		if (FindSlotBySlotID(slotID, (ImplAAFMobSlot **)&mobSlot) == AAFRESULT_SUCCESS)
		{
			aSequ = (ImplAAFSequence *)pDictionary->CreateImplObject(AUID_AAFSequence);
			if(aSequ == NULL)
				RAISE(E_FAIL);
			CHECK(aSequ->Initialize(&timecodeKind));
			CHECK(aSequ->AppendComponent(tccp));
			CHECK(mobSlot->SetSegment(aSequ));

		} /* FindTimecodeSlot */
		else
		{
			aSequ = (ImplAAFSequence *)pDictionary->CreateImplObject(AUID_AAFSequence);
			if(aSequ == NULL)
				RAISE(E_FAIL);
			CHECK(aSequ->Initialize(&timecodeKind));
			CHECK(aSequ->AppendComponent(tccp));
			CHECK(AppendNewTimelineSlot(editrate, aSequ, slotID,
										L"Timecode", zeroPos, &newSlot));
		}

		if(fullLength)
		{
			CHECK(PvtTimecodeToOffset(startTC.fps, 24, 0, 0, 0, 
											 startTC.drop, &maxLength));
			{
				CvtInt32toLength(maxLength, length);
				CHECK(tccp->SetLength(&length) );
				/* NOTE: What if the sequence already existed? */
				CHECK(aSequ->SetLength(&length) );
			}
		}
		if(newSlot != NULL)
			newSlot->ReleaseReference();
		if(aSequ != NULL)
			aSequ->ReleaseReference();
		if(tccp != NULL)
			tccp->ReleaseReference();

		pDictionary->ReleaseReference();
		pDictionary = NULL;
	} /* XPROTECT */
	XEXCEPT
	{
		if(newSlot != NULL)
			newSlot->ReleaseReference();
		if(aSequ != NULL)
			aSequ->ReleaseReference();
		if(tccp != NULL)
			tccp->ReleaseReference();
		if(pDictionary != NULL)
			pDictionary->ReleaseReference();
	}
	XEND;
									
	return (AAFRESULT_SUCCESS);
}



//****************
// AddEdgecodeClip()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::AppendEdgecodeSlot (aafRational_t  editrate,
                           aafInt32  slotID,
                          aafFrameOffset_t  startEC,
                           aafFrameLength_t  length32,
                           aafFilmType_t  filmKind,
                           aafEdgeType_t  codeFormat,
                           aafEdgecodeHeader_t  header)
{
	ImplAAFFiller *     filler1= NULL, *filler2 = NULL;
	ImplAAFSequence *ecSequence;
	ImplAAFEdgecode *edgecodeClip;
	AAFRESULT			aafError = AAFRESULT_SUCCESS;
	aafPosition_t	startPos, zeroPos;
	aafLength_t		length, zeroLen;
	ImplAAFTimelineMobSlot *	newSlot;
	aafEdgecode_t	edge;
	aafUID_t		edgekind = DDEF_Edgecode;
	ImplAAFDictionary *pDictionary = NULL;

	// Validate film mobs only, return AAFRESULT_FILM_DESC_ONLY
	CvtInt32toPosition(0, zeroPos);
	CvtInt32toLength(0, zeroLen);
	XPROTECT()
	{
		CHECK(GetDictionary(&pDictionary));
		filler1 = (ImplAAFFiller *)pDictionary->CreateImplObject(AUID_AAFFiller);
		if(filler1 == NULL)
			RAISE(E_FAIL);
		CHECK(filler1->Initialize(&edgekind, zeroLen));	
		filler2 = (ImplAAFFiller *)pDictionary->CreateImplObject(AUID_AAFFiller);
		if(filler2 == NULL)
			RAISE(E_FAIL);
		CHECK(filler2->Initialize(&edgekind, zeroLen));	

		ecSequence = (ImplAAFSequence *)pDictionary->CreateImplObject(AUID_AAFSequence);
		if(ecSequence == NULL)
			RAISE(E_FAIL);
		CHECK(ecSequence->Initialize(&edgekind));	

		CvtInt32toLength(length32, length);
		CvtInt32toPosition(startEC, startPos);
		edge.startFrame = startPos;
		edge.filmKind = filmKind;
		edge.codeFormat = codeFormat;
		strncpy((char *)&edge.header, (char *)&header, 8);
		
		edgecodeClip = (ImplAAFEdgecode *)pDictionary->CreateImplObject(AUID_AAFEdgecode);
		if(edgecodeClip == NULL)
			RAISE(E_FAIL);
		CHECK(edgecodeClip->Create(length, edge));	
		
		CHECK(ecSequence->AppendComponent(filler1));
		CHECK(ecSequence->AppendComponent(edgecodeClip));
		CHECK(ecSequence->AppendComponent(filler2));
		CHECK(AppendNewTimelineSlot(editrate, ecSequence, slotID,
									NULL, zeroPos, &newSlot));

		if(filler1 != NULL)
			filler1->ReleaseReference();
		if(filler2 != NULL)
			filler2->ReleaseReference();
		pDictionary->ReleaseReference();
		pDictionary = NULL;
	} /* XPROTECT */
	XEXCEPT
	{
		if(filler1 != NULL)
			filler1->ReleaseReference();
		if(filler2 != NULL)
			filler2->ReleaseReference();
		if(pDictionary != NULL)
			pDictionary->ReleaseReference();
	}
	XEND;

	return (AAFRESULT_SUCCESS);
}




//****************
// ValidateTimecodeRange()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::SpecifyValidCodeRange (ImplAAFDataDef *pEssenceKind,
                           aafSlotID_t  slotID,
                           aafRational_t  editrate,
                           aafFrameOffset_t  startOffset,
                           aafFrameLength_t  length32)
{
	ImplAAFSourceClip		*sclp = NULL;
	ImplAAFTimecode			*timecodeClip = NULL;
	ImplAAFSequence *		aSequ = NULL, *segSequ = NULL;
	ImplAAFFiller *			filler1 = NULL, *filler2 = NULL;
	ImplAAFSegment *		seg = NULL;
	ImplAAFComponent *		subSegment = NULL;
	ImplAAFDictionary *		pDict = NULL;
	aafPosition_t			pos, zeroPos, sequPos, begPos, endPos;
	aafLength_t				length, zeroLen, tcLen, endFillLen, firstFillLen, oldFillLen, segLen;
	aafLength_t				tcSlotLen, sequLen;
  	aafSourceRef_t			sourceRef;
	ImplAAFTimelineMobSlot	*newSlot, *slot;
  	aafFrameOffset_t		tcStartPos;
  	aafTimecode_t			timecode;
  	aafInt32				sequLoop, numSegs;
    ImplEnumAAFComponents	*sequIter = NULL;

	XPROTECT()
	{
		CHECK(FindTimecodeClip(startOffset, &timecodeClip, &tcStartPos,
								&tcSlotLen));
		CHECK(timecodeClip->GetLength(&tcLen));
		CHECK(timecodeClip->GetTimecode(&timecode));
		if(length32 == FULL_LENGTH)
		{
			CHECK(PvtTimecodeToOffset(timecode.fps, 24, 0, 0, 0,
			  						timecode.drop, &length32));
		}
		CvtInt32toLength(length32, length);
		CvtInt32toLength(0, zeroLen);
		
		CvtInt32toPosition(startOffset, pos);
		CvtInt32toPosition(0, zeroPos);
		endFillLen = tcSlotLen;
		CHECK(SubInt64fromInt64(pos, &endFillLen));
		CHECK(SubInt64fromInt64(length, &endFillLen));
		memset(&sourceRef, sizeof(aafUID_t), 0);
		sourceRef.sourceSlotID = 0;
		CvtInt32toPosition(0, sourceRef.startTime);
		CHECK(GetDictionary(&pDict));
		CHECK(pDict->CreateInstance(&AUID_AAFSourceClip, (ImplAAFObject **)&sclp));

		if(FindSlotBySlotID(slotID, (ImplAAFMobSlot **)&slot) != AAFRESULT_SUCCESS)
		{
			CHECK(pDict->CreateInstance(&AUID_AAFSequence, (ImplAAFObject **)&aSequ));
			CHECK(pDict->CreateInstance(&AUID_AAFFiller, (ImplAAFObject **)&filler1));
			if(aSequ == NULL || filler1 == NULL)
				RAISE(E_FAIL);
			CHECK(aSequ->AppendComponent(filler1));
			CHECK(aSequ->AppendComponent(sclp));
			CHECK(pDict->CreateInstance(&AUID_AAFFiller, (ImplAAFObject **)&filler2));
			CHECK(aSequ->AppendComponent(filler2));

			/* (SPR#343) Change to validate multiple ranges */
			CHECK(AppendNewTimelineSlot(editrate, aSequ, slotID,
												NULL, zeroPos, &newSlot));
		}
	  else
	  	{
			CHECK(slot->GetSegment(&seg));
			CHECK(seg->GenerateSequence(&segSequ));
  			CHECK(segSequ->EnumComponents(&sequIter));
  			CHECK(segSequ->GetNumComponents(&numSegs));
			for (sequLoop=0, sequPos = 0; sequLoop < numSegs; sequLoop++, sequPos += segLen)
			{
				CHECK(sequIter->NextOne(&subSegment));
				CHECK(subSegment->GetLength(&segLen));
				/* Skip zero-length clips, sometimes found in MC files */
				if (Int64Equal(segLen, zeroLen))
					continue;
				begPos = sequPos;
				endPos = sequPos;
				CHECK(AddInt64toInt64(segLen, &endPos));
				if (Int64Less(pos, endPos) &&
					Int64LessEqual(begPos, pos))
				{
//!!!					if(subSegment->IsTypeOf("FILL", &aafError) &&
//		 				(sequLoop == (numSegs-1)))
		 			{
						firstFillLen = pos;
						CHECK(SubInt64fromInt64(sequPos, &firstFillLen));
						CHECK(subSegment->GetLength(&oldFillLen));
						endFillLen = oldFillLen;
						CHECK(SubInt64fromInt64(length, &endFillLen));
						CHECK(SubInt64fromInt64(firstFillLen, &endFillLen));
						/****/
						CHECK(subSegment->SetLength(&firstFillLen));
						/* 1.x does not have a Sequence Length property */
						CHECK(segSequ->GetLength(&sequLen));
						SubInt64fromInt64(oldFillLen, &sequLen);
						AddInt64toInt64(firstFillLen, &sequLen);
						CHECK(segSequ->SetLength(&sequLen));

						CHECK(pDict->CreateInstance(&AUID_AAFFiller, (ImplAAFObject **)&filler2));
//!!!						filler2 = CreateImpl(CLSID_AAFFiller(_file, mediaKind, endFillLen);	
						CHECK(segSequ->AppendComponent(sclp));
						CHECK(segSequ->AppendComponent(filler2));
						break;
					}
//!!!					else
//						RAISE(AAFRESULT_NOT_IMPLEMENTED);
				}
			} /* for */
			sequIter->ReleaseReference();
			sequIter = NULL;
			
//!!!			/* Release reference, so the useCount is decremented */
//			if (subSegment)
//			  {
//				 subSegment->ReleaseObject();	
//				 subSegment = NULL;
//			  }
		}
		if(aSequ!= NULL)
		{
			aSequ->ReleaseReference();
			aSequ = NULL;
		}
		if(segSequ!= NULL)
		{
			segSequ->ReleaseReference();
			segSequ = NULL;
		}
		if(filler1!= NULL)
		{
			filler1->ReleaseReference();
			filler1 = NULL;
		}
		if(filler2!= NULL)
		{
			filler2->ReleaseReference();
			filler2 = NULL;
		}
		if(seg!= NULL)
		{
			seg->ReleaseReference();
			seg = NULL;
		}
		if(subSegment!= NULL)
		{
			subSegment->ReleaseReference();
			subSegment = NULL;
		}
		if(pDict!= NULL)
		{
			pDict->ReleaseReference();
			pDict = NULL;
		}
		if(sclp!= NULL)
		{
			sclp->ReleaseReference();
			sclp = NULL;
		}
		if(timecodeClip!= NULL)
		{
			timecodeClip->ReleaseReference();
			timecodeClip = NULL;
		}
	}
	XEXCEPT
	{
		if(aSequ!= NULL)
			aSequ->ReleaseReference();
		if(segSequ!= NULL)
			segSequ->ReleaseReference();
		if(filler1!= NULL)
			filler1->ReleaseReference();
		if(filler2!= NULL)
			filler2->ReleaseReference();
		if(seg!= NULL)
			seg->ReleaseReference();
		if(subSegment!= NULL)
			subSegment->ReleaseReference();
		if(pDict!= NULL)
			pDict->ReleaseReference();
		if(sclp!= NULL)
			sclp->ReleaseReference();
		if(timecodeClip!= NULL)
			timecodeClip->ReleaseReference();
	}
	XEND;

	return (AAFRESULT_SUCCESS);
}



//****************
// NewPhysSourceRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::NewPhysSourceRef (aafRational_t  editrate,
                           aafSlotID_t  aMobSlot,
                           aafUID_t *pEssenceKind,
							aafSourceRef_t  ref,
                           aafLength_t  srcRefLength)
{
	return(ImplAAFMob::AddPhysSourceRef(kAAFForceOverwrite, editrate, aMobSlot,
							pEssenceKind, ref, srcRefLength));
}

//****************
// AppendPhysSourceRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::AppendPhysSourceRef (aafRational_t  editrate,
                           aafSlotID_t  aMobSlot,
                           aafUID_t *pEssenceKind,
							aafSourceRef_t  ref,
                           aafLength_t  srcRefLength)
{
	return(ImplAAFMob::AddPhysSourceRef(kAAFAppend, editrate, aMobSlot,
							pEssenceKind, ref, srcRefLength));
}



//****************
// AddPulldownRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::AddPulldownRef (aafAppendOption_t  addType,
							aafRational_t  editrate,
                           aafSlotID_t  aMobSlot,
                           aafUID_t * pEssenceKind,
							aafSourceRef_t  ref,
                           aafLength_t  srcRefLength,
                           aafPulldownKind_t  pulldownKind,
                           aafPhaseFrame_t  phaseFrame,
                           aafPulldownDir_t  direction)
{
	ImplAAFSourceClip	*sclp = NULL;
	ImplAAFTimelineMobSlot		*trkd = NULL;
	aafPosition_t		zeroPos;
	aafSlotID_t 		tmpSlotID;
	ImplAAFMobSlot *	slot = NULL;
	aafBool 			isOneToOne;
	ImplAAFMobSlot * 	maskSlot = NULL;
	ImplAAFPulldown 	*pdwn = NULL;
	ImplAAFSequence *	sequence = NULL;
	ImplAAFDictionary*	dict = NULL;
	aafLength_t 		outLength, zero;
	aafInt32 			patternLen;
	AAFRESULT			status = AAFRESULT_SUCCESS;
	aafUInt32 			mask;
		
	XPROTECT()
	{
		GetDictionary(&dict);
		CvtInt32toInt64(0, &zero);
		XASSERT(direction == kAAFFilmToTapeSpeed || direction == kAAFTapeToFilmSpeed,
				AAFRESULT_PULLDOWN_DIRECTION);

		CvtInt32toPosition(0, zeroPos);
		{
			CHECK(dict->CreateInstance(&AUID_AAFPulldown, (ImplAAFObject **)&pdwn));
			CHECK(pdwn->SetDataDef(pEssenceKind));
			CHECK(pdwn->SetPulldownKind(pulldownKind));
			CHECK(pdwn->SetPhaseFrame(phaseFrame));
			CHECK(pdwn->SetPulldownDirection(direction));
			CHECK(pdwn->aafPvtGetPulldownMask(pulldownKind,
										&mask,  &patternLen, &isOneToOne));

			if(isOneToOne)
			{
				CHECK(pdwn->SetLength(&srcRefLength));
			}
			else
			{
				/* Remember, this routine is given the OUTPUT length, and must determine
				 * the input length (so the ratios look backwards)
				 */
				CHECK(pdwn->MapOffset(srcRefLength, AAFTrue, &outLength, NULL));
				CHECK(pdwn->SetLength(&outLength));
			}
		}
			
		/* If the slot exists, and there is a SCLP, extract it so that it can be appended
		 * to the mask or pullown object later
		 */
		status = FindSlotBySlotID(aMobSlot, &slot);
		if (status == AAFRESULT_SUCCESS)
		{
			ImplAAFSegment *seg;
			
			CHECK(slot->GetSlotID(&tmpSlotID));
			CHECK(slot->GetSegment(&seg));
			sequence = dynamic_cast<ImplAAFSequence*>(seg);
			if(sequence != NULL)
			{
				aafLength_t			foundLen;
				aafInt32			numSegments, n;
				ImplAAFComponent 	*subSeg;
				
				CHECK(sequence->GetNumComponents(&numSegments));
				if(numSegments == 0)
				{
					CHECK(sequence->AppendComponent(pdwn));
					CHECK(dict->CreateInstance(&AUID_AAFSourceClip, (ImplAAFObject **)&sclp));
					CHECK(sclp->Initialize(pEssenceKind, &srcRefLength, ref));
				}
				for(n = 0; n < numSegments; n++)
				{
					CHECK(sequence->GetNthComponent (0, &subSeg));
					CHECK(subSeg->GetLength(&foundLen));

					if(Int64NotEqual(foundLen, zero))
					{
						CHECK(sequence->SetNthComponent(n, pdwn));
						sclp = dynamic_cast<ImplAAFSourceClip*>(subSeg);
						break;
					}
					subSeg->ReleaseReference();
					subSeg = NULL;
				}
			}
			else
			{
				CHECK(slot->SetSegment(pdwn));
				sclp = dynamic_cast<ImplAAFSourceClip*>(seg);
			}
			
			XASSERT(sclp != NULL, AAFRESULT_NOT_SOURCE_CLIP);
			CHECK(sclp->Initialize(pEssenceKind, &srcRefLength, ref));
		}
		else
		{
			CHECK(dict->CreateInstance(&AUID_AAFSourceClip, (ImplAAFObject **)&sclp));
			CHECK(sclp->Initialize(pEssenceKind, &srcRefLength, ref));
			CHECK(AppendNewTimelineSlot(editrate, pdwn,
								aMobSlot, NULL, zeroPos, &trkd) );
		}

		/* Patch the MASK into the file mob if this is a Film Editrate */
		/* NOTE: This is assuming that there is a source clip on
		 * the file mob - if it is a nested structure (i.e., SEQU),
		 * this code is not patching the nested elements with the new
		 * editrate and length.
		 */
		{
			if(pdwn != NULL)
			{
				CHECK(pdwn->SetInputSegment(sclp));
			}
		}
		dict->ReleaseReference();
		dict = NULL;
	} /* XPROTECT */
	XEXCEPT
	{
		if(dict)
			dict->ReleaseReference();
	}
	XEND;

	return (AAFRESULT_SUCCESS);
}



//****************
// SearchSource()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::SearchSource (aafSlotID_t slotID,
                           aafPosition_t  offset,
                           aafMobKind_t  mobKind,
                           aafMediaCriteria_t *pMediaCrit,
                           aafOperationChoice_t *pOperationChoice,
                           ImplAAFFindSourceInfo **ppSourceInfo)
{
	return(InternalSearchSource(slotID, offset, mobKind, pMediaCrit, pOperationChoice,
										   ppSourceInfo));
}



//****************
// GetMobKind()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::GetMobKind (aafMobKind_t *pMobKind)
{
	ImplAAFEssenceDescriptor	*edesc = NULL;
	
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		CHECK(GetEssenceDescriptor (&edesc));
		if(edesc != NULL)
		{
			CHECK(edesc->GetOwningMobKind(pMobKind));
			edesc->ReleaseReference();
		}
		else
			*pMobKind = kAllMob;
	}
	XEXCEPT
	{
		if(edesc != NULL)
			edesc->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

//************************
// Function:  FindTimecodeClip	(INTERNAL)
// Finds the primary timecode track.  This will have a physical slot # of 0 (missing) or 1.
//
AAFRESULT ImplAAFSourceMob::FindTimecodeClip(
				aafFrameOffset_t	position,
				ImplAAFTimecode 		**result,
				aafFrameOffset_t	*tcStartPos,
				aafLength_t			*tcSlotLen)
{
	ImplAAFSegment *		seg = NULL;
	aafPosition_t			offset;
	AAFRESULT				status = AAFRESULT_SUCCESS;
	aafLength_t				zeroLen;
	aafPosition_t			sequPos;
	ImplAAFMobSlot			*slot = NULL;
 	ImplEnumAAFMobSlots		*slotIter = NULL;
  	aafBool					found = AAFFalse;
	aafUInt32				phys;
	aafUID_t				dataDef;

	XPROTECT()
	{
		CvtInt32toInt64(position, &offset);
		CvtInt32toInt64(0, &zeroLen);
		*tcStartPos = 0;
		*result = NULL;
		CHECK(EnumAAFAllMobSlots (&slotIter));
		while((found != AAFTrue) && slotIter->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->GetDataDef (&dataDef));
			if(EqualAUID(&dataDef, &DDEF_Timecode))
			{
				CHECK(slot->GetPhysicalNum (&phys));
				if((phys == 0) || (phys == 1))
					found = AAFTrue;
			}
		}
		if(found != AAFTrue)
			RAISE(AAFRESULT_MISSING_TRACKID);
		CHECK(slot->GetSegment(&seg));
		CHECK(seg->GetLength(tcSlotLen));
		CHECK(seg->OffsetToTimecodeClip(offset, result, &sequPos));
		*tcStartPos = sequPos;
		slot->ReleaseReference();
		slot = NULL;
		seg->ReleaseReference();
		seg = NULL;
		slotIter->ReleaseReference();
		slotIter = NULL;
	} /* XPROTECT */
	XEXCEPT
	{
		if(XCODE() == AAFRESULT_NO_MORE_OBJECTS)
			RERAISE(AAFRESULT_NO_TIMECODE);
		if(slotIter!= NULL)
			slotIter->ReleaseReference();
		if(slot != NULL)
			slot->ReleaseReference();
		if(seg != NULL)
			seg->ReleaseReference();
		*result = NULL;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

/************************
 * Function: omfsReconcileMobLength (INTERNAL)
 *
 * 	Given a master mob or file mob, make sure that all fields
 *		which contain the length of the mob are in agreement.  Currently
 *		only makes sure that mob length references are >= each of
 *		the track lengths.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
AAFRESULT ImplAAFSourceMob::ReconcileMobLength(void)
{
	aafInt32					numSlots, loop;
	aafLength_t					len;
	ImplAAFMobSlot				*slot = NULL;
	ImplAAFTimelineMobSlot		*timelineSlot = NULL;
	ImplAAFSegment				*seg = NULL;
	ImplEnumAAFMobSlots			*slotIter = NULL;
	aafRational_t				srcRate, destRate;
	ImplAAFEssenceDescriptor	*edesc = NULL;
	ImplAAFFileDescriptor		*physMedia = NULL;
		
	XPROTECT()
	{
		CHECK(GetEssenceDescriptor(&edesc));
		physMedia = dynamic_cast<ImplAAFFileDescriptor*>(edesc);
		if(physMedia != NULL)
		{
			CHECK(EnumAAFAllMobSlots (&slotIter));
			CHECK(GetNumSlots(&numSlots));
			for (loop = 1; loop <= numSlots; loop++)
			{
				CHECK(slotIter->NextOne((ImplAAFMobSlot **)&slot));
				timelineSlot = dynamic_cast<ImplAAFTimelineMobSlot*>(slot);
				if(timelineSlot != NULL)
				{
					CHECK(timelineSlot->GetSegment(&seg));
					CHECK(timelineSlot->GetEditRate(&destRate));
					timelineSlot->ReleaseReference();
					timelineSlot = NULL;
					CHECK(physMedia->GetLength(&len));
					CHECK(physMedia->GetSampleRate(&srcRate));
					physMedia->ReleaseReference();
					physMedia = NULL;
					
					if((srcRate.numerator != destRate.numerator) ||
						(srcRate.denominator != destRate.denominator))
					{
						CHECK(AAFConvertEditRate(	srcRate, len, destRate, kRoundFloor, &len));
					}
					CHECK(seg->SetLength(&len));
					seg->ReleaseReference();
					seg = NULL;
				}
				else
				{
					slot->ReleaseReference();
					slot = NULL;
				}
			}			
			slotIter->ReleaseReference();
			slotIter = NULL;
		}
	}
	XEXCEPT
	{
		if (slot)
			slot->ReleaseReference();
		if (timelineSlot)
			timelineSlot->ReleaseReference();
		if (physMedia)
			physMedia->ReleaseReference();
		if (seg)
			seg->ReleaseReference();
		if (slotIter)
			slotIter->ReleaseReference();
	}
	XEND
		
	return (AAFRESULT_SUCCESS);
}


OMDEFINE_STORABLE(ImplAAFSourceMob, AUID_AAFSourceMob);


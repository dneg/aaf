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

#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef>    ImplAAFDataDefSP;


ImplAAFSourceMob::ImplAAFSourceMob ()
: _essenceDesc(        PID_SourceMob_EssenceDescription,          L"EssenceDescription")
{
  _persistentProperties.put(_essenceDesc.address());
}

ImplAAFSourceMob::~ImplAAFSourceMob ()
{
	ImplAAFEssenceDescriptor *essenceDesc = _essenceDesc.clearValue();
	if (essenceDesc)
	{
	  essenceDesc->ReleaseReference();
	  essenceDesc = 0;
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

	if (pEdes->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	ImplAAFEssenceDescriptor *pOldEdes = _essenceDesc;
	if (pOldEdes)
	  pOldEdes->ReleaseReference();
	pOldEdes = 0;

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
                           const aafLength_t & length,
                           ImplAAFDataDef * pDataDef,
                           const aafRational_t & editRate)
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
    memset(&sourceRef, 0, sizeof(sourceRef));
		CvtInt32toPosition(0, sourceRef.startTime);
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdSourceClip()->
			  CreateInstance ((ImplAAFObject**) &sub));
		pDictionary->ReleaseReference();
		pDictionary = NULL;
		CHECK(sub->Initialize (pDataDef, length, sourceRef));
		CHECK(AppendNewTimelineSlot(editRate, sub, slotID, L"Test", zeroPos, 
												&newSlot));
		newSlot->ReleaseReference();
		newSlot = 0;
		sub->ReleaseReference();
		sub = 0;
	}
	XEXCEPT
	{
		if(sub != NULL)
		  sub->ReleaseReference();
		sub = 0;
		if(newSlot != NULL)
		  newSlot->ReleaseReference();
		newSlot = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
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
	aafFrameLength_t maxLength;
	aafPosition_t	zeroPos;
	aafLength_t		length;
	ImplAAFTimelineMobSlot *	newSlot = NULL, *mobSlot = NULL;
	aafBool			fullLength = kAAFFalse;
	ImplAAFDictionary *pDictionary = NULL;

	//!!!Validate tape mobs only, return AAFRESULT_TAPE_DESC_ONLY
	if(length32 == FULL_LENGTH)
	  {
		 fullLength = kAAFTrue;
		 length32 = 1;
	  }
	else
	  fullLength = kAAFFalse;
	
	CvtInt32toPosition(0, zeroPos);
 	CvtInt32toLength(length32, length);

	XPROTECT()
	{
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdTimecode()->
			  CreateInstance ((ImplAAFObject**) &tccp));

		tccp->Initialize(length, &startTC);		 
 		if (FindSlotBySlotID(slotID, (ImplAAFMobSlot **)&mobSlot)
			== AAFRESULT_SUCCESS)
		{
			CHECK(pDictionary->GetBuiltinDefs()->cdSequence()->
				  CreateInstance((ImplAAFObject**) &aSequ));
			CHECK(aSequ->Initialize(pDictionary->
									GetBuiltinDefs()->
									ddTimecode()));
			CHECK(aSequ->AppendComponent(tccp));
			CHECK(mobSlot->SetSegment(aSequ));

		} /* FindTimecodeSlot */
		else
		{
			CHECK(pDictionary->GetBuiltinDefs()->cdSequence()->
				  CreateInstance ((ImplAAFObject**) &aSequ));
			CHECK(aSequ->Initialize(pDictionary->
									GetBuiltinDefs()->
									ddTimecode()));
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
				CHECK(tccp->SetLength(length) );
				/* NOTE: What if the sequence already existed? */
				CHECK(aSequ->SetLength(length) );
			}
		}
		if(newSlot != NULL)
		  newSlot->ReleaseReference();
		newSlot = 0;
		if(aSequ != NULL)
		  aSequ->ReleaseReference();
		aSequ = 0;
		if(tccp != NULL)
		  tccp->ReleaseReference();
		tccp = 0;

		pDictionary->ReleaseReference();
		pDictionary = NULL;
	} /* XPROTECT */
	XEXCEPT
	{
		if(newSlot != NULL)
		  newSlot->ReleaseReference();
		newSlot = 0;
		if(aSequ != NULL)
		  aSequ->ReleaseReference();
		aSequ = 0;
		if(tccp != NULL)
		  tccp->ReleaseReference();
		tccp = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
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
	aafPosition_t	startPos, zeroPos;
	aafLength_t		length, zeroLen;
	ImplAAFTimelineMobSlot *	newSlot;
	aafEdgecode_t	edge;
	ImplAAFDictionary *pDictionary = NULL;

	// Validate film mobs only, return AAFRESULT_FILM_DESC_ONLY
	CvtInt32toPosition(0, zeroPos);
	CvtInt32toLength(0, zeroLen);
	XPROTECT()
	{
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdFiller()->
			  CreateInstance((ImplAAFObject**) &filler1));
		CHECK(filler1->Initialize(pDictionary->
								  GetBuiltinDefs()->
								  ddEdgecode(), zeroLen));	
		CHECK(pDictionary->GetBuiltinDefs()->cdFiller()->
			  CreateInstance ((ImplAAFObject**) &filler2));
		CHECK(filler2->Initialize(pDictionary->
								  GetBuiltinDefs()->
								  ddEdgecode(), zeroLen));	

		CHECK(pDictionary->GetBuiltinDefs()->cdSequence()->
			  CreateInstance ((ImplAAFObject**) &ecSequence));
		CHECK(ecSequence->Initialize(pDictionary->
									 GetBuiltinDefs()->
									 ddEdgecode()));	

		CvtInt32toLength(length32, length);
		CvtInt32toPosition(startEC, startPos);
		edge.startFrame = startPos;
		edge.filmKind = filmKind;
		edge.codeFormat = codeFormat;
		strncpy((char *)&edge.header, (char *)&header, 8);
		
		CHECK(pDictionary->GetBuiltinDefs()->cdEdgecode()->
			  CreateInstance ((ImplAAFObject**) &edgecodeClip));
		CHECK(edgecodeClip->Initialize(length, edge));	
		
		CHECK(ecSequence->AppendComponent(filler1));
		CHECK(ecSequence->AppendComponent(edgecodeClip));
		CHECK(ecSequence->AppendComponent(filler2));
		CHECK(AppendNewTimelineSlot(editrate, ecSequence, slotID,
									NULL, zeroPos, &newSlot));

		if(filler1 != NULL)
		  filler1->ReleaseReference();
		filler1 = 0;
		if(filler2 != NULL)
		  filler2->ReleaseReference();
		filler2 = 0;
		pDictionary->ReleaseReference();
		pDictionary = NULL;
	} /* XPROTECT */
	XEXCEPT
	{
		if(filler1 != NULL)
		  filler1->ReleaseReference();
		filler1 = 0;
		if(filler2 != NULL)
		  filler2->ReleaseReference();
		filler2 = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	}
	XEND;

	return (AAFRESULT_SUCCESS);
}




//****************
// ValidateTimecodeRange()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::SpecifyValidCodeRange (ImplAAFDataDef * /* pEssenceKind !!!*/,
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
  	aafUInt32				sequLoop;
	aafUInt32				numSegs;
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
		memset(&sourceRef, 0, sizeof(sourceRef));
		CvtInt32toPosition(0, sourceRef.startTime);
		CHECK(GetDictionary(&pDict));
		CHECK(pDict->GetBuiltinDefs()->cdSourceClip()->
			  CreateInstance((ImplAAFObject **)&sclp));

		if(FindSlotBySlotID(slotID, (ImplAAFMobSlot **)&slot) != AAFRESULT_SUCCESS)
		{
			CHECK(pDict->GetBuiltinDefs()->cdSequence()->
				  CreateInstance((ImplAAFObject **)&aSequ));
			CHECK(pDict->GetBuiltinDefs()->cdFiller()->
				  CreateInstance((ImplAAFObject **)&filler1));
			if(aSequ == NULL || filler1 == NULL)
				RAISE(E_FAIL);
			CHECK(aSequ->AppendComponent(filler1));
			CHECK(aSequ->AppendComponent(sclp));
			CHECK(pDict->GetBuiltinDefs()->cdFiller()->
				  CreateInstance((ImplAAFObject **)&filler2));
			CHECK(aSequ->AppendComponent(filler2));

			/* (SPR#343) Change to validate multiple ranges */
			CHECK(AppendNewTimelineSlot(editrate, aSequ, slotID,
												NULL, zeroPos, &newSlot));
		}
	  else
	  	{
			CHECK(slot->GetSegment(&seg));
			CHECK(seg->GenerateSequence(&segSequ));
  			CHECK(segSequ->GetComponents(&sequIter));
  			CHECK(segSequ->CountComponents(&numSegs));
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
					ImplAAFFiller	*test;		// Used only in test of type
					test = dynamic_cast<ImplAAFFiller*>(subSegment);
					if(test != NULL && (sequLoop == (numSegs-1)))
		 			{
						firstFillLen = pos;
						CHECK(SubInt64fromInt64(sequPos, &firstFillLen));
						CHECK(subSegment->GetLength(&oldFillLen));
						endFillLen = oldFillLen;
						CHECK(SubInt64fromInt64(length, &endFillLen));
						CHECK(SubInt64fromInt64(firstFillLen, &endFillLen));
						/****/
						CHECK(subSegment->SetLength(firstFillLen));
						/* 1.x does not have a Sequence Length property */
						CHECK(segSequ->GetLength(&sequLen));
						SubInt64fromInt64(oldFillLen, &sequLen);
						AddInt64toInt64(firstFillLen, &sequLen);
						CHECK(segSequ->SetLength(sequLen));

						CHECK(pDict->GetBuiltinDefs()->cdFiller()->
							  CreateInstance((ImplAAFObject **)&filler2));
//!!!						filler2 = CreateImpl(CLSID_AAFFiller(_file, mediaKind, endFillLen);	
						CHECK(segSequ->AppendComponent(sclp));
						CHECK(segSequ->AppendComponent(filler2));
						break;
					}
					else
						RAISE(AAFRESULT_NOT_IN_CURRENT_VERSION);
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
		aSequ = 0;
		if(segSequ!= NULL)
		  segSequ->ReleaseReference();
		segSequ = 0;
		if(filler1!= NULL)
		  filler1->ReleaseReference();
		filler1 = 0;
		if(filler2!= NULL)
		  filler2->ReleaseReference();
		filler2 = 0;
		if(seg!= NULL)
		  seg->ReleaseReference();
		seg = 0;
		if(subSegment!= NULL)
		  subSegment->ReleaseReference();
		subSegment = 0;
		if(pDict!= NULL)
		  pDict->ReleaseReference();
		pDict = 0;
		if(sclp!= NULL)
		  sclp->ReleaseReference();
		sclp = 0;
		if(timecodeClip!= NULL)
		  timecodeClip->ReleaseReference();
		timecodeClip = 0;
	}
	XEND;

	return (AAFRESULT_SUCCESS);
}



//****************
// NewPhysSourceRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::NewPhysSourceRef (const aafRational_t & editrate,
										aafSlotID_t  aMobSlot,
										ImplAAFDataDef * pEssenceKind,
										aafSourceRef_t  ref,
										aafLength_t  srcRefLength)
{
	if (! pEssenceKind)
	  return AAFRESULT_NULL_PARAM;
	aafUID_t essenceKind;
	AAFRESULT hr = pEssenceKind->GetAUID(&essenceKind);
	if (AAFRESULT_FAILED (hr))return hr;

	return(ImplAAFMob::AddPhysSourceRef(kAAFForceOverwrite,
										editrate,
										aMobSlot,
										essenceKind,
										ref,
										srcRefLength));
}

//****************
// AppendPhysSourceRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::AppendPhysSourceRef (const aafRational_t & editrate,
										   aafSlotID_t  aMobSlot,
										   ImplAAFDataDef * pEssenceKind,
										   aafSourceRef_t  ref,
										   aafLength_t  srcRefLength)
{
	if (! pEssenceKind)
	  return AAFRESULT_NULL_PARAM;
	aafUID_t essenceKind;
	AAFRESULT hr = pEssenceKind->GetAUID(&essenceKind);
	if (AAFRESULT_FAILED (hr))return hr;

	return(ImplAAFMob::AddPhysSourceRef(kAAFAppend,
										editrate,
										aMobSlot,
										essenceKind,
										ref,
										srcRefLength));
}



//****************
// AddPulldownRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::AddPulldownRef (aafAppendOption_t  /* addType !!!*/,
									  const aafRational_t & editrate,
									  aafSlotID_t  aMobSlot,
									  ImplAAFDataDef * pEssenceKind,
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
	ImplAAFPulldown 	*pdwn = NULL;
	ImplAAFSequence *	sequence = NULL;
	ImplAAFDictionary*	dict = NULL;
	aafLength_t 		outLength, zero;
	aafInt32 			patternLen;
	AAFRESULT			status = AAFRESULT_SUCCESS;
	aafUInt32 			mask;
		
	if (! pEssenceKind)
	  return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		GetDictionary(&dict);
		CvtInt32toInt64(0, &zero);
		XASSERT(direction == kAAFFilmToTapeSpeed || direction == kAAFTapeToFilmSpeed,
				AAFRESULT_PULLDOWN_DIRECTION);

		CvtInt32toPosition(0, zeroPos);
		{
			CHECK(dict->GetBuiltinDefs()->cdPulldown()->
				  CreateInstance((ImplAAFObject **)&pdwn));
			CHECK(pdwn->SetDataDef(pEssenceKind));
			CHECK(pdwn->SetPulldownKind(pulldownKind));
			CHECK(pdwn->SetPhaseFrame(phaseFrame));
			CHECK(pdwn->SetPulldownDirection(direction));
			CHECK(pdwn->aafPvtGetPulldownMask(pulldownKind,
										&mask,  &patternLen, &isOneToOne));

			if(isOneToOne)
			{
				CHECK(pdwn->SetLength(srcRefLength));
			}
			else
			{
				/* Remember, this routine is given the OUTPUT length, and must determine
				 * the input length (so the ratios look backwards)
				 */
				CHECK(pdwn->MapOffset(srcRefLength, kAAFTrue, &outLength, NULL));
				CHECK(pdwn->SetLength(outLength));
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
				aafUInt32			numSegments;
				aafUInt32			n;
				ImplAAFComponent 	*subSeg;
				
				CHECK(sequence->CountComponents(&numSegments));
				if(numSegments == 0)
				{
					CHECK(sequence->AppendComponent(pdwn));
					CHECK(dict->GetBuiltinDefs()->cdSourceClip()->
						  CreateInstance((ImplAAFObject **)&sclp));
					CHECK(sclp->Initialize(pEssenceKind, srcRefLength, ref));
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
			CHECK(sclp->Initialize(pEssenceKind, srcRefLength, ref));
		}
		else
		{
			CHECK(dict->GetBuiltinDefs()->cdSourceClip()->
				  CreateInstance((ImplAAFObject **)&sclp));
			CHECK(sclp->Initialize(pEssenceKind, srcRefLength, ref));
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
		dict = 0;
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
			edesc = 0;
		}
		else
			*pMobKind = kAAFAllMob;
	}
	XEXCEPT
	{
		if(edesc != NULL)
		  edesc->ReleaseReference();
		edesc = 0;
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
	aafPosition_t			sequPos;
	ImplAAFMobSlot			*slot = NULL;
 	ImplEnumAAFMobSlots		*slotIter = NULL;
  	aafBool					found = kAAFFalse;
	aafUInt32				phys;

	XPROTECT()
	{
		CvtInt32toInt64(position, &offset);
		*tcStartPos = 0;
		*result = NULL;
		CHECK(GetSlots (&slotIter));
		while((found != kAAFTrue) && slotIter->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
		  ImplAAFDataDefSP pDataDef;
		  CHECK(slot->GetDataDef (&pDataDef));
		  aafUID_t dataDef;
		  CHECK(pDataDef->GetAUID (&dataDef));
		  if(EqualAUID(&dataDef, &DDEF_Timecode))
			{
			  CHECK(slot->GetPhysicalNum (&phys));
			  if((phys == 0) || (phys == 1))
				found = kAAFTrue;
			}
		}
		if(found != kAAFTrue)
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
		slotIter = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(seg != NULL)
		  seg->ReleaseReference();
		seg = 0;
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
	aafNumSlots_t				numSlots;
	aafUInt32					loop;
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
			CHECK(GetSlots (&slotIter));
			CHECK(CountSlots(&numSlots));
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
					CHECK(seg->SetLength(len));
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
		slot = 0;
		if (timelineSlot)
		  timelineSlot->ReleaseReference();
		timelineSlot = 0;
		if (physMedia)
		  physMedia->ReleaseReference();
		physMedia = 0;
		if (seg)
		  seg->ReleaseReference();
		seg = 0;
		if (slotIter)
		  slotIter->ReleaseReference();
		slotIter = 0;
	}
	XEND
		
	return (AAFRESULT_SUCCESS);
}




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

#include <assert.h>
#include "ImplAAFEssenceDescriptor.h"
#include "ImplAAFSourceClip.h"
#include "aafCvt.h"
#include "aafErr.h"
#include "aafresult.h"
#include "ImplAAFObjectCreation.h"
#include "AAFDefUIDs.h"
#include "AAFUtils.h"
#include "ImplEnumAAFMobSlots.h"
#include "ImplAAFSequence.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFTimecode.h"
#include "ImplAAFFileDescriptor.h"
#include "ImplAAFFiller.h"
#include "ImplAAFEdgecode.h"

extern "C" const aafClassID_t	CLSID_AAFSourceClip;
extern "C" const aafClassID_t	CLSID_AAFSequence;
extern "C" const aafClassID_t	CLSID_AAFTimecode;
extern "C" const aafClassID_t	CLSID_AAFFiller;
extern "C" const aafClassID_t	CLSID_AAFEdgecode;

ImplAAFSourceMob::ImplAAFSourceMob ()
: _essenceDesc(        PID_SourceMob_MediaDescription,          "MediaDescription")
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
	
	if (editRate.denominator == 0)
		return AAFRESULT_BADRATE;
	
	XPROTECT()
	{
 		sourceRef.sourceID = NilMOBID;
		sourceRef.sourceSlotID = 0;
		CvtInt32toPosition(0, sourceRef.startTime);
		sub = (ImplAAFSourceClip *)CreateImpl(CLSID_AAFSourceClip);
		if(sub == NULL)
			return(E_FAIL);
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
		tccp = (ImplAAFTimecode *)CreateImpl(CLSID_AAFTimecode);
		tccp->Initialize(length, &startTC);		 
 		if (FindSlotBySlotID(slotID, (ImplAAFMobSlot **)&mobSlot) == AAFRESULT_SUCCESS)
		{
			aSequ = (ImplAAFSequence *)CreateImpl(CLSID_AAFSequence);
			if(aSequ == NULL)
				RAISE(E_FAIL);
			CHECK(aSequ->Initialize(&timecodeKind));
			CHECK(aSequ->AppendComponent(tccp));
			CHECK(mobSlot->SetSegment(aSequ));

		} /* FindTimecodeSlot */
		else
		{
			aSequ = (ImplAAFSequence *)CreateImpl(CLSID_AAFSequence);
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
	} /* XPROTECT */
	XEXCEPT
	{
		if(newSlot != NULL)
			newSlot->ReleaseReference();
		if(aSequ != NULL)
			aSequ->ReleaseReference();
		if(tccp != NULL)
			tccp->ReleaseReference();
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
#if 1
	ImplAAFFiller *     filler1= NULL, *filler2 = NULL;
	ImplAAFSequence *ecSequence;
	ImplAAFEdgecode *edgecodeClip;
	AAFRESULT			aafError = AAFRESULT_SUCCESS;
	aafPosition_t	startPos, zeroPos;
	aafLength_t		length, zeroLen;
	ImplAAFTimelineMobSlot *	newSlot;
	aafEdgecode_t	edge;
	aafUID_t		edgekind = DDEF_Edgecode;

	// Validate film mobs only, return AAFRESULT_FILM_DESC_ONLY
	CvtInt32toPosition(0, zeroPos);
	CvtInt32toLength(0, zeroLen);
	XPROTECT()
	{
		filler1 = (ImplAAFFiller *)CreateImpl(CLSID_AAFFiller);
		CHECK(filler1->Initialize(&edgekind, zeroLen));	
		filler2 = (ImplAAFFiller *)CreateImpl(CLSID_AAFFiller);
		CHECK(filler2->Initialize(&edgekind, zeroLen));	
		if(filler1 == NULL || filler2 == NULL)
			RAISE(E_FAIL);

		ecSequence = (ImplAAFSequence *)CreateImpl(CLSID_AAFSequence);
		if(ecSequence == NULL)
			RAISE(E_FAIL);
		CHECK(ecSequence->Initialize(&edgekind));	

		CvtInt32toLength(length32, length);
		CvtInt32toPosition(startEC, startPos);
		edge.startFrame = startPos;
		edge.filmKind = filmKind;
		edge.codeFormat = codeFormat;
		strncpy((char *)&edge.header, (char *)&header, 8);
		
		edgecodeClip = (ImplAAFEdgecode *)CreateImpl(CLSID_AAFEdgecode);
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
	} /* XPROTECT */
	XEXCEPT
	{
		if(filler1 != NULL)
			filler1->ReleaseReference();
		if(filler2 != NULL)
			filler2->ReleaseReference();
	}
	XEND;

	return (AAFRESULT_SUCCESS);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}




//****************
// ValidateTimecodeRange()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::SpecifyValidCodeRange (ImplAAFDataDef * /*pEssenceKind*/,
                           aafSlotID_t  /*slotID*/,
                           aafRational_t  /*editrate*/,
                           aafFrameOffset_t  /*startOffset*/,
                           aafFrameLength_t  /*length32*/)
{
#if FULL_TOOLKIT
	ImplAAFSourceClip		*sclp = NULL;
	ImplAAFTimecode			*timecodeClip = NULL;
	ImplAAFSequence *aSequ, *segSequ;
	ImplAAFFiller *filler1, *filler2;
	ImplAAFSegment *seg;
	ImplAAFComponent *subSegment;
	aafPosition_t	pos, zeroPos, sequPos, begPos, endPos;
	aafLength_t		length, zeroLen, tcLen, endFillLen, firstFillLen, oldFillLen, segLen;
	aafLength_t		tcSlotLen, sequLen;
  	aafSourceRef_t sourceRef;
	ImplAAFMobSlot *	newSlot, *slot;
  	aafFrameOffset_t	tcStartPos;
  	aafTimecode_t	timecode;
  	aafProperty_t	prop;
  	aafInt32		sequLoop, numSegs;
    ImplAAFIterate *		sequIter = NULL;
  	AAFRESULT		aafError;
  	
	XPROTECT(_file)
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
		sourceRef.sourceID.prefix = 0;
		sourceRef.sourceID.major = 0;
		sourceRef.sourceID.minor = 0;
		sourceRef.sourceSlotID = 0;
		CvtInt32toPosition(0, sourceRef.startTime);
		sclp = CreatreImpl(CLSID_AAFSourceClip);
		(_file, mediaKind, length, sourceRef);

		if(FindSlotBySlotID(slotID, &slot) != AAFRESULT_SUCCESS)
		{
			aSequ  = CreateImpl(CLSID_AAFSequence(_file, mediaKind);
			filler1 = CreateImpl(CLSID_AAFFiller(_file, mediaKind, pos);	
			if(aSequ == NULL || filler1 == NULL)
				RAISE(E_FAIL);
			CHECK(aSequ->AppendCpnt(filler1));
			CHECK(aSequ->AppendCpnt(sclp));
			filler2 = CreateImpl(CLSID_AAFFiller(_file, mediaKind, endFillLen);	
			CHECK(aSequ->AppendCpnt(filler2));

			/* (SPR#343) Change to validate multiple ranges */
			CHECK(AppendNewSlot(editrate, aSequ, zeroPos, slotID,
												NULL, &newSlot));
		}
	  else
	  	{
		  CHECK(slot->GetSegment(&seg));
		  CHECK(seg->GenerateSequence(&segSequ));
  			sequIter = new AAFIterate(_file);
  			CHECK(segSequ->GetNumCpnts(&numSegs));
			for (sequLoop=0; sequLoop < numSegs; sequLoop++)
			{
				CHECK(sequIter->SequenceGetNextCpnt(segSequ,
												NULL, &sequPos, &subSegment));
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
		 			if(subSegment->IsTypeOf("FILL", &aafError) &&
		 				(sequLoop == (numSegs-1)))
		 			{
						prop = OMCPNTLength;
						
						firstFillLen = pos;
						CHECK(SubInt64fromInt64(sequPos, &firstFillLen));
						CHECK(subSegment->ReadLength(prop, &oldFillLen));
						endFillLen = oldFillLen;
						CHECK(SubInt64fromInt64(length, &endFillLen));
						CHECK(SubInt64fromInt64(firstFillLen, &endFillLen));
						/****/
						CHECK(subSegment->WriteLength(prop, firstFillLen));
						/* 1.x does not have a Sequence Length property */
						  {
							CHECK(segSequ->ReadLength(prop, &sequLen));
							SubInt64fromInt64(oldFillLen, &sequLen);
							AddInt64toInt64(firstFillLen, &sequLen);
							CHECK(segSequ->WriteLength(prop, sequLen));
						  }

						filler2 = CreateImpl(CLSID_AAFFiller(_file, mediaKind, endFillLen);	
						CHECK(segSequ->AppendCpnt(sclp));
						CHECK(segSequ->AppendCpnt(filler2));
						break;
					}
					else
						RAISE(AAFRESULT_NOT_IMPLEMENTED);
				}
			} /* for */
			delete sequIter;
			sequIter = NULL;
			
//!!!			/* Release reference, so the useCount is decremented */
//			if (subSegment)
//			  {
//				 subSegment->ReleaseObject();	
//				 subSegment = NULL;
//			  }
		}
		if(sclp!= NULL)
			sclp->ReleaseReference();
		if(timecodeClip!= NULL)
			timecodeClip->ReleaseReference();
	}
	XEXCEPT
	{
		if(sclp!= NULL)
			sclp->ReleaseReference();
		if(timecodeClip!= NULL)
			timecodeClip->ReleaseReference();
	}
	XEND;

	return (AAFRESULT_SUCCESS);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
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
							aafRational_t  /*editrate*/,
                           aafSlotID_t  /*aMobSlot*/,
                           aafUID_t * /*pEssenceKind*/,
							aafSourceRef_t  ref,
                           aafLength_t  /*srcRefLength*/,
                           aafPulldownKind_t  /*pulldownKind*/,
                           aafPhaseFrame_t  /*phaseFrame*/,
                           aafPulldownDir_t  /*direction*/)
{
#if FULL_TOOLKIT
	ImplAAFSourceClip		*sclp = NULL;
	ImplAAFMobSlot		*trkd = NULL;
	aafSourceRef_t	ref;
	aafPosition_t	zeroPos;
	ImplAAFIterate * 	slotIter = NULL;
	aafSlotID_t 	tmpSlotID;
	ImplAAFMobSlot *		slot = NULL;
	aafBool 		foundSlot = FALSE, isOneToOne;
	ImplAAFMobSlot * 	maskSlot = NULL;
	ImplAAFPulldown 	*pdwn = NULL;
	ImplAAFDataKind *	datakind = NULL;
	aafLength_t 	outLength, zero;
	aafInt32 		patternLen;
	AAFRESULT		status = AAFRESULT_SUCCESS;
	aafProperty_t	prop, subProp;
	aafUInt32 		mask;
	
	aafAssertValidFHdl(_file);
	
	XPROTECT(_file)
	{
		CvtInt32toInt64(0, &zero);
		XASSERT(sourceRefObj != NULL, AAFRESULT_NULLOBJECT);
		XASSERT(direction == kAAFFilmToTapeSpeed || direction == kAAFTapeToFilmSpeed,
				AAFRESULT_PULLDOWN_DIRECTION);

		CvtInt32toPosition(0, zeroPos);
		CHECK(sourceRefObj->ReadUID(OMMOBJMobID, &ref.sourceID));
		ref.sourceSlotID = srcRefSlot;
		ref.startTime = srcRefOffset;

		{
			prop = OMSEQUComponents;
			subProp = OMCPNTLength;
		}

		{
			pdwn = CreateImpl(CLSID_AAFPulldown(_file);
			CHECK(pdwn->WriteObjRef(OMCPNTDatakind, mediaKind));
			CHECK(pdwn->WritePulldownKindType(OMPDWNPulldownKind, pulldownKind));
			CHECK(pdwn->WritePhaseFrameType(OMPDWNPhaseFrame, phaseFrame));
			CHECK(pdwn->WritePulldownDirectionType(OMPDWNDirection, direction));
			CHECK(pdwn->aafPvtGetPulldownMask(pulldownKind,
										&mask,  &patternLen, &isOneToOne));

			if(isOneToOne)
			{
				CHECK(pdwn->WriteLength(OMCPNTLength, srcRefLength));
			}
			else
			{
				/* Remember, this routine is given the OUTPUT length, and must determine
				 * the input length (so the ratios look backwards)
				 */
				CHECK(pdwn->MapOffset(srcRefLength, TRUE, &outLength, NULL));
				CHECK(pdwn->WriteLength(OMCPNTLength, outLength));
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
			if(seg->IsTypeOf("SEQU", &status))
			{
				aafLength_t		foundLen;
				aafInt32		numSegments, n;
				ImplAAFObject 		*subSeg;
				
				numSegments = seg->GetObjRefArrayLength(prop);
				if(numSegments == 0)
				{
					CHECK(seg->WriteNthObjRefArray(prop, pdwn, 1));
					sclp = CreateImpl(CLSID_AAFSourceClip(_file, mediaKind, srcRefLength, ref);
				}
				for(n = 1; n <= numSegments; n++)
				{
					CHECK(seg->ReadNthObjRefArray(prop, &subSeg, n));
					CHECK(subSeg->ReadLength(subProp, &foundLen));

					if(Int64NotEqual(foundLen, zero))
					{
						CHECK(seg->WriteNthObjRefArray(prop, pdwn, n));
						sclp = (ImplAAFSourceClip *)subSeg;	//!!CASTING
						break;
					}
				}
			}
			else
			{
					prop = OMMSLTSegment;
				CHECK(slot->WriteObjRef(prop, pdwn));
				sclp = (AAFSourceClip *)seg;		//!!CASTING
			}
			
			XASSERT(sclp->IsTypeOf("SCLP", &status), AAFRESULT_NOT_SOURCE_CLIP);
			CHECK(sclp->SetRef(ref));
			CHECK(sclp->WriteLength(subProp, srcRefLength));
		}
		else
		{
			sclp = CreateImpl(CLSID_AAFSourceClip(_file, mediaKind, srcRefLength, ref);
			CHECK(AppendNewSlot(editrate, pdwn,
								zeroPos, aMobSlot, NULL, &trkd) );
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
				CHECK(pdwn->WriteObjRef(OMPDWNInputSegment, sclp));
			}
		}

	} /* XPROTECT */
	XEXCEPT
	XEND;

	return (AAFRESULT_SUCCESS);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



//****************
// SearchSource()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::SearchSource (aafSlotID_t slotID,
                           aafPosition_t  offset,
                           aafMobKind_t  mobKind,
                           aafMediaCriteria_t *pMediaCrit,
                           aafEffectChoice_t *pEffectChoice,
                           ImplAAFFindSourceInfo **ppSourceInfo)
{
	return(InternalSearchSource(slotID, offset, mobKind, pMediaCrit, pEffectChoice,
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
//
AAFRESULT ImplAAFSourceMob::FindTimecodeClip(
				aafFrameOffset_t	position,
				ImplAAFTimecode 		**result,
				aafFrameOffset_t	*tcStartPos,
				aafLength_t			*tcSlotLen)
{
#if FULL_TOOLKIT
	ImplAAFSegment *	seg = NULL;
	aafPosition_t		offset;
	AAFRESULT			status = AAFRESULT_SUCCESS;
	aafLength_t			zeroLen;
	aafPosition_t		sequPos;
	
	XPROTECT()
	{
		CvtInt32toInt64(position, &offset);
		CvtInt32toInt64(0, &zeroLen);
		*tcStartPos = 0;
		*result = NULL;
		CHECK(FindTimecodeSlot(&seg));
		CHECK(seg->GetLength(tcSlotLen));
		CHECK(seg->OffsetToTimecodeClip(offset, result, &sequPos));
		*tcStartPos = sequPos;
	} /* XPROTECT */
	XEXCEPT
	{
		if(XCODE() == AAFRESULT_NO_MORE_OBJECTS)
			RERAISE(AAFRESULT_NO_TIMECODE);
		*result = NULL;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
#else
	return(AAFRESULT_NOT_IMPLEMENTED);
#endif
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
	ImplAAFTimelineMobSlot		*slot;	//!!! Assuming timeline
	ImplAAFSegment				*seg;
	ImplEnumAAFMobSlots			*slotIter = NULL;
	aafRational_t				srcRate, destRate;
	ImplAAFFileDescriptor		*physMedia;
		
	XPROTECT()
	{
		CHECK(GetEssenceDescriptor((ImplAAFEssenceDescriptor **)&physMedia));	//!!!
		{
			CHECK(EnumAAFAllMobSlots (&slotIter));
			CHECK(GetNumSlots(&numSlots));
			for (loop = 1; loop <= numSlots; loop++)
			{
				CHECK(slotIter->NextOne((ImplAAFMobSlot **)&slot));	//!!! Assuming timeline
				CHECK(slot->GetSegment(&seg));
				CHECK(slot->GetEditRate(&destRate));
				CHECK(physMedia->GetLength(&len));
				CHECK(physMedia->GetSampleRate(&srcRate));
//!!!				CHECK(slot->ConvertToEditRate(len,
//										aafRational_t destRate,
//										aafPosition_t *convertPos);
				if((srcRate.numerator != destRate.numerator) ||
				   (srcRate.denominator != destRate.denominator))
				{
					CHECK(AAFConvertEditRate(	srcRate, len, destRate, kRoundFloor, &len));
				}
				CHECK(seg->SetLength(&len));
			}			
//!!!			delete slotIter;
			slotIter = NULL;
		}
	}
	XEXCEPT
	XEND
		
	return (AAFRESULT_SUCCESS);
}


OMDEFINE_STORABLE(ImplAAFSourceMob, AUID_AAFSourceMob);


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

extern "C" const aafClassID_t	CLSID_AAFSourceClip;

ImplAAFSourceMob::ImplAAFSourceMob ()
: _essenceDesc(         PID_SOURCEMOB_EDESC,          "essenceDescriptor")
{
  _persistentProperties.put(_essenceDesc.address());
}

ImplAAFSourceMob::~ImplAAFSourceMob ()
{}

//****************
// GetEssenceDescription()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::GetEssenceDescription (ImplAAFEssenceDescriptor **ppEdes)
{
	if (ppEdes == NULL)
		return AAFRESULT_NULL_PARAM;

	*ppEdes = _essenceDesc;
	return AAFRESULT_SUCCESS;
}



//****************
// SetEssenceDescription()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::SetEssenceDescription (ImplAAFEssenceDescriptor *pEdes)
{
	if (pEdes == NULL)
		return AAFRESULT_NULL_PARAM;

	_essenceDesc = pEdes;
	return AAFRESULT_SUCCESS;
}



//****************
// Setup()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::Setup (aafWChar *  /*pName*/,
                           aafClassID_t *  /*pMdesClass*/)
{
	return AAFRESULT_NOT_IMPLEMENTED;
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
		CHECK(sub->InitializeSourceClip (dataDef, &length, sourceRef));
		CHECK(AppendNewTimelineSlot(editRate, sub, slotID, L"Test", zeroPos, 
												&newSlot));
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS);
}



//****************
// AddTimecodeClip()
//
AAFRESULT STDMETHODCALLTYPE
   ImplAAFSourceMob::AddTimecodeClip (aafRational_t editrate,
                           aafInt32 slotID,
                           aafTimecode_t startTC,
                           aafFrameLength_t length32)
{
#if FULL_TOOLKIT
	ImplAAFSegment *slotComponent= NULL;
	ImplAAFTimecode *tccp = NULL;
	ImplAAFFiller *fill = NULL;
	ImplAAFSegment *     aSubComponent = NULL;
	ImplAAFSequence	*aSequ = NULL;
	ImplAAFFiller *     filler1 = NULL, *filler2 = NULL;
	aafInt32         zeroL = 0L, sub, numSub;
	aafFrameLength_t maxLength;
	aafClassID_t	tag;
	ImplAAFDataKind *	timecodeKind = NULL;
	ImplAAFObject		*tmp;
	AAFRESULT			aafError = AAFRESULT_SUCCESS;
	aafPosition_t	zeroPos;
	aafLength_t		length, zeroLen, sequLen;
	ImplAAFMobSlot *	newSlot = NULL;
	aafBool			fullLength = FALSE;
	aafProperty_t	sequProp;
	
	{
		sequProp = OMSEQUComponents;
	}

	if(length32 == FULL_LENGTH)
	  {
		 fullLength = TRUE;
		 length32 = 1;
	  }
	else
	  fullLength = FALSE;
	
	CvtInt32toPosition(0, zeroPos);
	CvtInt32toLength(0, zeroLen);
 	CvtInt32toLength(length32, length);

	XPROTECT()
	  {
		 tccp = CreateImpl(CLSID_AAFTimecode);
		 (_file, length, startTC);		 
		 if (FindTimecodeSlot(&slotComponent) == AAFRESULT_SUCCESS)
			{
			  CHECK(slotComponent->GetClassID(tag));
			  if (strcmp(tag, "SEQU") == 0)
				 {
					numSub = slotComponent->GetObjRefArrayLength(sequProp);
					for (sub = numSub; sub >= 1; sub--)
					  {
						 CHECK(slotComponent->ReadNthObjRefArray(sequProp, &tmp, sub));
						 aSubComponent = (ImplAAFSegment *)tmp;
						 CHECK(aSubComponent->GetClassID(tag));
						 if (strcmp(tag, "FILL") != 0)
							{

							  CHECK(slotComponent->WriteNthObjRefArray(sequProp, tccp, sub + 1));
								  {
									CHECK(slotComponent->GetLength(&sequLen));
									AddInt64toInt64(length, &sequLen);
									CHECK(slotComponent->WriteLength(OMCPNTLength, sequLen));
								  }
							  if (sub != numSub)	/* At least one FILL
														 * found at the end */
								 {
									fill = CreateImpl(CLSID_AAFFiller);
									(_file, timecodeKind, zeroLen);	
									CHECK(slotComponent->WriteNthObjRefArray(sequProp, fill, sub + 2));
								 }
							  break;
							}
					  }
				 }
			  else
				 RAISE(OM_ERR_NOT_IMPLEMENTED);
			} /* FindTimecodeSlot */
		 else
			{
			  aSequ = CreateImpl(CLSID_AAFSequence);
			  (_file, timecodeKind);
			  filler1 = CreateImpl(CLSID_AAFFiller);
			  (_file, timecodeKind, zeroLen);	
			  CHECK(aSequ->AppendCpnt(filler1));
			  CHECK(aSequ->AppendCpnt(tccp));
			  filler2 = CreateImpl(CLSID_AAFFiller);
			  (_file, timecodeKind, zeroLen);	
			  CHECK(aSequ->AppendCpnt(filler2));
			  CHECK(AppendNewSlot(editrate, aSequ, zeroPos,
										slotID, NULL, &newSlot));
			}

		 /* Release Bento reference, so the useCount is decremented */
		 if (slotComponent)
			{
			  slotComponent->ReleaseObject();	
			}

		 if(fullLength)
			{
			  CHECK(PvtTimecodeToOffset(startTC.fps, 24, 0, 0, 0, 
											 startTC.drop, &maxLength));
				 {
					CvtInt32toLength(maxLength, length);
					CHECK(tccp->WriteLength(OMCPNTLength, length) );
					/* NOTE: What if the sequence already existed? */
					CHECK(aSequ->WriteLength(OMCPNTLength, length) );
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
// AddEdgecodeClip()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::AddEdgecodeClip (aafRational_t  /*editrate*/,
                           aafInt32  /*slotID*/,
                           aafFrameOffset_t  /*startEC*/,
                           aafFrameLength_t  /*length32*/,
                           aafFilmType_t  /*filmKind*/,
                           aafEdgeType_t  /*codeFormat*/,
                           aafEdgecodeHeader_t  /*header*/)
{
#if FULL_TOOLKIT
	ImplAAFFiller *     filler1, *filler2;
	ImplAAFSequence *ecSequence;
	ImplAAFEdgecode *edgecodeClip;
	ImplAAFRESULT			aafError = AAFRESULT_SUCCESS;
	aafPosition_t	startPos, zeroPos;
	aafLength_t		length, zeroLen;
	ImplAAFMobSlot *	newSlot;
	ImplAAFDataKind *	edgecodeKind;
	aafEdgecode_t	edge;
	
	CvtInt32toPosition(0, zeroPos);
	CvtInt32toLength(0, zeroLen);
	XPROTECT(_file)
	{
		_head->DatakindLookup(EDGECODEKIND, &edgecodeKind, &aafError);
		if (aafError != AAFRESULT_SUCCESS)
		{
			RAISE(aafError);
		}

		filler1 = CreateImpl(CLSID_AAFFiller);
		(_file, edgecodeKind, zeroLen);	
		filler2 = CreateImpl(CLSID_AAFFiller);
		(_file, edgecodeKind, zeroLen);	

		ecSequence = CreateImpl(CLSID_AAFSequence);
		(_file, edgecodeKind);

		CvtInt32toLength(length32, length);
		CvtInt32toPosition(startEC, startPos);
		edge.startFrame = startPos;
		edge.filmKind = filmKind;
		edge.codeFormat = codeFormat;
		strncpy((char *)edge.header, header, 8);
		
		edgecodeClip = CreateImpl(CLSID_AAFEdgecode);
		(_file, length, edge);
		
		CHECK(ecSequence->AppendCpnt(filler1));
		CHECK(ecSequence->AppendCpnt(edgecodeClip));
		CHECK(ecSequence->AppendCpnt(filler2));
		CHECK(AppendNewSlot(editrate, ecSequence, zeroPos,
											slotID, NULL, &newSlot));
	} /* XPROTECT */
	XEXCEPT
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
    ImplAAFSourceMob::ValidateTimecodeRange (ImplAAFDataDef * /*pEssenceKind*/,
                           aafSlotID_t  /*slotID*/,
                           aafRational_t  /*editrate*/,
                           aafFrameOffset_t  /*startOffset*/,
                           aafFrameLength_t  /*length32*/)
{
#if FULL_TOOLKIT
	ImplAAFSourceClip	 *sclp;
	ImplAAFTimecode *timecodeClip;
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
						RAISE(OM_ERR_NOT_IMPLEMENTED);
				}
			} /* for */
			delete sequIter;
			sequIter = NULL;
			
			/* Release Bento reference, so the useCount is decremented */
			if (subSegment)
			  {
				 subSegment->ReleaseObject();	
				 subSegment = NULL;
			  }
		}
	}
	XEXCEPT
	XEND;

	return (AAFRESULT_SUCCESS);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



//****************
// AddPhysSourceRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::AddPhysSourceRef (aafRational_t  /*editrate*/,
                           aafSlotID_t  /*aMobSlot*/,
                           ImplAAFDataDef * /*pEssenceKind*/,
                           ImplAAFSegment * /*pSourceRefObj*/,
                           aafPosition_t  /*srcRefOffset*/,
                           aafInt32  /*srcRefSlot*/,
                           aafLength_t  /*srcRefLength*/)
{
#if FULL_TOOLKIT
	ImplAAFSegment	*seg = NULL;
	ImplAAFMobSlot *slot = NULL;
	AAFRESULT	status = AAFRESULT_SUCCESS;
	aafSourceRef_t	ref;
	aafPosition_t	zeroPos;
	aafSlotID_t tmpSlotID;
	ImplAAFSourceClip *		sclp = NULL;
	ImplAAFMobSlot *trkd = NULL;
	
	aafAssertValidFHdl(_file);
	
	XPROTECT(_file)
	{
		if (sourceRefObj)
		{
			CvtInt32toPosition(0, zeroPos);
			CHECK(sourceRefObj->ReadUID(OMMOBJMobID, &ref.sourceID));
			ref.sourceSlotID = srcRefSlot;
			ref.startTime = srcRefOffset;
				
			status = FindSlotBySlotID(aMobSlot, &slot);
			if (status == AAFRESULT_SUCCESS)
			{
				CHECK(slot->GetSlotID(&tmpSlotID));
				CHECK(slot->GetSegment(&seg));
				if(seg->IsTypeOf("SEQU", &status))
				{
					
//!!!					CHECK(MobFindCpntByPosition(aMobSlot, seg, zeroPos, zeroPos,
//!!!														NULL, &foundPos, &seg, &foundLen));
				}
				XASSERT(seg->IsTypeOf("SCLP", &status), OM_ERR_NOT_SOURCE_CLIP);	/* OK Assertion */
				CHECK(((ImplAAFSourceClip *)seg)->SetRef(ref));
			}
			else
			{
				sclp = CreateImpl(CLSID_AAFSourceClip(_file, mediaKind, srcRefLength, ref);
				CHECK(AppendNewSlot(editrate, sclp,
									zeroPos, aMobSlot, NULL, &trkd) );
			}

		}
		else
		{
			CHECK(AddNilReference(aMobSlot, 
										  srcRefLength, mediaKind,  editrate));
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
// AddPulldownRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::AddPulldownRef (aafRational_t  /*editrate*/,
                           aafSlotID_t  /*aMobSlot*/,
                           ImplAAFDataDef * /*pEssenceKind*/,
                           ImplAAFSegment * /*pSourceRefObj*/,
                           aafPosition_t  /*srcRefOffset*/,
                           aafInt32  /*srcRefSlot*/,
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
		XASSERT(sourceRefObj != NULL, OM_ERR_NULLOBJECT);
		XASSERT(direction == kAAFFilmToTapeSpeed || direction == kAAFTapeToFilmSpeed,
				OM_ERR_PULLDOWN_DIRECTION);

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
			
			XASSERT(sclp->IsTypeOf("SCLP", &status), OM_ERR_NOT_SOURCE_CLIP);
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
    ImplAAFSourceMob::SearchSource (aafSlotID_t  /*slotID*/,
                           aafPosition_t  /*offset*/,
                           aafMobKind_t  /*mobKind*/,
                           aafMediaCriteria_t *  /*pMediaCrit*/,
                           aafEffectChoice_t *  /*pEffectChoice*/,
                           ImplAAFComponent ** /*ppThisCpnt*/,
                           ImplAAFFindSourceInfo ** /*ppSourceInfo*/)
{
#if FULL_TOOLKIT
	return(InternalMobSearchSource(slotID, offset, mobKind, mediaCrit,
								effectChoice, thisCpnt, sourceInfo));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



//****************
// GetMobKind()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceMob::GetMobKind (aafMobKind_t *pMobKind)
{
	ImplAAFEssenceDescriptor	*edesc;
	
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		CHECK(GetEssenceDescription (&edesc));
		CHECK(edesc->GetOwningMobKind(pMobKind));
	}
	XEXCEPT
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
	AAFSegment *		seg = NULL;
	AAFIterate *	sequIter = NULL;
	aafPosition_t	offset;
	AAFRESULT		status = AAFRESULT_SUCCESS;
	aafLength_t		zeroLen;
	aafPosition_t	sequPos;
	
	XPROTECT(_file)
	{
		CvtInt32toInt64(position, &offset);
		CvtInt32toInt64(0, &zeroLen);
		*tcStartPos = 0;
		*result = NULL;
		CHECK(FindTimecodeSlot(&seg));
		CHECK(seg->GetLength(tcSlotLen));
		CHECK(seg->OffsetToTimecodeClip(offset, result, &sequPos));
		CHECK(TruncInt64toUInt32(sequPos, tcStartPos));		/* OK FRAMEOFFSET */
	} /* XPROTECT */
	XEXCEPT
	{
		if(XCODE() == OM_ERR_NO_MORE_OBJECTS)
			RERAISE(OM_ERR_NO_TIMECODE);
		*result = NULL;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}
//************************
// Function:  FindTimecodeSlot	(INTERNAL)
//
//    Search a slot group for the timecode slot, and return a TCCP object.
//    This routine will search for the first nonzero length subcomponent
//    of a SEQU, because MC5.0 has zero length filler clips and
//    transitions on the timecode slots.  NULL is returned if there is
//    not a valid timecode object present.
//
// Argument Notes:
//		StuffNeededBeyondNotesInDefinition.labelNumber
//
// ReturnValue:
//		Error code (see below).
//
// Possible Errors:
// Standard errors (see top of file).
//
AAFRESULT ImplAAFSourceMob::FindTimecodeSlot(
				ImplAAFSegment **result)
{
#if FULL_TOOLKIT
	AAFSegment *			seg = NULL;
	AAFIterate *		slotIter = NULL;
	aafNumSlots_t		numSlots;
	aafInt32				loop;
	AAFMobSlot *		slot = NULL;
	aafBool				found = FALSE;
	AAFDataKind *      datakind = NULL;
	AAFRESULT          tmpError = AAFRESULT_SUCCESS;
	
	XPROTECT(_file)
	{
		slotIter = CreateImpl(CLSID_AAFIterate(_file);
		CHECK(GetNumSlots(&numSlots));
		for (loop = 1; loop <= numSlots; loop++)
		{
			CHECK(slotIter->MobGetNextSlot(this, NULL, &slot));
			CHECK(slot->GetSegment(&seg));
			CHECK(seg->GetDatakind(&datakind));
			if (datakind->IsTimecodeKind(kExactMatch, &tmpError))
			  {
				 *result = seg;
				 found = TRUE;
				 break;
			  }
			/* Release Bento reference, so the useCount is decremented */
			if (slot)
			  {
				 slot->ReleaseObject();	
				 slot = NULL;
			  }
		 } /* for */
		delete slotIter;
		slotIter = NULL;
		if(!found)
			RAISE(OM_ERR_NO_TIMECODE);
	} /* XPROTECT */
	XEXCEPT
	{
		if(XCODE() == OM_ERR_NO_MORE_OBJECTS)
			RERAISE(OM_ERR_NO_TIMECODE);
		if(slotIter != NULL)
			delete slotIter;
		*result = NULL;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

extern "C" const aafClassID_t CLSID_AAFSourceMob;

OMDEFINE_STORABLE(ImplAAFSourceMob, CLSID_AAFSourceMob);


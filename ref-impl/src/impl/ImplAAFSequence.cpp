/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "ImplAAFTimecode.h"
#include "ImplAAFComponent.h"
#include "ImplAAFSegment.h"
#include "ImplAAFDataDef.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFSequence.h"
#include "ImplAAFObjectCreation.h"


#include <assert.h>
#include "AAFResult.h"
#include "aafCvt.h"


extern "C" const aafClassID_t CLSID_EnumAAFComponents;

ImplAAFSequence::ImplAAFSequence ()
:   _components(			PID_SEQUENCE_COMPONENTS,		"Components")
{
	_persistentProperties.put(_components.address());
}

ImplAAFSequence::~ImplAAFSequence ()
{}

//***********************************************************
//
// SetInitialValue()
//
// This function creates a new sequence object with the given
// property values.  The length of the sequence is initially set to
// 0.  When components are appended to the sequence with the
// AppendCpnt() call, the length of the appended component is
// added to the length of the sequence.
// 
// Succeeds if all of the following are true:
// - the pDataDef pointer is valid.
// 
// If this method fails no state is changed.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pDataDef is null.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::SetInitialValue (aafUID_t * pDatadef)
{
	HRESULT	hr;

	hr = SetDataDef(pDatadef);

	return hr;
}

//***********************************************************
//
// AppendCpnt()
//
// This function appends the input component to the given sequence,
// enforcing bottom up creation of mobs.  The length of the sequence
// is incremented by the size of the component, unless the component
// is a transition.  If the component is a transition, it verifies
// that it is not the first object in a transition, and that it is
// not neighboring another transition.  It also verifies that there
// is enough source material on either side of the transition.  The
// function also verifies that the datakinds are compatible.
// 
// Succeeds if all of the following are true:
// - the -pComponent pointer is valid.
// 
// If this method fails no state is changed.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pComponent is null.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::AppendCpnt (ImplAAFComponent* pComponent)
{
	size_t				numCpnts;
	aafLength_t			sequLen, cpntLen, prevLen;
	aafUID_t			sequDataDef, cpntDataDef;
	aafBool				isPrevTran = AAFFalse;
	aafErr_t			aafError = AAFRESULT_SUCCESS;
	implCompType_t		type;

	if (pComponent == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(GetDataDef(&sequDataDef));
		CHECK(GetLength(&sequLen));
		CHECK(pComponent->GetDataDef(&cpntDataDef));
		CHECK(pComponent->GetLength(&cpntLen));

#if 0
		if (_file->IsSemanticCheckingEnabled())
		{
			aafUniqueName_t		datakindName;

			/* Verify that cpnt's datakind converts to sequence's datakind */
			CHECK(sequDatakind->GetName(OMUNIQUENAME_SIZE, datakindName));
			if (!cpntDatakind->DoesDatakindConvertTo(datakindName, &aafError))
			{
				RAISE(OM_ERR_INVALID_DATAKIND);
			}
		} /* semanticCheckEnable */
#endif

		/* Get the previous element in the sequence to verify neighboring
		 * transitions and source clip lengths.
		 */
		_components.getSize(numCpnts);
		if (numCpnts)
		{
			ImplAAFComponent*	pPrevCpnt = NULL;

		    _components.getValueAt(pPrevCpnt, numCpnts - 1);
			pPrevCpnt->GetComponentType(&type);
			if (type == kTransition)
			{
				isPrevTran = AAFTrue;
			}
			CHECK(pPrevCpnt->GetLength(&prevLen));

			// TODO: Release reference
			//pPrevCpnt->ReleaseObject();
		}

		/* Is the newly appended component a transition? */
		pComponent->GetComponentType(&type);
	    if (type == kTransition)
		{
			if (isPrevTran) 
			{
				RAISE(AAFRESULT_ADJACENT_TRAN);
			}
			else if (numCpnts == 0) /* It is the first cpnt in the sequ */
			{
				RAISE(AAFRESULT_LEADING_TRAN);
			}
			else
			{
				/* Verify that previous SCLP is at least as long as the tran */
				if (Int64Less(prevLen, cpntLen))
				{
					RAISE(AAFRESULT_INSUFF_TRAN_MATERIAL);
				}
			}

			SubInt64fromInt64(cpntLen, &sequLen);
			CHECK(SetLength(&sequLen));
		}
		else /* Not a transition */
		{
			if (isPrevTran)
			{
				/* Verify that length is at least as long as the prev tran */
				if (Int64Less(cpntLen, prevLen))
				{
					RAISE(AAFRESULT_INSUFF_TRAN_MATERIAL);
				}
			}
			/* Add length of component to sequence, if not transition */
			AddInt64toInt64(cpntLen, &sequLen);
			CHECK(SetLength(&sequLen));
		}

		/* If it all checks out, append the component to the sequence */
	    _components.appendValue(pComponent);
	}
	XEXCEPT
	{
		return(XCODE());
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}



//***********************************************************
//
// RemoveCpnt()
//
// This function removes the input component from the given
// sequence, deleting it from any associated AAF file.  The length
// of the sequence is decremented by the size of the component.
// 
// Succeeds if all of the following are true:
// - the -pComponent pointer is valid.
// 
// If this method fails no state is changed.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pComponent is null.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::RemoveCpnt (ImplAAFComponent* pComponent)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



//***********************************************************
//
// GetNumCpnts()
//
// // This function returns the number of components in the sequence.
// 
// Succeeds if all of the following are true:
// - the pNumCpnts pointer is valid.
// 
// If this method fails no state is changed.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pNumCpnts is null.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::GetNumCpnts (aafInt32*  pNumCpnts)
{
	size_t	numCpnts;

	_components.getSize(numCpnts);
	*pNumCpnts = numCpnts;

	return AAFRESULT_SUCCESS;
}



//***********************************************************
//
// EnumComponents()
//
// Places an enumerator for the components contained in the sequence
// into the *ppEnum argument.
// 
// Succeeds if all of the following are true:
// - the ppEnum pointer is valid.
// 
// If this method fails nothing will be written to *ppEnum.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppEnum is null.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::EnumComponents (ImplEnumAAFComponents ** ppEnum)
{
	ImplEnumAAFComponents	*theEnum = (ImplEnumAAFComponents *)CreateImpl (CLSID_EnumAAFComponents);
		
	XPROTECT()
	{
		CHECK(theEnum->SetEnumSequence(this));
		CHECK(theEnum->Reset());
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}



//***********************************************************
//
// SegmentOffsetToTC()
//
// Converts the given Segment offset to timecode.
// 
// Succeeds if all of the following are true:
// - the pOffset pointer is valid.
// - the pTimeCode pointer is valid.
// 
// If this method fails the value of pTimecode is left unchanged.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - any arg is NULL.
// 
// Override from AAFSegment
AAFRESULT STDMETHODCALLTYPE
ImplAAFSequence::SegmentOffsetToTC (/*[in]*/ aafPosition_t *  /*pOffset*/,
  /*[out]*/ aafTimecode_t *  /*pTimecode*/)
{
#if FULL_TOOLKIT
	AAFIterate		*sequIter = NULL;
	aafPosition_t	sequPos;
	AAFTimecode	*subSegment = NULL;
	AAFObject* tmpSlot = NULL;
	aafErr_t aafError = AAFRESULT_SUCCESS;
	aafLength_t		zeroLen = CvtInt32toLength(0, zeroLen);
	aafUInt32			frameOffset;	
	aafLength_t	junk;

	CvtInt32toInt64(0, &junk);
	XPROTECT(_file)
	{
		CHECK(OffsetToTimecodeClip(offset, &subSegment, &sequPos));
		if(subSegment != NULL)
		{
 			*found = TRUE;
 			CHECK(subSegment->GetTimecode(tc));
			SubInt64fromInt64(sequPos, &offset);
			CHECK(TruncInt64toUInt32(offset, &frameOffset));
			tc->startFrame += frameOffset;
		}
	} /* XPROTECT */
	XEXCEPT
	{
	  if (sequIter)
			delete sequIter;
	}
	XEND;

	return AAFRESULT_SUCCESS;
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

//***********************************************************
//
// SegmentTCToOffset()
//
// Converts the given Timecode to an Offset.
// 
// Succeeds if all of the following are true:
// - the pTimeCode pointer is valid.
// - the pEditRate pointer is valid
// - the pFrameOffset pointer is valid.
// 
// If this method fails the value of pOffset is left unchanged.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - any arg is NULL.
// 
// Override from AAFSegment
AAFRESULT STDMETHODCALLTYPE
ImplAAFSequence::SegmentTCToOffset (/*[in]*/ aafTimecode_t *  /*pTimecode*/,
  /*[in]*/ aafRational_t *  /*pEditRate*/,
  /*[out]*/ aafFrameOffset_t *  /*pOffset*/)
{
#if FULL_TOOLKIT
	aafTimecode_t	startTC;
	AAFIterate *sequIter = NULL;
	aafInt32		sequLoop, numSegs, segLen32, segStart, start32, frameOffset;
	aafPosition_t	sequPos;
	AAFComponent *subComponent;
	AAFSegment *subSegment;
	aafErr_t	aafError = OM_ERR_NONE;
	AAFPulldown *pdwn = NULL;
	aafLength_t	segLen, tcLen;
	AAFTimecode	*tccp;
	aafFrameOffset_t	begPos, endPos;
	aafPosition_t newStart, oldStart;
	aafFindSourceInfo_t	sourceInfo;
	aafLength_t	junk;
	aafBool		isMask;

	CvtInt32toInt64(0, &junk);
	XPROTECT(_file)
	{
		segStart = 0;

		sequIter = new AAFIterate(_file);
		CHECK(GetNumCpnts(&numSegs));
		for (sequLoop=0; sequLoop < numSegs; sequLoop++)
		{
			CHECK(sequIter->SequenceGetNextCpnt(this,
											NULL, &sequPos, &subComponent));
			subSegment = (AAFSegment *)subComponent;
			CHECK(subSegment->TraverseToClip(junk, &subSegment, NULL,
					 NULL, &junk, &isMask));
			CHECK(subSegment->GetLength(&segLen));
  			CHECK(TruncInt64toInt32(segLen, &segLen32));	/* OK FRAMEOFFSET */
			/* Skip zero-length clips, sometimes found in MC files */
			if (segLen32 == 0)
				continue;
	 		tccp = (AAFTimecode *)subSegment;
	 		CHECK(tccp->GetTimecode(&startTC));
			begPos = startTC.startFrame;
			endPos = startTC.startFrame + segLen32;
			if ((tc.startFrame < endPos) && (begPos <= tc.startFrame))
			{
  				CHECK(TruncInt64toInt32(sequPos, &segStart));	/* OK FRAMEOFFSET */
				CHECK(tccp->GetLength(&tcLen));
				*found = TRUE;
				break;
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

		if(found)
		{
					  /* Assume found at this point, so finish generating result */
			CvtInt32toInt64((tc.startFrame - startTC.startFrame) + segStart , &oldStart);
			 CHECK(AAFConvertEditRate(editRate, oldStart,
			  							sourceInfo.editrate , kRoundFloor, &newStart));
			CHECK(TruncInt64toInt32(sourceInfo.position, &frameOffset));	/* OK FRAMEOFFSET */
			CHECK(TruncInt64toInt32(newStart, &start32));		/* OK FRAMEOFFSET */
			*offset = start32 - frameOffset;

		   /* check for out of bound timecode */
		   if (tc.startFrame < startTC.startFrame) 
		   {
				  /* out of left bound */
				RAISE(AAFRESULT_BADSAMPLEOFFSET);
		   }
		   else
		   {
				aafUInt32 len;
				CHECK(TruncInt64toUInt32(tcLen, &len));
				if (tc.startFrame > (startTC.startFrame + len))
				{
						/* out of right bound */
					 RAISE(AAFRESULT_BADSAMPLEOFFSET);
				}
		   }
		}
	 }
	XEXCEPT
	{
	  if (sequIter)
		delete sequIter;
	}
	XEND;

	return AAFRESULT_SUCCESS;
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

// Internal to the toolkit functions
AAFRESULT
    ImplAAFSequence::GetNthComponent (aafInt32 index /*0-based*/, ImplAAFComponent **ppComponent)
{
	ImplAAFComponent	*obj;

	_components.getValueAt(obj, index);
	*ppComponent = obj;

	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_AAFSequence;

OMDEFINE_STORABLE(ImplAAFSequence, CLSID_AAFSequence);


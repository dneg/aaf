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
#include "AAFUtils.h"


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
	return (SetDataDef(pDatadef));
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

		// Verify that cpnt's datakind converts to sequence's datakind
		if (memcmp(&sequDataDef, &cpntDataDef, sizeof(aafUID_t)) != 0)
			RAISE(AAFRESULT_INVALID_DATAKIND);

		// Get the previous element in the sequence to verify neighboring
		// transitions and source clip lengths.
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
		}

		// Is the newly appended component a transition?
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
				// Verify that previous SCLP is at least as long as the tran
				if (Int64Less(prevLen, cpntLen))
				{
					RAISE(AAFRESULT_INSUFF_TRAN_MATERIAL);
				}
			}

			SubInt64fromInt64(cpntLen, &sequLen);
			CHECK(SetLength(&sequLen));
		}
		else // Not a transition
		{
			if (isPrevTran)
			{
				// Verify that length is at least as long as the prev tran
				if (Int64Less(cpntLen, prevLen))
				{
					RAISE(AAFRESULT_INSUFF_TRAN_MATERIAL);
				}
			}

			// Add length of component to sequence, if not transition
			AddInt64toInt64(cpntLen, &sequLen);
			CHECK(SetLength(&sequLen));
		}

		// If it all checks out, append the component to the sequence
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
	ImplEnumAAFComponents*	theEnum;
	HRESULT					hr;
	
	theEnum = (ImplEnumAAFComponents *)CreateImpl(CLSID_EnumAAFComponents);
	if (theEnum == NULL)
		return E_FAIL;
		
	hr = theEnum->SetEnumSequence(this);
	if (SUCCEEDED(hr))
	{
		theEnum->Reset();
		*ppEnum = theEnum;
	}
	else
	{
		theEnum->ReleaseRef();
		*ppEnum = NULL;
	}

	return hr;
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
ImplAAFSequence::SegmentOffsetToTC (aafPosition_t*  pOffset,
									aafTimecode_t*  pTimecode)
{
	ImplAAFTimecode*	pTC;
	aafPosition_t		sequPos;
	aafUInt32			frameOffset;	
	HRESULT				hr = AAFRESULT_SUCCESS;

	if (pOffset == NULL || pTimecode == NULL)
		return AAFRESULT_NULL_PARAM;

	hr = OffsetToTimecodeClip(*pOffset, &pTC, &sequPos);
	if(SUCCEEDED(hr))
	{
		aafTimecode_t timecode;

 		hr = pTC->GetTimecode(&timecode);
		if (SUCCEEDED(hr))
		{
			aafPosition_t	offset = *pOffset;

			SubInt64fromInt64(sequPos, &offset);
			TruncInt64toUInt32(offset, &frameOffset);
			timecode.startFrame += frameOffset;

			*pTimecode = timecode;
		}
		pTC->ReleaseRef();
	}

	return hr;
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
ImplAAFSequence::SegmentTCToOffset (aafTimecode_t*		pTimecode,
									aafRational_t*		pEditRate,
									aafFrameOffset_t*	pOffset)
{
	aafTimecode_t		startTC;
	size_t				index, numCpnts;
	aafInt32			segStart;
	ImplAAFComponent*	pComponent;
	ImplAAFSegment*		pSegment;
	HRESULT				hr = AAFRESULT_SUCCESS;
	aafLength_t			segLen, tcLen;
	ImplAAFTimecode*	pTC;
	aafFrameOffset_t	begPos, endPos;
	aafPosition_t		sequPos;
	aafBool				found = AAFFalse;
	aafLength_t			junk;

	if (pOffset == NULL || pTimecode == NULL || pEditRate == NULL)
		return AAFRESULT_NULL_PARAM;

	segStart = 0;
	CvtInt32toInt64(0, &junk);

	_components.getSize(numCpnts);
	for (index=0; index < numCpnts; index++)
	{
		ImplAAFSegment*	pSubSegment;
		aafBool			isMask;
		implCompType_t	type;

		_components.getValueAt(pComponent, index);

		pComponent->GetComponentType(&type);
		if (type == kTransition)
			continue;

		pSegment = static_cast<ImplAAFSegment*> (pComponent);

		/// !!!Jeff B - this needs some work for FILM
		hr = pSegment->TraverseToClip(junk, &pSubSegment, NULL, NULL, &junk, &isMask);
		if (SUCCEEDED(hr))
		{
			hr = pSubSegment->GetLength(&segLen);
			if (SUCCEEDED(hr))
			{
				aafInt32	segLen32 = 0;

  				TruncInt64toInt32(segLen, &segLen32);	// OK FRAMEOFFSET
				if (segLen32 == 0)
					continue;		// Skip zero-length clips, sometimes found in MC files

	 			pTC = (ImplAAFTimecode *)pSubSegment;
	 			pTC->GetTimecode(&startTC);
				begPos = startTC.startFrame;
				endPos = startTC.startFrame + segLen32;
				if ((pTimecode->startFrame < endPos) && (begPos <= pTimecode->startFrame))
				{
					pComponent->AccumulateLength(&sequPos);
  					TruncInt64toInt32(sequPos, &segStart);	// OK FRAMEOFFSET
					pTC->GetLength(&tcLen);
					found = AAFTrue;
					break;
				}
			}
			pSubSegment->ReleaseRef();
		}
	}
	
	if (found)
	{
		aafPosition_t	newStart, oldStart;
		aafInt32		start32, frameOffset;

		// Assume found at this point, so finish generating result
		CvtInt32toInt64((pTimecode->startFrame - startTC.startFrame) + segStart , &oldStart);
		hr = AAFConvertEditRate(*pEditRate, oldStart, *pEditRate, kRoundFloor, &newStart);
		TruncInt64toInt32(sequPos, &frameOffset);	// OK FRAMEOFFSET
		TruncInt64toInt32(newStart, &start32);		// OK FRAMEOFFSET
		*pOffset = start32 - frameOffset;

		// check for out of bound timecode
		if (pTimecode->startFrame < startTC.startFrame) 
		{
			// out of left bound 
			hr = AAFRESULT_BADSAMPLEOFFSET;
		}
		else
		{
			aafUInt32 len;

			TruncInt64toUInt32(tcLen, &len);
			if (pTimecode->startFrame > (startTC.startFrame + len))
			{
				// out of right bound
				 hr = AAFRESULT_BADSAMPLEOFFSET;
			}
		}
	}
	else
		hr = AAFRESULT_TIMECODE_NOT_FOUND;

	return hr;
}

//***********************************************************
//
// GetNthComponent()
//
// Get the component at the position specified by index from
// the components vector.
//
// NOTES:
//
//    - The vector is 0-based.
//    - AddRef the object being returned.
// 
AAFRESULT
    ImplAAFSequence::GetNthComponent (aafUInt32 index, ImplAAFComponent** ppComponent)
{
	ImplAAFComponent*	obj;
	size_t				numCpnts;
	HRESULT				hr;

	_components.getSize(numCpnts);
	if (index < numCpnts)
	{
		_components.getValueAt(obj, index);
		obj->AcquireReference();
		*ppComponent = obj;
		hr =  AAFRESULT_SUCCESS;
	}
	else
		hr = AAFRESULT_NO_MORE_OBJECTS;

	return hr;
}

extern "C" const aafClassID_t CLSID_AAFSequence;

OMDEFINE_STORABLE(ImplAAFSequence, CLSID_AAFSequence);


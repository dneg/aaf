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
{
	ImplAAFComponent *pComp = NULL;
	size_t size;

	_components.getSize(size);
	for (size_t i = 0; i < size; i++) {
		_components.getValueAt(pComp, i);

		if (pComp) {
			pComp->ReleaseReference();
			// Set value to 0 so the OM can perform any necessary cleanup.
			pComp = 0;
			_components.setValueAt(pComp, i);
		}
	}
}

//***********************************************************
//
// Initialize()
//
// This function sets the properties on a newly created sequence
// object with the given property values. The length of the sequence is
// initially set to 0.  When components are appended to the sequence with
// the AppendComponent() call, the length of the appended component is
// added to the length of the sequence.
// 
// Succeeds if all of the following are true:
// - the pDatadef pointer is valid.
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
//   - pDatadef is null.
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::Initialize (aafUID_t * pDatadef)
{
	return (SetDataDef(pDatadef));
}

//***********************************************************
//
// AppendComponent()
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
// If the component is successfully appended to the sequence, the
// reference count of the component is incremented.
// 
// Succeeds if all of the following are true:
// - the pComponent pointer is valid.
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
// AAFRESULT_INVALID_DATAKIND
//   - The data kind of the component is not compatible with the 
//     data kind of the sequence.
//
// AAFRESULT_LEADING_TRAN
//   - Attempted to append a transition as the first component of
//     a sequence.  A sequence can not start with a transition. 
//
// AAFRESULT_ADJACENT_TRAN
//   - Attempted to append a transition next to a transition.  A
//     sequence can not contain back to back transitions.
//
// AAFRESULT_INSUFF_TRAN_MATERIAL
//   - There is not enough source material to add this component.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::AppendComponent (ImplAAFComponent* pComponent)
{
	size_t			numCpnts;
	aafLength_t		sequLen, cpntLen, prevLen;
	aafUID_t		sequDataDef, cpntDataDef;
	aafBool			isPrevTran = AAFFalse;
	aafErr_t		aafError = AAFRESULT_SUCCESS;
	implCompType_t	type;
	HRESULT			hr;

	if (pComponent == NULL)
		return AAFRESULT_NULL_PARAM;

	// Verify that component's datakind converts to sequence's datakind
	GetDataDef(&sequDataDef);
	pComponent->GetDataDef(&cpntDataDef);
	if (memcmp(&sequDataDef, &cpntDataDef, sizeof(aafUID_t)) != 0)
			return AAFRESULT_INVALID_DATAKIND;

	GetLength(&sequLen);
	pComponent->GetLength(&cpntLen);

	// Get the previous component in the sequence to verify
	// neighboring transitions and source clip lengths.
	_components.getSize(numCpnts);
	if (numCpnts)
	{
		ImplAAFComponent*	pPrevCpnt = NULL;

		_components.getValueAt(pPrevCpnt, numCpnts - 1);
		pPrevCpnt->GetLength(&prevLen);
		pPrevCpnt->GetComponentType(&type);
		if (type == kTransition)
			isPrevTran = AAFTrue;
	}

	// Is the newly appended component a transition?
	pComponent->GetComponentType(&type);
	if (type == kTransition)
	{
		if (isPrevTran) 
		{
			// Can not have back to back transitions in a sequence
			hr = AAFRESULT_ADJACENT_TRAN;
		}
		else if (numCpnts == 0)
		{
			 // A transition can not be the first component in a sequence
			hr = AAFRESULT_LEADING_TRAN;
		}
		else
		{
			// Verify that previous component is at least as long as the transition
			if (Int64Less(prevLen, cpntLen))
			{
				hr = AAFRESULT_INSUFF_TRAN_MATERIAL;
			}
		}

		SubInt64fromInt64(cpntLen, &sequLen);
		hr = SetLength(&sequLen);
	}
	else // Not a transition
	{
		if (isPrevTran)
		{
			// Verify that component length is at least as long as the prev transition
			if (Int64Less(cpntLen, prevLen))
			{
				hr = AAFRESULT_INSUFF_TRAN_MATERIAL;
			}
		}

		// Add length of component to sequence, if not transition
		AddInt64toInt64(cpntLen, &sequLen);
		hr = SetLength(&sequLen);
	}

	// If it all checks out, append the component to the sequence
	if (SUCCEEDED(hr))
	{
		pComponent->AcquireReference();
		_components.appendValue(pComponent);
	}

	return(hr);
}

//***********************************************************
//
// RemoveComponent()
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
    ImplAAFSequence::RemoveComponent (ImplAAFComponent* pComponent)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

//***********************************************************
//
// GetNumComponents()
//
// This function returns the number of components in the sequence.
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
    ImplAAFSequence::GetNumComponents (aafInt32*  pNumCpnts)
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
// Places an IEnumAAFComponents enumerator for the components contained in the sequence
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
// E_FAIL
//   - Failed to create the enumerator.
//
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
		theEnum->ReleaseReference();
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
	ImplAAFTimecode*	pTC = NULL;;
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
		pTC->ReleaseReference();
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
			pSubSegment->ReleaseReference();
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
// the components vector.  This is used by the ImplEnumAAFComponents
// to retrieve the components from the sequence.
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


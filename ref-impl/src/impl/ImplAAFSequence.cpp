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

#include "ImplAAFTimecode.h"
#include "ImplAAFComponent.h"
#include "ImplAAFSegment.h"
#include "ImplAAFDataDef.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFSequence.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFDictionary.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"
#include "aafCvt.h"
#include "AAFUtils.h"

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef>    ImplAAFDataDefSP;
typedef ImplAAFSmartPointer<ImplAAFDictionary> ImplAAFDictionarySP;


extern "C" const aafClassID_t CLSID_EnumAAFComponents;

ImplAAFSequence::ImplAAFSequence ()
:   _components(			PID_Sequence_Components,		"Components")
{
	_persistentProperties.put(_components.address());
}

ImplAAFSequence::~ImplAAFSequence ()
{
	size_t size = _components.getSize();
	for (size_t i = 0; i < size; i++) {
		ImplAAFComponent *pComp = _components.setValueAt(0, i);

		if (pComp) {
		  pComp->ReleaseReference();
		  pComp = 0;
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
    ImplAAFSequence::Initialize (ImplAAFDataDef * pDataDef)
{
  if (! pDataDef)
	return AAFRESULT_NULL_PARAM;

  return (SetDataDef(pDataDef));
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
// function also verifies that the datadefs are compatible.
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
// AAFRESULT_INVALID_DATADEF
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
// AAFRESULT_OBJECT_ALREADY_ATTACHED
//   - Attempted to append a component that is already attached to
//     (owned by) another object.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::AppendComponent (ImplAAFComponent* pComponent)
{
	size_t			numCpnts;
	aafLength_t		sequLen, cpntLen, prevLen;
	ImplAAFDataDefSP sequDataDef, cpntDataDef;
	aafBool			isPrevTran = kAAFFalse, willConvert;
	aafErr_t		aafError = AAFRESULT_SUCCESS;
	implCompType_t	type;
	ImplAAFDictionary	*pDict = NULL;
	AAFRESULT		status, sclpStatus;

	if (pComponent == NULL)
		return AAFRESULT_NULL_PARAM;
	
	if (pComponent->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	XPROTECT()
	{
		// Verify that component's datadef converts to sequence's datadef
		if(GetDataDef(&sequDataDef) == AAFRESULT_SUCCESS)
		{
			pComponent->GetDataDef(&cpntDataDef);
			CHECK(cpntDataDef->DoesDataDefConvertTo(sequDataDef, &willConvert));
		
			if (willConvert == kAAFFalse)
				RAISE(AAFRESULT_INVALID_DATADEF);
		}
		else
			SetDataDef(cpntDataDef);
		
		status = GetLength(&sequLen);
		if(status == AAFRESULT_PROP_NOT_PRESENT /*AAFRESULT_BAD_PROP ???*/)
			sequLen = 0;
		else
		{
			CHECK(status);
		}
		
		// Here we have 4 cases:
		// 1) Sequence does not have a length, component DOES have a length
		//		Add component and set length on the sequence
		// 2) Sequence does not have a length, component does NOT have a length
		//		Add component without setting length on the sequence
		// 3) Sequence has a length, component DOES have a length
		//		Add component and adjust length on the sequence
		// 4) Sequence has a length, component does NOT have a length
		//		Add zero-length component and set length on the sequence
		sclpStatus = pComponent->GetLength(&cpntLen);
		if(sclpStatus == AAFRESULT_PROP_NOT_PRESENT /*AAFRESULT_BAD_PROP ???*/ && status == AAFRESULT_SUCCESS)
		{
			// Case #4
			sequLen = 0;
			sclpStatus = AAFRESULT_SUCCESS;
		}
		if(sclpStatus != AAFRESULT_PROP_NOT_PRESENT /*AAFRESULT_BAD_PROP??? */)
		{
			// Make it here on cases #1, #3, and #4
			CHECK(sclpStatus);
			// Get the previous component in the sequence to verify
			// neighboring transitions and source clip lengths.
			_components.getSize(numCpnts);
			if (numCpnts)
			{
				ImplAAFComponent*	pPrevCpnt = NULL;
				
				_components.getValueAt(pPrevCpnt, numCpnts - 1);
				CHECK(pPrevCpnt->GetLength(&prevLen));
				pPrevCpnt->GetComponentType(&type);
				if (type == kTransition)
					isPrevTran = kAAFTrue;
			}
			
			// Is the newly appended component a transition?
			pComponent->GetComponentType(&type);
			if (type == kTransition)
			{
				if (isPrevTran) 
				{
					// Can not have back to back transitions in a sequence
					RAISE(AAFRESULT_ADJACENT_TRAN);
				}
				else if (numCpnts == 0)
				{
					// A transition can not be the first component in a sequence
					RAISE(AAFRESULT_LEADING_TRAN);
				}
				else
				{
					// Verify that previous component is at least as long as the transition
					if (Int64Less(prevLen, cpntLen))
					{
						RAISE(AAFRESULT_INSUFF_TRAN_MATERIAL);
					}
				}
				
				SubInt64fromInt64(cpntLen, &sequLen);
				CHECK(SetLength(sequLen));
			}
			else // Not a transition
			{
				if (isPrevTran)
				{
					// Verify that component length is at least as long as the prev transition
					if (Int64Less(cpntLen, prevLen))
					{
						RAISE(AAFRESULT_INSUFF_TRAN_MATERIAL);
					}
				}
				
				// Add length of component to sequence, if not transition
				AddInt64toInt64(cpntLen, &sequLen);
				CHECK(SetLength(sequLen));
			}
		}
		// Else handle case #2
		
		// If it all checks out, append the component to the sequence
		pComponent->AcquireReference();
		_components.appendValue(pComponent);
	}
	XEXCEPT
	{
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::PrependComponent (ImplAAFComponent* pComponent)
{
  if (! pComponent) return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::InsertComponentAt (aafUInt32 index,
										ImplAAFComponent* pComponent)
{
  if (! pComponent) return AAFRESULT_NULL_PARAM;

  aafUInt32 count;
  AAFRESULT hr;
  hr = CountComponents (&count);
  if (AAFRESULT_FAILED (hr)) return hr;
  if (index > count)
	return AAFRESULT_BADINDEX;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::GetComponentAt (aafUInt32 index,
									 ImplAAFComponent ** ppComponent)
{
  if (! ppComponent) return AAFRESULT_NULL_PARAM;

  aafUInt32 count;
  AAFRESULT hr;
  hr = CountComponents (&count);
  if (AAFRESULT_FAILED (hr)) return hr;
  if (index >= count)
	return AAFRESULT_BADINDEX;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::RemoveComponentAt (aafUInt32 index)
{
  aafUInt32 count;
  AAFRESULT hr;
  hr = CountComponents (&count);
  if (AAFRESULT_FAILED (hr)) return hr;
  if (index >= count)
	return AAFRESULT_BADINDEX;

	ImplAAFComponent *pComp = NULL;
	pComp = 	_components.removeAt(index);
	if (pComp)
	{
		// We have removed an element from a "stong reference container" so we must
		// decrement the objects reference count. This will not delete the object
		// since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
		pComp->ReleaseReference ();
	}
	return AAFRESULT_SUCCESS;
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
	if (!_components.containsValue(pComponent))
	  return AAFRESULT_BADINDEX;

	_components.removeValue(pComponent);
	pComponent->ReleaseReference();

	return AAFRESULT_SUCCESS;
}

//***********************************************************
//
// CountComponents()
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
    ImplAAFSequence::CountComponents (aafUInt32 * pNumCpnts)
{
  if (! pNumCpnts) return AAFRESULT_NULL_PARAM;

	size_t	numCpnts;

	_components.getSize(numCpnts);
	*pNumCpnts = numCpnts;

	return AAFRESULT_SUCCESS;
}

//***********************************************************
//
// GetComponents()
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
    ImplAAFSequence::GetComponents (ImplEnumAAFComponents ** ppEnum)
{
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFComponents *)CreateImpl(CLSID_EnumAAFComponents);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);
	(*ppEnum)->SetEnumStrongProperty(this, &_components);

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
		pTC = 0;
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
	aafBool				found = kAAFFalse;
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
					found = kAAFTrue;
					break;
				}
			}
			pSubSegment->ReleaseReference();
			pSubSegment = 0;
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

AAFRESULT ImplAAFSequence::ChangeContainedReferences(aafMobID_constref from,
													 aafMobID_constref to)
{
	aafUInt32			n, count;
	ImplAAFComponent	*comp = NULL;
	
	XPROTECT()
	{
		CHECK(CountComponents (&count));
		for(n = 0; n < count; n++)
		{
			CHECK(GetNthComponent (n, &comp));
			CHECK(comp->ChangeContainedReferences(from, to));
			comp->ReleaseReference();
			comp = NULL;
		}
	}
	XEXCEPT
	{
		if(comp != NULL)
		  comp->ReleaseReference();
		comp = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

//SDK Internal
AAFRESULT
    ImplAAFSequence::SetNthComponent (aafUInt32 index, ImplAAFComponent* pComponent)
{
	size_t				numCpnts;
	HRESULT				hr;

	_components.getSize(numCpnts);
	if (index < numCpnts)
	{
		_components.setValueAt(pComponent, index);
		pComponent->AcquireReference();
		hr =  AAFRESULT_SUCCESS;
	}
	else
		hr = AAFRESULT_NO_MORE_OBJECTS;

	return hr;
}


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFTimecode.h"
#include "ImplAAFComponent.h"
#include "ImplAAFSegment.h"
#include "ImplAAFDataDef.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFSequence.h"
#include "ImplAAFSourceClip.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFEvent.h"
#include "ImplAAFTransition.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "OMAssertions.h"
#include "AAFResult.h"
#include "AAFUtils.h"

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef>    ImplAAFDataDefSP;
typedef ImplAAFSmartPointer<ImplAAFDictionary> ImplAAFDictionarySP;

#include <typeinfo>

extern "C" const aafClassID_t CLSID_EnumAAFComponents;

ImplAAFSequence::ImplAAFSequence ()
:   _components(			PID_Sequence_Components,		L"Components")
{
	_persistentProperties.put(_components.address());
}

ImplAAFSequence::~ImplAAFSequence ()
{
	aafUInt32 count = _components.count();
	for (aafUInt32 i = 0; i < count; i++) {
		ImplAAFComponent *pComp = _components.clearValueAt(i);

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
// AAFRESULT_EVENT_SEMANTICS
//   - Attempted to append an event to a non-event sequence or vice versa.
//	 - Attempted to append an event but an event type mismatched was found.
//   - Attempted to append an event that did not repect ordering conventions.
//
//
 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::AppendComponent (ImplAAFComponent* pComponent)
{
	aafUInt32 count=0;
	AAFRESULT ar = AAFRESULT_SUCCESS;
	ar = CountComponents (&count);
	if (AAFRESULT_FAILED (ar)) 
	{
		return ar;
	}
	return InsertComponentAt(count, pComponent);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::PrependComponent (ImplAAFComponent* pComponent)
{
	if(!pComponent)
		return(AAFRESULT_NULL_PARAM);

	if (pComponent->attached())
			return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	return  InsertComponentAt(0, pComponent);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::InsertComponentAt (aafUInt32 index,
										ImplAAFComponent* pComponent)
{
  if (!pComponent) 
    return AAFRESULT_NULL_PARAM;

  aafUInt32 count=0;
  AAFRESULT ar = AAFRESULT_SUCCESS;
  ar = CountComponents (&count);
  if (AAFRESULT_FAILED (ar)) 
  {
	  return ar;
  }

  if (index > count)
  {
	return AAFRESULT_BADINDEX;
  }

  if (pComponent->attached())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

  ImplAAFDataDefSP sequDataDef, cpntDataDef;
  aafBool			willConvert = kAAFFalse;

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
		SetDataDef(cpntDataDef); //This should never execute because the sequence the datadef must be set before manipulating the sequence.

	ImplAAFEvent* pEvent = dynamic_cast<ImplAAFEvent*>( pComponent );

	//index > count means an index = 0 and a count = 0, i.e. insert into an epmty list at index 0 is allowed.
	if ( 0 == count ) {

			CHECK( CheckFirstComponentSematics( pComponent ) );

			if ( pEvent ) {
				CHECK( UpdateSequenceLength(pEvent, index) ); 
			}
			else {
				CHECK( UpdateSequenceLength( pComponent ) );
			}
		
		}
		else if ( dynamic_cast<ImplAAFEvent*>( GetFirstComponent() ) ) {

			if(!pEvent)
			{
				return AAFRESULT_EVENT_SEMANTICS;
			}
			CHECK( CheckTypeSemantics( pEvent ) ); 
			CHECK( CheckPositionSemantics( pEvent, index ) ); 
			CHECK( CheckLengthSemantics( pEvent ) ); 
			CHECK( UpdateSequenceLength( pEvent, index ) );
		}
		else {

			if(pEvent)
			{
				return AAFRESULT_EVENT_SEMANTICS;
			}
			CHECK( CheckTypeSemantics( pComponent, index) );
			CHECK( CheckPositionSemantics( pComponent ) );
			CHECK( CheckLengthSemantics( pComponent ) );
			CHECK( UpdateSequenceLength( pComponent ) );
		}



	_components.insertAt(pComponent,index);
	pComponent->AcquireReference();
  }
  XEXCEPT
  {
  }
  XEND;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::GetComponentAt (aafUInt32 index,
									 ImplAAFComponent ** ppComponent)
{
  if (!ppComponent) 
    return AAFRESULT_NULL_PARAM;

  aafUInt32 count;
  AAFRESULT ar;
  ar = CountComponents (&count);
  if (AAFRESULT_FAILED (ar)) return ar;
  if (index >= count)
	return AAFRESULT_BADINDEX;

  ImplAAFComponent *pComponent;
  _components.getValueAt(pComponent,index);

  ASSERTU(pComponent);
  pComponent->AcquireReference();
  (*ppComponent)=pComponent;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSequence::RemoveComponentAt (aafUInt32 index)
{
  aafUInt32 count = 0;
  AAFRESULT hr = AAFRESULT_SUCCESS;
  hr = CountComponents (&count);
  if (AAFRESULT_FAILED (hr)) return hr;
  if (index >= count)
	return AAFRESULT_BADINDEX;

	ImplAAFComponent *pComp = NULL;
	pComp = 	_components.removeAt(index);
	ImplAAFEvent* pEvent = 0;
	pEvent = dynamic_cast<ImplAAFEvent*>( pComp );

	if (pComp)
	{
		if(count == 1)
		{
			//If count was 1 and we successfully removed an item, the list is now empty.
			aafLength_t dummy;
			//In order eveluation of and will not call GetLenght when pEvent is 0.
			if (pEvent != 0 && 
				AAFRESULT_PROP_NOT_PRESENT == pEvent->GetLength(&dummy) )
 		    {
				//If the event didn't have length defined, do noting with the sequence lenght.
				//If the sequence had a component with length, which was previously removed,
				//and then a component without lenght is removed and there are no components
				//left in the sequence, should its length be 0 or undefined?
			}
			else hr = SetLength( 0 );
		}
		else
		{
			XPROTECT()
			{
				if ( dynamic_cast<ImplAAFEvent*>( GetFirstComponent() ) ) 
				{
					CHECK( UpdateSequenceLengthOnRemove( pEvent, index ) );
				}
				else 
				{
					CHECK( UpdateSequenceLengthOnRemove( pComp) );
				}
			}
			XEXCEPT
			{
			}
			XEND;
		}
		// We have removed an element from a "stong reference container" so we must
		// decrement the objects reference count. This will not delete the object
		// since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
		pComp->ReleaseReference ();
	}

	return hr;
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
	AAFRESULT status;

	if (!_components.containsValue(pComponent))
	  return AAFRESULT_BADINDEX;

	if (!pComponent->attached())
	  return AAFRESULT_OBJECT_NOT_ATTACHED;

	OMUInt32 index; 
	if(_components.findIndex(pComponent, index))
	{
		status = RemoveComponentAt(index);
	}
	else 
	{
		status = AAFRESULT_PARAMETER_NOT_FOUND;
	}

	return status;
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

	aafUInt32 numCpnts = _components.count();
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
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFComponents *theEnum = (ImplEnumAAFComponents *)CreateImpl (CLSID_EnumAAFComponents);
	
  XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFComponent>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFComponent>(_components);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFComponents, this, iter));
	  *ppEnum = theEnum;
	}
  XEXCEPT
	{
	  if (theEnum)
		{
		  theEnum->ReleaseReference();
		  theEnum = 0;
		}
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
ImplAAFSequence::SegmentOffsetToTC (aafPosition_t*  pOffset,
									aafTimecode_t*  pTimecode)
{
	ImplAAFTimecode*	pTC = NULL;
	aafPosition_t		sequPos;
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

			offset -= sequPos;
			timecode.startFrame += offset;

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
	ImplAAFComponent*	pComponent;
	ImplAAFSegment*		pSegment;
	HRESULT				hr = AAFRESULT_SUCCESS;
	aafLength_t			segLen, tcLen;
	ImplAAFTimecode*	pTC;
	aafFrameOffset_t	begPos, endPos;
	aafPosition_t		sequPos, segStart;
	aafBool				found = kAAFFalse;
	aafLength_t			junk;

	if (pOffset == NULL || pTimecode == NULL || pEditRate == NULL)
		return AAFRESULT_NULL_PARAM;

	segStart = 0;
	junk = 0;

	aafUInt32 numCpnts = _components.count();
	for (aafUInt32 index=0; index < numCpnts; index++)
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
				if (segLen == 0)
					continue;		// Skip zero-length clips, sometimes found in MC files

	 			pTC = (ImplAAFTimecode *)pSubSegment;
	 			pTC->GetTimecode(&startTC);
				begPos = startTC.startFrame;
				endPos = startTC.startFrame + segLen;
				if ((pTimecode->startFrame < endPos) && (begPos <= pTimecode->startFrame))
				{
					pComponent->AccumulateLength(&sequPos);
  					segStart = sequPos;
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

		// Assume found at this point, so finish generating result
		oldStart = (pTimecode->startFrame - startTC.startFrame) + segStart;
		hr = AAFConvertEditRate(*pEditRate, oldStart, *pEditRate, kRoundFloor, &newStart);
		*pOffset = newStart - sequPos;

		// check for out of bound timecode
		if (pTimecode->startFrame < startTC.startFrame) 
		{
			// out of left bound 
			hr = AAFRESULT_BADSAMPLEOFFSET;
		}
		else
		{
			if (pTimecode->startFrame > (startTC.startFrame + tcLen))
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

//***************************************************************
//
// FindSubSeqment
//
// Gets a segment on the sequence. 
//
//
//
AAFRESULT ImplAAFSequence::FindSubSegment(aafPosition_t offset, 
										 aafPosition_t *sequPosPtr,
										 ImplAAFSegment **subseg,
										 aafBool *found)
{
	aafLength_t	segLen;
	aafPosition_t begPos, endPos, zero;
	aafUInt32 n = 0;
	ImplAAFSegment* seg = NULL;

	XPROTECT( )
	{
		CHECK(GetLength(&segLen));
		begPos = 0;
		zero = 0;
		endPos = begPos;
		endPos += segLen;
		if (begPos <= offset &&
			offset < endPos)
		{
			*found = kAAFFalse;
			*subseg = NULL;
			*sequPosPtr = 0;
			begPos = 0;
			endPos = begPos;
			CHECK(CountComponents(&n));
			for (aafUInt32 i = 0 ; *found != kAAFTrue && i < n ; i++) 
			{
				CHECK(GetComponentAt(i, (ImplAAFComponent**)&seg));
				CHECK(seg->GetLength(&segLen));
				endPos += segLen;
				if (begPos <= offset &&
					offset < endPos)
				{
					*found = kAAFTrue;
					*subseg = seg;
					// We are returning a reference to this object so bump the ref count
					seg->AcquireReference();
					*sequPosPtr = begPos;
				} 
				else 
				{
					begPos = endPos;
				}
				seg->ReleaseReference();
				seg = NULL;
			}
		}
		else if ((begPos == endPos) && (offset == zero)) 	//JeffB: Handle zero-length sourceClips
		{
			*found = kAAFTrue;
			*subseg = this;
			// We are returning a reference to this object so bump the ref count
			AcquireReference();
			*sequPosPtr = 0;
		}
		else
		{
			*found = kAAFFalse;
			*subseg = NULL;
			*sequPosPtr = 0;
		}
	} /* XPROTECT */
	XEXCEPT
	{
		if (seg) {
			seg->ReleaseReference();
			seg = NULL;
		}
	}
	XEND;

	return(AAFRESULT_SUCCESS);
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
	HRESULT				hr;

	size_t numCpnts = _components.count();
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
	HRESULT				hr;


	if( pComponent == NULL )
		return AAFRESULT_NULL_PARAM;

	size_t numCpnts = _components.count();
	if (index < numCpnts)
	{
		// Retreive the old component
		// If it's the same as one we are setting just return success.
		//
		ImplAAFComponent	*pOldComponent = NULL;
		_components.getValueAt( pOldComponent, index );
		ASSERTU(pOldComponent);
		if( pOldComponent != NULL && pOldComponent == pComponent )
			return AAFRESULT_SUCCESS;

		// Make sure the new component is not already in use.
		if (pComponent->attached())
			return AAFRESULT_OBJECT_ALREADY_ATTACHED;

		if( pOldComponent != NULL )
		    pOldComponent->ReleaseReference();

		_components.setValueAt(pComponent, index);
		pComponent->AcquireReference();
		hr =  AAFRESULT_SUCCESS;
	}
	else
		hr = AAFRESULT_NO_MORE_OBJECTS;

	return hr;
}

AAFRESULT ImplAAFSequence::TraverseToClip(aafLength_t length,
											ImplAAFSegment **sclp,
											ImplAAFPulldown ** /*pulldownObj*/,
											aafInt32 * /*pulldownPhase*/,
											aafLength_t *sclpLen,
											aafBool * /*isMask*/)
{
	XPROTECT()
	{
		aafUInt32	count = 0;
		CHECK(CountComponents( &count ));
		if( count != 1 )
		    return(AAFRESULT_TRAVERSAL_NOT_POSS);

		ImplAAFComponent *p_component = NULL;
		CHECK(GetComponentAt( 0, &p_component ));

		ImplAAFSourceClip *p_src_clip = dynamic_cast<ImplAAFSourceClip*>( p_component );
		if( !p_src_clip )
		{
		    p_component->ReleaseReference();
		    p_component = NULL;
		    return(AAFRESULT_TRAVERSAL_NOT_POSS);
		}

		*sclp = p_src_clip;

		CHECK((*sclp)->GetLength(sclpLen));
		if (length < *sclpLen)
		{
			*sclpLen = length;
		}
	} /* XPROTECT */
	
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

//***********************************************************
//

ImplAAFComponent* ImplAAFSequence::GetLastComponent()
{
	ImplAAFComponent* pComponent = 0;

	_components.getValueAt(pComponent, _components.count()-1 );	

	return pComponent;
}

ImplAAFComponent* ImplAAFSequence::GetFirstComponent()
{
	ImplAAFComponent* pComponent = 0;

	_components.getValueAt(pComponent, 0 );

	return pComponent;
}

AAFRESULT ImplAAFSequence::GetLastEvent(ImplAAFEvent*& pLastEvent )
{
	AAFRESULT status = AAFRESULT_SUCCESS;

	ImplAAFEvent* pEvent = dynamic_cast<ImplAAFEvent*>( GetLastComponent() );
	if ( pEvent ) {
		pLastEvent = pEvent;
	}
	else {
		status = AAFRESULT_EVENT_SEMANTICS;
	}

	return status;
}

AAFRESULT ImplAAFSequence::GetFirstEvent(ImplAAFEvent*& pFirstEvent )
{
	AAFRESULT status = AAFRESULT_SUCCESS;

	ImplAAFEvent* pEvent = dynamic_cast<ImplAAFEvent*>( GetFirstComponent() );
	if ( pEvent ) {
		pFirstEvent = pEvent;
	}
	else {
		status = AAFRESULT_EVENT_SEMANTICS;
	}
	
	return status;
}
                           
// This is static.
AAFRESULT ImplAAFSequence::GetOptionalComponentLength( ImplAAFComponent* pComponent, aafLength_t& refLength )
{
	AAFRESULT status;
	aafLength_t length = 0;

	status = pComponent->GetLength(&length);

	// Convert this to success
	if( AAFRESULT_PROP_NOT_PRESENT == status ) {
 	    refLength = 0;
		status = AAFRESULT_SUCCESS;
	}

	if (  AAFRESULT_SUCCESS == status ) {
		refLength = length;
	}

	return status;
}


// ImplAAFSequence private - First component sematics

AAFRESULT ImplAAFSequence::CheckFirstComponentSematics( ImplAAFComponent* pComponent )
{
	// The first component in a sequence must *not* be of type Transition.

	if ( dynamic_cast<ImplAAFTransition*>(pComponent) ) {
		return AAFRESULT_LEADING_TRAN;
	}

	return AAFRESULT_SUCCESS;
}

// ImplAAFSequence private - Event sematics

AAFRESULT ImplAAFSequence::CheckTypeSemantics( ImplAAFEvent* pEvent )
{
	// A sequence containing events must contain only events.
	// Further, these events must all be of the same type.

	// Haven't seen any rules against use of typeid(), and there
	// are already dynamic_casts all over the place... so let's
	// get with times here and use type_info equality to verify
	// concrete type identity.

// Ugly workaround for x86 optimisation bug in gcc-3.3.x
// The bug is not present in gcc-3.2.x or gcc-3.4.x
#if defined(__i386__) && __GNUC__ == 3 && __GNUC_MINOR__ == 3
	const char *comp_name = typeid(*GetLastComponent()).name();
	const char *event_name = typeid(*pEvent).name();
	if ( strcmp(comp_name, event_name) != 0 ) {
		return AAFRESULT_EVENT_SEMANTICS;
	}
#else
	if ( typeid( *GetLastComponent() ) != typeid( *pEvent) ) {
		return AAFRESULT_EVENT_SEMANTICS;
	}
#endif
	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFSequence::CheckPositionSemantics( ImplAAFEvent* pEvent, aafUInt32 index )
{
	//Events must be in order by position.

	AAFRESULT status;

	ImplAAFComponent* pComponentPrior = 0;
	ImplAAFComponent* pComponentNext = 0;
	ImplAAFEvent* pEventPrior = 0;
	ImplAAFEvent* pEventNext = 0;

	if(index > 0) _components.getValueAt(pComponentPrior, index-1 );
    //Existing values in this <c OMVector> at <p index> and higher are shifted up one index position on insert.
	if(_components.count() > index) _components.getValueAt(pComponentNext, index);

	if(pComponentPrior != 0)
	{
		pEventPrior = dynamic_cast<ImplAAFEvent*> (pComponentPrior);
		if(!pEventPrior) return AAFRESULT_EVENT_SEMANTICS;
	}

	if(pComponentNext != 0)
	{
		pEventNext = dynamic_cast<ImplAAFEvent*> (pComponentNext);
		if(!pEventNext) return AAFRESULT_EVENT_SEMANTICS;
	}

	aafPosition_t posNew;
	status = pEvent->GetPosition( &posNew );
	if ( AAFRESULT_SUCCESS != status ) {
		return status;
	}

	if(pEventPrior != 0)
	{
		aafPosition_t posPrior;
		status = pEventPrior->GetPosition( &posPrior );
		if ( AAFRESULT_SUCCESS != status ) return status;

		if ( posPrior > posNew ) return AAFRESULT_EVENT_SEMANTICS;
	}

	if(pEventNext != 0)
	{
		aafPosition_t posNext;
		status = pEventNext->GetPosition( &posNext );
		if ( AAFRESULT_SUCCESS != status ) return status;

		if ( posNext < posNew ) return AAFRESULT_EVENT_SEMANTICS;
	}

	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFSequence::CheckLengthSemantics( ImplAAFEvent* /*pEvent*/ )
{
	// Nothing to check here.  The length property is optional on events.
	// If it is specified, overlap may occur.  Hence, nothing to verify.

	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFSequence::getEventEnd(ImplAAFEvent *pEvent, aafLength_t &end)
{
	AAFRESULT status;
	aafPosition_t pos;
	status = pEvent->GetPosition( &pos);
	if ( AAFRESULT_SUCCESS != status ) return status;

	aafLength_t length;
	status = GetOptionalComponentLength( pEvent, length );
	if ( AAFRESULT_SUCCESS != status ) return status;

	end = pos+length;

	return AAFRESULT_SUCCESS;
}

aafLength_t ImplAAFSequence::FindEventSequenceEnd()
{
	AAFRESULT status;
	aafLength_t seqLen = 0;
	aafLength_t eventLen;
	ImplAAFComponent *pComp;
	aafUInt32 count = _components.count();
	/*
		Dyn. cast each comp to an event, sum its pos. and lenght, if total > sequence length then update the seqLen.
	*/
	for (aafUInt32 i = 0; i < count; i++) 
	{
		eventLen = 0;
		pComp = 0;
		_components.getValueAt(pComp, i);
		if (pComp) 
		{
			ImplAAFEvent *pEvent = 0;
			pEvent = dynamic_cast<ImplAAFEvent*>(pComp);
			if(pEvent)
			{
				status = getEventEnd(pEvent, eventLen);
				if ( AAFRESULT_SUCCESS == status ) 
				{
					if(seqLen > eventLen)
					{
						seqLen = seqLen;
					}
					else seqLen = eventLen;
				}
			}
		}
	}
	return seqLen;
}

AAFRESULT ImplAAFSequence::UpdateSequenceLengthOnRemove( ImplAAFEvent* pEvent, aafUInt32 index)
{
	AAFRESULT status;
	// Position of an event must be greater than or equal to
	// the position of the last event.

	//Get sequence lenght, if sequence has no lenght or lenght is 0, just return success.
	aafLength_t sequenceLength = 0; 
	if ( AAFRESULT_PROP_NOT_PRESENT == GetLength(&sequenceLength)) 
	{
		return AAFRESULT_SUCCESS;
	}
	//If we reach this point of execution it means that at least one event in this sequence had defined lenght
	//and so the sequence should have defined lenght even if it is 0.
	if (sequenceLength == 0) 
	{
		return AAFRESULT_SUCCESS;
	}

	aafLength_t eventLength = 0; 
	status = GetOptionalComponentLength(pEvent, eventLength); 
	if ( AAFRESULT_SUCCESS != status ) 
	{
		return status;
	}
	
	aafPosition_t eventPos;
	status = pEvent->GetPosition( &eventPos );
	if ( AAFRESULT_SUCCESS != status ) 
	{
		return status;
	}

	//pEvent was the first event in the sequence:
	if(index == 0)
	{
		ImplAAFEvent* pFirstEvent;
		status = GetFirstEvent( pFirstEvent );
		if ( AAFRESULT_SUCCESS != status ) return status;

		aafPosition_t posFirst;
		status = pFirstEvent->GetPosition( &posFirst );
		if ( AAFRESULT_SUCCESS != status ) return status;

		if(eventPos + eventLength < sequenceLength) 
		{
			sequenceLength = sequenceLength - (posFirst - eventPos);
		}
		else
		{
			sequenceLength = FindEventSequenceEnd() - posFirst;
		}
	}
	else if(index == _components.count()) //pEvent was the last event in the sequence
	{
		ImplAAFEvent* pLastEvent;
		status = GetLastEvent( pLastEvent );
		if ( AAFRESULT_SUCCESS != status ) return status;

		aafPosition_t posLast;
		status = pLastEvent->GetPosition( &posLast );
		if ( AAFRESULT_SUCCESS != status ) 
		{
			return status;
		}

		aafLength_t lastLength;
		status = GetOptionalComponentLength( pLastEvent, lastLength );
		if ( AAFRESULT_SUCCESS != status ) 
		{
			return status;
		}

		if(eventPos + eventLength < sequenceLength)
		{
			//Nada
		}
		else
		{
			ImplAAFEvent* pFirstEvent;
			status = GetFirstEvent( pFirstEvent );
			if ( AAFRESULT_SUCCESS != status ) return status;

			aafPosition_t posFirst;
			status = pFirstEvent->GetPosition( &posFirst );
			if ( AAFRESULT_SUCCESS != status ) return status;

			sequenceLength =  FindEventSequenceEnd() - posFirst;
		}
	}
	else if(eventPos + eventLength < sequenceLength)
	{
		//Nada
	}
	else
	{
		ImplAAFEvent* pFirstEvent;
		status = GetFirstEvent( pFirstEvent );
		if ( AAFRESULT_SUCCESS != status ) return status;

		aafPosition_t posFirst;
		status = pFirstEvent->GetPosition( &posFirst );
		if ( AAFRESULT_SUCCESS != status ) return status;

		sequenceLength =  FindEventSequenceEnd() - posFirst;
	}
	//Sanity check
	ASSERTU(sequenceLength >= 0);
	status = SetLength( sequenceLength );
	if ( AAFRESULT_SUCCESS != status ) return status;
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFSequence::UpdateSequenceLength( ImplAAFEvent* pEvent, aafUInt32 index)
{
	aafLength_t newEndPoint = 0;

	aafLength_t dummy;
	if ( AAFRESULT_PROP_NOT_PRESENT == GetLength(&dummy) &&
 		 AAFRESULT_PROP_NOT_PRESENT == pEvent->GetLength(&dummy) ) {
		return AAFRESULT_SUCCESS;
	}


    AAFRESULT status;

	aafPosition_t posNext;
	status = pEvent->GetPosition( &posNext );
	if ( AAFRESULT_SUCCESS != status ) {
		return status;
	}

	aafLength_t lengthNext;
	status = GetOptionalComponentLength( pEvent, lengthNext );
	if ( AAFRESULT_SUCCESS != status ) {
		return status;
	}

	if ( _components.count() == 0 ) {

		status = SetLength( lengthNext );
		if ( AAFRESULT_SUCCESS != status ) {
			return status;
		}

	}
	else {

		ImplAAFEvent* pFirstEvent;
		status = GetFirstEvent( pFirstEvent );
		if ( AAFRESULT_SUCCESS != status ) {
			return status;
		}

		aafPosition_t posFirst;
		status = pFirstEvent->GetPosition( &posFirst );
		if ( AAFRESULT_SUCCESS != status ) {
			return status;
		}

		aafLength_t seqLength;
		status = GetOptionalComponentLength( this, seqLength );
		if ( AAFRESULT_SUCCESS != status ) {
			return status;
		}


		if(((lengthNext + posNext) - posFirst) > seqLength) newEndPoint = (lengthNext + posNext) - posFirst;
		else newEndPoint = seqLength;


		if(index == 0)
		{

			if(posNext > posFirst) seqLength = newEndPoint;
			else seqLength = newEndPoint + (posFirst-posNext);
		}
		else
		{
			seqLength = newEndPoint;
		}
		status = SetLength( seqLength );
		if ( AAFRESULT_SUCCESS != status ) return status;
	}

	return status;
}


AAFRESULT ImplAAFSequence::CheckTypeSemantics( ImplAAFComponent* pComponent, aafUInt32 index) 
{
	//reject adjacent transitions:
	ImplAAFTransition *pTrans = dynamic_cast<ImplAAFTransition*>( pComponent );
	if(!pTrans) 
	{ 
		return AAFRESULT_SUCCESS;
	}
	if(pTrans && index == 0)
	{
		return AAFRESULT_LEADING_TRAN;
	}

	ImplAAFComponent* pComponentPrior = 0;
	ImplAAFComponent* pComponentNext = 0;

	if(index > 0)
	{
		_components.getValueAt(pComponentPrior, index-1 );
	}
    //Existing values in this <c OMVector> at <p index> and higher are shifted up one index position on insert.
	if(_components.count() > index) 
	{
		_components.getValueAt(pComponentNext, index);
	}

	if(pComponentPrior != 0 && dynamic_cast<ImplAAFTransition*>(pComponentPrior)) 
	{
		return AAFRESULT_ADJACENT_TRAN;
	}

	if(pComponentNext != 0 && dynamic_cast<ImplAAFTransition*>(pComponentNext)) 
	{
		return AAFRESULT_ADJACENT_TRAN;
	}

	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFSequence::CheckPositionSemantics( ImplAAFComponent* /*pComponent*/ )
{
	// There is nothing to check for a non-event.

	// One could check that the cut point is valid.

	return AAFRESULT_SUCCESS;
}



AAFRESULT ImplAAFSequence::CheckLengthSemantics( ImplAAFComponent* pComponentNext )
{
	// If either the new component, or the last component in the sequence, is
	// a transition, then verify they fully overlap.

	// Length properties are not optional for components and transitions.

	AAFRESULT status;

	aafLength_t lengthNext = 0;
	status = pComponentNext->GetLength(&lengthNext);
	if( AAFRESULT_PROP_NOT_PRESENT == status ) {
		return status;
	}

	aafLength_t lengthLast = 0;
	status = GetLastComponent()->GetLength(&lengthLast);
	if( AAFRESULT_PROP_NOT_PRESENT == status ) {
		return status;
	}

	if ( ( dynamic_cast<ImplAAFTransition*>( pComponentNext )     && ( (lengthLast) < (lengthNext) ) ) ||
		 ( dynamic_cast<ImplAAFTransition*>( GetLastComponent() ) && ( (lengthNext) < (lengthLast) ) ) ) {
		return AAFRESULT_INSUFF_TRAN_MATERIAL;
	}
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFSequence::UpdateSequenceLengthOnRemove( ImplAAFComponent* pComponent)
{
	AAFRESULT status;

	aafLength_t sequenceLength = 0; 
	status = GetLength(&sequenceLength);
	if ( AAFRESULT_SUCCESS != status ) return status;

	aafLength_t compLength = 0;
	status = pComponent->GetLength( &compLength );
	if ( AAFRESULT_SUCCESS != status ) return status;

	if(dynamic_cast<ImplAAFTransition*>(pComponent))
	{
		sequenceLength = sequenceLength + compLength;
	}
	else
	{
		sequenceLength = sequenceLength - compLength;
	}
	//No sanity check for comps, because a comps sequence can go negative!
	status = SetLength( sequenceLength );
	if ( AAFRESULT_SUCCESS != status ) return status;
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFSequence::UpdateSequenceLength( ImplAAFComponent* pComponent )
{
	// First get the length of pComponent.  It must have a length property.
	// AAFRESULT_PPROP_NOT_PRESENT is not toloerated.
	// 
	// Now, assuming pComponent has a length:
	//
	// If this sequence has no length property (as is the case when the first
	// component is added, use a default sequence length of zero, and update the
	// sequence length.
	//
	// If this sequence has a length,  then update this sequence's length.
	//
	// If the component is a transition subtract its length, else, add its length.

	AAFRESULT status;

	aafLength_t compLength = 0;
	status = pComponent->GetLength( &compLength );
	if ( AAFRESULT_SUCCESS != status ) {
		return status;
	}


	aafLength_t seqLength = 0;
	status = GetLength( &seqLength );
	if ( !(AAFRESULT_PROP_NOT_PRESENT == status  ||  AAFRESULT_SUCCESS == status) ) {
		return status;
	}
	
	if ( dynamic_cast<ImplAAFTransition*>( pComponent ) )  {

		seqLength -= compLength;
	}
	else {

		seqLength += compLength;
	}
	status = SetLength( seqLength );
	if ( AAFRESULT_SUCCESS != status ) {
		return status;
	}

	return AAFRESULT_SUCCESS;
}

void ImplAAFSequence::Accept(AAFComponentVisitor& visitor)
{
	aafUInt32 count = 0;
	CountComponents(&count);
	for(aafUInt32 i=0; i<count; i++)
	{
		ImplAAFComponent* pComponent = 0;
		GetNthComponent(i, &pComponent);

       	        pComponent->Accept(visitor);

		pComponent->ReleaseReference();
		pComponent = NULL;
	}

	// TODO
	// visitor.VisitSequence(this);
}


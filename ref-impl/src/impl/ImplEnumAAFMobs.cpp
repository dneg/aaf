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

#include "ImplEnumAAFMobs.h"
#include "AAFUtils.h"

ImplEnumAAFMobs::ImplEnumAAFMobs()
{
	_criteria.searchTag = kAAFNoSearch;
}

AAFRESULT STDMETHODCALLTYPE 
	ImplEnumAAFMobs::NextOne(ImplAAFMob ** ppMob)
{
	if(ppMob==NULL)
		return(AAFRESULT_NULL_PARAM);

	AAFRESULT ar;
	ImplAAFMob *pCandidate = NULL;
	while((ar=ImplAAFEnumerator<ImplAAFMob>::NextOne(&pCandidate))
		==AAFRESULT_SUCCESS)
	{
		// Check for Mob that matches our search criteria
		switch(_criteria.searchTag)
		{
		case kAAFNoSearch:
			(*ppMob)=pCandidate;
			return(AAFRESULT_SUCCESS);

		case kAAFByMobID:
		  
		  if ( pCandidate->IsMobIDEqual( &_criteria.tags.mobID ) ) {
		    *ppMob = pCandidate;
		    return(AAFRESULT_SUCCESS);
		  }
		  pCandidate->ReleaseReference();
		  pCandidate = NULL;
		  break;

		case kAAFByMobKind:
			aafMobKind_t	kind;
				
			ar = pCandidate->GetMobKind (&kind);
			if(ar != AAFRESULT_SUCCESS)
			{
				pCandidate->ReleaseReference();
				return ar;
			}
			if((kind == _criteria.tags.mobKind) 
				|| (kAAFAllMob == _criteria.tags.mobKind))
			{
				(*ppMob)=pCandidate;
				return(AAFRESULT_SUCCESS);
			}
			pCandidate->ReleaseReference();
			pCandidate = NULL;
			break;

		case kAAFByName:

		  if ( pCandidate->IsNameEqual( _criteria.tags.name ) ) {
		    *ppMob = pCandidate;
		    return(AAFRESULT_SUCCESS);
		  }
		  pCandidate->ReleaseReference();
		  pCandidate = NULL;
		  break;

		case kAAFByClass:

		  bool result;
		  ar = pCandidate->IsClassIDEqual( &_criteria.tags.objClass, result ); 
		  if ( AAFRESULT_SUCCESS != ar ) {
		    pCandidate->ReleaseReference();
		    return ar;
		  }

		  if ( result ) {
		    *ppMob = pCandidate;
		    return(AAFRESULT_SUCCESS);
		  }

		  pCandidate->ReleaseReference();
		  pCandidate = NULL;
		  break;

		case kAAFByDataDef:

		  {
		    // Check all slots for a matching data def.  Return first match.
		    
		    ImplAAFSmartPointer<ImplEnumAAFMobSlots> spSlotsIter;
		    ar = pCandidate->GetSlots( &spSlotsIter );
		    if ( AAFRESULT_SUCCESS != ar ) {
		      pCandidate->ReleaseReference();
		      return ar;
		    }
		    
		    ImplAAFSmartPointer<ImplAAFMobSlot> spMobSlot;
		    for( ar = spSlotsIter->NextOne( &spMobSlot );
			 ar != AAFRESULT_NO_MORE_OBJECTS;
			 ar = spSlotsIter->NextOne( &spMobSlot ) ) {
		      
		      AAFRESULT hr;
		      ImplAAFSmartPointer<ImplAAFSegment> spSegment;
		      
		      hr = spMobSlot->GetSegment( &spSegment );
		      if ( AAFRESULT_SUCCESS != hr ) {
			pCandidate->ReleaseReference();
			return hr;
		      }
		      
		      ImplAAFSmartPointer<ImplAAFDataDef> spDataDef;
		      hr = spSegment->GetDataDef( &spDataDef );
		      if ( AAFRESULT_SUCCESS != hr ) {
			pCandidate->ReleaseReference();
			return hr;
		      }

		      aafUID_t defId;
		      hr = spDataDef->GetAUID( &defId );
		      if ( AAFRESULT_SUCCESS != hr ) {
			pCandidate->ReleaseReference();
			return hr;
		      }
		      
		      if ( EqualAUID( &defId, &_criteria.tags.datadef ) ) {
			*ppMob = pCandidate;
			return AAFRESULT_SUCCESS;
		      }

		    }
		  }

		  pCandidate->ReleaseReference();
		  pCandidate = NULL;
		  break;

		case kAAFByMediaCrit:
			// intentional fall through
		default:
			pCandidate->ReleaseReference();
			return(AAFRESULT_NOT_IN_CURRENT_VERSION);
		};
	}
	return(AAFRESULT_NO_MORE_OBJECTS);
}
	
AAFRESULT STDMETHODCALLTYPE 
	ImplEnumAAFMobs::Next(
		aafUInt32  count,
		ImplAAFMob ** ppMobs,
		aafUInt32 *  pFetched)
{
	aafUInt32			numMobs;
	AAFRESULT			ar=AAFRESULT_SUCCESS;

	if(ppMobs==NULL||pFetched==NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(count==0)
		return(AAFRESULT_INVALID_PARAM);

	for (numMobs = 0; numMobs < count; numMobs++)
	{
		ar = NextOne(&ppMobs[numMobs]);
		if (FAILED(ar))
			break;
	}
	
	if (pFetched)
		*pFetched=numMobs;

	return(ar);
}

AAFRESULT STDMETHODCALLTYPE 
	ImplEnumAAFMobs::Skip(aafUInt32  count)
{
	if(count==0)
		return(AAFRESULT_INVALID_PARAM);

	aafUInt32 n;
	
	for(n=1;n<=count;n++)
	{
		// Defined behavior of skip is to NOT advance at all if it would push 
		// us off of the end
		ImplAAFMob *pMob;
		AAFRESULT ar = NextOne(&pMob);
		if (AAFRESULT_SUCCEEDED(ar))
		{
			pMob->ReleaseReference();
		}
		if(ar==AAFRESULT_NO_MORE_OBJECTS)
		{
			// Off the end, decrement n and iterator back to the starting 
			// position
			while(n>=1)
			{
				--(*_pIterator);
				n--;
			}
			return(ar);
		}
		else if(FAILED(ar))
		{
			return(ar);
		}
	}

	return(AAFRESULT_SUCCESS);
}

// Clone() wrapper for pointer compatibility

AAFRESULT STDMETHODCALLTYPE 
	ImplEnumAAFMobs::Clone(ImplEnumAAFMobs ** ppEnum)
{
	// MSDEV requires reinterpret_cast here, even though 
	// ImplAAFEnumerator<ImplAAFMob> is the base class for ImplEnumAAFMobs.
	return(ImplAAFEnumerator<ImplAAFMob>::Clone(
		reinterpret_cast<ImplAAFEnumerator<ImplAAFMob>**>(ppEnum)));
}

AAFRESULT ImplEnumAAFMobs::SetCriteria(aafSearchCrit_t *pCriteria)
{
	if(NULL == pCriteria)
		_criteria.searchTag = kAAFNoSearch;
	else
		_criteria = *pCriteria;

	return AAFRESULT_SUCCESS;
}

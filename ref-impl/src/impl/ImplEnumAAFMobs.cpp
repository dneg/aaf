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
 *  prior written permission of Avid Technology, Inc.
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

#include "ImplEnumAAFMobs.h"

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

#if(0)

#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"
#include "aafUtils.h"

ImplEnumAAFMobs::ImplEnumAAFMobs () : _enumObj(0), _iterator(0)
{
	_criteria.searchTag = kAAFNoSearch;
}


ImplEnumAAFMobs::~ImplEnumAAFMobs ()
{
  if (0 != _enumObj)
  {
    _enumObj->ReleaseReference();
	  _enumObj = 0;
  }
	delete _iterator;
	_iterator = 0;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::NextOne (ImplAAFMob **ppMob)
{
	AAFRESULT ar = AAFRESULT_NO_MORE_OBJECTS;
	aafBool	found = kAAFFalse;

	if(ppMob == NULL)
		return AAFRESULT_NULL_PARAM;

	if (_iterator->before() || _iterator->valid())
	{
		while(!found && ++(*_iterator))
		{
			*ppMob = _iterator->value();
				switch(_criteria.searchTag)
				{
				case kAAFNoSearch:
					found = kAAFTrue;
					break;

				case kAAFByMobKind:
					aafMobKind_t	kind;
				
					ar = (*ppMob)->GetMobKind (&kind);
					if(ar != AAFRESULT_SUCCESS)
						return ar;
					if((kind == _criteria.tags.mobKind) || (kAAFAllMob == _criteria.tags.mobKind))
						found = kAAFTrue;
					break;
				default:
					return AAFRESULT_NOT_IN_CURRENT_VERSION;
				}
		}
		if(found)
		{
			(*ppMob)->AcquireReference();
			ar = AAFRESULT_SUCCESS;
		}
		else
		{
			*ppMob = NULL;
			ar = AAFRESULT_NO_MORE_OBJECTS;
		}
	}
	return ar;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::Next (aafUInt32 count,
                           ImplAAFMob **ppMobs,
                           aafUInt32 *pFetched)
{
	ImplAAFMob		**	ppOneMob;
	aafUInt32			numDefs;
	HRESULT				hr;

	if(ppMobs == NULL)
		return AAFRESULT_NULL_PARAM;
	
//!!!	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
//		return E_INVALIDARG;

	// Point at the first component in the array.
	ppOneMob = ppMobs;
	for (numDefs = 0; numDefs < count; numDefs++)
	{
		hr = NextOne(&ppOneMob[numDefs]);
		if (FAILED(hr))
			break;
	}
	
	if (pFetched)
		*pFetched = numDefs;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::Skip (aafUInt32 count)
{
	AAFRESULT	ar = AAFRESULT_SUCCESS;
	aafUInt32	n;
	
	for(n = 1; n <= count; n++)
	{
		//!!! JeffB: Handle skip of subset of mobs correctly
		// Defined behavior of skip is to NOT advance at all if it would push us off of the end
		if(!++(*_iterator))
		{
			// Off the end, decrement n and iterator back to the starting position
			while(n >= 1)
			{
				--(*_iterator);
				n--;
			}
			ar = AAFRESULT_NO_MORE_OBJECTS;
			break;
		}
	}

	return ar;
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::Clone (ImplEnumAAFMobs **ppEnum)
{
	AAFRESULT			ar = AAFRESULT_SUCCESS;
	ImplEnumAAFMobs		*result;

	result = (ImplEnumAAFMobs *)CreateImpl(CLSID_EnumAAFMobs);
	if (result == NULL)
		return E_FAIL;

    ar = result->SetIterator(_enumObj,_iterator->copy());
	if (SUCCEEDED(ar))
	{
		*ppEnum = result;
	}
	else
	{
	  result->ReleaseReference();
	  result = 0;
	  *ppEnum = NULL;
	}
	
	return ar;
}

//Internal

AAFRESULT
	ImplEnumAAFMobs::SetCriteria(aafSearchCrit_t *pCriteria)
{
	if(NULL == pCriteria)
		_criteria.searchTag = kAAFNoSearch;
	else
		_criteria = *pCriteria;

	return AAFRESULT_SUCCESS;
}

#endif

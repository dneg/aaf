//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=



#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFDataDefs.h"
#include "AAFUtils.h"

#include "ImplAAFBuiltinDefs.h"

ImplAAFDataDef::ImplAAFDataDef ()
  : _pCachedDict (0)
{
}


ImplAAFDataDef::~ImplAAFDataDef ()
{
  // _pCachedDict is *NOT* reference counted!
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::Initialize (
      const aafUID_t & id,
	  const aafWChar * pName,
	  const aafWChar * pDesc)
{
	if (pName == NULL || pDesc == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}
	else
	{
	  return pvtInitialize(id, pName, pDesc);
	}
	// return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsPictureKind (
      aafBool *bIsPictureKind)
{
	AAFRESULT	hr;
	aafBool	isV10 = kAAFFalse;
	aafBool	isV11 = kAAFFalse;

	hr = IsDataDefOf(GetDict()->GetBuiltinDefs()->ddkAAFPicture(), &isV11);
	if (AAFRESULT_FAILED (hr)) return hr;
	if (!isV11)
	{
		hr = IsDataDefOf(GetDict()->GetBuiltinDefs()->ddPicture(), &isV10);
		if (AAFRESULT_FAILED (hr)) return hr;
	}

	if ((isV11==kAAFTrue) || (isV10==kAAFTrue))
		*bIsPictureKind = kAAFTrue;
	else
		*bIsPictureKind = kAAFFalse;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsMatteKind (
      aafBool *bIsMatteKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddkAAFMatte(), bIsMatteKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsPictureWithMatteKind (
      aafBool *bIsPictureWithMatteKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddkAAFPictureWithMatte(), bIsPictureWithMatteKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsSoundKind (
      aafBool *bIsSoundKind)
{
	AAFRESULT	hr;
	aafBool	isV10 = kAAFFalse;
	aafBool	isV11 = kAAFFalse;

	hr = IsDataDefOf(GetDict()->GetBuiltinDefs()->ddkAAFSound(), &isV11);
	if (AAFRESULT_FAILED (hr)) return hr;
	if (!isV11)
	{
		hr = IsDataDefOf(GetDict()->GetBuiltinDefs()->ddSound(), &isV10);
		if (AAFRESULT_FAILED (hr)) return hr;
	}

	if ((isV11==kAAFTrue) || (isV10==kAAFTrue))
		*bIsSoundKind = kAAFTrue;
	else
		*bIsSoundKind = kAAFFalse;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsTimecodeKind (
      aafBool *bIsTimecodeKind)
{
	AAFRESULT	hr;
	aafBool	isV10 = kAAFFalse;
	aafBool	isV11 = kAAFFalse;

	hr = IsDataDefOf(GetDict()->GetBuiltinDefs()->ddkAAFTimecode(), &isV11);
	if (AAFRESULT_FAILED (hr)) return hr;
	if (!isV11)
	{
		hr = IsDataDefOf(GetDict()->GetBuiltinDefs()->ddTimecode(), &isV10);
		if (AAFRESULT_FAILED (hr)) return hr;
	}

	if ((isV11==kAAFTrue) || (isV10==kAAFTrue))
		*bIsTimecodeKind = kAAFTrue;
	else
		*bIsTimecodeKind = kAAFFalse;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsEdgecodeKind (
      aafBool *bIsEdgecodeKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddkAAFEdgecode(), bIsEdgecodeKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsAuxiliaryKind (
      aafBool *bIsAuxiliaryKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddkAAFAuxiliary(), bIsAuxiliaryKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsDescriptiveMetadataKind (
      aafBool *bIsDescriptiveMetadataKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddkAAFDescriptiveMetadata(), bIsDescriptiveMetadataKind));
}


AAFRESULT ImplAAFDataDef::IsDataDefEquivalentOf (
      ImplAAFDataDef * pDataDef,
      aafBool *bIsDataDefEquivalentOf)
{
	if(bIsDataDefEquivalentOf == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(pDataDef == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	XPROTECT()
	{
		aafBool	result = kAAFFalse;
		
		CHECK(IsDataDefOf (pDataDef, &result));
		if(result == kAAFFalse)
		{
			aafBool	thisIsPicture = kAAFFalse;
			CHECK(IsPictureKind (&thisIsPicture));
			aafBool	thisIsSound = kAAFFalse;
			CHECK(IsSoundKind (&thisIsSound));
			aafBool	thisIsTimecode = kAAFFalse;
			CHECK(IsTimecodeKind (&thisIsTimecode));

			aafBool	argIsPicture = kAAFFalse;
			CHECK(pDataDef->IsPictureKind (&argIsPicture));
			aafBool	argIsSound = kAAFFalse;
			CHECK(pDataDef->IsSoundKind (&argIsSound));
			aafBool	argIsTimecode = kAAFFalse;
			CHECK(pDataDef->IsTimecodeKind (&argIsTimecode));

			if((thisIsPicture == kAAFTrue) && (argIsPicture == kAAFTrue)
					|| (thisIsSound == kAAFTrue) && (argIsSound == kAAFTrue)
					|| (thisIsTimecode == kAAFTrue) && (argIsTimecode == kAAFTrue))
				result = kAAFTrue;
		}

		*bIsDataDefEquivalentOf = result;
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::DoesDataDefConvertTo (
      ImplAAFDataDef * pDataDef,
      aafBool *bDoesConvertTo)
{
	if(bDoesConvertTo == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(pDataDef == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	XPROTECT()
	{
		aafBool	result = kAAFFalse;
		
		CHECK(IsDataDefEquivalentOf (pDataDef, &result));
		if(result == kAAFFalse)
		{
			aafBool	argIsPicture = kAAFFalse;
			CHECK(pDataDef->IsPictureKind (&argIsPicture));
			aafBool	thisIsPictureWithMatte = kAAFFalse;
			CHECK(IsPictureWithMatteKind (&thisIsPictureWithMatte));

			if((thisIsPictureWithMatte == kAAFTrue) && (argIsPicture == kAAFTrue))
				result = kAAFTrue;
		}
		*bDoesConvertTo = result;
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}

		   
AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsDataDefOf (
      ImplAAFDataDef * pDataDef,
      aafBool *bIsDataDefOf)
{
	aafUID_t	thisUid;
	aafUID_t	otherUid;
	AAFRESULT	hr;

	if ( ! pDataDef)
	  return AAFRESULT_NULL_PARAM;

	if(! bIsDataDefOf)
	  return AAFRESULT_NULL_PARAM;
	
	hr = GetAUID(&thisUid);
	if (AAFRESULT_FAILED (hr)) return hr;

	hr = pDataDef->GetAUID(&otherUid);
	if (AAFRESULT_FAILED (hr)) return hr;

	*bIsDataDefOf = EqualAUID(&thisUid, &otherUid);

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::DoesDataDefConvertFrom (
      ImplAAFDataDef * pDataDef,
      aafBool * bDoesConvertFrom)
{
  if (! bDoesConvertFrom)
	return(AAFRESULT_NULL_PARAM);
  if (! pDataDef)
	return(AAFRESULT_NULL_PARAM);
	
	XPROTECT()
	{
		aafBool	result = kAAFFalse;
		
		CHECK(IsDataDefEquivalentOf (pDataDef, &result));
		if(result == kAAFFalse)
		{
			aafBool	thisIsPicture;
			CHECK(IsPictureKind (&thisIsPicture));
			aafBool	argIsPictureWithMatte;
			CHECK(pDataDef->IsPictureWithMatteKind (&argIsPictureWithMatte));
			if((thisIsPicture == kAAFTrue) && (argIsPictureWithMatte == kAAFTrue))
				result = kAAFTrue;
		}
		*bDoesConvertFrom = result;
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}


ImplAAFDictionary * ImplAAFDataDef::GetDict()
{
  if (! _pCachedDict)
	{
	  AAFRESULT hr = GetDictionary (&_pCachedDict);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  // _pCachedDict is *NOT* reference counted here, so release the
	  // newly-added reference.
	  _pCachedDict->ReleaseReference();
	}
  assert (_pCachedDict);
  return _pCachedDict;
}

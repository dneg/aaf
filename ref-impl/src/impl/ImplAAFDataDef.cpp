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
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddPicture(), bIsPictureKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsMatteKind (
      aafBool *bIsMatteKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddMatte(), bIsMatteKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsPictureWithMatteKind (
      aafBool *bIsPictureWithMatteKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddPictureWithMatte(), bIsPictureWithMatteKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsSoundKind (
      aafBool *bIsSoundKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddSound(), bIsSoundKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsTimecodeKind (
      aafBool *bIsTimecodeKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddTimecode(), bIsTimecodeKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsEdgecodeKind (
      aafBool *bIsEdgecodeKind)
{
	return(IsDataDefOf(GetDict()->GetBuiltinDefs()->ddEdgecode(), bIsEdgecodeKind));
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
		aafBool	result;
		aafUID_t id;
		CHECK (pDataDef->GetAUID (&id));
		
		CHECK(IsDataDefOf (pDataDef, &result));
		if(result == kAAFFalse)
		{
			aafBool	isPWM;
			aafUID_t	picture = DDEF_Picture;
			CHECK(IsPictureWithMatteKind (&isPWM));
			if((isPWM == kAAFTrue) && EqualAUID(&picture, &id))
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
		aafBool	result;
		
		CHECK(IsDataDefOf (pDataDef, &result));
		if(result == kAAFFalse)
		{
			aafBool		isPict;
			aafUID_t	pictureMatte = DDEF_PictureWithMatte;
			CHECK(IsPictureKind (&isPict));
			aafUID_t id;
			CHECK(pDataDef->GetAUID (&id));
			if((isPict == kAAFTrue) && EqualAUID(&pictureMatte, &id))
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

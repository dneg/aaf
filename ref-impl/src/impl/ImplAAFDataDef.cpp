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



#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFDataDefs.h"
#include "aafUtils.h"

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
	return AAFRESULT_SUCCESS;
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
		if(result == AAFFalse)
		{
			aafBool	isPWM;
			aafUID_t	picture = DDEF_Picture;
			CHECK(IsPictureWithMatteKind (&isPWM));
			if((isPWM == AAFTrue) && EqualAUID(&picture, &id))
				result = AAFTrue;
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
		if(result == AAFFalse)
		{
			aafBool		isPict;
			aafUID_t	pictureMatte = DDEF_PictureWithMatte;
			CHECK(IsPictureKind (&isPict));
			aafUID_t id;
			CHECK(pDataDef->GetAUID (&id));
			if((isPict == AAFTrue) && EqualAUID(&pictureMatte, &id))
				result = AAFTrue;
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

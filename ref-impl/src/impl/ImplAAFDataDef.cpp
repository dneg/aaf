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


/******************************************\
*                                          *
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
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFDataDefs.h"
#include "aafUtils.h"

ImplAAFDataDef::ImplAAFDataDef ()
{}


ImplAAFDataDef::~ImplAAFDataDef ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsPictureKind (
      aafBool *bIsPictureKind)
{
	aafUID_t	uid = DDEF_Picture;
	return(IsDataDefOf(&uid, bIsPictureKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsMatteKind (
      aafBool *bIsMatteKind)
{
	aafUID_t	uid = DDEF_Matte;
	return(IsDataDefOf(&uid, bIsMatteKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsPictureWithMatteKind (
      aafBool *bIsPictureWithMatteKind)
{
	aafUID_t	uid = DDEF_PictureWithMatte;
	return(IsDataDefOf(&uid, bIsPictureWithMatteKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::IsSoundKind (
      aafBool *bIsSoundKind)
{
	aafUID_t	uid = DDEF_Sound;
	return(IsDataDefOf(&uid, bIsSoundKind));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::DoesDataDefConvertTo (
      aafUID_t *pAuid,
      aafBool *bDoesConvertTo)
{
	if(bDoesConvertTo == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	XPROTECT()
	{
		aafBool	result;
		
		CHECK(IsDataDefOf (pAuid, &result));
		if(result == AAFFalse)
		{
			aafBool	isPWM;
			aafUID_t	picture = DDEF_Picture;
			CHECK(IsPictureWithMatteKind (&isPWM));
			if((isPWM == AAFTrue) && EqualAUID(&picture, pAuid))
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
      aafUID_t *pAuid,
      aafBool *bIsDataDefOf)
{
	aafUID_t	uid;
	AAFRESULT	hr;

	if(bIsDataDefOf == NULL)
		return AAFRESULT_NULL_PARAM;
	
	hr = GetAUID(&uid);
	*bIsDataDefOf = EqualAUID(pAuid, &uid);

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataDef::DoesDataDefConvertFrom (
      aafUID_t *pAuid,
      aafBool * bDoesConvertFrom)
{
	if(bDoesConvertFrom == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	XPROTECT()
	{
		aafBool	result;
		
		CHECK(IsDataDefOf (pAuid, &result));
		if(result == AAFFalse)
		{
			aafBool		isPict;
			aafUID_t	pictureMatte = DDEF_PictureWithMatte;
			CHECK(IsPictureKind (&isPict));
			if((isPict == AAFTrue) && EqualAUID(&pictureMatte, pAuid))
				result = AAFTrue;
		}
		*bDoesConvertFrom = result;
	}
	XEXCEPT
	XEND;

	return AAFRESULT_SUCCESS;
}






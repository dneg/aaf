/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

 
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
    ImplAAFDataDef::GetName (
      aafString_t *  /*pstrName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


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
	return(IsDataDefOf (pAuid, bDoesConvertTo));	// !!!No conversion yet
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
	return(IsDataDefOf (pAuid, bDoesConvertFrom));	// !!!No conversion yet
}



OMDEFINE_STORABLE(ImplAAFDataDef, AUID_AAFDataDef);



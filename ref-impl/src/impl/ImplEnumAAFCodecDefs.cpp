/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/


#ifndef __ImplAAFCodecDef_h__
#include "ImplAAFCodecDef.h"
#endif





#ifndef __ImplEnumAAFCodecDefs_h__
#include "ImplEnumAAFCodecDefs.h"
#endif

#include <assert.h>
#include <string.h>


ImplEnumAAFCodecDefs::ImplEnumAAFCodecDefs ()
{}


ImplEnumAAFCodecDefs::~ImplEnumAAFCodecDefs ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::NextOne (
      ImplAAFCodecDef ** /*ppPluggableDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::Next (
      aafUInt32  /*count*/,
      ImplAAFCodecDef ** /*ppPluggableDefs*/,
      aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::Skip (
      aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::Reset ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::Clone (
      ImplEnumAAFCodecDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::SetEnumProperty( ImplAAFObject *pObj, codecDefWeakRefArrayProp_t *pProp)
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = pObj;

	if (pObj)
		pObj->AcquireReference();
	/**/
	_enumProp = pProp;				// Don't refcount, same lifetime as the object.
	_enumStrongProp = NULL;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFCodecDefs::SetEnumStrongProperty( ImplAAFObject *pObj, codecDefStrongRefArrayProp_t *pProp)
{
	if (_enumObj)
		_enumObj->ReleaseReference();
	_enumObj = pObj;
	if (pObj)
		pObj->AcquireReference();
	/**/
	_enumStrongProp = pProp;				// Don't refcount, same lifetime as the object.
	_enumProp = NULL;

	return AAFRESULT_SUCCESS;
}





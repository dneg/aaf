/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

#ifndef __ImplAAFContainerDef_h__
#include "ImplAAFContainerDef.h"
#endif





#ifndef __ImplEnumAAFContainerDefs_h__
#include "ImplEnumAAFContainerDefs.h"
#endif

#include <assert.h>
#include <string.h>


ImplEnumAAFContainerDefs::ImplEnumAAFContainerDefs ()
{}


ImplEnumAAFContainerDefs::~ImplEnumAAFContainerDefs ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFContainerDefs::NextOne (
      ImplAAFContainerDef ** /*ppPluggableDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFContainerDefs::Next (
      aafUInt32  /*count*/,
      ImplAAFContainerDef ** /*ppContainerDefs*/,
      aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFContainerDefs::Skip (
      aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFContainerDefs::Reset ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFContainerDefs::Clone (
      ImplEnumAAFContainerDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFContainerDefs::SetEnumProperty( ImplAAFObject *pObj, containerDefWeakRefArrayProp_t *pProp)
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
    ImplEnumAAFContainerDefs::SetEnumStrongProperty( ImplAAFObject *pObj, containerDefStrongRefArrayProp_t *pProp)
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





/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFTypeDefStrongObjRef::ImplAAFTypeDefStrongObjRef ()
  : _ReferencedType (0)
{}


ImplAAFTypeDefStrongObjRef::~ImplAAFTypeDefStrongObjRef ()
{
  if (_ReferencedType) _ReferencedType->ReleaseReference ();
}

// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::Initialize (
      aafUID_t *  pID,
      ImplAAFClassDef * pObjType,
      wchar_t *  pTypeName)
{
  if (! pID)       return AAFRESULT_NULL_PARAM;
  // BobT: temporarily allow null class defs until they're hooked up
  // if (! pObjType)  return AAFRESULT_NULL_PARAM;
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  assert (!_ReferencedType);
  _ReferencedType = pObjType;
  if (_ReferencedType)
    _ReferencedType->AcquireReference ();

  hr = SetIdentification (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::SetObject (ImplAAFPropertyValue * pPropVal,
										   ImplAAFObject * pObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pObject) return AAFRESULT_NULL_PARAM;

  aafMemPtr_t pBits;
  AAFRESULT hr;
  hr = pPropVal->GetBits (&pBits);
  if (AAFRESULT_FAILED (hr)) return hr;

  memcpy (pBits, pObject, sizeof (ImplAAFObject*));

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefStrongObjRef::GetObject (ImplAAFPropertyValue * pPropVal,
									   ImplAAFObject ** ppObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppObject) return AAFRESULT_NULL_PARAM;

  aafMemPtr_t pBits;
  AAFRESULT hr;
  hr = pPropVal->GetBits (&pBits);
  if (AAFRESULT_FAILED (hr)) return hr;

  memcpy (*ppObject, pBits, sizeof (ImplAAFObject*));

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::GetObjectType (ImplAAFClassDef ** ppObjType)
{
  if (! ppObjType) return AAFRESULT_NULL_PARAM;

  *ppObjType = _ReferencedType;
  if (*ppObjType)
	(*ppObjType)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}

// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::CreateValue (/*[in]*/ ImplAAFObject * /*pObj*/,
      /*[out]*/ ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatStrongObjRef;
  return AAFRESULT_SUCCESS;
}


aafBool ImplAAFTypeDefStrongObjRef::IsFixedSize (void)
{
  return AAFTrue;
}


size_t ImplAAFTypeDefStrongObjRef::PropValSize (void)
{
  return sizeof (ImplAAFObject*);
}


OMDEFINE_STORABLE(ImplAAFTypeDefStrongObjRef, AUID_AAFTypeDefStrongObjRef);

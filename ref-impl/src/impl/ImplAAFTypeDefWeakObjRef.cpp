/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFTypeDefWeakObjRef::ImplAAFTypeDefWeakObjRef ()
  : _referencedType (PID_TypeDefinitionWeakObjectReference_ReferencedType, "ReferencedType")
{
  _persistentProperties.put(_referencedType.address());
}


ImplAAFTypeDefWeakObjRef::~ImplAAFTypeDefWeakObjRef ()
{}


// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::Initialize (
      aafUID_t *  pID,
      ImplAAFClassDef * pObjType,
      wchar_t *  pTypeName)
{
  if (! pID)       return AAFRESULT_NULL_PARAM;
  if (! pObjType)  return AAFRESULT_NULL_PARAM;
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  if (! pObjType) return AAFRESULT_NULL_PARAM;
  _referencedType = pObjType;
  pObjType->AcquireReference ();

  hr = SetAUID (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::SetObject (ImplAAFPropertyValue * pPropVal,
										   ImplAAFObject * pObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pObject) return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefWeakObjRef::GetObject (ImplAAFPropertyValue * pPropVal,
									   ImplAAFObject ** ppObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppObject) return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::GetObjectType (ImplAAFClassDef ** ppObjType)
{
  if (! ppObjType) return AAFRESULT_NULL_PARAM;

  *ppObjType = _referencedType;
  assert (*ppObjType);
  (*ppObjType)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}

  // Override from AAFTypeDefObjectRef
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::CreateValue (/*[in]*/ ImplAAFObject * /*pObj*/,
      /*[out]*/ ImplAAFPropertyValue ** /*ppPropVal*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatWeakObjRef;
  return AAFRESULT_SUCCESS;
}


aafBool ImplAAFTypeDefWeakObjRef::IsFixedSize (void)
{
  return AAFTrue;
}


size_t ImplAAFTypeDefWeakObjRef::PropValSize (void)
{
  return sizeof (ImplAAFObject*);
}


OMDEFINE_STORABLE(ImplAAFTypeDefWeakObjRef, AUID_AAFTypeDefWeakObjRef);

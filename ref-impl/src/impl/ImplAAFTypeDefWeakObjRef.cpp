/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
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

#ifndef __ImplAAFHeader_h_
#include "ImplAAFHeader.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
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
      const aafUID_t *  pID,
      const aafUID_t * pRefdObjID,
      wchar_t *  pTypeName)
{
  if (! pID)       return AAFRESULT_NULL_PARAM;
  if (! pRefdObjID)  return AAFRESULT_NULL_PARAM;
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  _referencedType = *pRefdObjID;

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
    ImplAAFTypeDefWeakObjRef::GetObjectType (ImplAAFClassDef ** ppObjType) const
{
  if (! ppObjType) return AAFRESULT_NULL_PARAM;

  if (! _cachedObjType)
	{
	  ImplAAFHeaderSP pHead;
	  ImplAAFDictionarySP pDict;

	  AAFRESULT hr;
	  hr = MyHeadObject(&pHead);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (pHead);
	  hr = (pHead->GetDictionary(&pDict));
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (pDict);

	  ImplAAFTypeDefWeakObjRef * pNonConstThis =
		  (ImplAAFTypeDefWeakObjRef *) this;
	  aafUID_t id = _referencedType;
	  hr = pDict->LookupClass (&id, &pNonConstThis->_cachedObjType);
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (_cachedObjType);
	}
  assert (ppObjType);
  *ppObjType = _cachedObjType;
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


ImplAAFTypeDefSP ImplAAFTypeDefWeakObjRef::BaseType () const
{
  if (! _cachedAuidType)
	{
	  AAFRESULT hr;
	  ImplAAFDictionarySP pDict;
	  hr = GetDictionary (&pDict);
	  assert (AAFRESULT_SUCCEEDED(hr));
	  assert (pDict);

	  ImplAAFTypeDefWeakObjRef * pNonConstThis =
		(ImplAAFTypeDefWeakObjRef *) this;
	  hr = pDict->LookupType (&kAAFTypeID_AUID, &pNonConstThis->_cachedAuidType);
	  assert (AAFRESULT_SUCCEEDED(hr));
	  assert (_cachedAuidType);
	}
  return _cachedAuidType;
}


aafBool ImplAAFTypeDefWeakObjRef::IsFixedSize (void) const
{
  return AAFTrue;
}


size_t ImplAAFTypeDefWeakObjRef::PropValSize (void) const
{
  return BaseType()->PropValSize();
}


aafBool ImplAAFTypeDefWeakObjRef::IsRegistered (void) const
{
  return BaseType()->IsRegistered();
}


size_t ImplAAFTypeDefWeakObjRef::NativeSize (void) const
{
  return sizeof (ImplAAFObject*);
}


OMProperty * ImplAAFTypeDefWeakObjRef::pvtCreateOMPropertyMBS
  (OMPropertyId pid,
   const char * name) const
{
  assert (name);
  size_t elemSize = PropValSize ();
  OMProperty * result = new OMSimpleProperty (pid, name, elemSize);
  assert (result);
  return result;
}

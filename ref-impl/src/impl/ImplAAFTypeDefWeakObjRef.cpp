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


#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#ifndef __AAFClassDefUIDs_h__
#include "AAFClassDefUIDs.h"
#endif

#include "ImplAAFDictionary.h"
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


/*
// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::Initialize (
      const aafUID_t *  pID,
      const aafUID_t * pRefdObjID,
      const aafCharacter * pTypeName)
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
*/



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

  if (! _cachedObjType)
	{
	  ImplAAFDictionarySP pDict;

	  AAFRESULT hr;
	  hr = (GetDictionary(&pDict));
	  if (AAFRESULT_FAILED(hr))
		return hr;
	  assert (pDict);

	  hr = pDict->LookupClassDef (_referencedType, &_cachedObjType);
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
	  hr = pDict->LookupTypeDef (kAAFTypeID_AUID, &pNonConstThis->_cachedAuidType);
	  assert (AAFRESULT_SUCCEEDED(hr));
	  assert (_cachedAuidType);
	}
  return _cachedAuidType;
}


aafBool ImplAAFTypeDefWeakObjRef::IsFixedSize (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefWeakObjRef::PropValSize (void) const
{
  // Temp change: currently weak refs are represented as auids.
  // return BaseType()->PropValSize();
  aafUID_t tmpID = _referencedType;
	if(memcmp(&tmpID, &kAAFClassID_Mob, sizeof(aafUID_t)) == 0)
	{
		return sizeof (aafMobID_t);
	}
	else
	{
		return sizeof (aafUID_t);
	}
}


aafBool ImplAAFTypeDefWeakObjRef::IsRegistered (void) const
{
  return BaseType()->IsRegistered();
}


size_t ImplAAFTypeDefWeakObjRef::NativeSize (void) const
{
  // Temp change: currently weak refs are represented as auids.
  // return sizeof (ImplAAFObject*);
  aafUID_t tmpID = _referencedType;
	if(memcmp(&tmpID, &kAAFClassID_Mob, sizeof(aafUID_t)) == 0)
	{
		return sizeof (aafMobID_t);
	}
	else
	{
		return sizeof (aafUID_t);
	}
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

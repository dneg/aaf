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
  : _referencedType (PID_TypeDefinitionWeakObjectReference_ReferencedType, "ReferencedType", "/Dictionary/ClassDefinitions", PID_DefinitionObject_Identification)
{
  _persistentProperties.put(_referencedType.address());
}


ImplAAFTypeDefWeakObjRef::~ImplAAFTypeDefWeakObjRef ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefWeakObjRef::Initialize (
      const aafUID_t & id,
      ImplAAFClassDef * pObjType,
      const aafCharacter * pTypeName,
	  aafUID_t * pTargetHint,
	  aafUInt32 targetHintCount)
{
  if (! pObjType)  return AAFRESULT_NULL_PARAM;
  if (! pTypeName)  return AAFRESULT_NULL_PARAM;
  if (! pTargetHint)  return AAFRESULT_NULL_PARAM;

  return pvtInitialize (id, pObjType, pTypeName);
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
  if (! ppObjType)
	return AAFRESULT_NULL_PARAM;

   if(_referencedType.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFClassDef *pClassDef = _referencedType;

  *ppObjType = pClassDef;
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
	ImplAAFClassDef	*pClassDef;
	aafUID_t		tmpID;

  // Temp change: currently weak refs are represented as auids.
  // return BaseType()->PropValSize();
	pClassDef = _referencedType;
	pClassDef->GetAUID(&tmpID);

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
	ImplAAFClassDef	*pClassDef;
	aafUID_t		tmpID;

	// Temp change: currently weak refs are represented as auids.
  // return sizeof (ImplAAFObject*);
	pClassDef = _referencedType;
	pClassDef->GetAUID(&tmpID);
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

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

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFDictionary.h"


#include <assert.h>
#include <string.h>


extern "C" const aafClassID_t CLSID_AAFPropValData;

ImplAAFTypeDefStrongObjRef::ImplAAFTypeDefStrongObjRef ()
  : _referencedType ( PID_TypeDefinitionStrongObjectReference_ReferencedType,
                      L"ReferencedType",
                      L"/MetaDictionary/ClassDefinitions",
                      PID_MetaDefinition_Identification)
{
  _persistentProperties.put(_referencedType.address());
}


ImplAAFTypeDefStrongObjRef::~ImplAAFTypeDefStrongObjRef ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::Initialize (
      const aafUID_t & id,
      ImplAAFClassDef * pObjType,
      const aafCharacter * pTypeName)
{
  if (! pObjType)  return AAFRESULT_NULL_PARAM;
  if (! pTypeName)  return AAFRESULT_NULL_PARAM;

  return pvtInitialize (id, pObjType, pTypeName);
}


// Override from AAFTypeDefObjectRef
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::pvtInitialize (
      const aafUID_t & id,
      const ImplAAFClassDef * pClassDef,
      const aafCharacter * pTypeName)
{
  if (! pTypeName) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  _referencedType = pClassDef;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::SetObject (ImplAAFPropertyValue * pPropVal,
										   ImplAAFObject * pObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! pObject) return AAFRESULT_NULL_PARAM;

  OMStorable ** ppStorable = NULL;
  aafUInt32 bitsSize = 0;
  AAFRESULT hr;
  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  assert (pvd);

  hr = pvd->AllocateBits (sizeof (OMStorable*), (aafMemPtr_t*) &ppStorable);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (ppStorable);
  if (*ppStorable)
	{
	  // An object was already here.  Release it before we trash the
	  // reference to it.
	  ImplAAFObject * tmp = dynamic_cast<ImplAAFObject*>(*ppStorable);
	  assert (tmp);
	  tmp->ReleaseReference ();
	  tmp = 0;
	  *ppStorable = 0;
	}

  assert (! *ppStorable);
  *ppStorable = pObject;
  pObject->AcquireReference ();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFTypeDefStrongObjRef::GetObject (ImplAAFPropertyValue * pPropVal,
									   ImplAAFObject ** ppObject)
{
  if (! pPropVal) return AAFRESULT_NULL_PARAM;
  if (! ppObject) return AAFRESULT_NULL_PARAM;

  OMStorable ** ppStorable = NULL;
  aafUInt32 bitsSize = 0;
  AAFRESULT hr;
  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  assert (pvd);

  hr = pvd->GetBitsSize (&bitsSize);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (bitsSize >= sizeof (ImplAAFObject*));
  hr = pvd->GetBits ((aafMemPtr_t*) &ppStorable);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (*ppStorable);
  assert (ppObject);
  ImplAAFObject * pObj;
  pObj = dynamic_cast<ImplAAFObject*>(*ppStorable);
  assert (pObj);
  *ppObject = pObj;
  (*ppObject)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::GetObjectType (ImplAAFClassDef ** ppObjType)
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
    ImplAAFTypeDefStrongObjRef::CreateValue (/*[in]*/ ImplAAFObject * pObj,
      /*[out]*/ ImplAAFPropertyValue ** ppPropVal)
{
  if (! pObj)
	return AAFRESULT_NULL_PARAM;
  if (! ppPropVal)
	return AAFRESULT_NULL_PARAM;

  ImplAAFPropValData * pvd = 0;
  pvd = (ImplAAFPropValData*) CreateImpl (CLSID_AAFPropValData);
  if (!pvd) return AAFRESULT_NOMEMORY;

  ImplAAFPropValDataSP spPvd;
  spPvd = pvd;
  // SmartPointer operator= will automatically
  // AddRef; CreateImpl *also* will addref, so we've got one too
  // many.  Put us back to normal.
  pvd->ReleaseReference ();
  pvd = 0;

  AAFRESULT hr;
  hr = spPvd->Initialize (this);
  if (AAFRESULT_FAILED (hr)) return hr;

  hr = SetObject (spPvd, pObj);
  if (AAFRESULT_FAILED (hr))
	return hr;
  assert (ppPropVal);
  *ppPropVal = spPvd;
  assert (*ppPropVal);
  (*ppPropVal)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStrongObjRef::GetTypeCategory (eAAFTypeCategory_t *  pTid)
{
  if (!pTid) return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatStrongObjRef;
  return AAFRESULT_SUCCESS;
}



aafBool ImplAAFTypeDefStrongObjRef::IsFixedSize (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefStrongObjRef::PropValSize (void) const
{
  return sizeof (ImplAAFObject*);
}


aafBool ImplAAFTypeDefStrongObjRef::IsRegistered (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefStrongObjRef::NativeSize (void) const
{
  return sizeof (ImplAAFObject*);
}


OMProperty * ImplAAFTypeDefStrongObjRef::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  assert (name);
  OMProperty * result =
	new OMStrongReferenceProperty<ImplAAFObject> (pid, name);
  assert (result);
  return result;
}






// override from OMStorable.
const OMClassId& ImplAAFTypeDefStrongObjRef::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefStrongObjRef));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefStrongObjRef::onSave(void* clientContext) const
{
  ImplAAFTypeDefObjectRef::onSave(clientContext);
}

void ImplAAFTypeDefStrongObjRef::onRestore(void* clientContext) const
{
  ImplAAFTypeDefObjectRef::onRestore(clientContext);
}

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


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#ifndef __ImplAAFCollection_h__
#include "ImplAAFCollection.h"
#endif

#ifndef __ImplAAFEnumerator_h__
#include "ImplAAFEnumerator.h"
#endif

#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplEnumAAFPropertyDefs> ImplEnumAAFPropertyDefsSP;

#ifndef __ImplAAFBaseClassFactory_h__
#include "ImplAAFBaseClassFactory.h"
#endif

#include <assert.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "OMFile.h"
#include "ImplAAFHeader.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFMetaDictionary.h"
#include "ImplAAFClassDef.h"
#include "ImplAAFProperty.h"
#include "ImplAAFPropertyDef.h"
#include "ImplAAFPropertyValue.h"
#include "ImplAAFPropValData.h"
#include "ImplEnumAAFProperties.h"

#include "ImplAAFObjectCreation.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "OMPropertyDefinition.h"
#include "AAFUtils.h"


extern "C" const aafClassID_t CLSID_AAFProperty;
extern "C" const aafClassID_t CLSID_EnumAAFProperties;

// Temporarily disable creation and removal of optional property values for v1.0
// This feature will be enabled in v1.1.
#ifndef ENABLE_NEW_OPTIONAL_PROPVALUE
#define ENABLE_NEW_OPTIONAL_PROPVALUE 0
#endif


//
// Private class for implementing collections of properties.
//
class ImplPropertyCollection : public ImplAAFCollection<ImplAAFProperty *>
{
public:
  ImplPropertyCollection ();
  ~ImplPropertyCollection ();

  // overrides
  virtual AAFRESULT
    GetNumElements
        (aafUInt32 * pCount);

  virtual AAFRESULT
    GetNthElement
        (aafUInt32  index,
         ImplAAFProperty ** pElem);

  void SetPropertyValue (ImplAAFPropertyDef * pPropDef,
						 ImplAAFPropertyValue * pNewPropVal);

  AAFRESULT
    Initialize (ImplAAFObject * pObj,
				OMPropertySet * pOMPropSet);

private:
  ImplAAFPropertySP * _pProperties;
  aafUInt32           _numUsed;
  aafUInt32           _numAllocated;
  OMPropertySet     * _pOMPropSet;

  
};



//
// 1) See if there is an OMProperty in this OMPropertySet
//    corresponding to the given OMPropertyId.  If not, it's an
//    error.  If so, remember that OMProperty.
//
// 2) See if there is an existing ImplAAFProperty in _pProperties
//    corresponding to the given OMPropertyId.  If so, remember it.
//    If not, create one, and initialize it, and remember it.
//
// 3) Set the remembered property to contain the new property value.
//
// 4) Set the remembered OMProperty's bits to the new prop value's
//    bits.
//
void ImplPropertyCollection::SetPropertyValue
(
 ImplAAFPropertyDef * pPropDef,
 ImplAAFPropertyValue * pNewPropVal
)
{
  assert (pPropDef);
  assert (pNewPropVal);
  assert (_pOMPropSet);

  AAFRESULT hr;

  const OMPropertyId pid = pPropDef->OmPid ();

  //
  // 1) See if there is an OMProperty in this OMPropertySet
  //    corresponding to the given OMPropertyId.  If not, it's an
  //    error.  If so, remember that OMProperty.
  //
  const aafUInt32 numPropsDefined = _pOMPropSet->count();
  size_t omContext = 0;
  OMProperty * pOmProp = 0;
  aafUInt32 i;
  for (i = 0; i < numPropsDefined; i++)
	{
	  pOmProp = 0;
	  _pOMPropSet->iterate (omContext, pOmProp);
	  assert (pOmProp);
	  if (pOmProp->propertyId() == pid)
		{
		  break;
		}
	  else
		pOmProp = 0;
	}

  // If pOmProp is still null, that pid wasn't found.  This should
  // have been detected earlier.
  assert (pOmProp);

  //
  // 2) See if there is an existing ImplAAFProperty in _pProperties
  //    corresponding to the given OMPropertyId.  If so, remember it.
  //    If not, create one, and initialize it, and remember it.
  //
  ImplAAFPropertySP pProp;
  for (i = 0; i < _numUsed; i++)
	{
	  pProp = _pProperties[i];
	  assert (pProp);

	  ImplAAFPropertyDefSP pTestPropDef;
	  hr = pProp->GetDefinition (&pTestPropDef);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  assert (pTestPropDef);
	  OMPropertyId testPid = pTestPropDef->OmPid ();
	  if (testPid == pid)
		{
		  // If this pid matches, bail out of the loop with pProp
		  // pointing to the matching property.
		  assert (pProp);
		  break;
		}
	  pProp = 0;
	}
  if (! pProp)
	{
	  // There was no existing property in the collection.  Create and
	  // append a new one.

	  // We should have allocated enough space for all optional properties.
	  assert (_numUsed < _numAllocated);

	  ImplAAFProperty * tmp = (ImplAAFProperty*) CreateImpl (CLSID_AAFProperty);
	  if (! tmp) 
		throw AAFRESULT_NOMEMORY;
	  assert (! pProp);
	  pProp = tmp;
	  tmp->ReleaseReference ();

	  assert (pPropDef);
	  assert (pOmProp);
	  hr = pProp->Initialize (pPropDef, pOmProp);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  assert (! _pProperties[_numUsed]);
	  _pProperties[_numUsed] = pProp;
	  _numUsed++;
	}

  //
  // 3) Set the remembered property to contain the new property value.
  //
  assert (pProp);
  hr = pProp->pvtSetValue (pNewPropVal);
  assert (AAFRESULT_SUCCEEDED (hr));
	  
  //
  // 4) Set the remembered OMProperty's bits to the new prop value's
  // bits.
  //
  assert (pOmProp);
  assert (pNewPropVal);
  hr = pNewPropVal->WriteTo(pOmProp);
  assert (AAFRESULT_SUCCEEDED (hr));
}


ImplPropertyCollection::ImplPropertyCollection ()
  : _pProperties (0),
	_numUsed (0),
	_pOMPropSet (0)
{}


AAFRESULT ImplPropertyCollection::Initialize
(
 ImplAAFObject * pObj,
 OMPropertySet * pOMPropSet
)
{
  ImplAAFPropertyDefSP pPropDef;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;

  // make sure we haven't called this before
  assert (! _pProperties);

  if (! pOMPropSet)
	return AAFRESULT_NULL_PARAM;

  _pOMPropSet = pOMPropSet;

  // includes count of props both present and absent
  const aafUInt32 numPropsDefined = pOMPropSet->count();
  // only includes props that are present
  aafUInt32 numPropsPresent = 0;

  // count number of properties that are present
  size_t omContext = 0;
  OMProperty * pOmProp = NULL;
  for (aafUInt32 i = 0; i < numPropsDefined; i++)
	{
	  pOmProp = 0;
	  pOMPropSet->iterate (omContext, pOmProp);
	  assert (pOmProp);
	  // count it if it's mandatory, or (optional AND present)
	  if (!pOmProp->isOptional() || pOmProp->isPresent())
		numPropsPresent++;
	}

  _numUsed = numPropsPresent;

  // Allocate enough props for all defined (not just ones present) to
  // make it easier to set optional properties which are currently not
  // present.
  _pProperties = new ImplAAFPropertySP [numPropsDefined];
  if (! _pProperties)
	return AAFRESULT_NOMEMORY;
  _numAllocated = numPropsDefined;

  ImplAAFClassDefSP pClassDef;
  try
	{
	  AAFRESULT hr;
	  assert (pObj);
	  hr = pObj->GetDefinition(&pClassDef);
	  if (AAFRESULT_FAILED(hr)) throw hr;
	  assert (pClassDef);

	  size_t omContext = 0;
	  OMProperty * pOmProp = NULL;
	  aafUInt32 presentPropIdx = 0;
	  for (aafUInt32 definedPropIdx = 0;
		   definedPropIdx < numPropsDefined;
		   definedPropIdx++)
		{
		  pOMPropSet->iterate (omContext, pOmProp);
		  assert (pOmProp);
		  if (pOmProp->isOptional() && !pOmProp->isPresent())
			// optional property not present
			continue;

		  OMPropertyId opid = pOmProp->propertyId ();
		  assert (pClassDef);
		  AAFRESULT hr = pClassDef->LookupPropertyDefbyOMPid (opid, &pPropDef);
		  if (AAFRESULT_FAILED (hr)) throw hr;
		  assert (pPropDef);

		  // Create property; array is smart pointers, which will
		  // maintain their own reference counts.  First assign new
		  // prop to temp, so we can release it after the sp
		  // assignment.
		  ImplAAFProperty * tmp = (ImplAAFProperty*) CreateImpl (CLSID_AAFProperty);
		  if (! tmp) 
			throw AAFRESULT_NOMEMORY;
		  _pProperties[presentPropIdx] = tmp;
		  tmp->ReleaseReference ();
		  tmp = 0;

		  hr = _pProperties[presentPropIdx]->Initialize (pPropDef, pOmProp);
		  if (AAFRESULT_FAILED (hr)) throw hr;

		  presentPropIdx++;
		  assert (presentPropIdx <= numPropsPresent);
		}
	  assert (presentPropIdx == numPropsPresent);
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	  if (_pProperties)
		{
		  delete[] _pProperties;
		  _pProperties = NULL;
		}
	}

  return rReturned;
}


ImplPropertyCollection::~ImplPropertyCollection ()
{
  if (_pProperties)
	{
	  delete[] _pProperties;
	  _pProperties = NULL;
	}
}


AAFRESULT ImplPropertyCollection::GetNumElements
(aafUInt32 * pCount)
{
  if (! pCount)
	return AAFRESULT_NULL_PARAM;
  *pCount = _numUsed;
  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplPropertyCollection::GetNthElement
(
 aafUInt32  index,
 ImplAAFProperty ** pElem
)
{
  if (! pElem)
	return AAFRESULT_NULL_PARAM;

  if (index >= _numUsed)
	return AAFRESULT_NO_MORE_OBJECTS;

  assert (_pProperties);
  assert (_pProperties[index]);
  assert (pElem);
  *pElem = _pProperties[index];
  (*pElem)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


ImplAAFObject::ImplAAFObject ()
  : _generation(PID_InterchangeObject_Generation, L"Generation"),
	_pProperties (0),
	_cachedDefinition (0),
	_apSavedProps (0),
	_savedPropsSize (0),
	_savedPropsCount (0)
{
  _persistentProperties.put(_generation.address());

  const aafUID_t null_uid = { 0 };
  _soid = null_uid;
}


ImplAAFObject::SavedProp::SavedProp (OMProperty * p)
  : _p(p)
{
  assert (p);
}


ImplAAFObject::SavedProp::~SavedProp ()
{
  assert (_p);
  // The template argument here *must* match the type allocated in
  // ImplAAFTypeDefFixedArray::pvtCreateOMProperty() and
  // ImplAAFTypeDefVariableArray::pvtCreateOMProperty().
  OMStrongReferenceVectorProperty<ImplAAFObject> * srv =
	dynamic_cast<OMStrongReferenceVectorProperty <ImplAAFObject>*>(_p);
  if (srv)
	{
	  size_t size = srv->getSize();
	  for (size_t i = 0; i < size; i++)
		{
		  ImplAAFObject* oldObj = srv->setValueAt (0, i);
		  if (oldObj)
			{
			  oldObj->ReleaseReference ();
			  oldObj = 0;
			}
		}
	}
  else
	{
	  // The template argument here *must* match the type
	  // allocated in
	  // ImplAAFTypeDefStrongObjRef::pvtCreateOMProperty().
	  OMStrongReferenceProperty<ImplAAFObject> * sro =
		dynamic_cast<OMStrongReferenceProperty<ImplAAFObject>*>(_p);
	  if (sro)
		{
		  ImplAAFObject* oldObj = sro->setValue (0);
		  if (oldObj)
			{
			  oldObj->ReleaseReference ();
			  oldObj = 0;
			}
		}
	}
  delete _p;
}



ImplAAFObject::~ImplAAFObject ()
{
  _cachedDefinition = 0; // we don't need to reference count this defintion

  if (_pProperties)
	delete _pProperties;

  for (size_t i = 0;
	   i <_savedPropsCount;
	   i++)
	{
	  delete _apSavedProps[i];
	}
  delete [] _apSavedProps;
}


void ImplAAFObject::RememberAddedProp (OMProperty * pProp)
{
  assert (pProp);

  // make sure that number of slots allocated (size) is no smaller
  // than number used (count).
  assert (_savedPropsSize >= _savedPropsCount);

  // If we need a new one, allocate more.
  if (_savedPropsSize == _savedPropsCount)
	{
	  // Need more.  We'll allocate in chunks of 10 props.
	  aafUInt32 newSize = _savedPropsSize + 10;
	  SavedProp ** pNewSavedProps = new SavedProp*[newSize];
	  // Clear the allocated ones.
	  memset (pNewSavedProps, 0, sizeof (SavedProp*) * newSize);
	  // Copy over the ones already allocated.
	  for (size_t i = 0; i < _savedPropsCount; i++)
		{
		  pNewSavedProps[i] = _apSavedProps[i];
		}
	  // ditch the old storage
	  delete [] _apSavedProps;
	  // zero this to keep Bounds Checker happy
	  _apSavedProps = 0;
	  // remember the new storage
	  _apSavedProps = pNewSavedProps;
	  // zero this to keep Bounds Checker happy
	  pNewSavedProps = 0;
	  _savedPropsSize = newSize;
	}
  // We have the space.  Save the prop.
  assert (_savedPropsSize >= _savedPropsCount);
  _apSavedProps[_savedPropsCount] = new SavedProp(pProp);
  assert (_apSavedProps[_savedPropsCount]);
  _savedPropsCount++;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetGeneration (ImplAAFIdentification ** ppResult)
{
  if (!ppResult)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  if (! pvtIsGenerationTracked ())
	return AAFRESULT_INVALID_PARAM;

  aafUID_t gen;
  hr = GetGenerationAUID (&gen);
  if (AAFRESULT_FAILED (hr))
	return hr;

  ImplAAFHeaderSP pHead;
  hr = MyHeadObject (&pHead);
  if (AAFRESULT_FAILED (hr))
	// This object is not attached to a file.
	return AAFRESULT_OBJECT_NOT_ATTACHED;

  assert (ppResult);
  return pHead->LookupIdentification (gen, ppResult);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetGenerationAUID (aafUID_t * pResult)
{
  /*
   * BobT note!  We may eventually implement
   * InterchangeObject::Generation as a weak reference to an
   * Identification object (instead of an AUID).  If so, then this
   * method will simply call GetGeneration() for the Identification,
   * and get the AUID from the returned Identification.  It will
   * probably look something like this:
   * 
   * if (!pResult)
   *   return AAFRESULT_NULL_PARAM;
   * 
   * ImplAAFIdentificationSP pId;
   * AAFRESULT hr;
   * hr = GetGeneration (&pId);
   * if (AAFRESULT_FAILED (hr))
   *   return hr;
   * 
   * return pId->GetGeneration (pResult);
   * 
   * Instead, the Generation property is implemented as an AUID so
   * we'll simply return the AUID from the property without going
   * through the Identification object.
   */

  if (!pResult)
	return AAFRESULT_NULL_PARAM;

  if (! pvtIsGenerationTracked())
	return AAFRESULT_INVALID_PARAM;

  assert (_generation.isPresent());
  *pResult = _generation;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetDefinition (ImplAAFClassDef ** ppClassDef)
{
  if (! ppClassDef)
	return AAFRESULT_NULL_PARAM;

  if (! _cachedDefinition)
	{
	  AAFRESULT hr;
	  ImplAAFDictionarySP pDict;
	  hr = GetDictionary(&pDict);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	  assert (pDict);

	  aafUID_t classID;
	  hr = GetObjectClass (&classID);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  ImplAAFClassDef * tmp;
	  hr = pDict->LookupClassDef(classID, &tmp);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	  if (! _cachedDefinition)
		_cachedDefinition = tmp;
	  assert (_cachedDefinition);
		
	  // We don't need to reference count the definitions since
	  // they are owned by the dictionary.
	  aafInt32 count = _cachedDefinition->ReleaseReference();
	  assert(0 < count);
	}
  assert (ppClassDef);
  *ppClassDef = _cachedDefinition;
  assert (*ppClassDef);
  (*ppClassDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetObjectClass (aafUID_t * pClass)
{
  if (NULL == pClass)
    return AAFRESULT_NULL_PARAM;

  //
  // Take advantage of the virtual method that 
  // returns the stored class id for a given class of OMStorable.
  //
#ifndef _DEBUG
  *pClass = *reinterpret_cast<const aafUID_t *>(&classId());
#else
  // In a debug build copy all of the fields separately so
  // that we can detect changes in the definitions of public aafUID_t
  // and private OMClassId types.
  const OMClassId& id = classId();
  pClass->Data1 = id.Data1;
  pClass->Data2 = id.Data2;
  pClass->Data3 = id.Data3;
  pClass->Data4[0] = id.Data4[0];
  pClass->Data4[1] = id.Data4[1];
  pClass->Data4[2] = id.Data4[2];
  pClass->Data4[3] = id.Data4[3];
  pClass->Data4[4] = id.Data4[4];
  pClass->Data4[5] = id.Data4[5];
  pClass->Data4[6] = id.Data4[6];
  pClass->Data4[7] = id.Data4[7];
#endif

  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFObject::InitProperties ()
{
  if (! _pProperties)
	{
	  _pProperties = new ImplPropertyCollection;
	  if (! _pProperties)
		return AAFRESULT_NOMEMORY;
	  OMPropertySet * ps = propertySet();
	  assert (ps);
	  AAFRESULT hr = _pProperties->Initialize (this, ps);
	  if (AAFRESULT_FAILED (hr)) return hr;
	}
  assert (_pProperties);
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetProperties (ImplEnumAAFProperties ** ppEnum)
{
  if (! ppEnum)
	return AAFRESULT_NULL_PARAM;

  if (! _pProperties)
	{
	  AAFRESULT hr = InitProperties();
	  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
	}
  assert (_pProperties);
  
  ImplEnumAAFProperties * pEnum = NULL;
  pEnum = dynamic_cast<ImplEnumAAFProperties*>(CreateImpl(
	CLSID_EnumAAFProperties));
  if (!pEnum)
	return E_FAIL;
  assert (pEnum);
  AAFRESULT hr = pEnum->Initialize (_pProperties);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  
  assert (ppEnum);
  *ppEnum = pEnum;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::CountProperties (aafUInt32 * pCount)
{
  if (! pCount)
	return AAFRESULT_NULL_PARAM;

  assert (pCount);
  if (! _pProperties)
	{
	  AAFRESULT hr = InitProperties();
	  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
	}
  assert (_pProperties);
  AAFRESULT hr = _pProperties->GetNumElements (pCount);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetPropertyValue (ImplAAFPropertyDef * pPropDef,
									 ImplAAFPropertyValue ** ppPropVal)
{
  if (! pPropDef)
	return AAFRESULT_NULL_PARAM;

  if (! ppPropVal)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  ImplAAFClassDefSP pClass;
  hr = GetDefinition (&pClass);
  assert (AAFRESULT_SUCCEEDED (hr));

  const OMPropertyId pid = pPropDef->OmPid ();

  ImplAAFPropertyDefSP pTempProp;
  hr = pClass->LookupPropertyDefbyOMPid (pid, &pTempProp);
  // pTempProp is unused
  if (AAFRESULT_FAILED (hr))
	return AAFRESULT_BAD_PROP;

  if (! _pProperties)
	{
	  hr = InitProperties();
	  if (AAFRESULT_FAILED (hr))
		return hr;
	}
  assert (_pProperties);

  ImplEnumAAFPropertiesSP pPropEnum;
  hr = GetProperties (&pPropEnum);
  assert (AAFRESULT_SUCCEEDED (hr));

  ImplAAFPropertySP pProp;
  while (AAFRESULT_SUCCEEDED (pPropEnum->NextOne (&pProp)))
	{
	  assert (pProp);
	  ImplAAFPropertyDefSP pTestPropDef;
	  hr = pProp->GetDefinition (&pTestPropDef);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  OMPropertyId testPid;
	  testPid = pTestPropDef->OmPid ();

	  if (pid == testPid)
		{
		  assert (ppPropVal);
		  hr = pProp->GetValue (ppPropVal);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  return AAFRESULT_SUCCESS;
		}
	  pProp = 0;
	}
  return AAFRESULT_PROP_NOT_PRESENT;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::SetPropertyValue (ImplAAFPropertyDef * pPropDef,
									 ImplAAFPropertyValue * pPropVal)
{
  if (!pPropDef)
	return AAFRESULT_NULL_PARAM;

  if (!pPropVal)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  ImplAAFClassDefSP pClass;
  hr = GetDefinition (&pClass);
  assert (AAFRESULT_SUCCEEDED (hr));

  const OMPropertyId pid = pPropDef->OmPid ();

  ImplAAFPropertyDefSP pTempProp;
  hr = pClass->LookupPropertyDefbyOMPid (pid, &pTempProp);
  // pTempProp is unused
  if (AAFRESULT_FAILED (hr))
	return AAFRESULT_BAD_PROP;

  if (! _pProperties)
	{
	  hr = InitProperties();
	  if (AAFRESULT_FAILED (hr))
		return hr;
	}
  assert (_pProperties);

  _pProperties->SetPropertyValue (pPropDef, pPropVal);
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::IsPropertyPresent (ImplAAFPropertyDef * pPropDef,
									  aafBoolean_t * pResult)
{
  if (! pPropDef)
	return AAFRESULT_NULL_PARAM;

  if (! pResult)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;
  ImplAAFPropertyValueSP pTempPropVal;
  hr = GetPropertyValue (pPropDef, &pTempPropVal);
  // pTempPropVal unused

  if (AAFRESULT_PROP_NOT_PRESENT == hr)
	{
	  *pResult = kAAFFalse;
	  return AAFRESULT_SUCCESS;
	}
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  *pResult = kAAFTrue;
	  return AAFRESULT_SUCCESS;
	}
  return hr;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::RemoveOptionalProperty (
      ImplAAFPropertyDef * /*pPropDef*/)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::CreateOptionalPropertyValue (
      ImplAAFPropertyDef * pPropDef,
      ImplAAFPropertyValue ** ppPropVal)
{
  if (!pPropDef || !ppPropVal)
	  return AAFRESULT_NULL_PARAM;

#if ENABLE_NEW_OPTIONAL_PROPVALUE
  
  AAFRESULT result = AAFRESULT_SUCCESS;
  ImplAAFTypeDefSP pPropertyType;
  *ppPropVal = NULL;
  
  // 
  aafBoolean_t alreadyPresent = kAAFFalse;
  result = IsPropertyPresent(pPropDef, &alreadyPresent);
  if (AAFRESULT_SUCCEEDED(result))
  {
    if (!alreadyPresent)
    {
      result = pPropDef->GetTypeDef(&pPropertyType);
      if (AAFRESULT_SUCCEEDED(result))
      {
        result = pPropertyType->CreatePropertyValue(ppPropVal);
      }
    }
    else
    {
      // Cannot create a value if it is already present?
      result = AAFRESULT_PROP_ALREADY_PRESENT;
    }
  }
    
  return result;

#else	

  return AAFRESULT_NOT_IN_CURRENT_VERSION;
    
#endif
}


//************
// Interfaces ivisible inside the toolkit, but not exposed through the API

// Gets the head object of the file containing this object.
// This function is used to maintain MOB and Definition tables in the
// head object.
AAFRESULT ImplAAFObject::MyHeadObject
	(ImplAAFHeader **header) const
{
	OMFile			*myFile;
	OMStorable		*theRoot;
	ImplAAFHeader	*theHeader;

	XPROTECT()
	{
		if(header == NULL)
			RAISE(AAFRESULT_NULL_PARAM);

		if(!attached())
			RAISE(AAFRESULT_OBJECT_NOT_ATTACHED); 

                if(!inFile())
			RAISE(AAFRESULT_OBJECT_NOT_IN_FILE); 

		myFile = file();
		if(myFile == NULL)
			RAISE(AAFRESULT_OBJECT_NOT_IN_FILE);

		theRoot = myFile->root();
		if(theRoot == NULL)
			RAISE(AAFRESULT_BADHEAD);

		theHeader = dynamic_cast<ImplAAFHeader*>(theRoot);
		if(theRoot == NULL)
			RAISE(AAFRESULT_BADHEAD);
		theHeader->AcquireReference();

		*header = theHeader;
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS);
}

// Gets the dictionary used to create this instance.
AAFRESULT STDMETHODCALLTYPE
ImplAAFObject::GetDictionary(ImplAAFDictionary **ppDictionary) const
{
  if(NULL == ppDictionary)
    return AAFRESULT_NULL_PARAM;

  *ppDictionary = dynamic_cast<ImplAAFDictionary *>(classFactory());

  if (NULL == *ppDictionary)
  {
    // The other OMFactory is the meta dictionary. If so, then return the 
    // data dictionary set when the meta dictionary was created.
    // (NOTE: This may be temporary code...transdel:2000-APR-14)
    ImplAAFMetaDictionary *pMetaDictionary = dynamic_cast<ImplAAFMetaDictionary *>(classFactory());
    if (pMetaDictionary)
    {
      *ppDictionary = pMetaDictionary->dataDictionary(); // not reference counted!
    }
  }  
  
  assert(NULL != *ppDictionary);
  if (NULL == *ppDictionary)
    return AAFRESULT_INVALID_OBJ;
  
  // Bump the reference count...
  (*ppDictionary)->AcquireReference();
  
  return AAFRESULT_SUCCESS;
}


void ImplAAFObject::pvtSetSoid (const aafUID_t & id)
{
  const aafUID_t null_uid = { 0 };

  // make sure it hasn't been set already
  assert (!EqualAUID (&id, &null_uid));

  // make sure new one is valid
  assert (EqualAUID (&_soid, &null_uid));
  _soid = id;
}


//
// Here is the mapping of DM type defs to OMProperty concrete
// classes.
//
// DM TypeDef				Treatment
// ----------				-------------------------
// AAFTypeDefEnum			FixedData(sizeof rep'd type)
//
// AAFTypeDefExtEnum		FixedData(sizeof auid)
//
// AAFTypeDefFixedArray     FixedData(sizeof elem * num elems)
//
// AAFTypeDefInt			FixedData(sizeof int)
//
// AAFTypeDefRecord         FixedData(sum of sizes of elements)
//
// AAFTypeDefRename         <refer to referenced type>
//
// AAFTypeDefSet:
//   if elem type is StrRef	OMStrongReferenceVectorProperty<AAFObject>
//   if elem type is WkRef  VariableData(sizeof auid)
//   if elem is fixed data  VariableData(sizeof elem)
//   else                   <not yet supported; maybe never!>
//
// AAFTypeDefString			VariableData(sizeof elem)
//
// AAFTypeDefStrongObjRef	OMStrongReferenceProperty<AAFObject>
//
// AAFTypeDefVariableArray
//   if elem type is StrRef	OMStrongReferenceVectorProperty<AAFObject>
// 	 if elem type is WkRef  VariableData(sizeof auid)
// 	 if elem is fixed data  VariableData(sizeof elem)
//   else                   <not yet supported; maybe never!>
//
// AAFTypeDefWeakObjRef     FixedData(sizeof auid)
//


// Associate the existing OMProperties with corresponding property definitions from
// the given class definition. NOTE: This call is recursive, it calls itself again
// for the parent class of the given class until current class is a "root" class.
void ImplAAFObject::InitOMProperties (ImplAAFClassDef * pClassDef)
{
  assert (pClassDef);
  AAFRESULT hr;

  //
  // Init base class properties first
  //
  ImplAAFClassDefSP parentSP;
	hr = pClassDef->GetParent (&parentSP);
  // check that only a "root" will have no parent class definition.
  assert (AAFRESULT_SUCCEEDED(hr) || (AAFRESULT_FAILED(hr) && AAFRESULT_IS_ROOT_CLASS == hr));
  if(AAFRESULT_SUCCEEDED(hr))
  {
	  assert (parentSP);
	  InitOMProperties (parentSP);
  }

  // See if currently existing OM properties are defined in the class
  // def.
  //
  OMPropertySet * ps = propertySet();
  assert (ps);
  const size_t propCount = ps->count();

  // Loop through properties of this class
  ImplEnumAAFPropertyDefsSP pdEnumSP;
  hr = pClassDef->GetPropertyDefs (&pdEnumSP);
  assert (AAFRESULT_SUCCEEDED (hr));

  ImplAAFPropertyDefSP propDefSP;
  while (AAFRESULT_SUCCEEDED (pdEnumSP->NextOne (&propDefSP)))
	{
	  OMPropertyId defPid = propDefSP->OmPid ();
	  // assert (ps->isAllowed (defPid));
	  OMProperty * pProp = 0;
	  if (ps->isPresent (defPid))
		{
		  // Defined property was already in property set.  (Most
		  // probably declared in the impl constructor.)  Get that
		  // property.
		  pProp = ps->get (defPid);
		}		  
		else if(defPid != PID_InterchangeObject_ObjClass
			/* && (defPid != PID_InterchangeObject_Generation)
			 && (defPid != PID_PropertyDefinition_DefaultValue) */)
		{
		  // Defined property wasn't found in OM property set.
		  // We'll have to install one.
		  pProp = propDefSP->CreateOMProperty ();
		  assert (pProp);
		  
		  // Remember this property so we can delete it later.
		  RememberAddedProp (pProp);
		  
		  // Add the property to the property set.
		  ps->put (pProp);
	  }
	  
	if(defPid != PID_InterchangeObject_ObjClass
	   /* && (defPid != PID_InterchangeObject_Generation)
	      && (defPid != PID_PropertyDefinition_DefaultValue) */)
	{
		  ImplAAFPropertyDef * pPropDef =
			  (ImplAAFPropertyDef*) propDefSP;
		  OMPropertyDefinition * pOMPropDef =
			  dynamic_cast<OMPropertyDefinition*>(pPropDef);
		  assert (pOMPropDef);
		  
		  assert (pProp);
		  pProp->initialize (pOMPropDef);
		  
		  pPropDef = 0;
		  pOMPropDef = 0;
	  }
	  propDefSP = 0;
	  pProp = 0;
  }
}


//
// Define the symbol for the stored object id
//
const OMClassId& ImplAAFObject::classId(void) const
{
  const aafUID_t null_uid = { 0 };
  assert (! EqualAUID (&_soid, &null_uid));
  return *reinterpret_cast<const OMClassId* const>(&_soid);
}



// Create and intialize associated external extensions.
AAFRESULT ImplAAFObject::InitializeExtensions(void)
{
  AAFRESULT hr = AAFRESULT_SUCCESS;
  ImplAAFClassDef *pDef = NULL;
  ImplAAFClassDef *pParentDef;
  const aafClassID_t* id;


  try
  {
    // We need to walk the class definitions:
    // 1. If the class definition is an extension class, and
    // 2. If the class definition has an associated plugin code (clsid), and
    // 3. Attempt to intialize the associated extension for this object's container.
    // 4. If step 3 fails then goto step 1 for the parent class definition.

    hr = GetDefinition (&pDef);
  
    while (AAFRESULT_SUCCESS == hr)
    {
      aafUID_t auid;
      hr = pDef->GetAUID(&auid);
      if (AAFRESULT_SUCCESS != hr)
        break;

      // There should probably be a method on ImplAAFClassDef to 
      // determine whether or not a particular class is an extended
      // class. At the very least this method could cache the result
      // of the following lookup so that lookup is only performed
      // once per file. (TomR:991111)
      //
      // Lookup the code class id for the given stored object id.
      id = ImplAAFBaseClassFactory::LookupClassID(auid);
      if (NULL != id)
        break; // we don't support extenting built-in classes!
    
      // If the intialize is successful then we are done. We currently
      // only support a one class extension per object.
      hr = InitializeImplExtension(this, auid);
      if (AAFRESULT_SUCCESS == hr)
        break;

      // Try again with the parent class defintion.
      hr = pDef->GetParent (&pParentDef);
      if (AAFRESULT_SUCCESS == hr)
      {
        // NOTE:We only maintain one class definition reference in the loop.
        pDef->ReleaseReference();
        pDef = pParentDef;
      }

    } // while

    if (NULL != pDef)
      pDef->ReleaseReference();
  }
  catch (...)
  {
    if (NULL != pDef)
      pDef->ReleaseReference();
    
    throw;
  }


  return hr;
}



aafBoolean_t ImplAAFObject::pvtIsGenerationTracked() const
{
  return _generation.isPresent() ? kAAFTrue : kAAFFalse;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::IsGenerationTracked (aafBoolean_t * pResult) const
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;

  *pResult = pvtIsGenerationTracked();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::EnableGenerationTracking ()
{
  if (! _generation.isPresent())
	{
	  aafUID_t nullUid = { 0 };
	  _generation = nullUid;
	}
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::DisableGenerationTracking ()
{
  if (_generation.isPresent())
	{
	  _generation.remove();
	}
  return AAFRESULT_SUCCESS;
}


void ImplAAFObject::onSave(void* clientContext) const
{
  if (clientContext)
	{
	  aafUID_t * pGen = (aafUID_t*) clientContext;
	  if (pvtIsGenerationTracked())
		{
		  assert (pGen);
		  ImplAAFObject * pNonConstThis = (ImplAAFObject*) this;
		  pNonConstThis->_generation = *pGen;
		}
	}
}


void ImplAAFObject::onRestore(void* /*clientContext*/) const
{
  // clientContext currently unused

  // Cast away constness (maintaining logical constness)
  ((ImplAAFObject*) this)->setInitialized ();
}

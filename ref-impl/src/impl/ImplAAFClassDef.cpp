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


#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFHeader.h"
#include "AAFUtils.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFPropertyDefs;
extern "C" const aafClassID_t CLSID_AAFObject;

//
// locals obj mgmt utils
//
#define check_result(result)     \
  if (AAFRESULT_FAILED (result)) \
	return result;

/* Note!  Will modify argument... */
#define release_if_set(pIfc)     \
{                                \
  if (pIfc)                      \
	{                            \
	  pIfc->ReleaseReference (); \
	  pIfc = 0;                  \
	}                            \
}

/* Note!  Will modify argument... */
#define release_and_zero(pIfc)   \
{                                \
  assert (pIfc);                 \
  pIfc->ReleaseReference ();     \
  pIfc = 0;                      \
}



ImplAAFClassDef::ImplAAFClassDef ()
  : _ParentClass  ( PID_ClassDefinition_ParentClass,  L"ParentClass", L"/Dictionary/ClassDefinitions", PID_MetaDefinition_Identification),
	_Properties   ( PID_ClassDefinition_Properties,   L"Properties", PID_MetaDefinition_Identification),
	_IsConcrete   ( PID_ClassDefinition_IsConcrete,   L"IsConcrete"),
	_propTypesLoaded (false),
	_BootstrapParent(0)
{
  _persistentProperties.put(_ParentClass.address());
  _persistentProperties.put(_Properties.address());
  _persistentProperties.put(_IsConcrete.address());
}


ImplAAFClassDef::~ImplAAFClassDef ()
{
  aafUInt32 numProps;
  AAFRESULT hr = CountPropertyDefs (&numProps);
  if (AAFRESULT_FAILED (hr))
	throw hr;

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPropertyDef>propertyDefinitions(_Properties);
	while(++propertyDefinitions)
	{
		ImplAAFPropertyDef *pProperty = propertyDefinitions.setValue(0);
		if (pProperty)
		{
		  pProperty->ReleaseReference();
		  pProperty = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::Initialize (
      const aafUID_t & classID,
      ImplAAFClassDef * pParentClass,
      const aafCharacter * pClassName,
	  aafBool isConcrete)
{
  if (!pClassName) return AAFRESULT_NULL_PARAM;

  return pvtInitialize (classID, pParentClass, pClassName, isConcrete);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::pvtInitialize (
      const aafUID_t & classID,
      const ImplAAFClassDef * pParentClass,
      const aafCharacter * pClassName,
	  aafBool isConcrete)
{
//	ImplAAFClassDef	*oldParent;
  if (!pClassName) return AAFRESULT_NULL_PARAM;	
  if (pParentClass && !pParentClass->attached())
    return AAFRESULT_OBJECT_NOT_ATTACHED;	
	
  HRESULT hr;
  hr = ImplAAFMetaDefinition::Initialize(classID, pClassName, NULL);
  if (AAFRESULT_FAILED (hr))
    return hr;
	
  _ParentClass = pParentClass;
  _IsConcrete = isConcrete;
  return AAFRESULT_SUCCESS;
}


aafBool ImplAAFClassDef::pvtIsConcrete () const
{
  return _IsConcrete;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetPropertyDefs (
      ImplEnumAAFPropertyDefs ** ppEnum)
{
  ImplEnumAAFPropertyDefs * theEnum = 0;

  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;

  theEnum = (ImplEnumAAFPropertyDefs *)CreateImpl (CLSID_EnumAAFPropertyDefs);
  if (NULL == theEnum)
  	return AAFRESULT_NOMEMORY;

  AAFRESULT hr;
  OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPropertyDef>* iter = 
	new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPropertyDef>(_Properties);
  if(iter == 0)
	hr = AAFRESULT_NOMEMORY;
  else
	hr = theEnum->SetIterator(this, iter);
  if (AAFRESULT_FAILED (hr))
	{
		theEnum->ReleaseReference();
		theEnum = NULL;
		return hr;
	}
	
  *ppEnum = theEnum;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::CountPropertyDefs (
      aafUInt32 *  pCount)
{
  if (! pCount) return AAFRESULT_NULL_PARAM;

  assert (pCount);
  if (!_Properties.isPresent())
	*pCount = 0;

  else  *pCount = _Properties.getSize ();
	
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::RegisterNewPropertyDef (
      const aafUID_t &      id,
      const aafCharacter *  pName,
      ImplAAFTypeDef *      pTypeDef,
      aafBool               isOptional,
      aafBool               isIsUniqueIdentifier,
      ImplAAFPropertyDef ** ppPropDef)
{
  // This will only work if this class has not yet been registered.

  if (! pTypeDef)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;
  ImplAAFDictionarySP pDict;
  hr = GetDictionary (&pDict);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  // A dictionary has been identified; see if we're registered in
	  // it.
	  aafUID_t thisClassID;

	  // get our class ID
	  hr = GetAUID (&thisClassID);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  if (pDict->PvtIsClassPresent(thisClassID))
		{
		  // pClassDef is unused; we only want to know the result of
		  // the lookup.
		  return AAFRESULT_OBJECT_ALREADY_ATTACHED;
		}
	}

  // If we're here, we're not already registered.  OK to continue.

  aafUID_t typeId;
  hr = pTypeDef->GetAUID (&typeId);
  if (AAFRESULT_FAILED (hr))
	return hr;

  // Check that this class or any parent class is already uniquely
  // identified.
  if (isIsUniqueIdentifier)
  {
    aafBoolean_t alreadyUniquelyIdentified = kAAFFalse;
    hr = IsUniquelyIdentified(&alreadyUniquelyIdentified);
    if (AAFRESULT_FAILED(hr))
      return hr;
    
    // "There can be only one" unique identifier per class.
    if (alreadyUniquelyIdentified)
      return AAFRESULT_ALREADY_UNIQUELY_IDENTIFIED;
  }

	if(PvtIsPropertyDefRegistered(id))
		return AAFRESULT_SUCCESS;
	else
	{
		bool	isDuplicate;
		
		hr = pDict->PvtIsPropertyDefDuplicate(id, this, &isDuplicate);
		if (AAFRESULT_FAILED(hr))
			return hr;
		if(isDuplicate)
			return AAFRESULT_PROPERTY_DUPLICATE;
		return pvtRegisterPropertyDef (id,
									pName,
									typeId,
									isOptional,
									isIsUniqueIdentifier,
									ppPropDef);
	}
}	


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::RegisterOptionalPropertyDef (
      const aafUID_t &      id,
      const aafCharacter *  pName,
      ImplAAFTypeDef *      pTypeDef,
      ImplAAFPropertyDef ** ppPropDef)
{
  AAFRESULT hr;
  if (! pTypeDef)
	return AAFRESULT_NULL_PARAM;

  ImplAAFDictionarySP pDict;
  hr = GetDictionary (&pDict);
  assert (AAFRESULT_SUCCEEDED (hr));

  aafUID_t myAuid;
  hr = GetAUID (&myAuid);
  assert (AAFRESULT_SUCCEEDED (hr));

  // Test to see if this class is axiomatic; if so, then we can't
  // augment it.
  if (pDict->IsAxiomaticClass (myAuid))
	return AAFRESULT_NOT_EXTENDABLE;

  aafUID_t typeId;
  hr = pTypeDef->GetAUID (&typeId);
  if (AAFRESULT_FAILED (hr))
	return hr;

	if(PvtIsPropertyDefRegistered(id))
		return AAFRESULT_SUCCESS;
	else
	{
		bool	isDuplicate;
		
		hr = pDict->PvtIsPropertyDefDuplicate(id, this, &isDuplicate);
		if (AAFRESULT_FAILED(hr))
			return hr;
		if(isDuplicate)
			return AAFRESULT_PROPERTY_DUPLICATE;
	    return pvtRegisterPropertyDef (id,
								 pName,
								 typeId,
								 kAAFTrue,
								 kAAFFalse, /* cannot be a unique identifier */
								 ppPropDef);
	}
}


  // Low-level recursive methods the OM find method instead of a linear search
  // that will necessarily load all of the property definitions for the class
  // definition instance.
AAFRESULT
    ImplAAFClassDef::generalLookupPropertyDef (
      aafUID_constref propId,
      ImplAAFPropertyDef ** ppPropDef)
{
	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-MAR-11)
	if (_Properties.find((*reinterpret_cast<const OMObjectIdentification *>(&propId)),
                             *ppPropDef))
	{
		assert(NULL != *ppPropDef);
    (*ppPropDef)->AcquireReference();
	}
	else
	{
    // no recognized class guid in dictionary
    result = AAFRESULT_NO_MORE_OBJECTS; // s/b AAFRESULT_PROPERTY_NOT_FOUND
	}

	return (result);
}

/******/
bool
    ImplAAFClassDef::PvtIsPropertyDefRegistered (
      aafUID_constref propId)
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-MAR-11)
	return (_Properties.contains((*reinterpret_cast<const OMObjectIdentification *>(&propId))));
}

AAFRESULT /*STDMETHODCALLTYPE*/
    ImplAAFClassDef::generalLookupPropertyDef (
      const pvtPropertyIdentifier & propId,
      ImplAAFPropertyDef ** ppPropDef)
{
		///!!!JeffB: Leave this in original loop form until the OM  genericizes the lookup key
  if (! ppPropDef)
	return AAFRESULT_NULL_PARAM;

  ImplEnumAAFPropertyDefsSP pPropEnum;
  ImplAAFPropertyDefSP pPropDef;

  check_result (GetPropertyDefs (&pPropEnum));
  assert (pPropEnum);

  aafUInt32 count;
  check_result (CountPropertyDefs (&count));
  aafUInt32 i;
  for (i = 0; i < count; i++)
	{
	  assert (pPropEnum);
	  check_result (pPropEnum->NextOne (&pPropDef));
	  assert (pPropDef);
	  if (propId.DoesMatch (pPropDef))
		{
		  // Yup, this is the one.
		  assert (ppPropDef);
		  *ppPropDef = pPropDef;
		  assert (*ppPropDef);
		  (*ppPropDef)->AcquireReference ();

		  // Get us outta the loop
		  return AAFRESULT_SUCCESS;
		}
	  else
		{
		  // nope, this prop doesn't match.  Try the next one.
		  pPropDef = 0;
		}
	}
  // If we're here, there aren't any more props so that prop
  // wasn't found in this class; try any base classes.
  ImplAAFClassDefSP parentSP;
  aafBool			isRootObject;
  AAFRESULT hr = IsRoot (&isRootObject);
  if (AAFRESULT_FAILED (hr))
	  return hr;
  if(isRootObject)
	  return AAFRESULT_NO_MORE_OBJECTS;
  else
  {
	  hr = GetParent (&parentSP);
	  if (AAFRESULT_FAILED (hr))
	     return hr;
      return parentSP->generalLookupPropertyDef (propId, ppPropDef);
  }
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::LookupPropertyDef (
      const aafUID_t & propID,
      ImplAAFPropertyDef ** ppPropDef) const
{
  const pvtPropertyIdentifierAUID generalPropId = propID;

  // cast away bitwise const-ness; maintaining conceptual const-ness
  return ((ImplAAFClassDef*)this)->generalLookupPropertyDef
	(generalPropId, ppPropDef);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::LookupPropertyDefbyOMPid (
      OMPropertyId omPid,
      ImplAAFPropertyDef ** ppPropDef) const
{
  const pvtPropertyIdentifierOMPid generalPropId = omPid;

  return ((ImplAAFClassDef*)this)->generalLookupPropertyDef
	(generalPropId, ppPropDef);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetParent (
      ImplAAFClassDef ** ppClassDef)
{
	aafBool		isRoot;
	if (! ppClassDef) return AAFRESULT_NULL_PARAM;
	
	IsRoot (&isRoot);
	if(isRoot)
		return AAFRESULT_IS_ROOT_CLASS;

	if(_ParentClass.isVoid())
		*ppClassDef = _BootstrapParent;	// If we are in the bootstrap process
	else
		*ppClassDef = _ParentClass;		// else follow the weak reference
	assert(*ppClassDef != NULL);
	if(*ppClassDef != NULL)
		(*ppClassDef)->AcquireReference ();
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::IsRoot (
      aafBool * isRoot)
{
	aafUID_t	uid;
	
	if (! isRoot) return AAFRESULT_NULL_PARAM;
	
	GetAUID (&uid);
	*isRoot = memcmp(&uid, &AUID_AAFObject, sizeof(aafUID_t)) == 0;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::IsConcrete (
      aafBool * pResult)
{
	if (! pResult) return AAFRESULT_NULL_PARAM;
	*pResult = pvtIsConcrete ();
	return AAFRESULT_SUCCESS;
}


// Check that this class or any parent class is already uniquely
// identified.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::IsUniquelyIdentified (
      aafBoolean_t * isUniquelyIdentified)
{
  AAFRESULT hr = AAFRESULT_SUCCESS;
	if (! isUniquelyIdentified)
    return AAFRESULT_NULL_PARAM;
	
  if (NULL != pvtGetUniqueIdentifier())
    *isUniquelyIdentified = kAAFTrue;
  else
    *isUniquelyIdentified = kAAFFalse;


  return AAFRESULT_SUCCESS;
}


// Check that this class or any parent class is already uniquely
// identified.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetUniqueIdentifier (
      ImplAAFPropertyDef ** ppUniqueIdentifier)
{
  AAFRESULT hr = AAFRESULT_SUCCESS;
	if (! ppUniqueIdentifier)
    return AAFRESULT_NULL_PARAM;

  *ppUniqueIdentifier = pvtGetUniqueIdentifier();
  if (*ppUniqueIdentifier)
  {
    (*ppUniqueIdentifier)->AcquireReference();
    return AAFRESULT_SUCCESS;
  }
  else
  {
    // The unique identifier property in property definition
    // is optional. So if our low-level search did not find
    // a property definition with this property we should 
    // return corresponding result code.
    return AAFRESULT_PROP_NOT_PRESENT;
  }
}


// Find the unique identifier property defintion for this class or any parent class
// (RECURSIVE)
ImplAAFPropertyDef * ImplAAFClassDef::pvtGetUniqueIdentifier(void) // result is NOT reference counted.
{
  ImplAAFPropertyDef *result = NULL;

  aafBoolean_t isRoot, isUniquelyIdentified;
  AAFRESULT hr = IsRoot(&isRoot);
  if (AAFRESULT_FAILED(hr))
    return NULL;

  // Look for the unique identifier in the parent class(es) before looking in the
  // current class definition.
  if (!isRoot)
  {
    ImplAAFClassDefSP pParent;
    hr = GetParent(&pParent);
    if (AAFRESULT_FAILED(hr))
      return NULL;

    result = pParent->pvtGetUniqueIdentifier();
  }

  // If we have not already found the unique identifier in a parent
  // class then continue to look in this class' propert definitions.
  if (!result)
  {
	  OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPropertyDef>propertyDefinitions(_Properties);
	  while(++propertyDefinitions)
	  {
		  ImplAAFPropertyDef *pProperty = propertyDefinitions.value();
		  if (pProperty)
		  {
		    hr = pProperty->GetIsUniqueIdentifier(&isUniquelyIdentified);
        if (AAFRESULT_FAILED(hr))
          return NULL;
        if (isUniquelyIdentified)
        {
          result = pProperty;
          break;
        }
		  }
	  }
  }

  return result;
}


// SetParent is SDK INTERNAL
AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::SetParent (
      ImplAAFClassDef *pClassDef)
{
	if (! pClassDef)
		return AAFRESULT_NULL_PARAM;

//	assert(_ParentClass.isVoid());
	_ParentClass = pClassDef;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::SetBootstrapParent (
	ImplAAFClassDef *pClassDef)
{
	if (! pClassDef)
		return AAFRESULT_NULL_PARAM;
	
	_BootstrapParent = pClassDef;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::CreateInstance (
      ImplAAFObject ** ppObject)
{
  if (! ppObject)
	return AAFRESULT_NULL_PARAM;

  if (! pvtIsConcrete ())
	return AAFRESULT_ABSTRACT_CLASS;

  AAFRESULT hr;
  ImplAAFDictionarySP pDict;
  hr = GetDictionary (&pDict);
  if (AAFRESULT_FAILED (hr))
	return hr;
  assert (pDict);
  ImplAAFObject * pObj =
	pDict->CreateAndInit (this);
  assert (pObj);
  *ppObject = pObj;
  // don't bother with *ppObject->addref() and pObj->ReleaseRef();
  // consider the ref count to be transferred from pObj to *ppObject.
  pObj = 0;  // to keep boundschecker happy
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::pvtRegisterPropertyDef (
      const aafUID_t &      id,
      const aafCharacter *  pName,
      const aafUID_t &      typeId,
      aafBool               isOptional,
      aafBool               isUniqueIdentifier,
      ImplAAFPropertyDef ** ppPropDef)
{
  if (! pName) return AAFRESULT_NULL_PARAM;

  ImplAAFDictionarySP pDict;
  ImplAAFPropertyDefSP pd;

  check_result (GetDictionary (&pDict));
  assert (pDict);
  OMPropertyId omPid;
  check_result (pDict->GenerateOmPid (id, omPid));

  AAFRESULT hr;
  hr = pDict->CreateMetaInstance(AUID_AAFPropertyDef, (ImplAAFMetaDefinition **)&pd);
  if (AAFRESULT_FAILED (hr))
	return hr;

  check_result (pd->pvtInitialize (id,
								omPid,
								pName,
								typeId,
								isOptional,
                isUniqueIdentifier));

//  ImplAAFPropertyDef * pdTemp = pd;
//  _Properties.appendValue(pdTemp);
//  pdTemp->AcquireReference ();
  pvtRegisterExistingPropertyDef(pd);

  if (ppPropDef)
  {
	*ppPropDef = pd;
	(*ppPropDef)->AcquireReference ();
  }

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFClassDef::pvtRegisterExistingPropertyDef
(ImplAAFPropertyDef * pPropDef)
{
  if (!pPropDef)
	return AAFRESULT_NULL_PARAM;

  _Properties.appendValue(pPropDef);
  pPropDef->AcquireReference ();

  return AAFRESULT_SUCCESS;
}


//
// Implementations of private class
//
aafBool ImplAAFClassDef::pvtPropertyIdentifierOMPid:: DoesMatch
  (const ImplAAFPropertyDef * pTestPropDef) const
{
  OMPropertyId testPid;
  assert (pTestPropDef);
  testPid = pTestPropDef->OmPid ();
  return (_id == testPid) ? kAAFTrue : kAAFFalse;
}

aafBool ImplAAFClassDef::pvtPropertyIdentifierAUID::DoesMatch
  (const ImplAAFPropertyDef * pTestPropDef) const
{
  aafUID_t testUID;
  assert (pTestPropDef);
  AAFRESULT hr = pTestPropDef->GetAUID (&testUID);
  assert (AAFRESULT_SUCCEEDED (hr));
  return (EqualAUID (&_id, &testUID) ? kAAFTrue : kAAFFalse);
}


void ImplAAFClassDef::AssurePropertyTypesLoaded ()
{
  if (! _propTypesLoaded)
	{
	  _propTypesLoaded = true;

	  AAFRESULT hr;
	  ImplAAFClassDefSP spDef;
	  spDef	= this;
	  assert (spDef);
	  while (1)
		{
		  // Loop through this class and all its parents
		  ImplEnumAAFPropertyDefsSP pdEnumSP;
		  hr = spDef->GetPropertyDefs (&pdEnumSP);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  ImplAAFPropertyDefSP propDefSP;

		  // Loop through all properties of this class
		  while (AAFRESULT_SUCCEEDED (pdEnumSP->NextOne (&propDefSP)))
			{
			  // return value unused; just force the lookup.
			  propDefSP->type ();
			}

		  // Look at the parent of this class
		  ImplAAFClassDefSP parentSP;
		  aafBool	isRoot;

		  hr = spDef->IsRoot(&isRoot);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  if (isRoot)
			break;
		  hr = spDef->GetParent (&parentSP);
		  assert (AAFRESULT_SUCCEEDED (hr));
//!!!		  if (parentSP == NULL)
//		  {
//			 ImplAAFDictionary *pDict;
//			 ImplAAFClassDef	*curClass;
//			aafUID_t	uid;
//
//			hr = spDef->GetAUID (&uid);
//		    assert (AAFRESULT_SUCCEEDED (hr));
//			hr = spDef->GetDictionary(&pDict);
//		    assert (AAFRESULT_SUCCEEDED (hr));
//			hr = pDict->LookupClassDef (uid, &curClass);
//		    assert (AAFRESULT_SUCCEEDED (hr));
//			hr = curClass->GetParent(&parentSP);
//		    assert (AAFRESULT_SUCCEEDED (hr));
//		  }

		  assert (parentSP != NULL);
		  spDef = parentSP;
		}
	}
}

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

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
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
#include "AAFUtils.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFPropertyDefs;

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
  : _ParentClass  ( PID_ClassDefinition_ParentClass,  "ParentClass"),
	_Properties   ( PID_ClassDefinition_Properties,   "Properties"),
	_propTypesLoaded (false)
{
  _persistentProperties.put(_ParentClass.address());
  _persistentProperties.put(_Properties.address());
}


ImplAAFClassDef::~ImplAAFClassDef ()
{
  aafUInt32 numProps;
  AAFRESULT hr = CountPropertyDefs (&numProps);
  if (AAFRESULT_FAILED (hr))
	throw hr;

	OMStrongReferenceSetIterator<ImplAAFPropertyDef>propertyDefinitions(_Properties);
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
      const aafCharacter * pClassName)
{
  if (!pClassName) return AAFRESULT_NULL_PARAM;

  HRESULT hr;
  aafUID_t parentId;
  if (pParentClass)
	{
	  hr = pParentClass->GetAUID(&parentId);
	  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
	}
  else
	{
	  aafUID_t NULL_UID = { 0 };
	  parentId = NULL_UID;
	}

  return pvtInitialize (classID, &parentId, pClassName);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::pvtInitialize (
      const aafUID_t & classID,
      const aafUID_t * pParentClassId,
      const aafCharacter * pClassName)
{
  if (!pClassName) return AAFRESULT_NULL_PARAM;
  if (! pParentClassId) return AAFRESULT_NULL_PARAM;


  HRESULT hr;
  hr = SetName (pClassName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetAUID (classID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  _ParentClass = *pParentClassId;

  return AAFRESULT_SUCCESS;
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
  OMStrongReferenceSetIterator<ImplAAFPropertyDef>* iter = 
	new OMStrongReferenceSetIterator<ImplAAFPropertyDef>(_Properties);
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
	  ImplAAFClassDefSP pClassDef;
	  aafUID_t thisClassID;

	  // get our class ID
	  hr = GetAUID (&thisClassID);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = pDict->dictLookupClassDef (thisClassID, &pClassDef);
	  if (AAFRESULT_SUCCEEDED (hr))
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

  return pvtRegisterPropertyDef (id,
								 pName,
								 typeId,
								 isOptional,
								 ppPropDef);
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

  return pvtRegisterPropertyDef (id,
								 pName,
								 typeId,
								 kAAFTrue,
								 ppPropDef);
}


AAFRESULT STDMETHODCALLTYPE
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
  AAFRESULT hr = GetParent (&parentSP);
  if (AAFRESULT_FAILED (hr))
	return hr;
  if (parentSP)
    return parentSP->generalLookupPropertyDef (propId, ppPropDef);
  else
	return AAFRESULT_NO_MORE_OBJECTS;
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



void ImplAAFClassDef::pvtGetParentAUID (aafUID_t & result)
{
  result = _ParentClass;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetParent (
      ImplAAFClassDef ** ppClassDef)
{
  if (! ppClassDef) return AAFRESULT_NULL_PARAM;

  if (! _cachedParentClass)
	{
	  ImplAAFDictionarySP pDict;
	  AAFRESULT hr;

	  // If no parent, return NULL (and success)
	  aafUID_t parentClass;
	  pvtGetParentAUID (parentClass);
	  const aafUID_t null_UID = { 0 };
	  if (EqualAUID(&parentClass, &null_UID))
		{
		  assert (ppClassDef);
		  *ppClassDef = 0;
		  return AAFRESULT_SUCCESS;
		}

	  hr = GetDictionary(&pDict);
	  if (AAFRESULT_FAILED (hr)) return hr;
	  assert (pDict);

	  ImplAAFClassDefSP tmp;
	  assert (! _cachedParentClass);
	  hr = pDict->LookupClassDef (parentClass, &tmp);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	  // If _cachedParentClass was set during process of looking this
	  // up, then simply throw away the most recently looked-up one
	  // (in tmp).
	  if (! _cachedParentClass)
		_cachedParentClass = tmp;
	  assert (_cachedParentClass);
	}
  assert (ppClassDef);
  *ppClassDef = _cachedParentClass;
  assert (*ppClassDef);
  (*ppClassDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::CreateInstance (
      ImplAAFObject ** ppObject)
{
  if (! ppObject)
	return AAFRESULT_NULL_PARAM;

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
  hr = pDict->GetBuiltinDefs()->cdPropertyDef()->
	CreateInstance((ImplAAFObject**)&pd);
  if (AAFRESULT_FAILED (hr))
	return hr;

  check_result (pd->pvtInitialize (id,
								omPid,
								pName,
								typeId,
								isOptional));

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
		  hr = spDef->GetParent (&parentSP);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  if (! parentSP)
			break;
		  spDef = parentSP;
		}
	}
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
// AAFTypeDefStream			<not yet supported>
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


void ImplAAFClassDef::InitOMProperties (ImplAAFObject * pObj)
{
  assert (pObj);
  AAFRESULT hr;

  //
  // Init base class properties first
  //
  ImplAAFClassDefSP parentSP;
  hr = GetParent (&parentSP);
  assert (AAFRESULT_SUCCEEDED (hr));
  // parentSP will be NULL if this class has no parent.
  if (parentSP)
	parentSP->InitOMProperties (pObj);

  // See if currently existing OM properties are defined in the class
  // def.
  //
  OMPropertySet * ps = pObj->propertySet();
  assert (ps);
  const size_t propCount = ps->count();

  // Loop through properties of this class
  ImplEnumAAFPropertyDefsSP pdEnumSP;
  hr = GetPropertyDefs (&pdEnumSP);
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
	  else
		{
		  // Defined property wasn't found in OM property set.
		  // We'll have to install one.
		  pProp = propDefSP->CreateOMProperty ();
		  assert (pProp);

		  // Remember this property so we can delete it later.
		  pObj->RememberAddedProp (pProp);

		  // Add the property to the property set.
		  ps->put (pProp);
		}

	  ImplAAFPropertyDef * pPropDef =
		(ImplAAFPropertyDef*) propDefSP;
	  OMPropertyDefinition * pOMPropDef =
		dynamic_cast<OMPropertyDefinition*>(pPropDef);
	  assert (pOMPropDef);

	  assert (pProp);
	  pProp->initialize (pOMPropDef);

	  propDefSP = 0;
	  pProp = 0;
	  pOMPropDef = 0;
	  pPropDef = 0;
	}
}

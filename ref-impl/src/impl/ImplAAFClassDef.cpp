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
 *  prior written permission of Avid Technology, Inc.
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
	_Properties   ( PID_ClassDefinition_Properties,   "Properties")
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

  aafUInt32 i;
  for (i = 0; i < numProps; i++)
	{
	  ImplAAFPropertyDef *pd = _Properties.setValueAt(0, i);
	  if (pd)
		{
		  pd->ReleaseReference();
		  pd = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::Initialize (
      const aafUID_t * pID,
      ImplAAFClassDef * pParentClass,
      const wchar_t *  pClassName)
{
  if (!pID) return AAFRESULT_NULL_PARAM;
  if (!pClassName) return AAFRESULT_NULL_PARAM;

  HRESULT hr;
  hr = SetName (pClassName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  hr = SetAUID (pID);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  aafUID_t id;
  if (pParentClass)
	{
	  hr = pParentClass->GetAUID(&id);
	  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
	}
  else
	{
	  aafUID_t NULL_UID = { 0 };
	  id = NULL_UID;
	}
  _ParentClass = id;

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
  hr = theEnum->SetEnumStrongProperty(this, &_Properties);
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
    ImplAAFClassDef::AppendNewPropertyDef (
      aafUID_t *            pID,
      wchar_t *             pName,
      ImplAAFTypeDef *      pTypeDef,
      aafBool               isOptional,
      ImplAAFPropertyDef ** ppPropDef)
{
  // This will only work if this class has not yet been registered.

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

	  hr = pDict->dictLookupClass (&thisClassID, &pClassDef);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  // pClassDef is unused; we only want to know the result of
		  // the lookup.
		  return AAFRESULT_OBJECT_ALREADY_ATTACHED;
		}
	}

  // If we're here, we're not already registered.  OK to continue.
  return pvtAppendPropertyDef (pID,
							   pName,
							   pTypeDef,
							   isOptional,
							   ppPropDef);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::AppendOptionalPropertyDef (
      aafUID_t *            pID,
      wchar_t *             pName,
      ImplAAFTypeDef *      pTypeDef,
      ImplAAFPropertyDef ** ppPropDef)
{
  return pvtAppendPropertyDef (pID,
							   pName,
							   pTypeDef,
							   AAFTrue,
							   ppPropDef);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::pvtAppendNewPropertyDef (
      aafUID_t *  pID,
	  wchar_t *  pName,
	  ImplAAFTypeDef * pTypeDef,
	  aafBool  isOptional,
	  ImplAAFPropertyDef ** ppPropDef)
{
  return pvtAppendPropertyDef (pID,
							   pName,
							   pTypeDef,
							   isOptional,
							   ppPropDef);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::generalLookupPropertyDef (
      const pvtPropertyIdentifier & propId,
      ImplAAFPropertyDef ** ppPropDef)
{
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
      aafUID_t * pPropID,
      ImplAAFPropertyDef ** ppPropDef) const
{
  if (! pPropID)
	return AAFRESULT_NULL_PARAM;

  const pvtPropertyIdentifierAUID generalPropId = *pPropID;

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

	  hr = pDict->LookupClass (&parentClass, &_cachedParentClass);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	  assert (_cachedParentClass);
	}
  assert (ppClassDef);
  *ppClassDef = _cachedParentClass;
  assert (*ppClassDef);
  (*ppClassDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::pvtAppendPropertyDef (
      aafUID_t *            pID,
      wchar_t *             pName,
      ImplAAFTypeDef *      pTypeDef,
      aafBool               isOptional,
      ImplAAFPropertyDef ** ppPropDef)
{
  if (! pID) return AAFRESULT_NULL_PARAM;
  if (! pName) return AAFRESULT_NULL_PARAM;
  if (! pTypeDef) return AAFRESULT_NULL_PARAM;

  ImplAAFDictionarySP pDict;
  ImplAAFPropertyDefSP pd;

  check_result (GetDictionary (&pDict));
  assert (pDict);
  assert (pID);
  OMPropertyId omPid;
  check_result (pDict->GenerateOmPid (*pID, omPid));

  ImplAAFPropertyDef * tmp =
	(ImplAAFPropertyDef *)pDict->CreateImplObject (AUID_AAFPropertyDef);
  if (!tmp) return AAFRESULT_NOMEMORY;
  pd = tmp;
  // Bobt: Hack bugfix! SmartPointer operator= will automatically
  // AddRef; CreateImpl *also* will addref, so we've got one too
  // many.  Put us back to normal.
  tmp->ReleaseReference ();
  tmp = 0;

  check_result (pd->Initialize (pID,
								omPid,
								pName,
								pTypeDef,
								isOptional));

  ImplAAFPropertyDef * pdTemp = pd;
  _Properties.appendValue(pdTemp);
  pdTemp->AcquireReference ();

  if (ppPropDef)
  {
	*ppPropDef = pd;
	(*ppPropDef)->AcquireReference ();
  }

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
  return (_id == testPid) ? AAFTrue : AAFFalse;
}

aafBool ImplAAFClassDef::pvtPropertyIdentifierAUID::DoesMatch
  (const ImplAAFPropertyDef * pTestPropDef) const
{
  aafUID_t testUID;
  assert (pTestPropDef);
  AAFRESULT hr = pTestPropDef->GetAUID (&testUID);
  assert (AAFRESULT_SUCCEEDED (hr));
  return (EqualAUID (&_id, &testUID) ? AAFTrue : AAFFalse);
}


/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/


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
inline void check_result (AAFRESULT r)
{
  if (AAFRESULT_FAILED (r))
	throw r;
}

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
  check_result (CountPropertyDefs (&numProps));

  aafUInt32 i;
  for (i = 0; i < numProps; i++)
	{
		ImplAAFPropertyDef *pd = _Properties.setValueAt(0, i);
		if (pd)
		{
		  pd->ReleaseReference();
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
  ImplEnumAAFPropertyDefs *theEnum = 0;

  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;

 try
	{
	  theEnum = (ImplEnumAAFPropertyDefs *)CreateImpl (CLSID_EnumAAFPropertyDefs);
	
 	  AAFRESULT hr;
	  hr = theEnum->SetEnumStrongProperty(this, &_Properties);
	  if (AAFRESULT_FAILED (hr)) throw hr;
	  *ppEnum = theEnum;
	}
  catch (HRESULT & rCaught) 
	{
	  if (theEnum)
		theEnum->ReleaseReference();
	  return rCaught;
	}
	
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::CountPropertyDefs (
      aafUInt32 *  pCount)
{
  if (! pCount) return AAFRESULT_NULL_PARAM;

  assert (pCount);
  *pCount = _Properties.getSize ();
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
  ImplAAFDictionary * pDict = 0;
  hr = GetDictionary (&pDict);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  // A dictionary has been identified; see if we're registered in
	  // it.
	  ImplAAFClassDef * pClassDef = 0;
	  aafUID_t thisClassID;

	  // get our class ID
	  hr = GetAUID (&thisClassID);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  hr = pDict->dictLookupClass (&thisClassID, &pClassDef);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  // pClassDef is unused; we only want to know what was the
		  // result of the lookup.
		  assert (pClassDef);
		  pClassDef->ReleaseReference ();

		  // We're already in the dictionary.  Make it an error.
		  assert (pDict);
		  pDict->ReleaseReference ();
		  return AAFRESULT_OBJECT_ALREADY_ATTACHED;
		}

	  // Done with dict.
	  assert (pDict);
	  pDict->ReleaseReference ();
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
    ImplAAFClassDef::LookupPropertyDef (
      aafUID_t * pPropID,
      ImplAAFPropertyDef ** ppPropDef)
{
  AAFRESULT rReturned = AAFRESULT_SUCCESS;

  ImplEnumAAFPropertyDefs * pPropEnum = 0;
  ImplAAFPropertyDef * pPropDef = 0;

  try
	{
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
		  aafUID_t testId;
		  check_result (pPropDef->GetAUID (&testId));
		  if (EqualAUID (pPropID, &testId))
			{
			  // Yup, this is the one.
			  assert (ppPropDef);
			  *ppPropDef = pPropDef;

			  // AddRef() on *ppPropDef offsets the ReleaseRef on
			  // pPropDef which will happen at function exit; to keep
			  // from releasing at exit, zero out the pPropDef pointer
			  // and it won't be necessary to addref *ppPropDef.
			  // (*ppPropDef)->ReleaseReference ();
			  pPropDef = 0;

			  // Get us outta the loop
			  // throw AAFRESULT_SUCCESS;
			  release_if_set (pPropDef);
			  release_if_set (pPropEnum);
			  return AAFRESULT_SUCCESS;
			}
		  else
			{
			  // nope, this prop doesn't match.  Try the next one.
			  release_and_zero (pPropDef);
			}
		}
	  // If we're here, there aren't any more props so that prop
	  // wasn't found.
	  // throw AAFRESULT_NO_MORE_OBJECTS;
	  release_if_set (pPropDef);
	  release_if_set (pPropEnum);
	  return AAFRESULT_NO_MORE_OBJECTS;
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	}

  release_if_set (pPropDef);
  release_if_set (pPropEnum);

  return rReturned;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetParent (
      ImplAAFClassDef ** ppClassDef)
{
  if (! ppClassDef) return AAFRESULT_NULL_PARAM;

  ImplAAFDictionary * pDict = 0;
  ImplAAFClassDef * pcd = 0;
  AAFRESULT hr;

  hr = GetDictionary(&pDict);
  if (AAFRESULT_FAILED (hr)) return hr;
  assert (pDict);

  aafUID_t parentClass = _ParentClass;
  hr = pDict->LookupClass (&parentClass, &pcd);
  if (AAFRESULT_FAILED (hr))
    {
      assert (pDict);
      pDict->ReleaseReference ();
      pDict = 0;
      return hr;
    }
  assert (pcd);
  assert (ppClassDef);
  *ppClassDef = pcd;

  // release/acquire cancel each other
  // pcd->ReleaseReference ();
  // (*ppClassDef)->AcquireReference ();

  assert (pDict);
  pDict->ReleaseReference ();
  pDict = 0;

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
  if (! ppPropDef) return AAFRESULT_NULL_PARAM;

  ImplAAFDictionary * pDict = 0;
  ImplAAFPropertyDef *pd = 0;

  AAFRESULT rReturned = AAFRESULT_SUCCESS;

  try
	{
	  check_result (GetDictionary (&pDict));
	  assert (pDict);
	  assert (pID);
	  OMPropertyId omPid;
	  check_result (pDict->GenerateOmPid (*pID, omPid));

	  pd = (ImplAAFPropertyDef *)pDict->CreateImplObject (AUID_AAFPropertyDef);
	  if (!pd) throw AAFRESULT_NOMEMORY;

	  assert (pd);
	  check_result (pd->Initialize (pID,
									omPid,
									pName,
									pTypeDef,
									isOptional));

	  _Properties.appendValue(pd);
	  pd->AcquireReference ();

	  assert (ppPropDef);
	  *ppPropDef = pd;

	  // AddRef() on *ppPropDef offsets the ReleaseRef on pd which
	  // will happen at function exit; to keep from releasing at exit,
	  // zero out the pd pointer and it won't be necessary to addref
	  // *ppPropDef.
	  // (*ppPropDef)->ReleaseReference ();
	  pd = 0;
	}
  catch (HRESULT &rCaught)
	{
	  rReturned = rCaught;
	}

  release_if_set (pDict);
  release_if_set (pd);

  return rReturned;
}


OMDEFINE_STORABLE(ImplAAFClassDef, AUID_AAFClassDef);

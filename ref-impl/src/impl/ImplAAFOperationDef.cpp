//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#ifndef __ImplEnumAAFLocators_h__
#include "ImplEnumAAFLocators.h"
#endif

#ifndef __ImplEnumAAFParameterDefs_h__
#include "ImplEnumAAFParameterDefs.h"
#endif


#ifndef __ImplAAFOperationDef_h__
#include "ImplAAFOperationDef.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFDataDef.h"

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "aafErr.h"

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDictionary> ImplAAFDictionarySP;

extern "C" const aafClassID_t CLSID_EnumAAFOperationDefs;
extern "C" const aafClassID_t CLSID_EnumAAFParameterDefs;
 
ImplAAFOperationDef::ImplAAFOperationDef ()
: _dataDef(	PID_OperationDefinition_DataDefinition,	L"DataDefinition", L"/Header/Dictionary/DataDefinitions", PID_DefinitionObject_Identification),
  _isTimeWarp(		PID_OperationDefinition_IsTimeWarp,			L"IsTimeWarp"),
  _degradeTo(		PID_OperationDefinition_DegradeTo,			L"DegradeTo", L"/Header/Dictionary/OperationDefinitions", PID_DefinitionObject_Identification),
  _category(		PID_OperationDefinition_Category,			L"Category"),
  _numInputs(		PID_OperationDefinition_NumberInputs,		L"NumberInputs"),
  _bypass(			PID_OperationDefinition_Bypass,				L"Bypass"),
  _paramDefined(	PID_OperationDefinition_ParametersDefined,	L"ParametersDefined", L"/Header/Dictionary/ParameterDefinitions", PID_DefinitionObject_Identification)
{
	_persistentProperties.put(_dataDef.address());
	_persistentProperties.put(_isTimeWarp.address());
	_persistentProperties.put(_degradeTo.address());
	_persistentProperties.put(_category.address());
	_persistentProperties.put(_numInputs.address());
	_persistentProperties.put(_bypass.address());
	_persistentProperties.put(_paramDefined.address());
}


ImplAAFOperationDef::~ImplAAFOperationDef ()
{
}

  
AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::Initialize (
      const aafUID_t & id,
	  const aafWChar * pName,
	  const aafWChar * pDesc)
{
	if (pName == NULL || pDesc == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}

	return pvtInitialize(id, pName, pDesc);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetDataDef(
      ImplAAFDataDef ** ppDataDef)
{
  if(! ppDataDef)
	return AAFRESULT_NULL_PARAM;

   if(_dataDef.isVoid())
		return AAFRESULT_OBJECT_NOT_FOUND;
  ImplAAFDataDef *pDataDef = _dataDef;

  *ppDataDef = pDataDef;
  assert (*ppDataDef);
  (*ppDataDef)->AcquireReference ();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::SetDataDef (
      ImplAAFDataDef * pDataDef)
{
  if (! pDataDef)
	return AAFRESULT_NULL_PARAM;

  // Check if given data definition is in the dict.
  if( !aafLookupDataDef( this, pDataDef ) )
    return AAFRESULT_INVALID_OBJ;

  _dataDef = pDataDef;
	
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::IsTimeWarp (
      aafBool *bIsTimeWarp)
{
	if(bIsTimeWarp == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_isTimeWarp.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*bIsTimeWarp = _isTimeWarp;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::SetIsTimeWarp (
      aafBool  bIsTimeWarp)
{
	_isTimeWarp = bIsTimeWarp;
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::PrependDegradeToOperation (
      ImplAAFOperationDef  *pOperationDef)
{
	if (NULL == pOperationDef)
		return AAFRESULT_NULL_PARAM;

	// Check if given definition is in the dict.
	if( !aafLookupOperationDef( this, pOperationDef ) )
		return AAFRESULT_INVALID_OBJ;

	_degradeTo.prependValue(pOperationDef);
	// 2000-OCT-23 transdel : this is a weak reference. It is already owned by another
	// strong reference set so we do not need to reference count it!
	//	pOperationDef->AcquireReference();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::AppendDegradeToOperation (
      ImplAAFOperationDef  *pOperationDef)
{
	if (NULL == pOperationDef)
		return AAFRESULT_NULL_PARAM;

	// Check if given definition is in the dict.
	if( !aafLookupOperationDef( this, pOperationDef ) )
		return AAFRESULT_INVALID_OBJ;

	_degradeTo.appendValue(pOperationDef);
	// 2000-OCT-23 transdel : this is a weak reference. It is already owned by another
	// strong reference set so we do not need to reference count it!
	//	pOperationDef->AcquireReference();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::InsertDegradeToOperationAt (
	  aafUInt32 index,
      ImplAAFOperationDef  *pOperationDef)
{
	if (! pOperationDef) return AAFRESULT_NULL_PARAM;

	if (index > _degradeTo.count()) // we can "insert" one after the end
	  return AAFRESULT_BADINDEX;

	// Check if given definition is in the dict.
	if( !aafLookupOperationDef( this, pOperationDef ) )
		return AAFRESULT_INVALID_OBJ;

	_degradeTo.insertAt(pOperationDef, index);
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::RemoveDegradeToOperationAt (
	  aafUInt32 index)
{
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountDegradeToOperations (&count);
	if (AAFRESULT_FAILED (hr)) return hr;
	if (index >= count)
		return AAFRESULT_BADINDEX;
	
	_degradeTo.removeAt(index);
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetDegradeToOperations (
      ImplEnumAAFOperationDefs  **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFOperationDefs *theEnum = (ImplEnumAAFOperationDefs *)CreateImpl (CLSID_EnumAAFOperationDefs);
	
	XPROTECT()
	{
		OMWeakReferenceVectorIterator</*OMUniqueObjectIdentification,*/ ImplAAFOperationDef>* iter = 
			new OMWeakReferenceVectorIterator</*OMUniqueObjectIdentification,*/ ImplAAFOperationDef>(_degradeTo);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFOperationDefs, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::CountDegradeToOperations (
	  aafUInt32 * pResult)
{
  if (! pResult) return AAFRESULT_NULL_PARAM;

  *pResult = _degradeTo.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetCategory (
       aafUID_t		*pCategory)
{
	if(pCategory == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pCategory = _category;
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::SetCategory (
      const aafUID_t pCategory)
{
	_category = pCategory;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetNumberInputs (
      aafInt32 *  pNumberInputs)
{
	if(pNumberInputs == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pNumberInputs = _numInputs;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::SetNumberInputs (
      aafInt32  NumberInputs)
{
	_numInputs = NumberInputs;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetBypass (
      aafUInt32 *pBypass)
{
	if(pBypass == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_bypass.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBypass = _bypass;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::SetBypass (
      aafUInt32  bypass)
{
	_bypass = bypass;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::AddParameterDef (
      ImplAAFParameterDef *pAAFParameterDef)
{
	if (NULL == pAAFParameterDef)
		return AAFRESULT_NULL_PARAM;

	// Check if given definition is in the dict.
	if( !aafLookupParameterDef( this, pAAFParameterDef ) )
		return AAFRESULT_INVALID_OBJ;

	_paramDefined.appendValue(pAAFParameterDef);
	// 2000-OCT-23 transdel : this is a weak reference. It is already owned by another
	// strong reference set so we do not need to reference count it!
	//	pAAFParameterDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetParameterDefs (
      ImplEnumAAFParameterDefs **ppEnum)
{
	ImplEnumAAFParameterDefs	*theEnum;
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = NULL;
	theEnum = (ImplEnumAAFParameterDefs *)CreateImpl(CLSID_EnumAAFParameterDefs);
	if(theEnum == NULL)
		return(AAFRESULT_NOMEMORY);
	XPROTECT()
	{
		OMWeakReferenceSetIterator</*OMUniqueObjectIdentification,*/ ImplAAFParameterDef>* iter = 
			new OMWeakReferenceSetIterator</*OMUniqueObjectIdentification,*/ ImplAAFParameterDef>(_paramDefined);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFParameterDefs, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::CountParameterDefs (
      aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _paramDefined.count();
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::LookupParameterDef (
	  const aafUID_t & parameterDefId,
	  ImplAAFParameterDef ** ppParameterDef)
{
  if (!ppParameterDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_paramDefined.find((*reinterpret_cast<const OMObjectIdentification *>(&parameterDefId)),
                             *ppParameterDef))
	{
		assert(NULL != *ppParameterDef);
		(*ppParameterDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}

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


#include "AAFStoredObjectIDs.h"
#ifndef __AAFStoredObjectIDs_h__
#include "AAFPropertyIDs.h"
#endif
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
	aafUID_t	nilUID = { 0 };

//	ImplAAFEssenceDescriptor *dataDef = _dataDef.setValue(nilUID);
//	ImplAAFEssenceDescriptor *degradeTo = _degradeTo.setValue(0,0);
//	if (dataDef)
//	{
//		dataDef->ReleaseReference();
//	}
//	if (degradeTo)
//	{
//		degradeTo->ReleaseReference();
//	}
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
	else
	{
	  return pvtInitialize(id, pName, pDesc);
	}
	return AAFRESULT_SUCCESS;
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

	_degradeTo.prependValue(pOperationDef);
	pOperationDef->AcquireReference();
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::AppendDegradeToOperation (
      ImplAAFOperationDef  *pOperationDef)
{
	if (NULL == pOperationDef)
		return AAFRESULT_NULL_PARAM;

	_degradeTo.appendValue(pOperationDef);
	pOperationDef->AcquireReference();
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
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
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

	_paramDefined.appendValue(pAAFParameterDef);
	// trr - We are saving a copy of pointer in _paramDefined
	// so we need to bump its reference count.
	pAAFParameterDef->AcquireReference();
	
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
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
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

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
: _dataDef(			PID_OperationDefinition_DataDefinition,		"DataDefinition"),
  _isTimeWarp(		PID_OperationDefinition_IsTimeWarp,			"IsTimeWarp"),
  _degradeTo(		PID_OperationDefinition_DegradeTo,			"DegradeTo"),
  _category(		PID_OperationDefinition_Category,			"Category"),
  _numInputs(		PID_OperationDefinition_NumberInputs,		"NumberInputs"),
  _bypass(			PID_OperationDefinition_Bypass,				"Bypass"),
  _paramDefined(	PID_OperationDefinition_ParametersDefined,	"ParametersDefined")
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
    ImplAAFOperationDef::GetDataDef(
      ImplAAFDataDef ** ppDataDef)
{
  if(! ppDataDef)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;
  ImplAAFDictionarySP pDict;
  hr = GetDictionary (&pDict);
  if (AAFRESULT_FAILED (hr)) return hr;
  return pDict->LookupDataDef (_dataDef, ppDataDef);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::SetDataDef (
      ImplAAFDataDef * pDataDef)
{
  if (! pDataDef)
	return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;
  aafUID_t uid;
  hr = pDataDef->GetAUID(&uid);
  if (AAFRESULT_FAILED (hr))
	return hr;

  _dataDef = uid;
	
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
	aafUID_t	*tmp = NULL, newUID;
	aafInt32	oldCount;
	aafInt32	newCount;

	if(pOperationDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldCount = _degradeTo.count();
		newCount = oldCount + 1;
		CHECK(pOperationDef->GetAUID(&newUID));
		tmp = new aafUID_t[newCount];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldCount != 0)
			_degradeTo.copyToBuffer(&tmp[1], oldCount * sizeof(aafUID_t));
		tmp[0] = newUID;
		_degradeTo.setValue(tmp, newCount * sizeof(aafUID_t));
		delete [] tmp;
	}
	XEXCEPT
	{
		if(tmp != NULL)
			delete [] tmp;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::AppendDegradeToOperation (
      ImplAAFOperationDef  *pOperationDef)
{
	aafUID_t	*tmp, newUID;
	aafInt32	oldCount;
	aafInt32	newCount;

	if(pOperationDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldCount = _degradeTo.count();
		newCount = oldCount + 1;
		CHECK(pOperationDef->GetAUID(&newUID));
		tmp = new aafUID_t[newCount];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldCount != 0)
			_degradeTo.copyToBuffer(tmp, oldCount * sizeof(aafUID_t));
		tmp[newCount - 1] = newUID;
		_degradeTo.setValue(tmp, newCount * sizeof(aafUID_t));
		delete [] tmp;
	}
	XEXCEPT
	{
		if(tmp != NULL)
			delete [] tmp;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::InsertDegradeToOperationAt (
	  aafUInt32 index,
      ImplAAFOperationDef  *pOperationDef)
{
  if (! pOperationDef) return AAFRESULT_NULL_PARAM;

  aafUInt32 count;
  AAFRESULT hr;
  hr = CountDegradeToOperations (&count);
  if (AAFRESULT_FAILED (hr)) return hr;
  if (index > count)
	return AAFRESULT_BADINDEX;

  return AAFRESULT_NOT_IMPLEMENTED;
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

  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetDegradeToOperations (
      ImplEnumAAFOperationDefs  **ppEnum)
{
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFOperationDefs *)CreateImpl(CLSID_EnumAAFOperationDefs);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);
	(*ppEnum)->SetEnumProperty(this, &_degradeTo);

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::CountDegradeToOperations (
	  aafUInt32 * pResult)
{
  if (! pResult) return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetCategory (
       aafCharacter		*pCategory,
		aafUInt32	bufSize)
{
	bool stat;

	if(pCategory == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_category.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	stat = _category.copyToBuffer(pCategory, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}
 
  //****************
  // GetCategoryBufLen()
  //
AAFRESULT STDMETHODCALLTYPE
	ImplAAFOperationDef::GetCategoryBufLen (
			aafUInt32 *		pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_category.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pLen = _category.size();
	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::SetCategory (
      const aafCharacter *pCategory)
{
	if(pCategory == NULL)
		return(AAFRESULT_NULL_PARAM);

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
	aafUID_t					*tmp, newUID, oldUID;
	aafInt32					oldCount;
	aafInt32					newCount;
	ImplAAFParameterDef*		pParmDef = NULL;
	ImplEnumAAFParameterDefs*	pEnum = NULL;
	aafBool						parmDefFound = AAFFalse;

	tmp = NULL;

	if(pAAFParameterDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		CHECK(pAAFParameterDef->GetAUID(&newUID));
		CHECK(GetParameterDefs(&pEnum));
		pEnum->NextOne(&pParmDef);
		while(pParmDef)
		{
			CHECK(pParmDef->GetAUID(&oldUID));
			if ( memcmp(&newUID, &oldUID, sizeof(aafUID_t)) == 0)
			{
				parmDefFound = AAFTrue;
				break;
			}
			pParmDef->ReleaseReference();
			pParmDef = NULL;
			pEnum->NextOne(&pParmDef);
		}
		pEnum->ReleaseReference();
		pEnum = NULL;
		if (!parmDefFound)
		{
			if (!_paramDefined.isPresent())
				oldCount = 0;			
			else oldCount = _paramDefined.count();

			newCount = oldCount + 1;
			tmp = new aafUID_t[newCount];
			if(tmp == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			if(oldCount!= 0)
				_paramDefined.copyToBuffer(tmp, oldCount * sizeof(aafUID_t));
			tmp[newCount - 1] = newUID;
			_paramDefined.setValue(tmp, newCount * sizeof(aafUID_t));
			delete [] tmp;
		}
		else
		{
			pParmDef->ReleaseReference();
			pParmDef = NULL;
			RAISE(AAFRESULT_OBJECT_ALREADY_ATTACHED);
		}
	}
	XEXCEPT
	{
		if (pParmDef)
		  pParmDef->ReleaseReference();
		pParmDef = 0;
		if (pEnum)
		  pEnum->ReleaseReference();
		pEnum = 0;
		if(tmp != NULL)
			delete [] tmp;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::GetParameterDefs (
      ImplEnumAAFParameterDefs **ppEnum)
{
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFParameterDefs *)CreateImpl(CLSID_EnumAAFParameterDefs);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);
	(*ppEnum)->SetEnumProperty(this, &_paramDefined);

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::CountParameterDefs (
      aafUInt32 * pResult)
{
	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);

	return(AAFRESULT_NOT_IMPLEMENTED);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::LookupParameterDef (
	  const aafUID_t & /*parameterDefId*/,
	  ImplAAFParameterDef ** ppParameterDef)
{
	if(ppParameterDef == NULL)
		return(AAFRESULT_NULL_PARAM);

	return(AAFRESULT_NOT_IMPLEMENTED);
}

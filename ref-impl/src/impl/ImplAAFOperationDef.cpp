/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\************************************************/


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

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "aafErr.h"

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
    ImplAAFOperationDef::GetDataDefinitionID(
      aafUID_t *  pDataDefID)
{
	if(pDataDefID == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pDataDefID = _dataDef;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFOperationDef::SetDataDefinitionID (
      aafUID_t  *pDataDefID)
{
	if(pDataDefID == NULL)
		return(AAFRESULT_NULL_PARAM);
	_dataDef = *pDataDefID;
	
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
    ImplAAFOperationDef::PrependDegradeToOperations (
      ImplAAFOperationDef  *pOperationDef)
{
	aafUID_t	*tmp = NULL, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;
	aafInt32	n;

	if(pOperationDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldBufSize = _degradeTo.size();
		newBufSize = oldBufSize + sizeof(aafUID_t);
		CHECK(pOperationDef->GetAUID(&newUID));
		tmp = new aafUID_t[newBufSize];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldBufSize != 0)
			_degradeTo.copyToBuffer(tmp, oldBufSize);
		for(n = oldBufSize/sizeof(aafUID_t); n >= 0; n--)
		{
			tmp[n+1] = tmp[n];
		}
		tmp[0] = newUID;
		_degradeTo.setValue(tmp, newBufSize);
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
    ImplAAFOperationDef::AppendDegradeToOperations (
      ImplAAFOperationDef  *pOperationDef)
{
	aafUID_t	*tmp, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;

	if(pOperationDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldBufSize = _degradeTo.size();
		newBufSize = oldBufSize + sizeof(aafUID_t);
		CHECK(pOperationDef->GetAUID(&newUID));
		tmp = new aafUID_t[newBufSize];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldBufSize != 0)
			_degradeTo.copyToBuffer(tmp, oldBufSize);
		tmp[oldBufSize/sizeof(aafUID_t)] = newUID;
		_degradeTo.setValue(tmp, newBufSize);
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
    ImplAAFOperationDef::GetCategory (
       wchar_t		*pCategory,
		aafInt32	bufSize)
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
			aafInt32 *		pLen)
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
      wchar_t *pCategory)
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
    ImplAAFOperationDef::AddParameterDefs (
      ImplAAFParameterDef *pAAFParameterDef)
{
	aafUID_t					*tmp, newUID, oldUID;
	aafInt32					oldBufSize;
	aafInt32					newBufSize;
	ImplAAFParameterDef*		pParmDef = NULL;
	ImplEnumAAFParameterDefs*	pEnum = NULL;
	aafBool						parmDefFound = AAFFalse;

	tmp = NULL;

	if(pAAFParameterDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		CHECK(pAAFParameterDef->GetAUID(&newUID));
		CHECK(GetParameterDefinitions(&pEnum));
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
				oldBufSize = 0;			
			else oldBufSize = _paramDefined.size();

			newBufSize = oldBufSize + sizeof(aafUID_t);
			tmp = new aafUID_t[newBufSize];
			if(tmp == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			if(oldBufSize != 0)
				_paramDefined.copyToBuffer(tmp, oldBufSize);
			tmp[oldBufSize/sizeof(aafUID_t)] = newUID;
			_paramDefined.setValue(tmp, newBufSize);
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
    ImplAAFOperationDef::GetParameterDefinitions (
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



OMDEFINE_STORABLE(ImplAAFOperationDef, AUID_AAFOperationDef);



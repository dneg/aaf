/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
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


#ifndef __ImplAAFEffectDef_h__
#include "ImplAAFEffectDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "aafErr.h"
#include "ImplAAFReferenceValue.h"

extern "C" const aafClassID_t CLSID_EnumAAFEffectDefs;
 
ImplAAFEffectDef::ImplAAFEffectDef ()
: _dataDef(			PID_EffectDefinition_DataDefinition,	"DataDefinition"),
  _isTimeWarp(		PID_EffectDefinition_IsTimeWarp,		"IsTimeWarp"),
//!!!  _degradeTo(	PID_EffectDefinition_DegradeTo,				"DegradeTo"),
  _category(		PID_EffectDefinition_Category,			"Category"),
  _numInputs(		PID_EffectDefinition_NumberInputs,		"NumberInputs"),
  _bypass(			PID_EffectDefinition_Bypass,			"Bypass"),
  _paramDefined(	PID_EffectDefinition_ParametersDefined,	"ParametersDefined")
{
	_persistentProperties.put(_dataDef.address());
	_persistentProperties.put(_isTimeWarp.address());
//!!!	_persistentProperties.put(_degradeTo.address());
	_persistentProperties.put(_category.address());
	_persistentProperties.put(_numInputs.address());
	_persistentProperties.put(_bypass.address());
	_persistentProperties.put(_paramDefined.address());
}


ImplAAFEffectDef::~ImplAAFEffectDef ()
{
	aafUID_t	nilUID = { 0 };

//	ImplAAFEssenceDescriptor *dataDef = _dataDef.setValue(nilUID);
//	ImplAAFEssenceDescriptor *degradeTo = _degradeTo.setValue(0,0);
//	ImplAAFEssenceDescriptor *paramDefined = _paramDefined.setValue(0);
//	if (dataDef)
//	{
//		dataDef->ReleaseReference();
//	}
//	if (degradeTo)
//	{
//		degradeTo->ReleaseReference();
//	}
//	if (paramDefined)
//	{
//		paramDefined->ReleaseReference();
//	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::GetDataDefinitionID(
      aafUID_t *  pDataDefID)
{
	if(pDataDefID == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pDataDefID = _dataDef;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::SetDataDefinitionID (
      aafUID_t  *pDataDefID)
{
	if(pDataDefID == NULL)
		return(AAFRESULT_NULL_PARAM);
	_dataDef = *pDataDefID;
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::IsTimeWarp (
      aafBool *bIsTimeWarp)
{
	if(bIsTimeWarp == NULL)
		return(AAFRESULT_NULL_PARAM);
	*bIsTimeWarp = _isTimeWarp;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::SetIsTimeWarp (
      aafBool  bIsTimeWarp)
{
	_isTimeWarp = bIsTimeWarp;
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::PrependDegradeToEffects (
      ImplAAFEffectDef  *pEffectDef)
{
#if 0	//!!!
	aafUID_t	*tmp = NULL, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;
	aafInt32	n;

	if(pEffectDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldBufSize = _degradeTo.size();
		newBufSize = oldBufSize + 1;
		CHECK(pEffectDef->GetAUID(&newUID));
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
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::AppendDegradeToEffects (
      ImplAAFEffectDef  *pEffectDef)
{
#if 0	//!!!
	aafUID_t	*tmp, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;

	if(pEffectDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldBufSize = _degradeTo.size();
		newBufSize = oldBufSize + 1;
		CHECK(pEffectDef->GetAUID(&newUID));
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
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::GetDegradeToEffects (
      ImplEnumAAFEffectDefs  **ppEnum)
{
#if 0	//!!!
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFEffectDefs *)CreateImpl(CLSID_EnumAAFEffectDefs);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);
	(*ppEnum)->SetEnumProperty(this, &_degradeTo);

	return(AAFRESULT_SUCCESS);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::GetCategory (
       wchar_t		*pCategory,
		aafInt32	bufSize)
{
	bool stat;

	if(pCategory == NULL)
		return(AAFRESULT_NULL_PARAM);

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
	ImplAAFEffectDef::GetCategoryBufLen (
			aafInt32 *		pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pLen = _category.size();
	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::SetCategory (
      wchar_t *pCategory)
{
	if(pCategory == NULL)
		return(AAFRESULT_NULL_PARAM);

	_category = pCategory;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::GetNumberInputs (
      aafInt32 *  pNumberInputs)
{
	if(pNumberInputs == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pNumberInputs = _numInputs;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::SetNumberInputs (
      aafInt32  NumberInputs)
{
	_numInputs = NumberInputs;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::GetBypass (
      aafUInt32 *pBypass)
{
	if(pBypass == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pBypass = _bypass;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::SetBypass (
      aafUInt32  bypass)
{
	_bypass = bypass;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEffectDef::AddParameterDefs (
      ImplAAFParameterDef *pAAFParameterDef)
{
	aafUID_t	*tmp, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;

	if(pAAFParameterDef == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldBufSize = _paramDefined.size();
		newBufSize = oldBufSize + 1;
		CHECK(pAAFParameterDef->GetAUID(&newUID));
		tmp = new aafUID_t[newBufSize];
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldBufSize != 0)
			_paramDefined.copyToBuffer(tmp, oldBufSize);
		tmp[oldBufSize/sizeof(aafUID_t)] = newUID;
		_paramDefined.setValue(tmp, newBufSize);
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
    ImplAAFEffectDef::GetParameterDefinitions (
      ImplEnumAAFParameterDefs **ppEnum)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMDEFINE_STORABLE(ImplAAFEffectDef, AUID_AAFEffectDef);



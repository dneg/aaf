/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif


 

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFResult.h"
#include "aafErr.h"
#include "ImplAAFHeader.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFParameterDef.h"
#include "ImplAAFTypeDef.h"

ImplAAFParameter::ImplAAFParameter ()
: _parmDef(			PID_Parameter_Definition,	"Definition")//,
//  _typeDef(			PID_Parameter_Type,			"Type")
{
	_persistentProperties.put(_parmDef.address());
//	_persistentProperties.put(_typeDef.address());
}


ImplAAFParameter::~ImplAAFParameter ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::SetParameterDefinition (
      ImplAAFParameterDef *pParmDef)
{
	aafUID_t			newUID;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;
	ImplAAFParameterDef	*def;

	if(pParmDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pParmDef->GetAUID(&newUID));
		CHECK(pParmDef->MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
// This is a weak reference, not yet counted
//		if(dict->LookupParameterDef(&newUID, &def) == AAFRESULT_SUCCESS)
//			def->ReleaseReference();

		_parmDef = newUID;
		pParmDef->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
			head->ReleaseReference();
		if(dict)
			dict->ReleaseReference();
	}
	XEND

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::GetParameterDefinition (
      ImplAAFParameterDef **ppParmDef)
{
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;

	if(ppParmDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
		CHECK(dict->LookupParameterDefinition(&_parmDef, ppParmDef));
		(*ppParmDef)->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
			head->ReleaseReference();
		if(dict)
			dict->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

	

AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::SetTypeDefinition (
      ImplAAFTypeDef*  pTypeDef)
{
#if 0
	aafUID_t			newUID;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;
	ImplAAFTypeDef		*def;

	if(pTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pTypeDef->GetAUID(&newUID));
		CHECK(pTypeDef->MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
// Weak references not yet refcounted
//		if(dict->LookupTypeDef(&newUID, &def) == AAFRESULT_SUCCESS)
//			def->ReleaseReference();

		_typeDef = newUID;
		pTypeDef->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
			head->ReleaseReference();
		if(dict)
			dict->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef)
{
#if 0
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;

	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
		CHECK(dict->LookupPluggableDef(&_typeDef, (ImplAAFPluggableDef **)ppTypeDef) == AAFRESULT_SUCCESS);
		(*ppTypeDef)->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
			head->ReleaseReference();
		if(dict)
			dict->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



OMDEFINE_STORABLE(ImplAAFParameter, AUID_AAFParameter);





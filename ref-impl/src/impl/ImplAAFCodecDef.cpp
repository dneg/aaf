/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplEnumAAFCodecFlavours_h__
#include "ImplEnumAAFCodecFlavours.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "ImplAAFObjectCreation.h"

#ifndef __ImplAAFCodecDef_h__
#include "ImplAAFCodecDef.h"
#endif

#include "ImplAAFDataDef.h"
#include "ImplAAFPluginManager.h"
#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AafUtils.h"
#include "ImplAAFDictionary.h"

extern "C" const aafClassID_t CLSID_EnumAAFDataDefs;
extern "C" const aafClassID_t CLSID_EnumAAFCodecFlavours;

ImplAAFCodecDef::ImplAAFCodecDef ()
:  _dataDefs(		PID_CodecDefinition_DataDefinitions,			"DataDefinitions"),
   _fileDescClass(	PID_CodecDefinition_FileDescriptorClass,		"FileDescriptorClass")

{
	_persistentProperties.put(_dataDefs.address());
	_persistentProperties.put(_fileDescClass.address());
}


ImplAAFCodecDef::~ImplAAFCodecDef ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::IsEssenceKindSupported (
      aafUID_t *pEssenceKind,
      aafBool* pIsSupported)
{
	ImplEnumAAFDataDefs	*dataEnum = NULL;
	ImplAAFDataDef		*aVal = NULL;
	aafBool				result = AAFFalse;

	XPROTECT()
	{
		CHECK(GetDataDefinitions (&dataEnum));
		while((dataEnum->NextOne(&aVal) == AAFRESULT_SUCCESS)
		   && (result == AAFFalse))
		{
			CHECK(aVal->IsDataDefOf(pEssenceKind, &result));
			aVal->ReleaseReference();
			aVal = NULL;
		}
		*pIsSupported = result;
		dataEnum->ReleaseReference();
		dataEnum = NULL;

	}
	XEXCEPT
	{
		if(aVal)
			aVal->ReleaseReference();
		if(dataEnum)
			dataEnum->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

 
 AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::AppendEssenceKind (
      aafUID_t *pEssenceKind)
{
	aafUID_t	*tmp, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;

	if(pEssenceKind == NULL)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		oldBufSize = _dataDefs.size();
		newBufSize = oldBufSize + sizeof(aafUID_t);
		tmp = new aafUID_t[newBufSize];
		newUID = *pEssenceKind;
		if(tmp == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		if(oldBufSize != 0)
			_dataDefs.copyToBuffer(tmp, oldBufSize);
		tmp[oldBufSize/sizeof(aafUID_t)] = newUID;
		_dataDefs.setValue(tmp, newBufSize);
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
    ImplAAFCodecDef::AreThereFlavours (
      aafBool *pResult)
{
	aafUID_t						uid;
	ImplAAFPluginManager			*mgr = NULL;
	IAAFPlugin						*pPlug = NULL;
	IAAFEssenceCodec				*pCodec = NULL;
	aafBool							found;
	aafInt32						flavourCount;

	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		CHECK(GetAUID(&uid));
		mgr = ImplAAFPluginManager::GetPluginManager();
		// Only looks at first codec matching
		found = AAFFalse;
		if(mgr->GetPluginInstance(uid, &pPlug) == AAFRESULT_SUCCESS)
		{
			if(pPlug->QueryInterface(IID_IAAFEssenceCodec, (void **)&pCodec) == AAFRESULT_SUCCESS)
			{
				found = AAFTrue;
			}
		}
		if(!found)
			RAISE(AAFRESULT_CODEC_INVALID);

		CHECK(pCodec->GetFlavourCount(&flavourCount));
		*pResult = (flavourCount >= 2 ? AAFTrue : AAFFalse);
		pPlug->Release();
		pPlug = NULL;
		pCodec->Release();
		pCodec = NULL;
		mgr->ReleaseReference();
		mgr = NULL;
	}
	XEXCEPT
	{
		if(pPlug != NULL)
			pPlug->Release();
		if(pCodec != NULL)
			pCodec->Release();
		if(mgr != NULL)
			mgr->ReleaseReference();
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::GetFileDescriptorClass (
      ImplAAFClassDef **ppClass)
{
	aafUID_t			classID;
	ImplAAFDictionary	*pDict = NULL;
	AAFRESULT			status;

	if (ppClass == NULL)
	{
		status =  AAFRESULT_NULL_PARAM;
	}
	else
	{
		classID = _fileDescClass;
		status = GetDictionary(&pDict);
		if(status == AAFRESULT_SUCCESS)
			status = pDict->LookupClass(&classID, ppClass);
	}

	return status;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::SetFileDescriptorClass (
      ImplAAFClassDef *pClass)
{
	aafUID_t	classID;
	
	if (pClass == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		pClass->GetAUID(&classID);
		_fileDescClass = classID;
	}
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::EnumCodecFlavours (
      ImplEnumAAFCodecFlavours **ppEnum)
{
	aafUID_t						uid;
	ImplAAFPluginManager			*mgr = NULL;
	IAAFPlugin						*pPlug = NULL;
	IAAFEssenceCodec				*pCodec = NULL;
	aafBool							found;

	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		*ppEnum = (ImplEnumAAFCodecFlavours *)CreateImpl(CLSID_EnumAAFCodecFlavours);
		if(*ppEnum == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(GetAUID(&uid));
		mgr = ImplAAFPluginManager::GetPluginManager();
		// Only looks at first codec matching
		found = AAFFalse;
		if(mgr->GetPluginInstance(uid, &pPlug) == AAFRESULT_SUCCESS)
		{
			if(pPlug->QueryInterface(IID_IAAFEssenceCodec, (void **)&pCodec) == AAFRESULT_SUCCESS)
			{
				found = AAFTrue;
			}
		}
		if(!found)
			RAISE(AAFRESULT_CODEC_INVALID);

		(*ppEnum)->SetEnumCodec(pCodec);
		pPlug->Release();
		pPlug = NULL;
		pCodec->Release();
		pCodec = NULL;
		mgr->ReleaseReference();
		mgr = NULL;
	}
	XEXCEPT
	{
		if(pPlug != NULL)
			pPlug->Release();
		if(pCodec != NULL)
			pCodec->Release();
		if(mgr != NULL)
			mgr->ReleaseReference();
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

  
// SDK-private
AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::GetDataDefinitions (
      ImplEnumAAFDataDefs  **ppEnum)
{
	if(ppEnum == NULL)
		return(AAFRESULT_NULL_PARAM);

	*ppEnum = (ImplEnumAAFDataDefs *)CreateImpl(CLSID_EnumAAFDataDefs);
	if(*ppEnum == NULL)
		return(AAFRESULT_NOMEMORY);
	(*ppEnum)->SetEnumProperty(this, &_dataDefs);

	return(AAFRESULT_SUCCESS);
}

OMDEFINE_STORABLE(ImplAAFCodecDef, AUID_AAFCodecDef);



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

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef> ImplAAFDataDefSP;

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
      ImplAAFDataDef * pEssenceKind,
      aafBool* pIsSupported)
{
	ImplEnumAAFDataDefs	*dataEnum = NULL;
	ImplAAFDataDef		*aVal = NULL;
	aafBool				result = AAFFalse;

	if (! pEssenceKind)
	  return AAFRESULT_NULL_PARAM;

	if (! pIsSupported)
	  return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(GetEssenceKinds (&dataEnum));
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
		  {
			aVal->ReleaseReference();
			aVal = 0;
		  }
		if(dataEnum)
		  {
			dataEnum->ReleaseReference();
			dataEnum = 0;
		  }
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::AddEssenceKind (
      ImplAAFDataDef * pEssenceKind)
{
	aafUID_t	*tmp, newUID;
	aafInt32	oldBufSize;
	aafInt32	newBufSize;

	if (! pEssenceKind)
	  return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		oldBufSize = _dataDefs.size();
		newBufSize = oldBufSize + sizeof(aafUID_t);
		tmp = new aafUID_t[newBufSize];
		CHECK(pEssenceKind->GetAUID(&newUID));
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
    ImplAAFCodecDef::RemoveEssenceKind (
      ImplAAFDataDef * pEssenceKind)
{
  if (! pEssenceKind)
	return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::CountEssenceKinds (
      aafUInt32 * pResult)
{
  if (! pResult) return AAFRESULT_NULL_PARAM;

  return AAFRESULT_NOT_IMPLEMENTED;
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
		  {
			mgr->ReleaseReference();
			mgr = 0;
		  }
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
			status = pDict->LookupClassDef(classID, ppClass);
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
		  {
			mgr->ReleaseReference();
			mgr = 0;
		  }
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

  
// SDK-private
AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::GetEssenceKinds (
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




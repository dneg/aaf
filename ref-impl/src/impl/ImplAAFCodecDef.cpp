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
:  _dataDefs     ( PID_CodecDefinition_DataDefinitions,
                   L"DataDefinitions", 
                   L"/Dictionary/DataDefinitions", 
                   PID_DefinitionObject_Identification),
   _fileDescClass( PID_CodecDefinition_FileDescriptorClass,
                   L"FileDescriptorClass", 
                   L"/Dictionary/ClassDefinitions", 
                   PID_MetaDefinition_Identification)

{
	_persistentProperties.put(_dataDefs.address());
	_persistentProperties.put(_fileDescClass.address());
}


ImplAAFCodecDef::~ImplAAFCodecDef ()
{
}

  
AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::Initialize (
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
    ImplAAFCodecDef::IsEssenceKindSupported (
      ImplAAFDataDef * pEssenceKind,
      aafBool* pIsSupported)
{
	ImplEnumAAFDataDefs	*dataEnum = NULL;
	ImplAAFDataDef		*aVal = NULL;
	aafBool				result = kAAFFalse;

	if (! pEssenceKind)
	  return AAFRESULT_NULL_PARAM;

	if (! pIsSupported)
	  return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(GetEssenceKinds (&dataEnum));
		while((dataEnum->NextOne(&aVal) == AAFRESULT_SUCCESS)
		   && (result == kAAFFalse))
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
	if (! pEssenceKind)
	  return AAFRESULT_NULL_PARAM;

	_dataDefs.appendValue(pEssenceKind);

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::RemoveEssenceKind (
      ImplAAFDataDef * pEssenceKind)
{
	if (! pEssenceKind)
		return AAFRESULT_NULL_PARAM;
	
	if(_dataDefs.countOfValue(pEssenceKind) == 0)
		return AAFRESULT_OBJECT_NOT_FOUND;
	_dataDefs.removeValue(pEssenceKind);

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::CountEssenceKinds (
      aafUInt32 * pResult)
{
	if (! pResult)
		return AAFRESULT_NULL_PARAM;

	*pResult = _dataDefs.count();

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
	aafUInt32						flavourCount;

	if(pResult == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		CHECK(GetAUID(&uid));
		mgr = ImplAAFPluginManager::GetPluginManager();
		// Only looks at first codec matching
		found = kAAFFalse;
		if(mgr->GetPluginInstance(uid, &pPlug) == AAFRESULT_SUCCESS)
		{
			if(pPlug->QueryInterface(IID_IAAFEssenceCodec, (void **)&pCodec) == AAFRESULT_SUCCESS)
			{
				found = kAAFTrue;
			}
		}
		if(!found)
			RAISE(AAFRESULT_CODEC_INVALID);

		CHECK(pCodec->CountFlavours(&flavourCount));
		*pResult = (flavourCount >= 2 ? kAAFTrue : kAAFFalse);
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
	AAFRESULT			status = AAFRESULT_SUCCESS;

	if (ppClass == NULL)
	{
		status =  AAFRESULT_NULL_PARAM;
	}
	else
	{
		*ppClass = _fileDescClass;
		if (*ppClass)
			(*ppClass)->AcquireReference();
	}

	return status;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::SetFileDescriptorClass (
      ImplAAFClassDef *pClass)
{
	if (pClass == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!pClass->attached())
	{
		return AAFRESULT_OBJECT_NOT_ATTACHED;
	}
	else
	{
		_fileDescClass = pClass;
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
		found = kAAFFalse;
		if(mgr->GetPluginInstance(uid, &pPlug) == AAFRESULT_SUCCESS)
		{
			if(pPlug->QueryInterface(IID_IAAFEssenceCodec, (void **)&pCodec) == AAFRESULT_SUCCESS)
			{
				found = kAAFTrue;
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
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFDataDefs *theEnum = (ImplEnumAAFDataDefs *)CreateImpl (CLSID_EnumAAFDataDefs);
	
	XPROTECT()
	{
		OMWeakReferenceVectorIterator</*OMUniqueObjectIdentification,*/ ImplAAFDataDef>* iter = 
			new OMWeakReferenceVectorIterator</*OMUniqueObjectIdentification,*/ ImplAAFDataDef>(_dataDefs);
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




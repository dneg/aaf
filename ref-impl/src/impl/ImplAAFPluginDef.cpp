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

#ifndef __ImplAAFNetworkLocator_h__
#include "ImplAAFNetworkLocator.h"
#endif

#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#ifndef __ImplEnumAAFPluginLocators_h__
#include "ImplEnumAAFPluginLocators.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFPluginDef_h__
#include "ImplAAFPluginDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFDictionary.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluginLocators;


ImplAAFPluginDef::ImplAAFPluginDef ()
: _categoryClass(		PID_PluginDefinition_CategoryClass,		L"CategoryClass"),
 _pluginVersion(        PID_PluginDefinition_VersionNumber,     L"VersionNumber"),
 _pluginVersionString(	PID_PluginDefinition_VersionString,		L"VersionString"),
 _pluginManufacturerName(PID_PluginDefinition_Manufacturer,		L"Manufacturer"),
 _manufacturerURL(		PID_PluginDefinition_ManufacturerInfo,  L"ManufacturerInfo"),
 _pluginManufacturerID(	PID_PluginDefinition_ManufacturerID,	L"ManufacturerID"),
 _platform(				PID_PluginDefinition_Platform,			L"Platform"),
 _minPlatformVersion(   PID_PluginDefinition_MinPlatformVersion,L"MinPlatformVersion"),
 _maxPlatformVersion(   PID_PluginDefinition_MaxPlatformVersion,L"MaxPlatformVersion"),
 _engine(				PID_PluginDefinition_Engine,			L"Engine"),
 _minEngineVersion(     PID_PluginDefinition_MinEngineVersion,  L"MinEngineVersion"),
 _maxEngineVersion(     PID_PluginDefinition_MaxEngineVersion,  L"MaxEngineVersion"),
 _pluginAPI(			PID_PluginDefinition_PluginAPI,			L"PluginAPI"),
 _minPluginAPIVersion(  PID_PluginDefinition_MinPluginAPI,      L"MinPluginAPI"),
 _maxPluginAPIVersion(  PID_PluginDefinition_MaxPluginAPI,      L"MaxPluginAPI"),
 _softwareOnly(			PID_PluginDefinition_SoftwareOnly,		L"SoftwareOnly"),
 _accelerator(			PID_PluginDefinition_Accelerator,		L"Accelerator"),
 _locators(				PID_PluginDefinition_Locators,			L"Locators"),
 _authentication(		PID_PluginDefinition_Authentication,	L"Authentication"),
 _defObj(				PID_PluginDefinition_DefinitionObject,	L"DefinitionObject")
{
  _persistentProperties.put(_categoryClass.address());
  _persistentProperties.put(_pluginVersion.address());
  _persistentProperties.put(_pluginVersionString.address());
  _persistentProperties.put(_pluginManufacturerName.address());
  _persistentProperties.put(_manufacturerURL.address());
  _persistentProperties.put(_pluginManufacturerID.address());
  _persistentProperties.put(_platform.address());
  _persistentProperties.put(_minPlatformVersion.address());
  _persistentProperties.put(_maxPlatformVersion.address());
  _persistentProperties.put(_engine.address());
  _persistentProperties.put(_minEngineVersion.address());
  _persistentProperties.put(_maxEngineVersion.address());
  _persistentProperties.put(_pluginAPI.address());
  _persistentProperties.put(_minPluginAPIVersion.address());
  _persistentProperties.put(_maxPluginAPIVersion.address());
  _persistentProperties.put(_softwareOnly.address());
  _persistentProperties.put(_accelerator.address());
  _persistentProperties.put(_locators.address());
  _persistentProperties.put(_authentication.address());
  _persistentProperties.put(_defObj.address());
}


ImplAAFPluginDef::~ImplAAFPluginDef ()
{
	// Release the manufacturer locator
	ImplAAFNetworkLocator *pNetLocator = _manufacturerURL.clearValue();
	if (pNetLocator)
	{
	  pNetLocator->ReleaseReference();
	  pNetLocator = 0;
	}

	// Release all of the other locator pointers.
	size_t size = _locators.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFLocator *pLocator = _locators.clearValueAt(i);
		if (pLocator)
		{
		  pLocator->ReleaseReference();
		  pLocator = 0;
		}
	}
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::Initialize (
      const aafUID_t & id,
	  const aafCharacter * pName,
	  const aafCharacter * pDesc)
{
	if (pName == NULL || pDesc == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		ImplAAFDefObject::pvtInitialize(id, pName, pDesc);
	}
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetCategoryClass (
      aafUID_t *pCategoryClass)
{
	if (pCategoryClass == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pCategoryClass = _categoryClass;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetCategoryClass (
      const aafUID_t & categoryClass)
{
  _categoryClass = categoryClass;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPluginVersion (
      aafVersionType_t *pVersion)
{
	if (pVersion == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pVersion = _pluginVersion;
	}
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPluginVersion (
      aafVersionType_t *pVersion)
{
	if (pVersion == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else
	{
		_pluginVersion = *pVersion;
	}
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPluginVersionString (
      aafCharacter *pVersionString,
      aafUInt32  bufSize)
{
	bool stat;

	if(pVersionString == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_pluginVersionString.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	stat = _pluginVersionString.copyToBuffer(pVersionString, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPluginVersionStringBufLen (
      aafUInt32 *pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_pluginVersionString.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pLen = _pluginVersionString.size();
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPluginVersionString (
      const aafCharacter * pVersionString)
{
	if(pVersionString == NULL)
		return(AAFRESULT_NULL_PARAM);

	_pluginVersionString = pVersionString;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPluginManufacturerName (
      aafCharacter *  pManufacturerName,
      aafUInt32  bufSize)
{
	if(pManufacturerName == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (!_pluginManufacturerName.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
		
	stat = _pluginManufacturerName.copyToBuffer(pManufacturerName, bufSize);
	if (! stat)
	{
		return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}
	
	return AAFRESULT_SUCCESS; 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPluginManufacturerNameBufLen (
      aafUInt32 * pLen)
{
	if(pLen == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_pluginManufacturerName.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	*pLen = _pluginManufacturerName.size();
	return AAFRESULT_SUCCESS; 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPluginManufacturerName (
      const aafCharacter * pManufacturerName)
{
	if(pManufacturerName == NULL)
		return(AAFRESULT_NULL_PARAM);

	_pluginManufacturerName = pManufacturerName;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetManufacturerInfo (
      ImplAAFNetworkLocator **ppResult)
{
	if (ppResult == NULL)
		return AAFRESULT_NULL_PARAM;
	
	if (!_manufacturerURL.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	
	*ppResult = _manufacturerURL;
	if (*ppResult)
		(*ppResult)->AcquireReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetManufacturerInfo (
      ImplAAFNetworkLocator *pManufacturerInfo)
{
	if (pManufacturerInfo == NULL)
		return AAFRESULT_NULL_PARAM;

	if (_manufacturerURL.isPresent ())
	  {
		ImplAAFNetworkLocator *pOldLoc = _manufacturerURL;
		if (pOldLoc)
		  pOldLoc->ReleaseReference();
		pOldLoc = 0;
	  }

	_manufacturerURL = pManufacturerInfo;
	
	if (pManufacturerInfo)
		pManufacturerInfo->AcquireReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetManufacturerID (
      aafUID_t *pManufacturerID)
{
	if (pManufacturerID == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_pluginManufacturerID.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pManufacturerID = _pluginManufacturerID;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetManufacturerID (
      const aafUID_t & manufacturerID)
{
	_pluginManufacturerID = manufacturerID;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetHardwarePlatform (
      aafHardwarePlatform_t *pHardwarePlatform)
{
	if (pHardwarePlatform == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_platform.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pHardwarePlatform = _platform;
	}

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetHardwarePlatform (
      aafHardwarePlatform_constref hardwarePlatform)
{
	_platform = hardwarePlatform;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPlatformVersionRange (
      aafVersionType_t *pMinVersion,
      aafVersionType_t *pMaxVersion)
{
	if (pMinVersion == NULL || pMaxVersion == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_minPlatformVersion.isPresent() || !_maxPlatformVersion.isPresent() )
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pMinVersion = _minPlatformVersion;
		*pMaxVersion = _maxPlatformVersion;
	}

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPlatformMinimumVersion (
      const aafVersionType_t & minVersion)
{
  _minPlatformVersion = minVersion;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPlatformMaximumVersion (
      const aafVersionType_t & maxVersion)
{
  _maxPlatformVersion = maxVersion;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetEngine (
      aafEngine_t * pEngine)
{
	if (pEngine == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_engine.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pEngine = _engine;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetEngine (
      aafEngine_constref  engine)
{
	_engine = engine;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetEngineVersionRange (
      aafVersionType_t *pMinVersion,
      aafVersionType_t *pMaxVersion)
{
	if (pMinVersion == NULL || pMaxVersion == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_minEngineVersion.isPresent() || !_maxEngineVersion.isPresent() )
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pMinVersion = _minEngineVersion;
		*pMaxVersion = _maxEngineVersion;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetEngineMinimumVersion (
      const aafVersionType_t & minVersion)
{
  _minEngineVersion = minVersion;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetEngineMaximumVersion (
      const aafVersionType_t & maxVersion)
{
  _maxEngineVersion = maxVersion;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPluginAPI (
      aafPluginAPI_t *pPluginAPI)
{
	if (pPluginAPI == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_pluginAPI.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pPluginAPI = _pluginAPI;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPluginAPI (
      aafPluginAPI_constref  pluginAPI)
{
	_pluginAPI = pluginAPI;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPluginAPIVersionRange (
      aafVersionType_t *pMinVersion,
      aafVersionType_t *pMaxVersion)
{
	if (pMinVersion == NULL || pMaxVersion == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_minPluginAPIVersion.isPresent() || !_maxPluginAPIVersion.isPresent() )
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pMinVersion = _minPluginAPIVersion;
		*pMaxVersion = _maxPluginAPIVersion;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPluginAPIMinimumVersion (
      const aafVersionType_t & minVersion)
{
  _minPluginAPIVersion = minVersion;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPluginAPIMaximumVersion (
      const aafVersionType_t & maxVersion)
{
  _maxPluginAPIVersion = maxVersion;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::IsSoftwareOnly (
      aafBool *pIsSoftwareOnly)
{
	if (pIsSoftwareOnly == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_softwareOnly.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pIsSoftwareOnly = _softwareOnly;
	}

	return AAFRESULT_SUCCESS;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetIsSoftwareOnly (
      aafBool  isSoftwareOnly)
{
	_softwareOnly = isSoftwareOnly;
	return AAFRESULT_SUCCESS;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::IsAccelerated (
      aafBool *pIsAccelerated)
{
	if (pIsAccelerated == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_accelerator.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pIsAccelerated = _accelerator;
	}

	return AAFRESULT_SUCCESS;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetIsAccelerated (
      aafBool  isAccelerated)
{
	_accelerator = isAccelerated;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SupportsAuthentication (
      aafBool *pSupportsAuthentication)
{
	if (pSupportsAuthentication == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	else if (!_authentication.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pSupportsAuthentication = _authentication;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetSupportsAuthentication (
      aafBool  SupportsAuthentication)
{
	_authentication = SupportsAuthentication;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::CountLocators (
      aafUInt32 *pCount)
{
	size_t	siz;
	if (! pCount)
	{
		return AAFRESULT_NULL_PARAM;
	}

	_locators.getSize(siz);
	*pCount = siz;
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::AppendLocator (
      ImplAAFLocator *pLocator)
{
	if(pLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	_locators.appendValue(pLocator);
	pLocator->AcquireReference();

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::PrependLocator (
      ImplAAFLocator *pLocator)
{
	if(pLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	_locators.prependValue(pLocator);
	pLocator->AcquireReference();

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::InsertLocatorAt (
	  aafUInt32 index,
      ImplAAFLocator *pLocator)
{
	if(pLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	aafUInt32 count;
	AAFRESULT ar;
	ar = CountLocators (&count);
	if (AAFRESULT_FAILED (ar)) return ar;

	if (index > count)
	  return AAFRESULT_BADINDEX;

	_locators.insertAt(pLocator,index);
	pLocator->AcquireReference();
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetLocatorAt (
	  aafUInt32 index,
      ImplAAFLocator ** ppLocator)
{
	if(ppLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	aafUInt32 count;
	AAFRESULT hr;
	hr = CountLocators (&count);
	if (AAFRESULT_FAILED (hr)) return hr;

	if (index >= count)
	  return AAFRESULT_BADINDEX;

	ImplAAFLocator *pLocator;
	_locators.getValueAt(pLocator,index);

	assert(pLocator);
	pLocator->AcquireReference();
	(*ppLocator)=pLocator;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::RemoveLocatorAt (
	  aafUInt32 index)
{
	aafUInt32 count;
	AAFRESULT hr;
	ImplAAFLocator	*pLoc;
	
	hr = CountLocators (&count);
	if (AAFRESULT_FAILED (hr)) return hr;

	if (index >= count)
	  return AAFRESULT_BADINDEX;

	pLoc = _locators.removeAt(index);
	if(pLoc)
		pLoc->ReleaseReference();

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetPluginDescriptorID (
      aafUID_t *pDescriptorID)
{
	if(pDescriptorID == NULL)
		return(AAFRESULT_NULL_PARAM);
	return ImplAAFDefObject::GetAUID(pDescriptorID);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetPluginDescriptorID (
      aafUID_t  descriptorID)
{
	return ImplAAFDefObject::SetAUID(descriptorID);
}

  

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetLocators (
      ImplEnumAAFPluginLocators **ppEnum)
{
	ImplEnumAAFPluginLocators		*theEnum = (ImplEnumAAFPluginLocators *)CreateImpl (CLSID_EnumAAFPluginLocators);
		
	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFLocator>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFLocator>(_locators);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFPluginLocators, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

// Internal to the toolkit functions
AAFRESULT
    ImplAAFPluginDef::GetNthLocator (aafInt32 index, ImplAAFLocator **ppLocator)
{
	if(ppLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	ImplAAFLocator	*obj = NULL;
	_locators.getValueAt(obj, index);
	*ppLocator = obj;
	if (obj)
		obj->AcquireReference();
	else
		return AAFRESULT_NO_MORE_OBJECTS; // AAFRESULT_BADINDEX ???

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::SetDefinitionObjectID (aafUID_t def)
{
  _defObj = def;

  return AAFRESULT_SUCCESS;
//	assert(_dataDef.isVoid());
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDef::GetDefinitionObjectID (aafUID_t *pDef)
{
  aafUID_t	uid;

  
  if (! pDef)
	return AAFRESULT_NULL_PARAM;

  uid = _defObj;
	*pDef = uid;

  return AAFRESULT_SUCCESS;
}
// Internal to the toolkit functions
/*
AAFRESULT
    ImplAAFPluginDef::GetNumLocators (aafInt32 *pCount)
{
	size_t	siz;
	if (! pCount)
	{
		return AAFRESULT_NULL_PARAM;
	}

	_locators.getSize(siz);
	*pCount = siz;
	return(AAFRESULT_SUCCESS);
}
*/




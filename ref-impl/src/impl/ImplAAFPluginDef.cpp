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

#ifndef __ImplAAFPluggableCode_h__
#include "ImplAAFPluggableCode.h"
#endif

#ifndef __ImplEnumAAFPluginLocators_h__
#include "ImplEnumAAFPluginLocators.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFPluginDescriptor_h__
#include "ImplAAFPluginDescriptor.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluginLocators;


ImplAAFPluginDescriptor::ImplAAFPluginDescriptor ()
: _name           (		PID_PluginDescriptor_Name,				"Name"),
  _description    (		PID_PluginDescriptor_Description,		"Description"),
  _identification (		PID_PluginDescriptor_Identification,	"Identification"),
 _categoryClass(		PID_PluginDescriptor_CategoryClass,		"CategoryClass"),
 _pluginVersion(        PID_PluginDescriptor_VersionNumber,     "VersionNumber"),
 _pluginVersionString(	PID_PluginDescriptor_VersionString,		"VersionString"),
 _pluginManufacturerName(PID_PluginDescriptor_Manufacturer,		"Manufacturer"),
 _manufacturerURL(		PID_PluginDescriptor_ManufacturerInfo,  "ManufacturerInfo"),
 _pluginManufacturerID(	PID_PluginDescriptor_ManufacturerID,	"ManufacturerID"),
 _platform(				PID_PluginDescriptor_Platform,			"Platform"),
 _minPlatformVersion(   PID_PluginDescriptor_MinPlatformVersion,"MinPlatformVersion"),
 _maxPlatformVersion(   PID_PluginDescriptor_MaxPlatformVersion,"MaxPlatformVersion"),
 _engine(				PID_PluginDescriptor_Engine,			"Engine"),
 _minEngineVersion(     PID_PluginDescriptor_MinEngineVersion,  "MinEngineVersion"),
 _maxEngineVersion(     PID_PluginDescriptor_MaxEngineVersion,  "MaxEngineVersion"),
 _pluginAPI(			PID_PluginDescriptor_PluginAPI,			"PluginAPI"),
 _minPluginAPIVersion(  PID_PluginDescriptor_MinPluginAPI,      "MinPluginAPI"),
 _maxPluginAPIVersion(  PID_PluginDescriptor_MaxPluginAPI,      "MaxPluginAPI"),
 _softwareOnly(			PID_PluginDescriptor_SoftwareOnly,		"SoftwareOnly"),
 _accelerator(			PID_PluginDescriptor_Accelerator,		"Accelerator"),
 _locators(				PID_PluginDescriptor_Locators,			"Locators"),
 _authentication(		PID_PluginDescriptor_Authentication,	"Authentication")
{
  _persistentProperties.put(_name.address());
  _persistentProperties.put(_description.address());
  _persistentProperties.put(_identification.address());
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
}


ImplAAFPluginDescriptor::~ImplAAFPluginDescriptor ()
{
	// Release the manufacturer locator
	ImplAAFNetworkLocator *pNetLocator = _manufacturerURL.setValue(0);
	if (pNetLocator)
	{
	  pNetLocator->ReleaseReference();
	  pNetLocator = 0;
	}

	// Release all of the other locator pointers.
	size_t size = _locators.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFLocator *pLocator = _locators.setValueAt(0, i);
		if (pLocator)
		{
		  pLocator->ReleaseReference();
		  pLocator = 0;
		}
	}
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::Initialize (
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
		_identification = id;
		_name = pName;
		_description = pDesc;
	}
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::SetName (
      const aafCharacter *  pName)
{
	if (! pName)
	{
		return AAFRESULT_NULL_PARAM;
	}
	
	_name = pName;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetName (
      aafCharacter *  pName,
      aafUInt32  bufSize)
{
	bool stat;
	if (! pName)
	{
		return AAFRESULT_NULL_PARAM;
	}
	stat = _name.copyToBuffer(pName, bufSize);
	if (! stat)
	{
		return AAFRESULT_SMALLBUF;
	}
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetNameBufLen (
      aafUInt32 *  pBufSize)  //@parm [in,out] Definition Name length
{
  if (! pBufSize)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pBufSize = _name.size();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::SetDescription (
      const aafCharacter * pDescription)
{
	if (! pDescription)
	{
		return AAFRESULT_NULL_PARAM;
	}
	
	_description = pDescription;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetDescription (
      aafCharacter * pDescription,
      aafUInt32 bufSize)
{
	if (! pDescription)
	{
		return AAFRESULT_NULL_PARAM;
	}
	
	if (!_description.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	bool stat;
	
	stat = _description.copyToBuffer(pDescription, bufSize);
	if (! stat)
	{
		return AAFRESULT_SMALLBUF;
	}
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetDescriptionBufLen (
      aafUInt32 * pBufSize)  //@parm [in,out] Definition Name length
{
	if (! pBufSize)
	{
		return AAFRESULT_NULL_PARAM;
	}

	if (!_description.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
		
	*pBufSize = _description.size();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetAUID (
      aafUID_t *pAuid) const
{
  if (pAuid == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  else
	{
	  *pAuid = _identification;
	}

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::SetAUID (
      const aafUID_t & id)
{
  _identification = id;

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetCategoryClass (
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
    ImplAAFPluginDescriptor::SetCategoryClass (
      const aafUID_t & categoryClass)
{
  _categoryClass = categoryClass;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetPluginVersion (
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
    ImplAAFPluginDescriptor::SetPluginVersion (
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
    ImplAAFPluginDescriptor::GetPluginVersionString (
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
    ImplAAFPluginDescriptor::GetPluginVersionStringBufLen (
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
    ImplAAFPluginDescriptor::SetPluginVersionString (
      const aafCharacter * pVersionString)
{
	if(pVersionString == NULL)
		return(AAFRESULT_NULL_PARAM);

	_pluginVersionString = pVersionString;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetPluginManufacturerName (
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
    ImplAAFPluginDescriptor::GetPluginManufacturerNameBufLen (
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
    ImplAAFPluginDescriptor::SetPluginManufacturerName (
      const aafCharacter * pManufacturerName)
{
	if(pManufacturerName == NULL)
		return(AAFRESULT_NULL_PARAM);

	_pluginManufacturerName = pManufacturerName;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetManufacturerInfo (
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
    ImplAAFPluginDescriptor::SetManufacturerInfo (
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
    ImplAAFPluginDescriptor::GetManufacturerID (
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
    ImplAAFPluginDescriptor::SetManufacturerID (
      const aafUID_t & manufacturerID)
{
	_pluginManufacturerID = manufacturerID;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetHardwarePlatform (
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
    ImplAAFPluginDescriptor::SetHardwarePlatform (
      aafHardwarePlatform_constref hardwarePlatform)
{
	_platform = hardwarePlatform;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetPlatformVersionRange (
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
    ImplAAFPluginDescriptor::SetPlatformMinimumVersion (
      const aafVersionType_t & minVersion)
{
  _minPlatformVersion = minVersion;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::SetPlatformMaximumVersion (
      const aafVersionType_t & maxVersion)
{
  _maxPlatformVersion = maxVersion;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetEngine (
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
    ImplAAFPluginDescriptor::SetEngine (
      aafEngine_constref  engine)
{
	_engine = engine;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetEngineVersionRange (
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
    ImplAAFPluginDescriptor::SetEngineMinimumVersion (
      const aafVersionType_t & minVersion)
{
  _minEngineVersion = minVersion;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::SetEngineMaximumVersion (
      const aafVersionType_t & maxVersion)
{
  _maxEngineVersion = maxVersion;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetPluginAPI (
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
    ImplAAFPluginDescriptor::SetPluginAPI (
      aafPluginAPI_constref  pluginAPI)
{
	_pluginAPI = pluginAPI;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetPluginAPIVersionRange (
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
    ImplAAFPluginDescriptor::SetPluginAPIMinimumVersion (
      const aafVersionType_t & minVersion)
{
  _minPluginAPIVersion = minVersion;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::SetPluginAPIMaximumVersion (
      const aafVersionType_t & maxVersion)
{
  _maxPluginAPIVersion = maxVersion;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::IsSoftwareOnly (
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
    ImplAAFPluginDescriptor::SetIsSoftwareOnly (
      aafBool  isSoftwareOnly)
{
	_softwareOnly = isSoftwareOnly;
	return AAFRESULT_SUCCESS;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::IsAccelerated (
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
    ImplAAFPluginDescriptor::SetIsAccelerated (
      aafBool  isAccelerated)
{
	_accelerator = isAccelerated;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::SupportsAuthentication (
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
    ImplAAFPluginDescriptor::SetSupportsAuthentication (
      aafBool  SupportsAuthentication)
{
	_authentication = SupportsAuthentication;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetPluggableCode (
      ImplAAFPluggableCode ** /*pCode*/)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::CountLocators (
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
    ImplAAFPluginDescriptor::AppendLocator (
      ImplAAFLocator *pLocator)
{
	if(pLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	_locators.appendValue(pLocator);
	pLocator->AcquireReference();

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::PrependLocator (
      ImplAAFLocator *pLocator)
{
	if(pLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	size_t			siz;
	long			n;
	ImplAAFLocator	*obj = NULL;

	_locators.getSize(siz);
	for(n = siz-1; n >= 0; n--)
	{
		_locators.getValueAt(obj, n);
		_locators.setValueAt(NULL, n);
		_locators.setValueAt(obj, n+1);
	}
	_locators.setValueAt(pLocator, 0);
	pLocator->AcquireReference();

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::InsertLocatorAt (
	  aafUInt32 index,
      ImplAAFLocator *pLocator)
{
	if(pLocator == NULL)
		return(AAFRESULT_NULL_PARAM);

	aafUInt32 count;
	AAFRESULT hr;
	hr = CountLocators (&count);
	if (AAFRESULT_FAILED (hr)) return hr;

	if (index > count)
	  return AAFRESULT_BADINDEX;

	return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetLocatorAt (
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

	return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::RemoveLocatorAt (
	  aafUInt32 index)
{
	aafUInt32 count;
	AAFRESULT hr;
	hr = CountLocators (&count);
	if (AAFRESULT_FAILED (hr)) return hr;

	if (index >= count)
	  return AAFRESULT_BADINDEX;

	return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::IsPluginLocal (
      aafBool *  /*pIsLocal*/)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetPluginDescriptorID (
      aafUID_t *pDescriptorID)
{
	if(pDescriptorID == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pDescriptorID = _identification;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::SetPluginDescriptorID (
      aafUID_t  descriptorID)
{
	_identification = descriptorID;
	return AAFRESULT_SUCCESS;
}

  

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPluginDescriptor::GetLocators (
      ImplEnumAAFPluginLocators **ppEnum)
{
	ImplEnumAAFPluginLocators		*theEnum = (ImplEnumAAFPluginLocators *)CreateImpl (CLSID_EnumAAFPluginLocators);
		
	XPROTECT()
	{
		CHECK(theEnum->SetPluginDescriptor(this));
		CHECK(theEnum->Reset());
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
    ImplAAFPluginDescriptor::GetNthLocator (aafInt32 index, ImplAAFLocator **ppLocator)
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

// Internal to the toolkit functions
/*
AAFRESULT
    ImplAAFPluginDescriptor::GetNumLocators (aafInt32 *pCount)
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




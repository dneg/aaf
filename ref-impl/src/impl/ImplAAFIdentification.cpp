
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFIdentification_h__
#include "ImplAAFIdentification.h"
#endif

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFUtils.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <wchar.h>


ImplAAFIdentification::~ImplAAFIdentification ()
{}

ImplAAFIdentification::ImplAAFIdentification():
  _companyName(         PID_Identification_CompanyName,          L"CompanyName"),
  _productName(         PID_Identification_ProductName,          L"ProductName"),
  _productVersion(      PID_Identification_ProductVersion,       L"ProductVersion"),
  _productVersionString(PID_Identification_ProductVersionString, L"ProductVersionString"),
  _productId(			  PID_Identification_ProductID,            L"ProductID"),
  _date(                PID_Identification_Date,                 L"Date"),
  _toolkitVersion(      PID_Identification_ToolkitVersion,       L"ToolkitVersion"),
  _platform(            PID_Identification_Platform,             L"Platform"),
  _generation(       PID_Identification_GenerationAUID,       L"GenerationAUID")
{
  // Insert the properties into the persistent property set.
  //
  _persistentProperties.put(   _companyName.address());
  _persistentProperties.put(   _productName.address());
  _persistentProperties.put(_productVersion.address());
  _persistentProperties.put(   _productVersionString.address());
  _persistentProperties.put(_productId.address());
  _persistentProperties.put(   _date.address());
  _persistentProperties.put(_toolkitVersion.address());
  _persistentProperties.put(   _platform.address());
  _persistentProperties.put(_generation.address());

  _platform = L"AAFSDK (" PLATFORM_NAME L")";
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFIdentification::Initialize
 (aafCharacter_constptr companyName,
  aafCharacter_constptr productName,
  aafCharacter_constptr productVersionString,
  aafUID_constref productID)
{
  if (isInitialized ()) return AAFRESULT_ALREADY_INITIALIZED;

  if (! companyName)          return AAFRESULT_NULL_PARAM;
  if (! productName)          return AAFRESULT_NULL_PARAM;
  if (! productVersionString) return AAFRESULT_NULL_PARAM;

  AAFRESULT hr;

  hr = SetCompanyName (companyName);
  if (AAFRESULT_FAILED (hr)) return hr;
  hr = SetProductName (productName);
  if (AAFRESULT_FAILED (hr)) return hr;
  hr = SetProductVersionString (productVersionString);
  if (AAFRESULT_FAILED (hr)) return hr;
  hr = SetProductID (productID);
  if (AAFRESULT_FAILED (hr)) return hr;

  aafUID_t gen;
  hr = aafAUIDNew (&gen);
  if (AAFRESULT_FAILED (hr)) return hr;
  _generation = gen;

  aafTimeStamp_t timestamp;
  AAFGetDateTime (&timestamp);
  _date = timestamp;

  _toolkitVersion = AAFReferenceImplementationVersion;

  setInitialized ();

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetCompanyName (aafWChar *  pName,
										   aafUInt32 bufSize)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  bool stat;
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  stat = _companyName.copyToBuffer(pName, bufSize);
  if (! stat)
	{
	  return AAFRESULT_SMALLBUF;
	}
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetCompanyNameBufLen (aafUInt32 *  pLen)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  if (! pLen)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pLen = _companyName.size();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductName (aafWChar *  pProductName,
										   aafUInt32 bufSize)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  bool stat;
  if (! pProductName)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  stat = _productName.copyToBuffer(pProductName, bufSize);
  if (! stat)
	{
	  return AAFRESULT_SMALLBUF;
	}
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductNameBufLen (aafUInt32 *  pLen)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  if (! pLen)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pLen = _productName.size();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductVersion (aafProductVersion_t* productVersion)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

	if (! productVersion)
	{
		return AAFRESULT_NULL_PARAM;
	}

	if(!_productVersion.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}	
	*productVersion = _productVersion;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductVersionString (aafWChar *  pVS,
													aafUInt32 bufSize)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  bool stat;
  if (! pVS)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  stat = _productVersionString.copyToBuffer(pVS, bufSize);
  if (! stat)
	{
	  return AAFRESULT_SMALLBUF;
	}
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductVersionStringBufLen (aafUInt32 *  pLen)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  if (! pLen)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pLen = _productVersionString.size();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductID (aafUID_t * pPID)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  if (! pPID)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  *pPID = _productId;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetDate (aafTimeStamp_t * pTS)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  if (! pTS)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pTS = _date;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetRefImplVersion (aafProductVersion_t* productVersion)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

	if (! productVersion)
	{
		return AAFRESULT_NULL_PARAM;
	}
	
	if(!_toolkitVersion.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}

	*productVersion = _toolkitVersion;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetPlatform (aafWChar *  pPlatform,
										aafUInt32 bufSize)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

	if (! pPlatform)
	{
		return AAFRESULT_NULL_PARAM;
	}
	
	if(!_platform.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}

	bool stat;
	
	stat = _platform.copyToBuffer(pPlatform, bufSize);
	if (! stat)
	{
		return AAFRESULT_SMALLBUF;
	}
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetPlatformBufLen (aafUInt32 *  pLen)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

	if (! pLen)
	{
		return AAFRESULT_NULL_PARAM;
	}

	if(!_platform.isPresent())
	{
		return AAFRESULT_PROP_NOT_PRESENT;
	}
		
	*pLen = _platform.size();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetGenerationID (aafUID_t *  pGen)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

  if (! pGen)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  *pGen = _generation;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetCompanyName (const aafCharacter * pName)
{
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
	  return AAFRESULT_BAD_SIZE;
	}

  _companyName = pName;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductName (const aafCharacter * pName)
{
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (wcslen(pName)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
	  return AAFRESULT_BAD_SIZE;
	}

  _productName = pName;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductVersion (aafProductVersion_constref productVersion)
{
  if (! isInitialized ()) return AAFRESULT_NOT_INITIALIZED;

	_productVersion = productVersion;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductVersionString (const aafCharacter * pVS)
{
	if (! pVS)
	{
		return AAFRESULT_NULL_PARAM;
	}
	
	if (wcslen(pVS)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
	{
		return AAFRESULT_BAD_SIZE;
	}

	_productVersionString = pVS;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductID (const aafUID_t & PID)
{
  _productId = PID;
  return AAFRESULT_SUCCESS;
}

void ImplAAFIdentification::onCopy(void* /*clientContext*/) const
{
  const_cast<ImplAAFIdentification*>(this)->setInitialized();
}

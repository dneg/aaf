
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/






#ifndef __ImplAAFIdentification_h__
#include "ImplAAFIdentification.h"
#endif

#include "AAFTypes.h"
#include "AAFResult.h"

#include "AAFPropertyIDs.h"

#include <assert.h>


ImplAAFIdentification::~ImplAAFIdentification ()
{}

ImplAAFIdentification::ImplAAFIdentification():
_companyName(         PID_Identification_CompanyName,          "Company Name"),
_productName(         PID_Identification_ProductName,          "Product Name"),
// _productVersion(   PID_Identification_ProductVersion,       "Product Version")
_productVersionString(PID_Identification_ProductVersionString, "Product Version String"),
// _productId(        PID_Identification_ProductID,            "Product ID"),
_date(                PID_Identification_Date,                 "Date"),
// _toolKitVersion(   PID_Identification_ToolkitVersion,       "Toolkit Version"),
_platform(            PID_Identification_Platform,             "Platform")
// _generation(       PID_Identification_GenerationAUID,       "Generation AUID")
{
  // Insert the properties into the persistent property set.
  //
  _persistentProperties.put(   _companyName.address());
  _persistentProperties.put(   _productName.address());
  // _persistentProperties.put(_productVersion.address());
  _persistentProperties.put(   _productVersionString.address());
  // _persistentProperties.put(_productId.address());
  _persistentProperties.put(   _date.address());
  // _persistentProperties.put(_toolKitVersion.address());
  _persistentProperties.put(   _platform.address());
  // _persistentProperties.put(_generation.address());

  
#if defined(_WIN32)
  _platform = L"Win32";
#elif defined(macintosh) || defined(_MAC)
  _platform = L"MacOS";
#else
  _platform = L"Unknown";
#endif

}

ImplAAFIdentification::ImplAAFIdentification(
                                     const wchar_t* companyName,
                                     const wchar_t* productName,
                                     const aafProductVersion_t* productVersion,
                                     const wchar_t* productVersionString,
                                     // const AUID* productId,
                                     const aafTimeStamp_t date,
                                     const aafProductVersion_t* toolKitVersion,
                                     const wchar_t* platform
                                     // const AUID* generation
                                     ):
_companyName(         PID_Identification_CompanyName,          "Company Name"),
_productName(         PID_Identification_ProductName,          "Product Name"),
// _productVersion(   PID_Identification_ProductVersion,       "Product Version")
_productVersionString(PID_Identification_ProductVersionString, "Product Version String"),
// _productId(        PID_Identification_ProductID,            "Product ID"),
_date(                PID_Identification_Date,                 "Date"),
// _toolKitVersion(   PID_Identification_ToolkitVersion,       "Toolkit Version"),
_platform(            PID_Identification_Platform,             "Platform")
// _generation(       PID_Identification_GenerationAUID,       "Generation AUID")
{
  // Insert the properties into the persistent property set.
  //
  _persistentProperties.put(   _companyName.address());
  _persistentProperties.put(   _productName.address());
  // _persistentProperties.put(_productVersion.address());
  _persistentProperties.put(   _productVersionString.address());
  // _persistentProperties.put(_productId.address());
  _persistentProperties.put(   _date.address());
  // _persistentProperties.put(_toolKitVersion.address());
  _persistentProperties.put(   _platform.address());
  // _persistentProperties.put(_generation.address());

  // Initialize members.
  //
  _companyName = companyName;
  _productName = productName;
  // _productVersion.???;
  _productVersionString = productVersionString;
  // _productId.???;
  _date = date;
  // _toolKitVersion.???;
  _platform = platform;
  // _generation.???;

};


extern "C" const aafClassID_t CLSID_AAFIdentification;

OMDEFINE_STORABLE(ImplAAFIdentification, CLSID_AAFIdentification);


AAFRESULT STDMETHODCALLTYPE
ImplAAFIdentification::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFIdentification::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFIdentification, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetCompanyName (aafWChar *  pName,
										   aafInt32 bufSize)
{
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
    ImplAAFIdentification::GetCompanyNameBufLen (aafInt32 *  pLen)
{
  if (! pLen)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pLen = _companyName.size();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductName (aafWChar *  pProductName,
										   aafInt32 bufSize)
{
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
    ImplAAFIdentification::GetProductNameBufLen (aafInt32 *  pLen)
{
  if (! pLen)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pLen = _productName.size();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductVersion (aafProductVersion_t * pV)
{
  if (! pV)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductVersionString (aafWChar *  pVS,
													aafInt32 bufSize)
{
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
    ImplAAFIdentification::GetProductVersionStringBufLen (aafInt32 *  pLen)
{
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
  if (! pPID)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetDate (aafTimeStamp_t * pTS)
{
  if (! pTS)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetRefImplVersion (aafProductVersion_t * pV)
{
  if (! pV)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetPlatform (aafWChar *  pPlatform,
										aafInt32 bufSize)
{
  bool stat;
  if (! pPlatform)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  stat = _platform.copyToBuffer(pPlatform, bufSize);
  if (! stat)
	{
	  return AAFRESULT_SMALLBUF;
	}
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetPlatformBufLen (aafInt32 *  pLen)
{
  if (! pLen)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pLen = _platform.size();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetGeneration (aafUID_t *  pGen)
{
  if (! pGen)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetCompanyName (aafWChar *  pName)
{
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  _companyName = pName;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductName (aafWChar *  pName)
{
  if (! pName)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  _productName = pName;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductVersion (aafProductVersion_t * pV)
{
  if (! pV)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductVersionString (aafWChar * pVS)
{
  if (! pVS)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  _productVersionString = pVS;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductID (aafUID_t * pPID)
{
  if (! pPID)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}


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





#ifndef __ImplAAFIdentification_h__
#include "ImplAAFIdentification.h"
#endif

#include "AAFTypes.h"
#include "AAFResult.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>


ImplAAFIdentification::~ImplAAFIdentification ()
{}

ImplAAFIdentification::ImplAAFIdentification():
_companyName(         PID_Identification_CompanyName,          "CompanyName"),
_productName(         PID_Identification_ProductName,          "ProductName"),
_productVersion(      PID_Identification_ProductVersion,       "ProductVersion"),
_productVersionString(PID_Identification_ProductVersionString, "ProductVersionString"),
_productId(			  PID_Identification_ProductID,            "ProductID"),
_date(                PID_Identification_Date,                 "Date"),
_toolkitVersion(      PID_Identification_ToolkitVersion,       "ToolkitVersion"),
_platform(            PID_Identification_Platform,             "Platform")
// _generation(       PID_Identification_GenerationAUID,       "GenerationAUID")
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
                                     const aafUID_t* productId,
                                     const aafTimeStamp_t date,
                                     const aafProductVersion_t* toolKitVersion,
                                     const wchar_t* platform
                                     // const AUID* generation
                                     ):
_companyName(         PID_Identification_CompanyName,          "Company Name"),
_productName(         PID_Identification_ProductName,          "Product Name"),
_productVersion(      PID_Identification_ProductVersion,       "Product Version"),
_productVersionString(PID_Identification_ProductVersionString, "Product Version String"),
_productId(			  PID_Identification_ProductID,            "Product ID"),
_date(                PID_Identification_Date,                 "Date"),
_toolkitVersion(      PID_Identification_ToolkitVersion,       "Toolkit Version"),
_platform(            PID_Identification_Platform,             "Platform")
// _generation(       PID_Identification_GenerationAUID,       "Generation AUID")
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
  // _persistentProperties.put(_generation.address());

  // Initialize members.
  //
  _companyName = companyName;
  _productName = productName;
	_productVersion = *productVersion;
  _productVersionString = productVersionString;
  _productId = *productId;
  _date = date;
	_toolkitVersion = *toolKitVersion;
  _platform = platform;
  // _generation.???;

};


OMDEFINE_STORABLE(ImplAAFIdentification, AUID_AAFIdentification);


AAFRESULT STDMETHODCALLTYPE
ImplAAFIdentification::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetCompanyName (aafWChar *  pName,
										   aafUInt32 bufSize)
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
    ImplAAFIdentification::GetCompanyNameBufLen (aafUInt32 *  pLen)
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
										   aafUInt32 bufSize)
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
    ImplAAFIdentification::GetProductNameBufLen (aafUInt32 *  pLen)
{
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

  *pPID = _productId;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetDate (aafTimeStamp_t * pTS)
{
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
    ImplAAFIdentification::SetProductVersion (aafProductVersion_t* productVersion)
{
	
	if (! productVersion)
	{
		return AAFRESULT_NULL_PARAM;
	}

	_productVersion = *productVersion;

	return AAFRESULT_SUCCESS;
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
  _productId = *pPID;
  return AAFRESULT_SUCCESS;
}

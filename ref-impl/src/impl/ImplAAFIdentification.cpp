
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

#ifndef __AAFIdentification_h__
#include "AAFIdentification.h"
#endif

#include <assert.h>


ImplAAFIdentification::~ImplAAFIdentification ()
{}

// tjb - this is temporary and should be removed
ImplAAFIdentification::ImplAAFIdentification():
_companyName(         PID_IDENTIFICATION_COMPANYNAME,          "companyName"),
_productName(         PID_IDENTIFICATION_PRODUCTNAME,          "productName"),
// _productVersion(   PID_IDENTIFICATION_PRODUCTVERSION,       "productVersion")
_productVersionString(PID_IDENTIFICATION_PRODUCTVERSIONSTRING, "productVersionString"),
// _productId(        PID_IDENTIFICATION_PRODUCTID,            "productId"),
_date(                PID_IDENTIFICATION_DATE,                 "date"),
// _toolKitVersion(   PID_IDENTIFICATION_TOOLKITVERSION,       "toolkitVersion"),
_platform(            PID_IDENTIFICATION_PLATFORM,             "platform")
// _generation(       PID_IDENTIFICATION_GENERATION,           "generation")
{}

ImplAAFIdentification::ImplAAFIdentification(
                                     const char* companyName,
                                     const char* productName,
                                     const aafProductVersion_t* productVersion,
                                     const char* productVersionString,
                                     // const AUID* productId,
                                     const aafTimeStamp_t date,
                                     const aafProductVersion_t* toolKitVersion,
                                     const char* platform
                                     // const AUID* generation
                                     ):
_companyName(         PID_IDENTIFICATION_COMPANYNAME,          "companyName"),
_productName(         PID_IDENTIFICATION_PRODUCTNAME,          "productName"),
// _productVersion(   PID_IDENTIFICATION_PRODUCTVERSION,       "productVersion")
_productVersionString(PID_IDENTIFICATION_PRODUCTVERSIONSTRING, "productVersionString"),
// _productId(        PID_IDENTIFICATION_PRODUCTID,            "productId"),
_date(                PID_IDENTIFICATION_DATE,                 "date"),
// _toolKitVersion(   PID_IDENTIFICATION_TOOLKITVERSION,       "toolkitVersion"),
_platform(            PID_IDENTIFICATION_PLATFORM,             "platform")
// _generation(       PID_IDENTIFICATION_GENERATION,           "generation")
{
  TRACE("AAFIdentification::AAFIdentification");
  PRECONDITION("Valid companyName",          validString(companyName));
  PRECONDITION("Valid productName",          validString(productName));
  PRECONDITION("Valid productVersionString", validString(productVersionString));
  PRECONDITION("Valid platform",             validString(platform));

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

int ImplAAFIdentification::classId(void) const
{
  return CLSID_AAFIDENTIFICATION;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetCompanyName (aafString_t *  /*pCompanyName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductName (aafString_t *  /*pProductName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductVersionString (aafString_t *  /*pProductVersionString*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetProductID (aafUID_t *  /*pProductID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetDate (aafTimeStamp_t *  /*pTimestamp*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetToolkitVersion (aafProductVersion_t *  /*pToolkitVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetPlatform (aafString_t *  /*pPlatform*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::GetGeneration (aafUID_t *  /*pGeneration*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetCompanyName (aafString_t *  /*pCompanyName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductName (aafString_t *  /*pProductName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductVersion (aafProductVersion_t *  /*pProductVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductVersionString (aafString_t *  /*pProductVersionString*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFIdentification::SetProductID (aafUID_t *  /*pProductID*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

	


//@doc
//@class    AAFIdentification | Implementation class for AAFIdentification
#ifndef __ImplAAFIdentification_h__
#define __ImplAAFIdentification_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/







#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMProperty.h"
#include "OMTypes.h"

const int PID_IDENTIFICATION_COMPANYNAME          = 0;
const int PID_IDENTIFICATION_PRODUCTNAME          = 1;
//const int PID_IDENTIFICATION_PRODUCTVERSION       = 2;
const int PID_IDENTIFICATION_PRODUCTVERSIONSTRING = 2;
//const int PID_IDENTIFICATION_PRODUCTID            = 4;
const int PID_IDENTIFICATION_DATE                 = 3;
//const int PID_IDENTIFICATION_TOOLKITVERSION       = 6;
const int PID_IDENTIFICATION_PLATFORM             = 4;
//const int PID_IDENTIFICATION_GENERATION           = 8;

class ImplAAFIdentification : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ~ImplAAFIdentification ();
  
  ImplAAFIdentification();

  ImplAAFIdentification(
    const char* companyName,
    const char* productName,
    const aafProductVersion_t* productVersion,
    const char* productVersionString,
    // const AUID* productId,
    const aafTimeStamp_t date,
    const aafProductVersion_t* toolKitVersion,
    const char* platform
    // const AUID* generation
    );

  OMDECLARE_STORABLE(AAFIdentification);

  //****************
  // GetCompanyName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCompanyName
        (aafString_t *  pCompanyName);  //@parm [out,retval] The Company Name


  //****************
  // GetProductName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductName
        (aafString_t *  pProductName);  //@parm [out,retval] The Product Name


  //****************
  // GetProductVersionString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductVersionString
        (aafString_t *  pProductVersionString);  //@parm [out,retval] The Product Version String


  //****************
  // GetProductID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductID
        (aafUID_t *  pProductID);  //@parm [out,retval] The Product ID


  //****************
  // GetDate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDate
        (aafTimeStamp_t *  pTimestamp);  //@parm [out,retval] The date-time stamp


  //****************
  // GetToolkitVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetToolkitVersion
        (aafProductVersion_t *  pToolkitVersion);  //@parm [out,retval] The Toolkit Version


  //****************
  // GetPlatform()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPlatform
        (aafString_t *  pPlatform);  //@parm [out,retval] The Platform


  //****************
  // GetGeneration()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGeneration
        (aafUID_t *  pGeneration);  //@parm [out,retval] The unique generation


  //****************
  // SetCompanyName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCompanyName
        (aafString_t *  pCompanyName);  //@parm [in,ref] The Company Name


  //****************
  // SetProductName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductName
        (aafString_t *  pProductName);  //@parm [in,ref] The Product Name


  //****************
  // SetProductVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductVersion
        (aafProductVersion_t *  pProductVersion);  //@parm [in,ref] The Product Version


  //****************
  // SetProductVersionString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductVersionString
        (aafString_t *  pProductVersionString);  //@parm [in,ref] The Product Version String


  //****************
  // SetProductID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductID
        (aafUID_t *  pProductID);  //@parm [in,ref] The Product ID

	
public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFIdentificationTest.cpp.
  static AAFRESULT test();
private:
  OMStringProperty                             _companyName;
  OMStringProperty                             _productName;
  // StructuredProperty<aafProductVersion_t> _productVersion;
  OMStringProperty                             _productVersionString;
  // FixedSizeProperty<AUID>                 _productId;
  OMFixedSizeProperty<aafTimeStamp_t>          _date;
  // StructuredProperty<aafProductVersion_t> _toolkitVersion;
  OMStringProperty                             _platform;
  // FixedSizeProperty<AUID>                 _generation;
};

#endif // ! __ImplAAFIdentification_h__


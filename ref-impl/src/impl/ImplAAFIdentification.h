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

#include "AAFPropertyIDs.h"

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
    const wchar_t* companyName,
    const wchar_t* productName,
    const aafProductVersion_t* productVersion,
    const wchar_t* productVersionString,
    // const AUID* productId,
    const aafTimeStamp_t date,
    const aafProductVersion_t* toolKitVersion,
    const wchar_t* platform
    // const AUID* generation
    );

  OMDECLARE_STORABLE(ImplAAFIdentification)


  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // GetCompanyName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCompanyName
	    (// @parm [out, size_is(bufSize), string] buffer into which name is written
         wchar_t *  pName,

		 // @parm [in] The size of the pName buffer
		 aafInt32  bufSize);


  //****************
  // GetCompanyNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCompanyNameBufLen
		// @parm [out] required buffer length
        (aafInt32 *  pLen);


  //****************
  // SetCompanyName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCompanyName
		// @parm [in, string] The Company Name
        (wchar_t *  pName);


  //****************
  // GetProductName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductName
	    (// @parm [out, size_is(bufSize), string] buffer into which name is written
         wchar_t *  pName,

		 // @parm [in] The size of the pName buffer
		 aafInt32  bufSize);


  //****************
  // GetProductNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductNameBufLen
		// @parm [out] Returned length
        (aafInt32 *  pLen);


  //****************
  // SetProductName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductName
		// @parm [in, string] The Product Name
        (wchar_t *  pName);


  //****************
  // GetProductVersionString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductVersionString
	    (// @parm [out, size_is(bufSize), string] buffer into which the string is written
         wchar_t *  pVersionString,

		 // @parm [in] The size of the pVersionString buffer
		 aafInt32  bufSize);


  //****************
  // GetProductVersionStringBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductVersionStringBufLen
		// @parm [out] required buffer length
        (aafInt32 *  pLen);


  //****************
  // SetProductVersionString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductVersionString
		// @parm [in, string] The Product Version String
        (wchar_t *  pVersionString);


  //****************
  // GetProductVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductVersion
		// @parm [out] The Product Version
        (aafProductVersion_t *  pVersion);


  //****************
  // SetProductVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductVersion
		// @parm [in] The Product Version
        (aafProductVersion_t *  pVersion);


  //****************
  // GetProductID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductID
		// @parm [out] The Product ID
        (aafUID_t *  pProductID);


  //****************
  // SetProductID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductID
		// @parm [in] The Product ID
        (aafUID_t *  pProductID);


  //****************
  // GetDate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDate
		// @parm [out] The date-time stamp
        (aafTimeStamp_t *  pTimestamp);


  //****************
  // GetRefImplVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRefImplVersion
		// @parm [out] The Reference Implementation Version
        (aafProductVersion_t *  pVersion);


  //****************
  // GetPlatform()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPlatform
	    (// @parm [out, size_is(bufSize), string] The Platform
         wchar_t *  pPlatform,

		 // @parm [in] The size of the pPlatform buffer
		 aafInt32  bufSize);


  //****************
  // GetPlatformBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPlatformBufLen
		// @parm [out] Returned length
        (aafInt32 *  pLen);


  //****************
  // GetGeneration()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGeneration
		// @parm [out] The unique generation
        (aafUID_t *  pGeneration);

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFIdentificationTest.cpp.
  static AAFRESULT test();

  // Return this objects stored object class.
  virtual AAFRESULT STDMETHODCALLTYPE
	GetObjectClass(aafUID_t * pClass);

private:
  OMWideStringProperty                             _companyName;
  OMWideStringProperty                             _productName;
  // StructuredProperty<aafProductVersion_t> _productVersion;
  OMWideStringProperty                             _productVersionString;
  // FixedSizeProperty<AUID>                 _productId;
  OMFixedSizeProperty<aafTimeStamp_t>          _date;
  // StructuredProperty<aafProductVersion_t> _toolkitVersion;
  OMWideStringProperty                             _platform;
  // FixedSizeProperty<AUID>                 _generation;
};

#endif // ! __ImplAAFIdentification_h__


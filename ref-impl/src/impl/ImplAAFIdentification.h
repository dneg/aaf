//@doc
//@class    AAFIdentification | Implementation class for AAFIdentification
#ifndef __ImplAAFIdentification_h__
#define __ImplAAFIdentification_h__

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


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMWideStringProperty.h"

class ImplAAFIdentification : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ~ImplAAFIdentification ();
  
  ImplAAFIdentification();

  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize (aafCharacter_constptr companyName,
				aafCharacter_constptr productName,
				aafCharacter_constptr productVersionString,
				aafUID_constref productID);


  //****************
  // GetCompanyName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCompanyName
	    (// @parm [out, size_is(bufSize), string] buffer into which name is written
         wchar_t *  pName,

		 // @parm [in] The size of the pName buffer
		 aafUInt32  bufSize);


  //****************
  // GetCompanyNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCompanyNameBufLen
		// @parm [out] required buffer length
        (aafUInt32 *  pLen);


  //****************
  // SetCompanyName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCompanyName
		// @parm [in, string] The Company Name
        (const aafCharacter * pName);


  //****************
  // GetProductName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductName
	    (// @parm [out, size_is(bufSize), string] buffer into which name is written
         wchar_t *  pName,

		 // @parm [in] The size of the pName buffer
		 aafUInt32  bufSize);


  //****************
  // GetProductNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductNameBufLen
		// @parm [out] Returned length
        (aafUInt32 *  pLen);


  //****************
  // SetProductName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductName
		// @parm [in, string] The Product Name
        (const aafCharacter * pName);


  //****************
  // GetProductVersionString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductVersionString
	    (// @parm [out, size_is(bufSize), string] buffer into which the string is written
         wchar_t *  pVersionString,

		 // @parm [in] The size of the pVersionString buffer
		 aafUInt32  bufSize);


  //****************
  // GetProductVersionStringBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetProductVersionStringBufLen
		// @parm [out] required buffer length
        (aafUInt32 *  pLen);


  //****************
  // SetProductVersionString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetProductVersionString
		// @parm [in, string] The Product Version String
        (const aafCharacter * pVersionString);


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
        (aafProductVersion_constref version);


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
        (const aafUID_t & productID);


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
		 aafUInt32  bufSize);


  //****************
  // GetPlatformBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPlatformBufLen
		// @parm [out] Returned length
        (aafUInt32 *  pLen);


  //****************
  // GetGeneration()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGenerationID
		// @parm [out] The unique generation
        (aafUID_t *  pGeneration);

  // OM callbacks
  virtual void onCopy(void* clientContext) const;

private:
  OMWideStringProperty                      _companyName;
  OMWideStringProperty						_productName;
  OMFixedSizeProperty<aafProductVersion_t>	_productVersion;
  OMWideStringProperty                      _productVersionString;
  OMFixedSizeProperty<aafUID_t>             _productId;
  OMFixedSizeProperty<aafTimeStamp_t>       _date;
  OMFixedSizeProperty<aafProductVersion_t>	_toolkitVersion;
  OMWideStringProperty                      _platform;
  OMFixedSizeProperty<aafUID_t>             _generation;
};

#endif // ! __ImplAAFIdentification_h__


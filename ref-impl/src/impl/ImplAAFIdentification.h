//@doc
//@class    AAFIdentification | Implementation class for AAFIdentification
#ifndef __ImplAAFIdentification_h__
#define __ImplAAFIdentification_h__

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






#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif



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
    const aafUID_t* productId,
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
        (wchar_t *  pName);


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
        (wchar_t *  pName);


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
    GetGeneration
		// @parm [out] The unique generation
        (aafUID_t *  pGeneration);

private:
  OMWideStringProperty                      _companyName;
  OMWideStringProperty						_productName;
  OMFixedSizeProperty<aafProductVersion_t>	_productVersion;
  OMWideStringProperty                      _productVersionString;
  OMFixedSizeProperty<aafUID_t>             _productId;
  OMFixedSizeProperty<aafTimeStamp_t>        _date;
  OMFixedSizeProperty<aafProductVersion_t>	_toolkitVersion;
  OMWideStringProperty                        _platform;
  // FixedSizeProperty<AUID>                 _generation;
};

#endif // ! __ImplAAFIdentification_h__


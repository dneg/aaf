//@doc
//@class    AAFIdentification | Implementation class for AAFIdentification
#ifndef __CAAFIdentification_h__
#define __CAAFIdentification_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif





#ifndef __CAAFObject_h__
#include "CAAFObject.h"
#endif


class CAAFIdentification
  : public IAAFIdentification,
    public CAAFObject
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFIdentification (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFIdentification ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated IAAFIdentification-supporting
  // object.  This method must be called after allocation, and before
  // any other method can be called.
  //
  // This method fills in the mandatory properties of the object.  The
  // values of the following required properties are set based on the
  // information given in arguments to this method:
  // - companyName
  // - productname
  // - productVersionString
  // - productID
  // 
  // The following mandatory properties will be filled in
  // automatically by the reference implementation:
  // - date
  // - generationAUID
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - companyName is a valid pointer
  // - productName is a valid pointer
  // - productVersionString is a valid pointer
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  // AAFRESULT_NULL_PARAM
  //   - companyName, productName, or productVersionString is NULL.
  //
  STDMETHOD (Initialize) (
    // company name string
    /*[in]*/ aafCharacter_constptr  companyName,

    // product name string
    /*[in]*/ aafCharacter_constptr  productName,

    // product version string
    /*[in]*/ aafCharacter_constptr  productVersionString,

    // product identification
    /*[in]*/ aafUID_constref  productID);


  //***********************************************************
  //
  // GetCompanyName()
  //
  // Gets the Company Name string property.
  // 
  // Writes the CompanyName property, with a trailing null
  // character, into the pCompanyName buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the CompanyName property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetCompanyNameBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pCompanyName.
  // 
  // Succeeds if:
  // - The pCompanyName pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   CompanyName.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCompanyName arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold CompanyName.
  //
  STDMETHOD (GetCompanyName) (
    // buffer into which CompanyName is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pCompanyName,

    // size of *pCompanyName buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetCompanyNameBufLen()
  //
  // Returns size of buffer (in bytes) required for GetCompanyName().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetCompanyNameBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // GetProductName()
  //
  // Gets the Product Name string property.
  // 
  // Writes the ProductName property, with a trailing null
  // character, into the pProductName buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the ProductName property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetProductNameBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pProductName.
  // 
  // Succeeds if:
  // - The pProductName pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   ProductName.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pProductName arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold ProductName.
  //
  STDMETHOD (GetProductName) (
    // buffer into which ProductName is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pProductName,

    // size of *pProductName buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetProductNameBufLen()
  //
  // Returns size of buffer (in bytes) required for GetProductName().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetProductNameBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // GetProductVersionString()
  //
  // Gets the Product Version string property.
  // 
  // Writes the ProductVersionString property, with a trailing null
  // character, into the pProductVersionString buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the ProductVersionString property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetProductVersionStringBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pProductVersionString.
  // 
  // Succeeds if:
  // - The pProductVersionString pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   ProductVersionString.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pProductVersionString arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold ProductVersionString.
  //
  STDMETHOD (GetProductVersionString) (
    // buffer into which ProductVersionString is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pProductVersionString,

    // size of *pProductVersionString buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetProductVersionStringBufLen()
  //
  // Returns size of buffer (in bytes) required for GetProductVersionString().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetProductVersionStringBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // GetProductVersion()
  //
  // Gets the Product Version property associated with this
  // identification object and places it into *pVersion.
  // 
  // Succeeds if all of the following are true:
  // - the pVersion pointer is valid.
  // 
  // If this method fails, nothing will be written to *pVersion.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pVersion arg is NULL.
  //
  STDMETHOD (GetProductVersion) (
    // The Product Version 
    /*[out]*/ aafProductVersion_t *  pVersion);


  //***********************************************************
  //
  // SetProductVersion()
  //
  // Set the Product Version property to the value specified in
  // pVersion.  A copy is made of the data so the caller retains
  // ownership of the *pVersion struct and is responsible for
  // de-allocating it.
  // 
  // If this method fails the Product Version property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  STDMETHOD (SetProductVersion) (
    // The Product Version 
    /*[in]*/ aafProductVersion_constref  version);


  //***********************************************************
  //
  // GetProductID()
  //
  // Obtains the Product ID, which is the identification number
  // assigned to the application and vendor of the application which
  // attached this object to the AAF file.  This ID is written into
  // the caller-allocated aafUID_t specified by the pProductID
  // argument.
  // 
  // Succeeds if all of the following are true:
  // - the pProductID pointer is valid.
  // 
  // If this method fails nothing will be written to *pProductID.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pProductID arg is NULL.
  //
  STDMETHOD (GetProductID) (
    // The Product ID 
    /*[out]*/ aafUID_t *  pProductID);


  //***********************************************************
  //
  // GetDate()
  //
  // Writes the Date-time Stamp property into the caller-allocated
  // aafTimeStamp_t specified by the pTimeStamp argument.  The
  // date-time stamp recorded in this object corresponds to the time
  // that this file was created or modified upon the occasion that
  // this object was added to the file.
  //
  // Note: This is a read-only property.
  // 
  // Succeeds if all of the following are true:
  // - the pTimeStamp pointer is valid.
  // 
  // If this method fails nothing will be written to *pTimeStamp.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pTimeStamp arg is NULL.
  //
  STDMETHOD (GetDate) (
    // The date-time stamp 
    /*[out]*/ aafTimeStamp_t *  pTimestamp);


  //***********************************************************
  //
  // GetRefImplVersion()
  //
  // Obtains the version of the Reference Implementation which
  // created this identification object and writes it into the
  // caller-allocated aafProductVersion_t specified by the pVersion
  // argument.
  //
  // Note: This is a read-only property.
  // 
  // Succeeds if all of the following are true:
  // - the pVersion pointer is valid.
  // 
  // If this method fails nothing will be written to *pVersion.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pVersion arg is NULL.
  //
  STDMETHOD (GetRefImplVersion) (
    // The Reference Implementation Version 
    /*[out]*/ aafProductVersion_t *  pVersion);


  //***********************************************************
  //
  // GetPlatform()
  //
  // Gets the Platform string property.
  //
  // This information is provided only to allow diagnostic printing of
  // platform information to be read by humans.  The format of the
  // strings is not guaranteed to remain the same for a given
  // platform.  Having said that, the possible values currently
  // returned are:
  // - "Win32" for Intel/Win32 platforms
  // - "MacOS" for MacOS platforms
  // - "Unknown" for unknown platforms
  // 
  // Writes the Platform property, with a trailing null
  // character, into the pPlatform buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the Platform property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetPlatformBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pPlatform.
  // 
  // Succeeds if:
  // - The pPlatform pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   Platform.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pPlatform arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold Platform.
  //
  STDMETHOD (GetPlatform) (
    // buffer into which Platform is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pPlatform,

    // size of *pPlatform buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetPlatformBufLen()
  //
  // Returns size of buffer (in bytes) required for GetPlatform().
  // 
  // Succeeds if:
  // - The pBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pBufSize arg is NULL.
  //
  STDMETHOD (GetPlatformBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // GetGenerationID()
  //
  // Obtains the generation of this AAF file, which was generated at
  // the time this identification object was created.  If a file was
  // opened for modification by many applications in its lifetime,
  // then there will be multiple Identification objects.  This is
  // written into the caller-allocated aafUID_t specified by the
  // pGeneration argument.
  // 
  // Note: This is a read-only property.
  // 
  // Succeeds if all of the following are true:
  // - the pGeneration pointer is valid.
  // 
  // If this method fails nothing will be written to *pGeneration.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pGeneration arg is NULL.
  //
  STDMETHOD (GetGenerationID) (
    // The unique generation 
    /*[out]*/ aafUID_t *  pGeneration);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFIdentification_h__



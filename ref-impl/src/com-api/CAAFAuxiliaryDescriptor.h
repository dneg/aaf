//@doc
//@class    AAFAuxiliaryDescriptor | Implementation class for AAFAuxiliaryDescriptor
#ifndef __CAAFAuxiliaryDescriptor_h__
#define __CAAFAuxiliaryDescriptor_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif








#ifndef __CAAFPhysicalDescriptor_h__
#include "CAAFPhysicalDescriptor.h"
#endif


class CAAFAuxiliaryDescriptor
  : public IAAFAuxiliaryDescriptor,
    public CAAFPhysicalDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFAuxiliaryDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFAuxiliaryDescriptor ();

public:



  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated,
  /// IAAFAuxiliaryDescriptor object.  This method must be called
  /// after allocation, and before any other method can be called.
  ///
  /// Return codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_ALREADY_INITIALIZED
  ///   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize)
     ();


  //***********************************************************
  //
  // SetMimeType()
  //
  // Sets The registered mime type of the data per RFC 2046 and RFC 2048.
  ///
  /// Set the MimeType property to the value specified in
  /// pMimeType.  A copy is made of the data so the caller
  /// retains ownership of the *pMimeType buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pMimeType pointer is valid.
  /// 
  /// If this method fails the MimeType property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pMimeType arg is NULL.
  //
  STDMETHOD (SetMimeType) (
    // buffer from which MimeType is to be read 
    /*[in, string]*/ aafCharacter_constptr  pMimeType);


  //***********************************************************
  //
  // GetMimeType()
  //
  // Gets The registered mime type of the data per RFC 2046 and RFC 2048.
  /// 
  /// Writes the MimeType property, with a trailing null
  /// character, into the pMimeType buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the MimeType property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetMimeTypeBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pMimeType.
  /// 
  /// Succeeds if:
  /// - The pMimeType pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   MimeType.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pMimeType arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold MimeType.
  //
  STDMETHOD (GetMimeType) (
    // buffer into which MimeType is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pMimeType,

    // size of *pMimeType buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetMimeTypeBufLen()
  //
  // Returns size of buffer (in bytes) required for GetMimeType().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetMimeTypeBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);

  //***********************************************************
  //
  // SetCharSet()
  //
  // Sets The registered character set per RFC 2048.
  ///
  /// Set the CharSet property to the value specified in
  /// pCharSet.  A copy is made of the data so the caller
  /// retains ownership of the *pCharSet buffer and is
  /// responsible for de-allocating it.  There is no pre-set limit to
  /// the length of the name, other than available system memory or
  /// disk space.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pCharSet pointer is valid.
  /// 
  /// If this method fails the CharSet property will not be
  /// changed.
  /// 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCharSet arg is NULL.
  //
  STDMETHOD (SetCharSet) (
    // buffer from which CharSet is to be read 
    /*[in, string]*/ aafCharacter_constptr  pCharSet);


  //***********************************************************
  //
  // GetCharSet()
  //
  // Gets The registered character set per RFC 2048.
  /// 
  /// Writes the CharSet property, with a trailing null
  /// character, into the pCharSet buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the CharSet property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetCharSetBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pCharSet.
  /// 
  /// Succeeds if:
  /// - The pCharSet pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   CharSet.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCharSet arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold CharSet.
  //
  STDMETHOD (GetCharSet) (
    // buffer into which CharSet is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pCharSet,

    // size of *pCharSet buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetCharSetBufLen()
  //
  // Returns size of buffer (in bytes) required for GetCharSet().
  /// 
  /// Succeeds if:
  /// - The pBufSize pointer is valid.
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
  STDMETHOD (GetCharSetBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


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

#endif // ! __CAAFAuxiliaryDescriptor_h__



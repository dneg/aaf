//@doc
//@class    AAFFileEncoding | Implementation class for AAFFileEncoding
#ifndef __CAAFFileEncoding_h__
#define __CAAFFileEncoding_h__

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
// The Original Code of this file is Copyright 1998-2012, licensor of the
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





#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFFileEncoding
  : public IAAFFileEncoding,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFFileEncoding (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFFileEncoding ();

public:


  //***********************************************************
  //
  // GetName()
  //
  // Gets the name of this file encoding.
  /// 
  /// Writes the Name property, with a trailing null
  /// character, into the pName buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the Name property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetNameBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pName.
  /// 
  /// Succeeds if:
  /// - The pName pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   Name.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pName arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold Name.
  //
  STDMETHOD (GetName) (
    // buffer into which Name is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pName,

    // size of *pName buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetNameBufLen()
  //
  // Returns size of buffer (in bytes) required for GetName().
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
  STDMETHOD (GetNameBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // GetDescription()
  //
  // Gets the description of this file encoding.
  /// 
  /// Writes the Description property, with a trailing null
  /// character, into the pDescription buffer.  The
  /// buffer is allocated by the caller.  The size of the buffer is
  /// given by bufSize.  If the Description property has not yet
  /// been set, a zero-length string will be written (that is,
  /// only the trailing null character). 
  /// 
  /// Caller may call GetDescriptionBufLen() to determine the
  /// required buffer size.
  /// 
  /// If this method fails nothing will be written to
  /// *pDescription.
  /// 
  /// Succeeds if:
  /// - The pDescription pointer is valid.
  /// - bufSize indicates that the buffer is large enough to hold
  ///   Description.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pDescription arg is NULL.
  ///
  /// AAFRESULT_SMALL_BUF
  ///   - bufSize indicates that the allocated buffer is not large
  ///     enough to hold Description.
  //
  STDMETHOD (GetDescription) (
    // buffer into which Description is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pDescription,

    // size of *pDescription buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetDescriptionBufLen()
  //
  // Returns size of buffer (in bytes) required for GetDescription().
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
  STDMETHOD (GetDescriptionBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);



  //***********************************************************
  //
  // GetFileKind()
  //
  // This method returns the unique ID associated with this file encoding.
  ///
  /// Succeeds if all of the following are true:
  /// - the object is initialized.
  /// - the pFileKind pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pFileKind.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - the object is not initialized.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileKind argument is NULL.
  //
  STDMETHOD (GetFileKind) (
    // The unique file encoding ID 
    /*[out]*/ aafUID_t *  pFileKind);


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

#endif // ! __CAAFFileEncoding_h__



#ifndef __ImplAAFFileEncoding_h__
#define __ImplAAFFileEncoding_h__


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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"

#include "ImplAAFRoot.h"


class OMStoredObjectFactory;


class ImplAAFFileEncoding : public ImplAAFRoot
{
public:
  //********************************
  // AAF SDK exported methods start
  //********************************


  //***********************************************************
  //
  // GetName()
  //
  // Gets the name of this file encoding.
  //
  // Arguments:
  // pNameBuf       [out] buffer into which name is to be written
  // nameBufSize    [in] size of pName buffer in bytes
  //
  // Writes the Name property, with a trailing null
  // character, into the pName buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by nameBufSize.  If the Name property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetNameBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pName.
  // 
  // Succeeds if:
  // - This FileEncoding object is initialized.
  // - The pName pointer is valid.
  // - nameBufSize indicates that the buffer is large enough to hold
  //   Name.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - The object is not initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - pName arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - nameBufSize indicates that the allocated buffer is not large
  //     enough to hold Name.
  //
  virtual AAFRESULT STDMETHODCALLTYPE GetName(
      aafCharacter* pName,
      aafUInt32     bufSize) const;


  //***********************************************************
  //
  // GetNameBufLen()
  //
  // Returns size of buffer (in bytes) required for GetName().
  // 
  // Arguments:
  // pNameBufSize   [out] size of required buffer, in bytes
  //
  // Succeeds if:
  // - This FileEncoding object is initialized.
  // - The pNameBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - The object is not initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - pNameBufSize arg is NULL.
  //
  virtual AAFRESULT STDMETHODCALLTYPE GetNameBufLen(
      aafUInt32*    pNameBufSize) const;


  //***********************************************************
  //
  // GetDescription()
  //
  // Gets the description of this file encoding.
  // 
  // Arguments:
  // pDescriptionBuf    [out] buffer into which description is to be written
  // descriptionBufSize [in] size of pDescriptionBuf buffer in bytes
  //
  // Writes the Description property, with a trailing null
  // character, into the pDescriptionBuf buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by descriptionBufSize.  If the Description property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetDescriptionBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pDescriptionBuf.
  // 
  // Succeeds if:
  // - This FileEncoding object is initialized.
  // - The pDescriptionBuf pointer is valid.
  // - descriptionBufSize indicates that the buffer is large enough to hold
  //   Description.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - The object is not initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - pDescriptionBuf arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - descriptionBufSize indicates that the allocated buffer is not large
  //     enough to hold Description.
  //
  virtual AAFRESULT STDMETHODCALLTYPE GetDescription(
      aafCharacter* pDescriptionBuf,
      aafUInt32     descriptionBufSize) const;


  //***********************************************************
  //
  // GetDescriptionBufLen()
  //
  // Returns size of buffer (in bytes) required for GetDescription().
  //
  // Arguments: 
  // pDescriptionBufSize    [out] size of required buffer, in bytes
  //
  // Succeeds if:
  // - This FileEncoding object is initialized.
  // - The pDescriptionBufSize pointer is valid.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - The object is not initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - pDescriptionBufSize arg is NULL.
  //
  virtual AAFRESULT STDMETHODCALLTYPE GetDescriptionBufLen(
      aafUInt32*    pDescriptionBufSize) const;


  //***********************************************************
  //
  // GetFileKind()
  //
  // This method returns the unique ID associated with this file encoding.
  //
  // Arguments: 
  // pFileKind      [out] file encoding UID
  //
  // Succeeds if all of the following are true:
  // - the object is initialized.
  // - the pFileKind pointer is valid.
  // 
  // If this method fails nothing will be written to *pFileKind.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - the object is not initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - pFileKind argument is NULL.
  //
  virtual AAFRESULT STDMETHODCALLTYPE GetFileKind(
      aafUID_t*     pFileKind) const;


  //********************************
  // AAF SDK exported methods end
  //********************************


public:
  //****************
  // Constructor
  //
  ImplAAFFileEncoding ();


  //****************
  // Initialize()
  //
  // Initializes this FileEncoding.
  //
  // Arguments: 
  // pEncoding  [in] pointer to OM representation of file
  //            encoding to be represented by this object
  //
  // Succeeds if all of the following are true:
  // - the object is not already initialized.
  // - the pEncoding pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - the object is already initialized.
  //
  // AAFRESULT_NULL_PARAM
  //   - pEncoding argument is NULL.
  //
  virtual AAFRESULT STDMETHODCALLTYPE Initialize(
      OMStoredObjectFactory*    pEncoding);


protected:
  //****************
  // Destructor
  //
  virtual ~ImplAAFFileEncoding ();


protected:
  //****************
  // Stores OM representation of file encoding
  // represented by this object.
  OMStoredObjectFactory*    _p_encoding;
};

#endif // ! __ImplAAFFileEncoding_h__



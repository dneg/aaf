//@doc
//@class    AAFTapeDescriptor | Implementation class for AAFTapeDescriptor
#ifndef __CAAFTapeDescriptor_h__
#define __CAAFTapeDescriptor_h__

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

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif





#ifndef __CAAFEssenceDescriptor_h__
#include "CAAFEssenceDescriptor.h"
#endif


class CAAFTapeDescriptor
  : public IAAFTapeDescriptor,
    public CAAFEssenceDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTapeDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTapeDescriptor ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  // IAAFTapeDescriptor-supporting object.  This method must be called
  // after allocation, and before any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
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
  STDMETHOD (Initialize)
     ();



  //***********************************************************
  //
  // SetTapeManufacturer()
  //
  // Sets the Tape Manufacturer string property.
  //
  // Set the TapeManufacturer property to the value specified in
  // pTapeManufacturer.  A copy is made of the data so the caller
  // retains ownership of the *pTapeManufacturer buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pTapeManufacturer pointer is valid.
  // 
  // If this method fails the TapeManufacturer property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTapeManufacturer arg is NULL.
  //
  STDMETHOD (SetTapeManufacturer) (
    // buffer from which TapeManufacturer is to be read 
    /*[in, string]*/ aafCharacter_constptr  pTapeManufacturer);


  //***********************************************************
  //
  // GetTapeManufacturer()
  //
  // Gets the Tape Manufacturer string property.
  // 
  // Writes the TapeManufacturer property, with a trailing null
  // character, into the pTapeManufacturer buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the TapeManufacturer property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetTapeManufacturerBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pTapeManufacturer.
  // 
  // Succeeds if:
  // - The pTapeManufacturer pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   TapeManufacturer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTapeManufacturer arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold TapeManufacturer.
  //
  STDMETHOD (GetTapeManufacturer) (
    // buffer into which TapeManufacturer is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pTapeManufacturer,

    // size of *pTapeManufacturer buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetTapeManufacturerBufLen()
  //
  // Returns size of buffer (in bytes) required for GetTapeManufacturer().
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
  STDMETHOD (GetTapeManufacturerBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // SetTapeModel()
  //
  // Sets the Tape Model string property.
  //
  // Set the TapeModel property to the value specified in
  // pTapeModel.  A copy is made of the data so the caller
  // retains ownership of the *pTapeModel buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pTapeModel pointer is valid.
  // 
  // If this method fails the TapeModel property will not be
  // changed.
  // 
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTapeModel arg is NULL.
  //
  STDMETHOD (SetTapeModel) (
    // buffer from which TapeModel is to be read 
    /*[in, string]*/ aafCharacter_constptr  pTapeModel);


  //***********************************************************
  //
  // GetTapeModel()
  //
  // Gets the Tape Model string property.
  // 
  // Writes the TapeModel property, with a trailing null
  // character, into the pTapeModel buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the TapeModel property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetTapeModelBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pTapeModel.
  // 
  // Succeeds if:
  // - The pTapeModel pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   TapeModel.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pTapeModel arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold TapeModel.
  //
  STDMETHOD (GetTapeModel) (
    // buffer into which TapeModel is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pTapeModel,

    // size of *pTapeModel buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetTapeModelBufLen()
  //
  // Returns size of buffer (in bytes) required for GetTapeModel().
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
  STDMETHOD (GetTapeModelBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // SetTapeFormFactor()
  //
  // Sets the form factor [case size] of the tape.
  // 
  // This method succeeds if all of the following are true:
  // - formFactor represents a valid format.
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
  // AAFRESULT_BAD_TYPE
  //	- formFactor is invalid.
  //
  STDMETHOD (SetTapeFormFactor) (
    // ex: kVHSVideoTape, kDATCartridge  
    /*[in]*/ aafTapeCaseType_t  formFactor);


  //***********************************************************
  //
  // GetTapeFormFactor()
  //
  // Gets the form factor [case size] of the tape.
  // 
  // This method succeeds if all of the following are true:
  // - the pFormFactor pointer is valid.
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
  //   - pFormFactor arg is NULL.
  //
  STDMETHOD (GetTapeFormFactor) (
    // ex: kVHSVideoTape, kDATCartridge 
    /*[out]*/ aafTapeCaseType_t *  formFactor);


  //***********************************************************
  //
  // SetSignalType()
  //
  // Sets the signal standard recorded on the tape.
  // 
  // This method succeeds if all of the following are true:
  //	videoSignal represents a valid video signal type.
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
  // AAFRESULT_BAD_TYPE
  //	- videoSignal is invalid.
  //
  STDMETHOD (SetSignalType) (
    // ex: kNTSCSignal 
    /*[in]*/ aafVideoSignalType_t  videoSignal);


  //***********************************************************
  //
  // GetSignalType()
  //
  // Gets the signal standard recorded on the tape.
  // 
  // This method succeeds if all of the following are true:
  // - the pVideoSignal pointer is valid.
  //
  // If this method fails nothing will be written to *pVideoSignal.
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
  //   - pVideoSignal arg is NULL.
  //
  STDMETHOD (GetSignalType) (
    // ex: kNTSCSignal 
    /*[out]*/ aafVideoSignalType_t *  pVideoSignal);


  //***********************************************************
  //
  // SetTapeFormat()
  //
  // Sets the recording method of the tape.
  // 
  // This method succeeds if all of the following are true:
  // - tapeFormat represents a valid tape format type.
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
  // AAFRESULT_BAD_TYPE
  //	- tapeFormat is invalid.
  //
  STDMETHOD (SetTapeFormat) (
    // ex: kBetacamFormat, kBetacamSPFormat 
    /*[in]*/ aafTapeFormatType_t  tapeFormat);


  //***********************************************************
  //
  // GetTapeFormat()
  //
  // Gets the recording method of the tape.
  //
  // Succeeds if all of the following are true:
  // - the pTapeFormat pointer is valid.
  // 
  // This method succeeds if all of the following are true:
  // - the pTapeFormat pointer is valid.
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
  //   - pTapeFormat arg is NULL.
  //
  STDMETHOD (GetTapeFormat) (
    // ex: kBetacamFormat, kBetacamSPFormat 
    /*[out]*/ aafTapeFormatType_t *  pTapeFormat);


  //***********************************************************
  //
  // SetTapeLength()
  //
  // Sets the length of the tape in minutes.
  // 
  // This method succeeds if all of the following are true:
  // - tapeLength is a positive number.
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
  // AAFRESULT_BAD_LENGTH
  //	- tapeLength is negative.
  //
  STDMETHOD (SetTapeLength) (
    // The length of the tape in minutes. 
    /*[in]*/ aafUInt32  tapeLength);


  //***********************************************************
  //
  // GetTapeLength()
  //
  // Gets the length of the tape in minutes.
  //
  // This method succeeds if all of the following are true:
  // - the pTapeLength pointer is valid.
  // 
  // If this method fails nothing will be written to *pTapeLength.
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
  //   - pTapeLength arg is NULL.
  //
  STDMETHOD (GetTapeLength) (
    // The length of the tape in minutes. 
    /*[out]*/ aafUInt32 *  pTapeLength);

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

#endif // ! __CAAFTapeDescriptor_h__



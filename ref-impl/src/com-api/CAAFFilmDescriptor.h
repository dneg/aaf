//@doc
//@class    AAFFilmDescriptor | Implementation class for AAFFilmDescriptor
#ifndef __CAAFFilmDescriptor_h__
#define __CAAFFilmDescriptor_h__

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


class CAAFFilmDescriptor
  : public IAAFFilmDescriptor,
    public CAAFEssenceDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFFilmDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFFilmDescriptor ();

public:


  //***********************************************************
  //
  // SetFilmManufacturer()
  //
  // Sets the Film Manufacturer string property.
  //
  // Set the FilmManufacturer property to the value specified in
  // pFilmManufacturer.  A copy is made of the data so the caller
  // retains ownership of the *pFilmManufacturer buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pFilmManufacturer pointer is valid.
  // 
  // If this method fails the FilmManufacturer property will not be
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
  //   - pFilmManufacturer arg is NULL.
  //
  STDMETHOD (SetFilmManufacturer) (
    // buffer from which FilmManufacturer is to be read 
    /*[in, string]*/ aafCharacter_constptr  pFilmManufacturer);


  //***********************************************************
  //
  // GetFilmManufacturer()
  //
  // Gets the Film Manufacturer string property.
  // 
  // Writes the FilmManufacturer property, with a trailing null
  // character, into the pFilmManufacturer buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the FilmManufacturer property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetFilmManufacturerBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pFilmManufacturer.
  // 
  // Succeeds if:
  // - The pFilmManufacturer pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   FilmManufacturer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pFilmManufacturer arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold FilmManufacturer.
  //
  STDMETHOD (GetFilmManufacturer) (
    // buffer into which FilmManufacturer is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pFilmManufacturer,

    // size of *pFilmManufacturer buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetFilmManufacturerBufLen()
  //
  // Returns size of buffer (in bytes) required for GetFilmManufacturer().
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
  STDMETHOD (GetFilmManufacturerBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // SetFilmModel()
  //
  // Sets the Film Model string property.
  //
  // Set the FilmModel property to the value specified in
  // pFilmModel.  A copy is made of the data so the caller
  // retains ownership of the *pFilmModel buffer and is
  // responsible for de-allocating it.  There is no pre-set limit to
  // the length of the name, other than available system memory or
  // disk space.
  // 
  // Succeeds if all of the following are true:
  // - the pFilmModel pointer is valid.
  // 
  // If this method fails the FilmModel property will not be
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
  //   - pFilmModel arg is NULL.
  //
  STDMETHOD (SetFilmModel) (
    // buffer from which FilmModel is to be read 
    /*[in, string]*/ aafCharacter_constptr  pFilmModel);


  //***********************************************************
  //
  // GetFilmModel()
  //
  // Gets the Film Model string property.
  // 
  // Writes the FilmModel property, with a trailing null
  // character, into the pFilmModel buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.  If the FilmModel property has not yet
  // been set, a zero-length string will be written (that is,
  // only the trailing null character). 
  // 
  // Caller may call GetFilmModelBufLen() to determine the
  // required buffer size.
  // 
  // If this method fails nothing will be written to
  // *pFilmModel.
  // 
  // Succeeds if:
  // - The pFilmModel pointer is valid.
  // - bufSize indicates that the buffer is large enough to hold
  //   FilmModel.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pFilmModel arg is NULL.
  //
  // AAFRESULT_SMALL_BUF
  //   - bufSize indicates that the allocated buffer is not large
  //     enough to hold FilmModel.
  //
  STDMETHOD (GetFilmModel) (
    // buffer into which FilmModel is to be written
    /*[out, string, size_is(bufSize)]*/ aafCharacter *  pFilmModel,

    // size of *pFilmModel buffer in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetFilmModelBufLen()
  //
  // Returns size of buffer (in bytes) required for GetFilmModel().
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
  STDMETHOD (GetFilmModelBufLen) (
    // size of required buffer, in bytes 
    /*[out]*/ aafUInt32 *  pBufSize);


  //***********************************************************
  //
  // GetFilmFormat()
  //
  // Gets the film format.
  //
  // Valid values include:
  //  - kFt35MM
  //  - kFt16MM
  //  - kFt8MM
  //  - kFt65MM
  // 
  // This method succeeds if all of the following are true:
  // - the pFilmFormat pointer is valid.
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
  //   - pFilmFormat arg is NULL.
  //
  STDMETHOD (GetFilmFormat) (
    // pointer to the filmFormat 
    /*[out]*/ aafFilmType_t*  pFilmFormat);


  //***********************************************************
  //
  // GetFrameRate()
  //
  // Get the frame rate of the film.
  // 
  // This method succeeds if all of the following are true:
  // - the pRate pointer is valid.
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
  //   - pRate parameter is NULL.
  //
  STDMETHOD (GetFrameRate) (
    // Frame Rate 
    /*[out]*/ aafUInt32*  pRate);


  //***********************************************************
  //
  // GetPerfPerFrame()
  //
  // Get the number of perforations per frame.
  // 
  // This method succeeds if all of the following are true:
  // - the pPerfPerFrame pointer is valid.
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
  //   - pPerfPerFrame parameter is NULL.
  //
  STDMETHOD (GetPerfPerFrame) (
    // Perforations per frame 
    /*[out]*/ aafUInt8*  pPerfPerFrame);


  //***********************************************************
  //
  // GetFilmAspectRatio()
  //
  // Get the image aspect ratio.
  // 
  // This method succeeds if all of the following are true:
  // - the pAspectRatio pointer is valid.
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
  //   - pAspectRatio arg is NULL.
  //
  STDMETHOD (GetFilmAspectRatio) (
    // Film Aspect Ratio 
    /*[out]*/ aafRational_t*  pAspectRatio);


  //***********************************************************
  //
  // SetFilmFormat()
  //
  // Sets the film format of the film.
  // Valid values include:
  //  - kFt35MM
  //  - kFt16MM
  //  - kFt8MM
  //  - kFt65MM
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
  //	- filmFormat is invalid.
  //
  STDMETHOD (SetFilmFormat) (
    // Film Format 
    /*[in]*/ aafFilmType_t  filmFormat);


  //***********************************************************
  //
  // SetFrameRate()
  //
  // Set the frame rate of the film.
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
  STDMETHOD (SetFrameRate) (
    // Frame Rate 
    /*[in]*/ aafUInt32  rate);


  //***********************************************************
  //
  // SetPerfPerFrame()
  //
  // Set the number of perforations per frame.
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
  STDMETHOD (SetPerfPerFrame) (
    // Perforations per frame 
    /*[in]*/ aafUInt8  perfPerFrame);


  //***********************************************************
  //
  // SetFilmAspectRatio()
  //
  // Set the image aspect ratio.
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
  STDMETHOD (SetFilmAspectRatio) (
    // Film Aspect Ratio 
    /*[in]*/ aafRational_t  aspectRatio);

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

#endif // ! __CAAFFilmDescriptor_h__



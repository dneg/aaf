//@doc
//@class    AAFTIFFDescriptor | Implementation class for AAFTIFFDescriptor
#ifndef __CAAFTIFFDescriptor_h__
#define __CAAFTIFFDescriptor_h__

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


  



#ifndef __CAAFFileDescriptor_h__
#include "CAAFFileDescriptor.h"
#endif


class CAAFTIFFDescriptor
  : public IAAFTIFFDescriptor,
    public CAAFFileDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTIFFDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTIFFDescriptor ();

public:


  //***********************************************************
  //
  // SetIsUniform()
  //
  // Set to TRUE if essence data has the same number of rows per strip
  // throughout.
  //
  // Always succeeds.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetIsUniform) (
    // Sets the IsUniform flag value 
    /*[in]*/ aafBoolean_t  IsUniform);


  //***********************************************************
  //
  // GetIsUniform()
  //
  // Places TRUE into *pIsUniform if the data has the same number of
  // rows per strip throughout.
  //
  // Succeeds if all of the following are true:
  // - the pIsUniform pointer is valid.
  // 
  // If this method fails nothing will be written to *pIsUniform.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pIsUniform arg is NULL.
  //
  STDMETHOD (GetIsUniform) (
    // Gets the IsUniform flag value 
    /*[out]*/ aafBoolean_t *  pIsUniform);


  //***********************************************************
  //
  // SetIsContiguous()
  //
  // Set to TRUE if essence data is stored in contiguous bytes.
  // 
  // Always succeeds.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetIsContiguous) (
    // is data stores in contiguous bytes 
    /*[in]*/ aafBoolean_t  IsContiguous);


  //***********************************************************
  //
  // GetIsContiguous()
  //
  // Places TRUE into *pIsContiguous if essence data is stored in
  // contiguous bytes.
  //
  // Succeeds if all of the following are true:
  // - the pIsContiguous pointer is valid.
  // 
  // If this method fails nothing will be written to *pIsContiguous.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pIsContiguous arg is NULL.
  //
  STDMETHOD (GetIsContiguous) (
    // is this data stored in contiguous bytes 
    /*[out]*/ aafBoolean_t *  pIsContiguous);


  //***********************************************************
  //
  // SetLeadingLines()
  //
  // Sets the number of leading lines in the TIFF image file.
  // 
  // This method succeeds if all of the following are true:
  // - LeadingLines is  equal or greater than 0(zero).
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
  STDMETHOD (SetLeadingLines) (
    // Number of leading lines to be thrown away Optional 
    /*[in]*/ aafInt32  LeadingLines);


  //***********************************************************
  //
  // GetLeadingLines()
  //
  // Gets the leading lines of the TIFF image.
  //
  // This method succeeds if all of the following are true:
  // - the pLeadingLines pointer is valid.
  // 
  // If this method fails nothing will be written to *pLeadingLines.
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
  //   - pLeadingLines arg is NULL.
  //
  STDMETHOD (GetLeadingLines) (
    // Address to store the number of leading lines 
    /*[out]*/ aafInt32 *  pLeadingLines);


  //***********************************************************
  //
  // SetTrailingLines()
  //
  // Sets the number of trailing lines in the TIFF image file
  // 
  // This method succeeds if all of the following are true:
  // - TrailingLines is  equal or greater than 0(zero).
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
  STDMETHOD (SetTrailingLines) (
    // Number of trailing lines to be thrown away Optional. 
    /*[in]*/ aafInt32  TrailingLines);


  //***********************************************************
  //
  // GetTrailingLines()
  //
  // Gets the trailing lines of the TIFF image.
  //
  // This method succeeds if all of the following are true:
  // - the pTrailingLines pointer is valid.
  // 
  // If this method fails nothing will be written to *pTrailingLines.
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
  //   - pTrailingLines arg is NULL.
  //
  STDMETHOD (GetTrailingLines) (
    // Address to store the number of trailing lines. 
    /*[out]*/ aafInt32 *  pTrailingLines);


  //***********************************************************
  //
  // SetJPEGTableID()
  //
  // Sets the JPEG table code for  the TIFF image file
  // 
  // This method always succeeds .
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetJPEGTableID) (
    // Registered JPEG table code or JT_NULL. 
    /*[in]*/ aafJPEGTableID_t  JPEGTableID);


  //***********************************************************
  //
  // GetJPEGTableID()
  //
  // Gets the JPEG table code of the TIFF image.
  //
  // This method succeeds if all of the following are true:
  // - the pJPEGTableID pointer is valid.
  // 
  // If this method fails nothing will be written to *pJPEGTableID.
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
  //   - pJPEGTableID arg is NULL.
  //
  STDMETHOD (GetJPEGTableID) (
    // Address to store the nJPEG table code 
    /*[out]*/ aafJPEGTableID_t *  pJPEGTableID);


  //***********************************************************
  //
  // GetSummary()
  //
  // Gets a copy of the TIFF IFD file information without the media.
  // 
  // Succeeds if all of the following are true:
  // - pSummary is a valid pointer.
  // - The size of the buffer is large enough to hold the TIFF IFD
  //   file information. 
  // 
  // If this method fails pSummary will not be changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSummary arg is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - The buffer is too small to hold the WAVE file information.
  //
  STDMETHOD (GetSummary) (
    // Size of preallocated buffer
    /*[in]*/ aafUInt32  size,

    // Preallocated buffer to hold the TIFF IFD file information
    /*[out, size_is(size)]*/ aafDataValue_t  pSummary);


  //***********************************************************
  //
  // GetSummaryBufferSize()
  //
  // Returns the size of the buffer required for the GetSummary()
  // method.  The value is placed into the location specified by pSize.
  //
  // Succeeds if all of the following are true:
  // - the pSize pointer is valid.
  //
  // If this method fails nothing will be written to *pSize.
  //
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSize arg is NULL.
  //
  STDMETHOD (GetSummaryBufferSize) (
    // required buffer size 
    /*[out]*/ aafUInt32 *  pSize);


  //***********************************************************
  //
  // SetSummary()
  //
  // Sets the TIFF IFD file information.
  // 
  // Succeeds if all of the following are true:
  // - pSummary is a valid pointer
  // 
  // If this method fails the summary property will not be
  // changed.
  // 
  // This method will return the following codes:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSummary arg is NULL.
  //
  STDMETHOD (SetSummary) (
    // Size of preallocated buffer
    /*[in]*/ aafUInt32  size,

    // buffer containing value
    /*[in, size_is(size)]*/ aafDataValue_t  pSummary);

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

#endif // ! __CAAFTIFFDescriptor_h__



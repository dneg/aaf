//@doc
//@class    AAFFileDescriptor | Implementation class for AAFFileDescriptor
#ifndef __CAAFFileDescriptor_h__
#define __CAAFFileDescriptor_h__

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





#ifndef __CAAFEssenceDescriptor_h__
#include "CAAFEssenceDescriptor.h"
#endif


class CAAFFileDescriptor
  : public IAAFFileDescriptor,
    public CAAFEssenceDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFFileDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFFileDescriptor ();

public:


  //***********************************************************
  //
  // SetLength()
  //
  // // 
  // Sets the length of the essence in samples [not edit units].
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
  STDMETHOD (SetLength) (
    // length of the essence in samples 
    /*[in]*/ aafLength_t  length);


  //***********************************************************
  //
  // GetLength()
  //
  // // Gets the length of the essence in samples [not edit units].
  //
  // Succeeds if all of the following are true:
  // - the pLength pointer is valid.
  // 
  // If this method fails nothing will be written to *pLength.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLength arg is NULL.
  //
  STDMETHOD (GetLength) (
    // returns length of the essence in samples 
    /*[out]*/ aafLength_t *  pLength);


  //***********************************************************
  //
  // SetCodecDef()
  //
  // // Set to the exact codec which was used as a hint.
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
  STDMETHOD (SetCodecDef) (
    // Which codec was used 
    /*[in]*/ IAAFCodecDef * codecDef);


  //***********************************************************
  //
  // GetCodecDef()
  //
  // // Get to the exact codec which was used as a hint.
  //
  // Succeeds if all of the following are true:
  // - the pCodecDef pointer is valid.
  // 
  // If this method fails nothing will be written to *pCodecDef.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pCodecDef arg is NULL.
  //
  STDMETHOD (GetCodecDef) (
    // Which codec was used 
    /*[out]*/ IAAFCodecDef ** pCodecDef);


  //***********************************************************
  //
  // SetSampleRate()
  //
  // Sets sample rate of the essence as opposed to the edit rate.
  // 
  // If this method fails the sample rate property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetSampleRate) (
    // sample rate of the essence 
    /*[in]*/ aafRational_constref  rate);


  //***********************************************************
  //
  // GetSampleRate()
  //
  // Gets sample rate of the essence as opposed to the edit rate. and
  // writes it into the *pRate argument.
  //
  // Succeeds if all of the following are true:
  // - the pRate pointer is valid.
  // 
  // If this method fails nothing will be written to *pRate.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pRate arg is NULL.
  //
  STDMETHOD (GetSampleRate) (
    // sample rate of the essence 
    /*[out]*/ aafRational_t*  pRate);


  //***********************************************************
  //
  // SetContainerFormat()
  //
  // Identifies the file format.  The container format is an optional
  // property.
  // 
  // If this method fails the container format property will not be
  // changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetContainerFormat) (
    // file format 
    /*[in]*/ IAAFContainerDef * format);


  //***********************************************************
  //
  // GetContainerFormat()
  //
  // // Optional. Identifies the file format.
  //
  // Succeeds if all of the following are true:
  // - the pFormat pointer is valid.
  // 
  // If this method fails nothing will be written to *pFormat.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pFormat arg is NULL.
  //
  STDMETHOD (GetContainerFormat) (
    // Identifies the file format 
    /*[out]*/ IAAFContainerDef ** pFormat);

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

#endif // ! __CAAFFileDescriptor_h__



//@doc
//@class    AAFWAVEDescriptor | Implementation class for AAFWAVEDescriptor
#ifndef __CAAFWAVEDescriptor_h__
#define __CAAFWAVEDescriptor_h__

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





#ifndef __CAAFFileDescriptor_h__
#include "CAAFFileDescriptor.h"
#endif


class CAAFWAVEDescriptor
  : public IAAFWAVEDescriptor,
    public CAAFFileDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFWAVEDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFWAVEDescriptor ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  // IAAFWaveDescriptor-supporting object.  This method must be
  // called after allocation, and before any other method can be
  // called.
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
  // GetSummary()
  //
  // Gets a copy of the WAVE file information without the media.
  // 
  // Succeeds if all of the following are true:
  // - pSummary is a valid pointer.
  // - The size of the buffer is large enough to hold the WAVE file
  //   information.
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

    // Preallocated buffer to hold the WAVE file information
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
  // Sets the WAVE file information.
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

#endif // ! __CAAFWAVEDescriptor_h__



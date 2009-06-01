//@doc
//@class    AAFDataEssenceDescriptor | Implementation class for AAFDataEssenceDescriptor
#ifndef __CAAFDataEssenceDescriptor_h__
#define __CAAFDataEssenceDescriptor_h__

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







#ifndef __CAAFFileDescriptor_h__
#include "CAAFFileDescriptor.h"
#endif


class CAAFDataEssenceDescriptor
  : public IAAFDataEssenceDescriptor,
    public CAAFFileDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFDataEssenceDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFDataEssenceDescriptor ();

public:




  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty IAAFDataEssenceDescriptor-supporting
  /// object.  This method must be called after allocation, and before
  /// any other method can be called.
  ///
  /// Succeeds if:
  /// - Initialize() has not yet been called on this object.
  ///
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
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
  // SetDataEssenceCoding()
  //
  // Sets the DataEssenceCoding property
  /// This property is optional.  
  /// 
  /// If this method fails the Data Definition property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetDataEssenceCoding) (
    // Optional. 
    /*[in]*/ aafUID_constref  dataEssenceCoding);


  //***********************************************************
  //
  // GetDataEssenceCoding()
  //
  // Gets the DataEssenceCoding property
  /// information of the video essence data.  This property is
  /// optional.
  ///
  /// Succeeds if all of the following are true:
  /// - the pDataEssenceCoding pointer is valid.
  /// 
  /// If this method fails nothing will be written to *pDataEssenceCoding.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pDataEssenceCoding arg is NULL.
  //
  STDMETHOD (GetDataEssenceCoding) (
    // Optional. 
    /*[out]*/ aafUID_t *  pDataEssenceCoding);


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

#endif // ! __CAAFDataEssenceDescriptor_h__



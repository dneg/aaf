//@doc
//@class    AAFMultipleDescriptor | Implementation class for AAFMultipleDescriptor
#ifndef __CAAFMultipleDescriptor_h__
#define __CAAFMultipleDescriptor_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#ifndef __AAF_h__
#include "AAF.h"
#endif








#ifndef __CAAFFileDescriptor_h__
#include "CAAFFileDescriptor.h"
#endif


class CAAFMultipleDescriptor
  : public IAAFMultipleDescriptor,
    public CAAFFileDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFMultipleDescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFMultipleDescriptor ();

public:



  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty
  /// IAAFMultipleDescriptor-supporting object.  This method must be
  /// called after allocation, and before any other method can be
  /// called.
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
  // CountFileDescriptors()
  //
  // Return the number of FileDescriptors attached to this essence
  /// descriptor.  The number of FileDescriptors may be zero if the essence is
  /// in the current file.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pCount pointer is valid.
  /// 
  /// If this method fails nothing is written to *pCount.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pCount is null.
  //
  STDMETHOD (CountFileDescriptors) (
    // Returns the number of FileDescriptors 
    /*[out]*/ aafUInt32 *  pResult);


  //***********************************************************
  //
  // AppendFileDescriptor()
  //
  // Append another FileDescriptor to this essence descriptor.  Use this
  /// function to add a FileDescriptor to the end of the interleave pattern.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pFileDescriptor pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileDescriptor is null.
  //
  STDMETHOD (AppendFileDescriptor) (
    // FileDescriptor to append 
    /*[in]*/ IAAFFileDescriptor * pFileDescriptor);


  //***********************************************************
  //
  // PrependFileDescriptor()
  //
  // Prepend another FileDescriptor to this essence descriptor.  Use this
  /// function to add a FileDescriptor to to the beginning of the interleave pattern.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pFileDescriptor pointer is valid.
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileDescriptor is null.
  //
  STDMETHOD (PrependFileDescriptor) (
    // FileDescriptor to append 
    /*[in]*/ IAAFFileDescriptor * pFileDescriptor);


  //***********************************************************
  //
  // InsertFileDescriptorAt()
  //
  // Inserts the given FileDescriptor at the given index.  FileDescriptors already
  /// existing at the given and higher indices will be moved to the
  /// next higher index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pFileDescriptor pointer is valid.
  /// - index is less than or equal to the value returned by
  ///   CountFileDescriptors().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileDescriptor is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than the value returned by
  ///     CountFileDescriptors().
  //
  STDMETHOD (InsertFileDescriptorAt) (
    // index at which FileDescriptor is to be inserted
    /*[in]*/ aafUInt32  index,

    // FileDescriptor to append
    /*[in]*/ IAAFFileDescriptor * pFileDescriptor);


  //***********************************************************
  //
  // GetFileDescriptorAt()
  //
  // Retrieves the FileDescriptor at the given index.
  /// 
  /// Succeeds if all of the following are true:
  /// - the pFileDescriptor pointer is valid.
  /// - index is less than the value returned by CountFileDescriptors().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pFileDescriptor is null.
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountFileDescriptors().
  //
  STDMETHOD (GetFileDescriptorAt) (
    // index of FileDescriptor to retrieve
    /*[in]*/ aafUInt32  index,

    // returned FileDescriptor
    /*[out, retval]*/ IAAFFileDescriptor ** ppFileDescriptor);


  //***********************************************************
  //
  // RemoveFileDescriptorAt()
  //
  // Removes the FileDescriptor at the given index.  FileDescriptors already
  /// existing at indices higher than the given index will be moved to
  /// the next lower index to accommodate.
  /// 
  /// Succeeds if all of the following are true:
  /// - index is less than the value returned by CountFileDescriptors().
  /// 
  /// If this method fails no state will be changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_BADINDEX
  ///   - index is greater than or equal to the value returned by
  ///     CountFileDescriptors().
  //
  STDMETHOD (RemoveFileDescriptorAt) (
    // index of FileDescriptor to remove 
    /*[in]*/ aafUInt32  index);



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

#endif // ! __CAAFMultipleDescriptor_h__



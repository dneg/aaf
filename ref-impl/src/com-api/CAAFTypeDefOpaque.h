//@doc
//@class    AAFTypeDefOpaque | Implementation class for AAFTypeDefOpaque
#ifndef __CAAFTypeDefOpaque_h__
#define __CAAFTypeDefOpaque_h__

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







#ifndef __CAAFTypeDefIndirect_h__
#include "CAAFTypeDefIndirect.h"
#endif


class CAAFTypeDefOpaque
  : public IAAFTypeDefOpaque,
    public CAAFTypeDefIndirect
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefOpaque (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefOpaque ();

public:



  //***********************************************************
  //
  // GetActualTypeID()
  //
  // Return the type id of the  actual data within the opaque property value.
  //
  //
  // Succeeds if all of the following are true:
  // - the pOpaquePropertyValue pointer is valid.
  // - the pActualType pointer is valid.
  //
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
  //   - either pOpaquePropertyValue or pActualTypeID arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - the type of pOpaquePropertyValue was not opaque. 
  //
  //
  STDMETHOD (GetActualTypeID) (
    // indirect property value to read
    /*[in]*/ IAAFPropertyValue * pOpaquePropertyValue,

    // the type id of the actual data
    /*[out]*/ aafUID_t *  pActualTypeID);

  //***********************************************************
  //
  // GetHandle()
  //
  // Used to read and save an unknown property without having to know its contents.
  // This call fills in a handle to a block of data which can be saved in you application, and
  // later written to another file using SetHandle().  Writes the data into the pHandle buffer. 
  // The buffer is allocated by the caller, and the size of the buffer is given by
  // handleSize.
  // 
  // Caller may call GetHandleBufLen() to determine the
  // required buffer size.
  // 
  // Succeeds if all of the following are true:
  // - the pHandle pointer is valid.
  // - handleSize indicates the buffer is large enough to hold the
  //   name.
  // 
  // If this method fails nothing will be written to *pHandle.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pHandle arg is NULL.
  //
  // AAFRESULT_SMALLBUF
  //   - handleSize indicates the buffer is too small to hold the
  //     handle.
  //
  STDMETHOD (GetHandle) (
    // value to get data from.
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // Size of preallocated buffer
    /*[in]*/ aafUInt32  handleSize,

    // Preallocated buffer to hold handle
    /*[out, size_is(handleSize),length_is(*bytesRead)]*/ aafDataBuffer_t  pHandle,

    // Number of actual bytes read
    /*[out]*/ aafUInt32*  bytesRead);


  //***********************************************************
  //
  // GetHandleBufLen()
  //
  // Returns the length of buffer required for the GetHandle()
  // method.  The handle is placed into the location specified by
  // pLen.
  //
  // Succeeds if all of the following are true:
  // - the pLen pointer is valid.
  //
  // If this method fails nothing will be written to *pLen.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pLen arg is NULL.
  //
  STDMETHOD (GetHandleBufLen) (
    // value to set.
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // Pointer to an variable used to return the length
    /*[out]*/ aafUInt32 *  pLen);

  //***********************************************************
  //
  // SetHandle()
  //
  // This call takes a block of unknown data created by a previous call to GetHandle()
  // and saved in you application, and sets an opaque property value.
  //
  // Succeeds if all of the following are true:
  // - the pHandle pointer is valid.
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
  //   - pHandle is null.
  //
  STDMETHOD (SetHandle) (
    // value to write data to.
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // Size of preallocated buffer
    /*[in]*/ aafUInt32  handleSize,

    // buffer containing handle
    /*[in, size_is(handleSize)]*/ aafDataBuffer_t  pHandle);

  //***********************************************************
  //
  // CreateValueFromHandle()
  //
  // This call takes a block of unknown data created by GetHandle()
  // and saved in you application, and creates an opaque property value.
  // Returns the newly-created property value in ppOpaquePropertyValue.
  //
  // Succeeds if all of the following are true:
  // - the pInitData pointer is valid.
  // - the ppOpaquePropertyValue pointer is valid.
  // - initDataSize indicates pInitData is the correct size for 
  //   the actual type.
  // - compile-time struct has had its member offests registered.
  //
  // If this method fails nothing will be written to *ppOpaquePropertyValue.
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
  //   - either pInitData or ppOpaquePropertyValue arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - initDataSize indicates pInitData is of the wrong size.
  //
  STDMETHOD (CreateValueFromHandle) (
    // pointer to buffer containing handle to use
    /*[in, size_is(initDataSize)]*/ aafMemPtr_t  pInitData,

    // size of handle in pInitData
    /*[in]*/ aafUInt32  initDataSize,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppOpaquePropertyValue);




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

#endif // ! __CAAFTypeDefOpaque_h__



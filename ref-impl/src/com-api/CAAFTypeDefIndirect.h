//@doc
//@class    AAFTypeDefIndirect | Implementation class for AAFTypeDefIndirect
#ifndef __CAAFTypeDefIndirect_h__
#define __CAAFTypeDefIndirect_h__

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







#ifndef __CAAFTypeDef_h__
#include "CAAFTypeDef.h"
#endif


class CAAFTypeDefIndirect
  : public IAAFTypeDefIndirect,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefIndirect (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefIndirect ();

public:




  //***********************************************************
  //
  // CreateValueFromActualValue()
  //
  // Creates an indirect property value which contains the actual type.  
  // Returns the newly-created property value in ppIndirectPropertyValue.
  //
  // Succeeds if all of the following are true:
  // - the pActualValue pointer is valid.
  // - the ppIndirectPropertyValue pointer is valid.
  //
  // If this method fails nothing will be written to *ppIndirectPropertyValue.
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
  //   - either pActualValue or ppIndirectPropertyValue arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - initDataSize indicates pInitData is of the wrong size.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for this typedef.
  //
  STDMETHOD (CreateValueFromActualValue) (
    // pointer to actual value
    /*[in]*/ IAAFPropertyValue * pActualValue,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppIndirectPropertyValue);

  //***********************************************************
  //
  // CreateValueFromActualData()
  //
  // Creates an indirect property value which contains data of the 
  // actual type.  Returns the newly-created property value in
  // ppIndirectPropertyValue.
  //
  // Succeeds if all of the following are true:
  // - the pActualType pointer is valid and registered
  // - the pInitData pointer is valid.
  // - the ppIndirectPropertyValue pointer is valid.
  // - initDataSize indicates pInitData is the correct size for 
  //   the actual type.
  // - compile-time struct has had its member offests registered.
  //
  // If this method fails nothing will be written to *ppIndirectPropertyValue.
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
  //   - either pActualType or InitData or ppIndirectPropertyValue arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - initDataSize indicates pInitData is of the wrong size.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for this typedef.
  //
  STDMETHOD (CreateValueFromActualData) (
    // the actual type of the data
    /*[in]*/ IAAFTypeDef * pActualType,

    // pointer to buffer containing data to use
    /*[in, size_is(initDataSize)]*/ aafMemPtr_t  pInitData,

    // size of data in pInitData
    /*[in]*/ aafUInt32  initDataSize,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppIndirectPropertyValue);


  //***********************************************************
  //
  // GetActualValue()
  //
  // Gets a property value corresponding to the actual data. 
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - The pIndirectPropertyValue pointer is valid.
  // - The ppActualPropertyValue pointer is valid.
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
  //   - either pIndirectPropertyValue or ppActualPropertyValue arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - initDataSize indicates pInitData is of the wrong size.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for this typedef.
  //
  STDMETHOD (GetActualValue) (
    // indirect property value to read
    /*[in]*/ IAAFPropertyValue * pIndirectPropertyValue,

    // value that is read
    /*[out]*/ IAAFPropertyValue ** ppActualPropertyValue);
  //)

  //***********************************************************
  //
  // GetActualSize()
  //
  // Return the size of the actual data withing the indirect property value.
  //
  //
  // Succeeds if all of the following are true:
  // - the pIndirectPropertyValue pointer is valid.
  // - the pActualSize pointer is valid.
  //
  // If this method fails nothing will be written to *pActualSize.
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
  //   - either pIndirectPropertyValue or pActualSize arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - the type of pIndirectPropertyValue was not indirect. 
  //
  // AAFRESULT_TYPE_NOT_FOUND
  //   - the actual type cannot be found in the dictionary.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for 
  //    the actual typedef.
  //
  STDMETHOD (GetActualSize) (
    // indirect property value to read
    /*[in]*/ IAAFPropertyValue * pIndirectPropertyValue,

    // actual data size
    /*[out]*/ aafUInt32 *  pActualSize);
  //***********************************************************
  //
  // GetActualType()
  //
  // Return the type definition of the  actual data within the indirect property value.
  //
  //
  // Succeeds if all of the following are true:
  // - the pIndirectPropertyValue pointer is valid.
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
  //   - either pIndirectPropertyValue or pActualType arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - the type of pIndirectPropertyValue was not indirect. 
  //
  // AAFRESULT_TYPE_NOT_FOUND
  //   - the actual type cannot be found in the dictionary.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for 
  //    the actual typedef.
  //
  STDMETHOD (GetActualType) (
    // indirect property value to read
    /*[in]*/ IAAFPropertyValue * pIndirectPropertyValue,

    // the type definition of the actual data
    /*[out]*/ IAAFTypeDef ** pActualType);

  //***********************************************************
  //
  // GetActualData()
  //
  // Copies all the actual data contained in the given indirect property value,
  // interpreted as the actual type for this indirect property value, into the 
  // buffer pointed to by pData.
  //
  // Succeeds if all of the following are true:
  // - the pPropVal pointer is valid.
  // - the pData pointer is valid.
  // - dataSize indicates pData is large enough to hold the data.
  //
  // If this method fails nothing will be written to *ppPropVal.
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
  //   - either pPropVal or pData arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - dataSize indicates pData is too small.
  //
  // AAFRESULT_TYPE_NOT_FOUND
  //   - the actual type cannot be found in the dictionary.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for 
  //    the actual typedef.
  //
  STDMETHOD (GetActualData) (
    // indirect property value to read
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // buffer into which actual data should be written
    /*[out, size_is(dataSize)]*/ aafMemPtr_t  pData,

    // size of pData buffer in bytes
    /*[in]*/ aafUInt32  dataSize);




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

#endif // ! __CAAFTypeDefIndirect_h__



//@doc
//@class    AAFTypeDefRecord | Implementation class for AAFTypeDefRecord
#ifndef __CAAFTypeDefRecord_h__
#define __CAAFTypeDefRecord_h__

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


class CAAFTypeDefRecord
  : public IAAFTypeDefRecord,
    public CAAFTypeDef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefRecord (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefRecord ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to be identified by the given guid, and
  // to contain the given members (types and names).  It is considered
  // an error if multiple members have the same name.  Note that it is
  // only possible to use certain types as member types.  Those
  // permissible types include:
  // - AAFTypeDefInt
  // - AAFTypeDefRecord
  // - AAFTypeDefEnum
  // - AAFTypeDefExtEnum
  // - AAFTypeDefFixedArray
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pID is a valid pointer.
  // - pMemberTypes is a valid pointer.
  // - pMemberNames is a valid pointer.
  // - pTypeName is a valid pointer.
  // - all specified member types are permissible for use in a Record.
  // - all member names are unique.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - This object has already had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pMemberTypes, pMemberNames, or pTypeName arg is NULL.
  //
  // AAFRESULT_BAD_TYPE
  //   - an illegal member type was given.
  //
  // AAFRESULT_DUPLICATE
  //   - a duplicate member name was given.
  //
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in, ref]*/ aafUID_constref  id,

    // array of member types to be represented in this record
    // type
    /*[in, size_is(numMembers)]*/ IAAFTypeDef ** ppMemberTypes,

    // array of member names to be represented in this enumerated
    // type
    /*[in, size_is(numMembers)]*/ aafString_t *  pMemberNames,

    // number of members in pMemberInfo array
    /*[in]*/ aafUInt32  numMembers,

    // friendly name of this type definition
    /*[in]*/ aafCharacter_constptr  pTypeName);


  //***********************************************************
  //
  // GetMemberType()
  //
  // Returns the type definition of the indexed member in this
  // record type.  Index is zero-based, and must be less than the
  // value returned by GetCount().
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - the index exists in this record type def.
  // - The ppTypeDef pointer is valid.
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
  //   - ppTypeDef arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - The given index is out of range for this record type def.
  //
  STDMETHOD (GetMemberType) (
    // zero-based index into members in this record type
    /*[in]*/ aafUInt32  index,

    // type definition of indexed member
    /*[out]*/ IAAFTypeDef ** ppTypeDef);


  //***********************************************************
  //
  // GetMemberName()
  //
  // Writes the human-legible tag associated with the indexed member
  // in this record type.  Index is zero-based, and must be less
  // than the value returned by GetCount().  The name is written,
  // with a trailing null character, into the pName buffer.  The
  // buffer is allocated by the caller.  The size of the buffer is
  // given by bufSize.
  // 
  // Caller may call GetMemberNameBufLen() to determine the required
  // buffer size.
  // 
  // Succeeds if all of the following are true:
  // - the pName pointer is valid.
  // - bufSize indicates the buffer is large enough to hold the name.
  // 
  // If this method fails nothing will be written to
  // *pName.
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
  //   - pName arg is NULL.
  // 
  // AAFRESULT_SMALLBUF
  //   - bufSize indicates the buffer is too small to hold the string.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - the given integer value is not associated with a member of
  //     this type.
  //
  STDMETHOD (GetMemberName) (
    // zero-based index into members in this record type
    /*[in]*/ aafUInt32  index,

    // buffer into which the member name is written
    /*[out, size_is(bufSize), string]*/ aafCharacter *  pName,

    // The size of the pName buffer, in bytes
    /*[in]*/ aafUInt32  bufSize);


  //***********************************************************
  //
  // GetMemberNameBufLen()
  //
  // Returns the length of buffer required for the GetMemberName()
  // method.  Index is zero-based, and must be less than the value
  // returned by GetCount().  The value is placed into the location
  // specified by pLen. The value will include space required for the
  // trailing null character.
  //
  // Succeeds if all of the following are true:
  // - the pLen pointer is valid.
  // - the integer value is associated with a member of this
  //   enumerated type.
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
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pLen arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - the given integer value is not associated with a member of
  //     this type.
  //
  STDMETHOD (GetMemberNameBufLen) (
    // zero-based index into members in this record type
    /*[in]*/ aafUInt32  index,

    // required buffer length, in bytes
    /*[out]*/ aafUInt32 *  pLen);


  //***********************************************************
  //
  // CreateValueFromValues()
  //
  // Creates a property value which contains a record type.  The
  // record members in the property value are initialized to
  // contain the given values, passed in the pMemberValues array.
  // numMembers, which indicates the size of the pMemberValues array,
  // must match the value returned by GetCount().  Returns the
  // newly-created property value in ppPropVal.
  //
  // Succeeds if all of the following are true:
  // - the pMemberValues pointer is valid.
  // - the ppPropVal pointer is valid.
  // - numMembers matches the number of members in this record.
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
  //   - either pMemberValues or ppPropVal arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - numMembers does not match GetCount().
  //
  STDMETHOD (CreateValueFromValues) (
    // array of property values for members of record value which
    // is to be created.
    /*[in, size_is(numMembers)]*/ IAAFPropertyValue ** pMemberValues,

    // size of pMemberValues array.
    /*[in]*/ aafUInt32  numMembers,

    // newly-created property value
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // CreateValueFromStruct()
  //
  // Creates a property value which contains a record type.  The
  // record members in the property value are initialized from data
  // in a struct which is pointed to by pInitData.  Requires that the
  // structure pointed to by pInitData has had its offsets registered
  // with this type.  Returns the newly-created property value in
  // ppPropVal.
  //
  // Succeeds if all of the following are true:
  // - the pInitData pointer is valid.
  // - the ppPropVal pointer is valid.
  // - initDataSize indicates pInitData is the correct size.
  // - compile-time struct has had its member offests registered.
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
  //   - either pInitData or ppPropVal arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - initDataSize indicates pInitData is of the wrong size.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for this typedef.
  //
  STDMETHOD (CreateValueFromStruct) (
    // pointer to compile-time struct containing data to use
    /*[in, size_is(initDataSize)]*/ aafMemPtr_t  pInitData,

    // size of data in pInitData
    /*[in]*/ aafUInt32  initDataSize,

    // newly created property value
    /*[out]*/ IAAFPropertyValue ** ppPropVal);


  //***********************************************************
  //
  // GetValue()
  //
  // Gets a single property value corresponding to the indicated
  // record member.  Places a property value representing the
  // record member identified by the index into ppOutPropval.
  // Index is zero-based, and must be less than the value returned by
  // GetCount().
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - the index exists in this record type def.
  // - The pInPropVal pointer is valid.
  // - The ppOutPropVal pointer is valid.
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
  //   - either pInPropVal or ppOutPropVal arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - The given index is out of range for this record type def.
  //
  STDMETHOD (GetValue) (
    // property value to read
    /*[in]*/ IAAFPropertyValue * pInPropVal,

    // zero-based index into members in this record type
    /*[in]*/ aafUInt32  index,

    // value that is read
    /*[out]*/ IAAFPropertyValue ** ppOutPropVal);


  //***********************************************************
  //
  // GetStruct()
  //
  // Copies all the member data contained in the given property value,
  // interpreted as a record of this type, into the struct pointed
  // to by pData.  Requires that the struct pointed to by pData has
  // had its offsets registered with this type.
  //
  // Succeeds if all of the following are true:
  // - the pPropVal pointer is valid.
  // - the pData pointer is valid.
  // - dataSize indicates pData is large enough to hold the data.
  // - compile-time struct has had its member offests registered.
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
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for this typedef.
  //
  STDMETHOD (GetStruct) (
    // property value to read
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // buffer into which struct data should be written
    /*[out, size_is(dataSize)]*/ aafMemPtr_t  pData,

    // size of pData buffer in bytes
    /*[in]*/ aafUInt32  dataSize);


  //***********************************************************
  //
  // SetValue()
  //
  // Sets the value of the single, indicated record member of the
  // record contained in pPropVal, to the value contained in
  // pMemberPropVal.  Index is zero-based, and must be less than the
  // value returned by GetCount().
  // 
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - the index exists in this record type def.
  // - The pInPropVal pointer is valid.
  // - The ppOutPropVal pointer is valid.
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
  //   - either pInPropVal or ppOutPropVal arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - The given index is out of range for this record type def.
  //
  STDMETHOD (SetValue) (
    // property value to write
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // zero-based index into members in this record type
    /*[in]*/ aafUInt32  index,

    // value to be placed into this record
    /*[in]*/ IAAFPropertyValue * pMemberPropVal);


  //***********************************************************
  //
  // SetStruct()
  //
  // Copies all the member data contained in the struct pointed to by
  // pData into the given property value, interpreting the data as a record of
  // this type.  Requires that the struct pointed to by pData has had
  // its offsets registered with this type.
  //
  // Succeeds if all of the following are true:
  // - the pPropVal pointer is valid.
  // - the pData pointer is valid.
  // - dataSize indicates pData contains the correct amount of data.
  // - compile-time struct has had its member offests registered.
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
  //   - dataSize indicates pData is not the correct size.
  //
  // AAFRESULT_NOT_REGISTERED
  //  - struct offsets have not yet been registered for this typedef.
  //
  STDMETHOD (SetStruct) (
    // property value to write
    /*[in]*/ IAAFPropertyValue * pPropVal,

    // buffer from which struct data should be read
    /*[in, size_is(dataSize)]*/ aafMemPtr_t  pData,

    // size of pData buffer in bytes
    /*[in]*/ aafUInt32  dataSize);


  //***********************************************************
  //
  // GetCount()
  //
  // Returns number of members in this record type.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pCount is a valid pointer.
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
  //   - pCount arg is NULL.
  //
  STDMETHOD (GetCount) (
    // count of members in this record type 
    /*[out]*/ aafUInt32 *  pCount);


  //***********************************************************
  //
  // RegisterMembers()
  //
  // Allows client to register to the reference implementation a
  // runtime C struct to represent objects of this TypeDef.  Offsets
  // for each member in the struct are passed in as an array of
  // integers; size indicates the number of members in the array.
  // The offset is given in bytes from the start address of the
  // struct.  This allows the reference implementation to write
  // property values into compile-time-defined C structs intelligible
  // by the local machine and compiler.
  //
  // Succeeds if:
  // - Initialize() has already been called on this object.
  // - pOffsets is a valid pointer.
  // - numMembers matches the number of members defined for this
  //   record type.
  // - all types of the fields in this record already have their
  //   offsets registered.
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
  //   - pOffsets arg is NULL.
  //
  // AAFRESULT_ILLEGAL_VALUE
  //   - numMembers does not match number of members in this record
  //     type.
  //
  // AAFRESULT_NOT_REGISTERED
  //   - The type of any field in this record has not yet had its
  //     offsets registered.
  //
  // AAFRESULT_DEFAULT_ALREADY_USED
  //   - The default registration for this type has already been used
  //     to persist or unpersist a property containing this type.
  //
  STDMETHOD (RegisterMembers) (
    // array containing offset for each record member
    /*[in]*/ aafUInt32 *  pOffsets,

    // number of members in pOffsets
    /*[in]*/ aafUInt32  numMembers,

    // size of this struct
    /*[in]*/ aafUInt32  structSize);

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

#endif // ! __CAAFTypeDefRecord_h__



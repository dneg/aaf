//@doc
//@class    AAFTypeDefRecord | Implementation class for AAFTypeDefRecord
#ifndef __ImplAAFTypeDefRecord_h__
#define __ImplAAFTypeDefRecord_h__

//=---------------------------------------------------------------------=
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

class ImplAAFPropertyValue;

class ImplEnumAAFPropertyValues;

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMWeakRefVectorProperty.h"

class ImplAAFTypeDefRecord : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefRecord ();

protected:
  virtual ~ImplAAFTypeDefRecord ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in, size_is(numMembers)] array of member types to
		 // be represented in this record type
         ImplAAFTypeDef ** ppMemberTypes,

         // @parm [in, size_is(numMembers)] array of member names to
		 // be represented in this enumerated  type
         aafString_t *  pMemberNames,

         // @parm [in] number of members in pMemberInfo array
         aafUInt32  numMembers,

         // @parm [in] friendly name of this type definition
         const aafCharacter *  pTypeName);


  //****************
  // GetMemberType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMemberType
        (// @parm [in] zero-based index into members in this record
		 // type
         aafUInt32  index,

         // @parm [out] type definition of indexed member
         ImplAAFTypeDef ** ppTypeDef);


  //****************
  // GetMemberName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMemberName
        (// @parm [in] zero-based index into members in this record
		 // type
         aafUInt32  index,

         // @parm [out, size_is(bufSize), string] buffer into which
		 // the member name is written
         aafCharacter *  pName,

         // @parm [in] The size of the pName buffer, in bytes
         aafUInt32  bufSize);


  //****************
  // GetMemberNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMemberNameBufLen
        (// @parm [in] zero-based index into members in this record
		 // type
         aafUInt32  index,

         // @parm [out] required buffer length, in bytes
         aafUInt32 *  pLen);


  //****************
  // CreateValueFromValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromValues
        (// @parm [in, size_is(numMembers)] array of property values
		 // for members of record value which is to be created.
         ImplAAFPropertyValue ** pMemberValues,

         // @parm [in] size of pMemberValues array.
         aafUInt32  numMembers,

         // @parm [out] newly-created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // CreateValueFromStruct()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromStruct
        (// @parm [in, size_is(initDataSize)] pointer to compile-time
		 // struct containing data to use
         aafMemPtr_t pInitData,

         // @parm [in] size of data in pInitData
         aafUInt32  initDataSize,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValue
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in] zero-based index into members in this record
		 // type
         aafUInt32  index,

         // @parm [out] value that is read
         ImplAAFPropertyValue ** ppOutPropVal);


  //****************
  // GetStruct()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStruct
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pPropVal,

         // @parm [out, size_is(dataSize)] buffer into which struct
		 // data should be written
         aafMemPtr_t  pData,

         // @parm [in] size of pData buffer in bytes
         aafUInt32  dataSize);


  //****************
  // SetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetValue
        (// @parm [in] property value to write
         ImplAAFPropertyValue * pPropVal,

         // @parm [in] zero-based index into members in this record
		 // type
         aafUInt32  index,

         // @parm [in] value to be placed into this record
         ImplAAFPropertyValue * pMemberPropVal);


  //****************
  // SetStruct()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStruct
        (// @parm [in] property value to write
         ImplAAFPropertyValue * pPropVal,

         // @parm [in, size_is(dataSize)] buffer from which struct
		 // data should be read
         aafMemPtr_t  pData,

         // @parm [in] size of pData buffer in bytes
         aafUInt32  dataSize);


  //****************
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        // @parm [out] count of members in this record type
        (aafUInt32 *  pCount) const;


  //****************
  // RegisterMembers()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterMembers
        (// @parm [in] array containing offset for each record member
         aafUInt32 *  pOffsets,

         // @parm [in] number of members in pOffsets
         aafUInt32  numMembers,

         // @parm [in] size of this struct
         aafUInt32  structSize);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);


  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const;

  virtual size_t externalSize(const OMByte* internalBytes,
							  size_t internalBytesSize) const;

  virtual void externalize(const OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(const OMByte* externalBytes,
							  size_t externalBytesSize) const;

  virtual void internalize(const OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;


  //****************
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in, size_is(numMembers)] array of member types to
		 // be represented in this record type
         ImplAAFTypeDef ** pMemberTypeIDs,

         // @parm [in, size_is(numMembers)] array of member names to
		 // be represented in this enumerated  type
         aafString_t *  pMemberNames,

         // @parm [in] number of members in pMemberInfo array
         aafUInt32  numMembers,

         // @parm [in] friendly name of this type definition
         const aafCharacter *  pTypeName);


private:

  void pvtInitInternalSizes (void) const;


  // types of members in this record
  //
  // BobT Note!!! This should be weak reference vector property...
  //OMVariableSizeProperty<aafUID_t> _memberTypes;
  OMWeakReferenceVectorProperty<ImplAAFTypeDef> _memberTypes;

  // names of members in this record; stored as single aafCharacter
  // array with embedded nulls
  OMVariableSizeProperty<aafCharacter> _memberNames;

  // when registered, will point to array of ints with registered
  // offsets of each field
  aafUInt32 * _registeredOffsets;

  // when registered, will contain native size of this record
  aafUInt32 _registeredSize;

  // will contain internal (native) size of each data member.  If
  // registered, will be determined from registered offsets.  If not
  // registered, will be determined from PropValSize()s.
  aafUInt32 * _internalSizes;

  ImplAAFTypeDef **  _cachedMemberTypes;

  aafUInt32          _cachedCount;

  aafInt32           _cachedPropValSize;
  aafBool            _propValSizeIsCached;
  aafBool            _registrationAttempted;

  aafBool            _defaultRegistrationUsed;

public:

  // overrides from ImplAAFTypeDef
  //
  aafBool IsFixedSize (void) const;
  size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;
  void AttemptBuiltinRegistration (void);

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        (ImplAAFTypeDef ** ppRawTypeDef);

public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;



  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefRecord> ImplAAFTypeDefRecordSP;

#endif // ! __ImplAAFTypeDefRecord_h__

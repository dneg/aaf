//@doc
//@class    AAFTypeDefRecord | Implementation class for AAFTypeDefRecord
#ifndef __ImplAAFTypeDefRecord_h__
#define __ImplAAFTypeDefRecord_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

class ImplAAFPropertyValue;

class ImplEnumAAFPropertyValues;

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


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
         aafUID_t *  pID,

         // @parm [in, size_is(numMembers)] array of member types to
		 // be represented in this record type
         ImplAAFTypeDef ** ppMemberTypes,

         // @parm [in, size_is(numMembers)] array of member names to
		 // be represented in this enumerated  type
         aafString_t *  pMemberNames,

         // @parm [in] number of members in pMemberInfo array
         aafUInt32  numMembers,

         // @parm [in] friendly name of this type definition
         wchar_t *  pTypeName);


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
         wchar_t *  pName,

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
         aafMemPtr_t *  pInitData,

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
        (aafUInt32 *  pCount);


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


private:
  // types of members in this record
  //
  // BobT Note!!! This should be weak reference vector property...
  OMVariableSizeProperty<aafUID_t> _memberTypes;

  // names of members in this record; stored as single wchar_t array
  // with embedded nulls
  OMVariableSizeProperty<wchar_t> _memberNames;

  // Pointer to array of ints with registered offsets of each field
  aafUInt32 * _registeredOffsets;

  // when registered, will contain native size of this record
  aafUInt32 _registeredSize;

public:
  //
  // non-published methods
  //

  // Returns true if property values of this type are of a fixed size.
  virtual aafBool IsFixedSize (void);

  // If this->IsFixedSize(), then will return the size of property
  // values of this type.  If not fixed size, will assert().
  virtual size_t PropValSize (void);

  // Returns true if offsets have been registered for this type def.
  aafBool IsRegistered (void);

  // If this->IsRegistered(), then will return the native size of
  // this type.  If not registered, will assert().
  size_t NativeSize (void);

  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefRecord)
};

#endif // ! __ImplAAFTypeDefRecord_h__

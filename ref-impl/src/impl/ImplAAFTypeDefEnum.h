//@doc
//@class    AAFTypeDefEnum | Implementation class for AAFTypeDefEnum
#ifndef __ImplAAFTypeDefEnum_h__
#define __ImplAAFTypeDefEnum_h__

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


#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMWeakRefProperty.h"

class ImplAAFPropertyValue;


class ImplAAFTypeDefEnum : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefEnum ();

protected:
  virtual ~ImplAAFTypeDefEnum ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] Type of values in this enumeration
         ImplAAFTypeDef * pType,

         // @parm [in, size_is(numElems)] array of element values to be represented in this enumerated
    // type
         aafInt64 * pElementValues,

         // @parm [in, size_is(numElems)] array of element names to be represented in this enumerated
    // type
         aafString_t *  pElementNames,

         // @parm [in] number of members in pElementValues and pElementNames arrays
         aafUInt32  numElems,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);


  //****************
  // CreateValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE CreateValue (
      aafMemPtr_t  pVal,
      aafUInt32  valSize,
      ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetElementType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementType
        // @parm [out] type definition of values of this enumeration
        (ImplAAFTypeDef ** ppTypeDef) const;


  //****************
  // CountElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountElements
        // @parm [out] count of elements within this enumeration
        (aafUInt32 *  pCount);


  //****************
  // GetElementValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementValue
        (// @parm [in] index of element to retrieve
         aafUInt32 index,

         // @parm [out] requested value
         aafInt64 * pOutValue);



  //****************
  // GetNameFromValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameFromValue
        (// @parm [in] value of element to get
         ImplAAFPropertyValue * pValue,

         // @parm [out, size_is(bufSize), string] buffer into which the element name is written
         wchar_t *  pName,

         // @parm [in] The size of the pName buffer, in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLenFromValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLenFromValue
        (// @parm [in] value of element to get
         ImplAAFPropertyValue * pValue,

         // @parm [out] required buffer length, in bytes
         aafUInt32 *  pLen);


  //****************
  // GetNameFromInteger()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameFromInteger
        (// @parm [in] value of element to get
         aafInt64  value,

         // @parm [out, size_is(bufSize), string] buffer into which the element name is written
         wchar_t *  pName,

         // @parm [in] The size of the pName buffer, in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLenFromInteger()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLenFromInteger
        (// @parm [in] value of element to get
         aafInt64  value,

         // @parm [out] required buffer length, in bytes
         aafUInt32 *  pLen);


  //****************
  // GetIntegerValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIntegerValue
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pPropValIn,

         // @parm [out] value of the enum represented by the given input property value
         aafInt64 *  pValueOut);


  //****************
  // SetIntegerValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIntegerValue
        (// @parm [in] property value to set
         ImplAAFPropertyValue * pPropValToSet,

         // @parm [in] new value of the enum represented by the given property value
         aafInt64  valueIn);

///////////////////
  virtual AAFRESULT STDMETHODCALLTYPE
		CreateValueFromName (
    // the Name of a valid Enum symbol
    /*[in]*/ aafCharacter_constptr  Name,

    // newly created property value
    /*[out]*/ ImplAAFPropertyValue ** ppPropVal);

  virtual AAFRESULT STDMETHODCALLTYPE
	  GetElementName (
    // index of element to retrieve
    /*[in]*/ aafUInt32  index,

    // requested value
    /*[out, size_is(bufSize)]*/ aafCharacter *  pOutValue,

    // The size of the pOutValue buffer in bytes
    /*[in]*/ aafUInt32  bufSize);
  
  virtual AAFRESULT STDMETHODCALLTYPE
	 GetElementNameBufLen (
    // index of element to retrieve
    /*[in]*/ aafUInt32  index,

    // required buffer length, in bytes
    /*[out]*/ aafUInt32 *  pLen);


  //****************
  // RegisterSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RegisterSize
        (// @parm [in] size of this enum
         aafUInt32  enumSize);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);



  // non-published overrides from AAFTypeDef
  aafBool IsFixedSize (void) const;
  size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;
  void AttemptBuiltinRegistration (void);

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;


  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const;

  virtual size_t externalSize(OMByte* internalBytes,
                              size_t internalBytesSize) const;

  virtual void externalize(OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(OMByte* externalBytes,
                              size_t externalBytesSize) const;

  virtual void internalize(OMByte* externalBytes,
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

         // @parm [in] Type of values in this enumeration
         const ImplAAFTypeDef * pType,

         // @parm [in, size_is(numElems)] array of element values to be represented in this enumerated
    // type
         aafInt64 * pElementValues,

         // @parm [in, size_is(numElems)] array of element names to be represented in this enumerated
    // type
         aafString_t *  pElementNames,

         // @parm [in] number of members in pElementValues and pElementNames arrays
         aafUInt32  numElems,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);


private:
   OMWeakReferenceProperty<ImplAAFTypeDef> _ElementType;

  // names of elements in this record; stored as single wchar_t array
  // with embedded nulls
  OMVariableSizeProperty<wchar_t> _ElementNames;

  // array of values for elements.
  OMVariableSizeProperty<aafInt64> _ElementValues;

  aafBool          _isRegistered;
  aafBool          _registrationAttempted;
  aafUInt32        _registeredSize;

  //
  // private methods
  //

  ImplAAFTypeDefSP BaseType (void) const;


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

typedef ImplAAFSmartPointer<ImplAAFTypeDefEnum> ImplAAFTypeDefEnumSP;

#endif // ! __ImplAAFTypeDefEnum_h__

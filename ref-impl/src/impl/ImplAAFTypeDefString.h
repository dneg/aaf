//@doc
//@class    AAFTypeDefString | Implementation class for AAFTypeDefString
#ifndef __ImplAAFTypeDefString_h__
#define __ImplAAFTypeDefString_h__

//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

class ImplAAFPropertyValue;
class OMTypeVisitor;

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMStringType.h"
#include "OMWeakRefVectorProperty.h"
#include "OMWeakRefProperty.h"

class ImplAAFTypeDefString : public ImplAAFTypeDef, public OMStringType
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefString ();

protected:
  virtual ~ImplAAFTypeDefString ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this array
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);


  //****************
  // GetType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef) const;


  //****************
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        (// @parm [in] property value of array
         ImplAAFPropertyValue * pPropVal,

         // @parm [out] count of elements in the specified string property value
         aafUInt32 *  pCount);


  //****************
  // CreateValueFromCString()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromCString
        (// @parm [in] pointer to compile-time C string containing data to use
		 aafMemPtr_t pInitData,

		 // @parm [in] size of data in pInitData, in bytes
		 aafUInt32  initDataSize,

		 // @parm [out] newly created property value
		 ImplAAFPropertyValue ** ppPropVal);


  //****************
  // SetCString()
  //
  //****************
  // SetCArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCString
        (// @parm [in] property value to write
         ImplAAFPropertyValue * pPropVal,

         // @parm [in, size_is(dataSize)] buffer from which C string data should be read
         aafMemPtr_t  pData,

         // @parm [in] size of pData buffer in bytes
         aafUInt32  dataSize);


  //****************
  // AppendElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElements
        (// @parm [in] property value corresponding to string to which elements are to
    // be appended
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in] Null-terminated array of elements to be appended
         aafMemPtr_t  pElements);


  //****************
  // GetElements()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElements
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [out] array of values that are read
         aafMemPtr_t  pBuffer,

         // @parm [in] size of pBuffer, in bytes
         aafUInt32  bufferSize);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);



  //*************************************************************
  //
  // Overrides from OMDefinition
  //
  //*************************************************************

  virtual const OMUniqueObjectIdentification& identification(void) const;

  virtual const wchar_t* name(void) const;

  virtual bool hasDescription(void) const;

  virtual const wchar_t* description(void) const;

  virtual bool isPredefined(void) const;

  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual bool isFixedSize(void) const;

  virtual void reorder(OMByte* externalBytes,
                       OMUInt32 externalBytesSize) const;

  virtual OMUInt32 externalSize(const OMByte* internalBytes,
								OMUInt32 internalBytesSize) const;

  virtual OMUInt32 externalSize(void) const;

  virtual void externalize(const OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual OMUInt32 internalSize(const OMByte* externalBytes,
								OMUInt32 externalBytesSize) const;

  virtual OMUInt32 internalSize(void) const;

  virtual void internalize(const OMByte* externalBytes,
                           OMUInt32 externalBytesSize,
                           OMByte* internalBytes,
                           OMUInt32 internalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual void accept(OMTypeVisitor& visitor) const;

  //*************************************************************
  //
  // Overrides from OMStringType
  //
  //*************************************************************

  virtual OMType* elementType(void) const;

  //****************
  // pvtInitialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type of each element to be contained in this array
         const ImplAAFTypeDef *pType,

         // @parm [in] friendly name of this type definition
         const aafCharacter *  pTypeName);


  // overrides from ImplAAFTypeDef
  //
  aafBool IsFixedSize (void) const;
  OMUInt32 PropValSize (void) const;
  aafBool IsRegistered (void) const;
  OMUInt32 NativeSize (void) const;

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

private:
  OMWeakReferenceProperty<OMUniqueObjectIdentification, ImplAAFTypeDef> _ElementType;

  ImplAAFTypeDefSP _cachedBaseType;

  ImplAAFTypeDefSP BaseType (void) const;
};

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefString> ImplAAFTypeDefStringSP;

#endif // ! __ImplAAFTypeDefString_h__

#ifndef __ImplAAFTypeDefVariableArray_h__
#define __ImplAAFTypeDefVariableArray_h__

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

#ifndef __ImplAAFTypeDefArray_h__
#include "ImplAAFTypeDefArray.h"
#endif

#include "OMWeakRefProperty.h"

class ImplEnumAAFPropertyValues;

class ImplAAFTypeDefVariableArray : public ImplAAFTypeDefArray
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefVariableArray ();

protected:
  virtual ~ImplAAFTypeDefVariableArray ();

public:

  // override from ImplAAFTypeDefArray
  virtual AAFRESULT STDMETHODCALLTYPE
    GetType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef) const;


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
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        (// @parm [in] property value of array
         ImplAAFPropertyValue * pPropVal,

         // @parm [out] count of elements in the specified array property value
         aafUInt32 *  pCount) const;


  //****************
  // AppendElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElement
        (// @parm [in] property value corresponding to array to which element is appended
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in] value to be appended to this array
         ImplAAFPropertyValue * pMemberPropVal);


  //****************
  // CreateEmptyValue()
  //
  // Implementation inherited from ImplAAFTypeDefArray


  //****************
  // CreateValueFromValues() 
  //
  // Implementation inherited from ImplAAFTypeDefArray


  //****************
  // GetElements() 
  //
  virtual AAFRESULT STDMETHODCALLTYPE
		GetElements (
								ImplAAFPropertyValue *pInPropVal,
								ImplEnumAAFPropertyValues **ppEnum);

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

         // @parm [in] type of each element to be contained in this array
         const ImplAAFTypeDef *pType,

         // @parm [in] friendly name of this type definition
         const aafCharacter * pTypeName);

  /**********************************
	   EX - METHODS
	   ***********************************************/
  
  virtual AAFRESULT STDMETHODCALLTYPE
	  PrependElement(
	  // property value corresponding to array to which element is prepended, [in]
	  ImplAAFPropertyValue * pInPropVal, 
	  // value to be prepended to this array,  [in]
	  ImplAAFPropertyValue * pMemberPropVal );
  
  virtual AAFRESULT STDMETHODCALLTYPE
	  RemoveElement(
	  // property value corresponding to array;  [in] 
	  ImplAAFPropertyValue * pInPropVal,
	  // zero-based index into elements in this array type; [in] 
	  aafUInt32  index);
  
  virtual AAFRESULT STDMETHODCALLTYPE  
	  InsertElement(
	  // property value corresponding to array; [in] 
	  ImplAAFPropertyValue * pInPropVal,
	  // zero-based index into elements in this array type;  [in]
	  aafUInt32  index,
	  // value to be inserted into this array; [in]
	  ImplAAFPropertyValue * pMemberPropVal);
  
  /////////////////////////////////////////////////////////
  
protected:
	// overrides from ImplAAFTypeDefArray ...

  virtual aafUInt32 pvtCount (ImplAAFPropertyValue * pInPropVal) const;

  virtual AAFRESULT STDMETHODCALLTYPE
	  ValidateInputParams (ImplAAFPropertyValue ** ppElementValues,
						aafUInt32  numElements);

  virtual bool IsArrayable(ImplAAFTypeDef * pSourceTypeDef) const;

public:

  // overrides from ImplAAFTypeDef
  //
  virtual aafBool IsFixedSize (void) const;
  virtual size_t PropValSize (void) const;
  virtual aafBool IsRegistered (void) const;
  virtual size_t NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  // Allocate and initialize the correct subclass of ImplAAFPropertyValue 
  // for the given OMProperty.
  virtual AAFRESULT STDMETHODCALLTYPE
    CreatePropertyValue(OMProperty *property, 
                        ImplAAFPropertyValue ** pPropertyValue) const;

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
   OMWeakReferenceProperty<ImplAAFTypeDef> _ElementType;

  ImplAAFTypeDefSP BaseType (void) const;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefVariableArray> ImplAAFTypeDefVariableArraySP;

#endif // ! __ImplAAFTypeDefVariableArray_h__

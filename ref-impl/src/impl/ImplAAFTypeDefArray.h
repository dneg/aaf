#ifndef __ImplAAFTypeDefArray_h__
#define __ImplAAFTypeDefArray_h__

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

class ImplAAFPropertyValue;

class ImplAAFTypeDefArray : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefArray ();

protected:
  virtual ~ImplAAFTypeDefArray ();

public:

  //****************
  // GetType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef) const = 0;


  //****************
  // CreateEmptyValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateEmptyValue
        (// @parm [out] newly-created property empty value,
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // CreateValueFromValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromValues
        (// @parm [in, size_is(numElements)] array of property values for elements of array value which
    // is to be created.
         ImplAAFPropertyValue ** ppElementValues,

         // @parm [in] size of pElementValues array.
         aafUInt32  numElements,

         // @parm [out] newly-created property value
         ImplAAFPropertyValue ** ppPropVal) ; //= 0;   //--cf make this a pure virtual method


  //****************
  // CreateValueFromCArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValueFromCArray
        (// @parm [in, size_is(initDataSize)] pointer to compile-time C array containing data to use
         aafMemPtr_t  pInitData,

         // @parm [in] size of data in pInitData, in bytes
         aafUInt32  initDataSize,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetElementValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementValue
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [in] zero-based index into elements in this array type
         aafUInt32  index,

         // @parm [out] value that is read
         ImplAAFPropertyValue ** ppOutPropVal);


  //****************
  // GetCArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCArray
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pPropVal,

         // @parm [out, size_is(dataSize)] buffer into which C array data should be written
         aafMemPtr_t  pData,

         // @parm [in] size of pData buffer in bytes
         aafUInt32  dataSize);


  //****************
  // SetElementValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetElementValue
        (// @parm [in] property value to write
         ImplAAFPropertyValue * pPropVal,

         // @parm [in] zero-based index into members in this array type
         aafUInt32  index,

         // @parm [in] value to be placed into this array
         ImplAAFPropertyValue * pMemberPropVal);


  //****************
  // SetCArray()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCArray
        (// @parm [in] property value to write
         ImplAAFPropertyValue * pPropVal,

         // @parm [in, size_is(dataSize)] buffer from which C array data should be read
         aafMemPtr_t  pData,

         // @parm [in] size of pData buffer in bytes
         aafUInt32  dataSize);


  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

  // Method is called after class has been added to MetaDictionary.
  // If this method fails the class is removed from the MetaDictionary and the
  // registration method will fail.
  virtual HRESULT CompleteClassRegistration(void);

protected:
	
	//returns number of elements in this array
  virtual aafUInt32 pvtCount (ImplAAFPropertyValue * pInPropVal) const = 0;

  //Validates the input source 
  virtual AAFRESULT STDMETHODCALLTYPE
	  ValidateInputParams (ImplAAFPropertyValue ** ppElementValues,
						aafUInt32  numElements);

  //Creates a new PV
  virtual AAFRESULT STDMETHODCALLTYPE
	  CreateValue(ImplAAFPropertyValue ** ppPropVal, 
								 aafUInt32 dataSize = 0); //overloaded function

  //Copies individual element-values into the new PV
  virtual AAFRESULT STDMETHODCALLTYPE
	  CopyValuesIntoValue (ImplAAFPropertyValue ** ppElementValues,
										  aafUInt32  numElements,
										  aafUInt32  sizeElem,
										  ImplAAFPropertyValue ** ppPropVal) ;

  //Function to test appropriate array-ability;  pure-virtual
  virtual bool IsArrayable(ImplAAFTypeDef * pSourceTypeDef) const = 0;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefArray> ImplAAFTypeDefArraySP;


#endif // ! __ImplAAFTypeDefArray_h__

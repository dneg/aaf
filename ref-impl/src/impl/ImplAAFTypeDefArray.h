#ifndef __ImplAAFTypeDefArray_h__
#define __ImplAAFTypeDefArray_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


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
        (ImplAAFTypeDef ** ppTypeDef) = 0;


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
         ImplAAFPropertyValue ** ppPropVal);


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


protected:
  virtual aafUInt32 pvtCount (ImplAAFPropertyValue * pInPropVal) const = 0;
  //
  // returns number of elements in this array
};

#endif // ! __ImplAAFTypeDefArray_h__

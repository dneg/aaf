//@doc
//@class    AAFTypeDefEnum | Implementation class for AAFTypeDefEnum
#ifndef __ImplAAFTypeDefEnum_h__
#define __ImplAAFTypeDefEnum_h__


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
         aafUID_t *  pID,

         // @parm [in] Type of values in this enumeration
         ImplAAFTypeDef * pType,

         // @parm [in, size_is(numElems)] array of element values to be represented in this enumerated
    // type
         aafInt32*  pElementValues,

         // @parm [in, size_is(numElems)] array of element names to be represented in this enumerated
    // type
         aafString_t *  pElementNames,

         // @parm [in] number of members in pElementValues and pElementNames arrays
         aafUInt32  numElems);


  //****************
  // GetElementType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementType
        // @parm [out] type definition of values of this enumeration
        (ImplAAFTypeDef ** ppTypeDef);


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
    GetElementValues
        (// @parm [out, size_is(numElems)] array to hold values
         aafUInt32 *  pValues,

         // @parm [in] number of elements in the pValues array
         aafUInt32  numElems);


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
         aafInt32  value,

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
         aafInt32  value,

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
         aafInt32 *  pValueOut);


  //****************
  // SetIntegerValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIntegerValue
        (// @parm [in] property value to set
         ImplAAFPropertyValue * pPropValToSet,

         // @parm [in] new value of the enum represented by the given property value
         aafInt32  valueIn);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefEnum)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefEnumTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFTypeDefEnum_h__



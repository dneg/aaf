//@doc
//@class    AAFTypeDefExtEnum | Implementation class for AAFTypeDefExtEnum
#ifndef __ImplAAFTypeDefExtEnum_h__
#define __ImplAAFTypeDefExtEnum_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplAAFPropertyValue;





#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif


class ImplAAFTypeDefExtEnum : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefExtEnum ();

protected:
  virtual ~ImplAAFTypeDefExtEnum ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         aafUID_t *  pID,

         // @parm [in] friendly name of this type definition
         wchar_t *  pTypeName);


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
         aafUID_t *  pValues,

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
  // GetNameFromAUID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameFromAUID
        (// @parm [in] value of element to get
         aafUID_t *  pValue,

         // @parm [out, size_is(bufSize), string] buffer into which the element name is written
         wchar_t *  pName,

         // @parm [in] The size of the pName buffer, in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLenFromAUID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLenFromAUID
        (// @parm [in] value of element to get
         aafUID_t *  pValue,

         // @parm [out] required buffer length, in bytes
         aafUInt32 *  pLen);


  //****************
  // GetAUIDValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAUIDValue
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pPropValIn,

         // @parm [out] value of the enum represented by the given input property value
         aafUID_t *  pValueOut);


  //****************
  // SetAUIDValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAUIDValue
        (// @parm [in] property value to set
         ImplAAFPropertyValue * pPropValToSet,

         // @parm [in] new value of the enum represented by the given property value
         aafUID_t *  pValueIn);


  //****************
  // AppendElement()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendElement
        (// @parm [in] value of appended element
         ImplaafUID_t * * pValue,

         // @parm [in] name of appended element
         Implwchar_t * * pName);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefExtEnum)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefExtEnumTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFTypeDefExtEnum_h__



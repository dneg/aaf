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



#if 0 
typedef struct _aafTypeDefEnumElementPair_t
{
  [string, size_is(nameSize)] wchar_t * pName;
  IAAFPropertyValue * pValue;
  aafUInt32 nameSize;
} aafTypeDefEnumElementPair_t;
#endif 
// 
typedef struct _aafTypeDefEnumElementPair_t 
{ 
  wchar_t * pName; 
  IAAFPropertyValue * pValue; 
  aafUInt32 nameSize; 
} aafTypeDefEnumElementPair_t; 





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

         // @parm [in, size_is(numElems)] array of element name/value pairs to be represented in this
	// enumerated type
         ImplaafTypeDefEnumElementPair_t ** * pElements),

         // @parm [in] number of elements in pElements array
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
  // GetElementName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementName
        (// @parm [in] value of element to get
         ImplAAFPropertyValue * value,

         // @parm [out, size_is(bufSize), string] buffer into which the element name is written
         wchar_t *  pName,

         // @parm [in] The size of the pName buffer, in bytes
         aafInt32  bufSize);
  //
  // AAFRESULT_SMALLBUF
  //   - bufSize indicates the buffer is too small to hold the string.
  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of
  //     this type.)


  //****************
  // GetElementNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetElementNameBufLen
        (// @parm [in] value of element to get
         ImplAAFPropertyValue * pValue,

         // @parm [out] required buffer length, in bytes
         aafInt32 *  pLen);
  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of this
  //     type.)


  //****************
  // GetValueAsValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValueAsValue
        (// @parm [in] property value to read
         ImplAAFPropertyValue * pPropValIn,

         // @parm [out] value of the enum represented by the given input property value
         ImplAAFPropertyValue ** ppPropValOut);


  //****************
  // SetValueFromValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetValueFromValue
        (// @parm [in] property value to set
         ImplAAFPropertyValue * pPropValToSet,

         // @parm [in] new value of the enum represented by the given property value
         ImplAAFPropertyValue * pPropValIn);
  // AAFRESULT_BAD_TYPE
  //   - pPropValIn's type doesn't match GetElementType\(\))


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefEnum)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefEnumTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFTypeDefEnum_h__



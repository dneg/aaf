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
         const aafUID_t *  pID,

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
         wchar_t *  pTypeName);


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


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);



  // non-published overrides from AAFTypeDef
  aafBool IsFixedSize (void) const;
  size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;


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


private:
  // OMWeakReferenceProperty<ImplAAFTypeDef> _ElementType;
  OMFixedSizeProperty<aafUID_t>   _ElementType;

  // names of elements in this record; stored as single wchar_t array
  // with embedded nulls
  OMVariableSizeProperty<wchar_t> _ElementNames;

  // array of values for elements.
  OMVariableSizeProperty<aafInt64> _ElementValues;


  //
  // private methods
  //
  AAFRESULT STDMETHODCALLTYPE
    GetElementNameBufLen (aafUInt32  index,
						  aafUInt32 * pLen);

  AAFRESULT STDMETHODCALLTYPE
    GetElementName (aafUInt32 index,
					wchar_t * pName,
					aafUInt32  bufSize);

  ImplAAFTypeDef * GetBaseType (void);

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefEnum)
};

#endif // ! __ImplAAFTypeDefEnum_h__

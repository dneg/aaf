#ifndef __ImplAAFTypeDefVariableArray_h__
#define __ImplAAFTypeDefVariableArray_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

class ImplAAFPropertyValue;

#ifndef __ImplAAFTypeDefArray_h__
#include "ImplAAFTypeDefArray.h"
#endif


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
         const aafUID_t *  pID,

         // @parm [in] type of each element to be contained in this array
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] friendly name of this type definition
         wchar_t *  pTypeName);


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


protected:
  // override from ImplAAFTypeDefArray
  virtual aafUInt32 pvtCount (ImplAAFPropertyValue * pInPropVal) const;


public:

  // overrides from ImplAAFTypeDef
  //
  virtual aafBool IsFixedSize (void) const;
  virtual size_t PropValSize (void) const;
  virtual aafBool IsRegistered (void) const;
  virtual size_t NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMPropertyMBS (OMPropertyId pid,
							const char * name) const;


private:
  // OMWeakReferenceProperty<ImplAAFTypeDef> _ElementType;
  OMFixedSizeProperty<aafUID_t>           _ElementType;

  ImplAAFTypeDefSP _cachedElemType;

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

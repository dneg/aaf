#ifndef __ImplAAFTypeDefFixedArray_h__
#define __ImplAAFTypeDefFixedArray_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFTypeDefArray_h__
#include "ImplAAFTypeDefArray.h"
#endif


class ImplAAFTypeDefFixedArray : public ImplAAFTypeDefArray
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefFixedArray ();

protected:
  virtual ~ImplAAFTypeDefFixedArray ();

public:

  // override from ImplAAFTypeDefArray
  virtual AAFRESULT STDMETHODCALLTYPE
    GetType
        // @parm [out] type of elements in this array
        (ImplAAFTypeDef ** ppTypeDef);


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         aafUID_t *  pID,

         // @parm [in] type of each element to be contained in this array
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] number of elements to be in this array
         aafUInt32  nElements,

         // @parm [in] friendly name of this type definition
         wchar_t *  pTypeName);


  //****************
  // GetCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCount
        // @parm [out] count of elements in this array
        (aafUInt32 *  pCount);


  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);


protected:
  virtual aafUInt32 pvtCount (ImplAAFPropertyValue * pInPropVal);
  //
  // returns number of elements in this array

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefFixedArray)

  // overrides from ImplAAFTypeDef
  //
  virtual aafBool IsFixedSize (void);
  virtual size_t PropValSize (void);
  aafBool IsRegistered (void);
  size_t NativeSize (void);

private:
  OMWeakReferenceProperty<ImplAAFTypeDef> _ElementType;
  OMFixedSizeProperty<aafUInt32>          _ElementCount;
};

#endif // ! __ImplAAFTypeDefFixedArray_h__

#ifndef __ImplAAFTypeDefVariableArray_h__
#define __ImplAAFTypeDefVariableArray_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
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
         aafUInt32 *  pCount);


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


protected:
  // override from ImplAAFTypeDefArray
  virtual aafUInt32 pvtCount (ImplAAFPropertyValue * pInPropVal);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefVariableArray)

  // overrides from ImplAAFTypeDef
  //
  virtual aafBool IsFixedSize (void);
  virtual size_t PropValSize (void);

private:
  OMWeakReferenceProperty<ImplAAFTypeDef> _ElementType;
};

#endif // ! __ImplAAFTypeDefVariableArray_h__

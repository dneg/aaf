//@doc
//@class    AAFTypeDef | Implementation class for AAFTypeDef
#ifndef __ImplAAFTypeDef_h__
#define __ImplAAFTypeDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplEnumAAFReferenceValues;





#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif


class ImplAAFTypeDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDef ();

protected:
  virtual ~ImplAAFTypeDef ();

public:

  //****************
  // GetTypeCategory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory
        // @parm [out] Returned type category
        (eAAFTypeCategory_t *  pTid);


  //****************
  // RawAccessType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        // @parm [out] the raw access type definition
        (ImplAAFTypeDef ** ppRawTypeDef);


  //****************
  // GetMinVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMinVersion
        // @parm [out, retval] Pointer to the minimum version 
        (aafVersionType_t *  pMinVersion);


  //****************
  // SetMinVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMinVersion
        // @parm [in] Minimum AAF Version
        (aafVersionType_t *  pMinVersion);


  //****************
  // GetMaxVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMaxVersion
        // @parm [out, retval] Pointer to the maximum version 
        (aafVersionType_t *  pMaxVersion);


  //****************
  // SetMaxVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMaxVersion
        // @parm [in] Maximum AAF Version
        (aafVersionType_t  MaxVersion);


  //****************
  // GetSwapNeeded()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSwapNeeded
        // @parm [out, retval] Specifies whether the bytes in this type need to be swapped
        (aafSwapNeeded_t *  pSwapNeeded);


  //****************
  // SetSwapNeeded()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSwapNeeded
        // @parm [in] Specifies wheter the bytes in this type need to be swapped
        (aafSwapNeeded_t  SwapNeeded);


  //****************
  // GetRefValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRefValues
        // @parm [out,retval] Reference value Enumeration
        (ImplEnumAAFReferenceValues ** ppEnum);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFTypeDef_h__



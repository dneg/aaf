//@doc
//@class    AAFTypeDefString | Implementation class for AAFTypeDefString
#ifndef __ImplAAFTypeDefString_h__
#define __ImplAAFTypeDefString_h__


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


class ImplAAFTypeDefString : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefString ();

protected:
  virtual ~ImplAAFTypeDefString ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         aafUID_t *  pID,

         // @parm [in] type of each element to be contained in this array
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] value of terminator element
         ImplAAFPropertyValue * pTermValue,

         // @parm [in] friendly name of this type definition
         wchar_t *  pTypeName);


  //****************
  // GetTermValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTermValue
        // @parm [out] terminator value
        (ImplAAFPropertyValue ** ppTermValue);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefString)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefStringTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFTypeDefString_h__



//@doc
//@class    AAFClassDef | Implementation class for AAFClassDef
#ifndef __ImplAAFClassDef_h__
#define __ImplAAFClassDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplEnumAAFPropertyDefs;

class ImplAAFPropertyDef;

class ImplAAFDefObject;

class ImplAAFTypeDef;





#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif


class ImplAAFClassDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFClassDef ();

protected:
  virtual ~ImplAAFClassDef ();

public:

  //****************
  // GetPropertyDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPropertyDefs
        // @parm [out, retval] Property Definition enumeration
        (ImplEnumAAFPropertyDefs ** ppEnum);


  //****************
  // CountPropertyDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountPropertyDefs
        // @parm [out] number of properties contained in this class definition
        (aafUInt32 *  pCount);


  //****************
  // AppendNewPropertyDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendNewPropertyDef
        (// @parm [in] auid to be used to identify this property
         aafUID_t *  pID,

         // @parm [in, string] name of the new property
         wchar_t *  pName,

         // @parm [in] type of the new property
         ImplAAFTypeDef * pTypeDef,

         // @parm [in] true if new property is to be optional
         aafBool  isOptional,

         // @parm [out] return pointer to newly created property def
         ImplAAFPropertyDef ** ppPropDef);


  //****************
  // LookupPropertyDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupPropertyDef
        (// @parm [in] auid reprepresenting property to look up
         aafUID_t *  pPropID,

         // @parm [out] resulting property definition
         ImplAAFPropertyDef ** ppPropDef);


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (// @parm [out, string, size_is(bufSize)] buffer into which name is to be written
         wchar_t *  pName,

         // @parm [in] size of *pName buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);


  //****************
  // GetParent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParent
        // @parm [out, retval] parent class definition
        (ImplAAFClassDef ** pClassDef);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFClassDef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFClassDefTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFClassDef_h__



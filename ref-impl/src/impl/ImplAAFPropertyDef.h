//@doc
//@class    AAFPropertyDef | Implementation class for AAFPropertyDef
#ifndef __ImplAAFPropertyDef_h__
#define __ImplAAFPropertyDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplEnumAAFPropertyValues;

class ImplAAFPropertyValue;

class ImplAAFTypeDef;

 



#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif


class ImplAAFPropertyDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFPropertyDef ();

protected:
  virtual ~ImplAAFPropertyDef ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
       (// @parm [in] auid to be used to identify this property definition
        aafUID_t * pID,
			
        // @parm [in] Type definition of this property definition,
	    ImplAAFTypeDef * pTypeDef,

        // @parm [in, string] friendly name of this type definition
	    wchar_t * pTypeName);
	


  //****************
  // GetTypeDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDef
        // @parm [out] definition of type contained by this property
        (ImplAAFTypeDef ** ppTypeDef);


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
        // @parm [in] Pointer to the minimum AAF Version
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
        // @parm [in] Pointer to the maximum AAF Version
        (aafVersionType_t *  pMaxVersion);


  //****************
  // GetIsSearchable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsSearchable
        // @parm [out, retval] pointer to the result
        (aafBool *  pIsSearchable);


  //****************
  // SetIsSearchable()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsSearchable
        // @parm [in] is searchable value
        (aafBool  IsSearchable);


  //****************
  // GetDefaultValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDefaultValue
        // @parm [out, retval] Pointer to default data value
        (ImplAAFPropertyValue ** ppDataValue);


  //****************
  // SetDefaultValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDefaultValue
        // @parm [in] default data value
        (ImplAAFPropertyValue * pDataValue);


  //****************
  // GetRefValues()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRefValues
        // @parm [out,retval] Reference value Enumeration
        (ImplEnumAAFPropertyValues ** ppEnum);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFPropertyDef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFPropertyDefTest.cpp.
  static AAFRESULT test();

private:
  // BobT: This should be persistent Real Soon Now...
  ImplAAFTypeDef * _pTypeDef;
};

#endif // ! __ImplAAFPropertyDef_h__

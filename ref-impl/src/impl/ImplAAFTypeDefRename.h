//@doc
//@class    AAFTypeDefRename | Implementation class for AAFTypeDefRename
#ifndef __ImplAAFTypeDefRename_h__
#define __ImplAAFTypeDefRename_h__


//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include "OMWeakRefProperty.h"

class ImplAAFPropertyValue;


class ImplAAFTypeDefRename : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefRename ();

protected:
  virtual ~ImplAAFTypeDefRename ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] type to which this is an alias
         ImplAAFTypeDef * pBaseType,

         // @parm [in, string] friendly name of this type definition
         const aafCharacter * pTypeName);


  //****************
  // GetBaseType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBaseType
        // @parm [out] type definition for which this is an alias
        (ImplAAFTypeDef ** ppBaseType) const;

  //****************
  // GetBaseValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBaseValue
        (// @parm [in] property value from which value is to be read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [out] pointer to property value represented by base type
         ImplAAFPropertyValue ** ppOutPropVal);


  //****************
  // CreateValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValue
        (// @parm [in] property value from which value is to be read
         ImplAAFPropertyValue * pInPropVal,

         // @parm [out] pointer to property value represented by typedef type
         ImplAAFPropertyValue ** ppOutPropVal);


  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual void reorder(OMByte* externalBytes,
                       size_t externalBytesSize) const;

  virtual size_t externalSize(const OMByte* internalBytes,
							  size_t internalBytesSize) const;

  virtual void externalize(const OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(const OMByte* externalBytes,
							  size_t externalBytesSize) const;

  virtual void internalize(const OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;


  // overrides from ImplAAFTypeDef
  //
  virtual aafBool IsFixedSize (void) const;
  virtual size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;

  OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  //****************
  // GetTypeCategory()
  //
  // (override from ImplAAFTypeDef)
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory
        // @parm [out] Returned type category
        (eAAFTypeCategory_t *  pTid);

  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        (ImplAAFTypeDef ** ppRawTypeDef);

public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;



  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

private:
  ImplAAFTypeDefSP BaseType () const;

   OMWeakReferenceProperty<ImplAAFTypeDef> _RenamedType;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefRename> ImplAAFTypeDefRenameSP;

#endif // ! __ImplAAFTypeDefRename_h__

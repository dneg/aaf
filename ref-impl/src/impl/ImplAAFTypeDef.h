//@doc
//@class    AAFTypeDef | Implementation class for AAFTypeDef
#ifndef __ImplAAFTypeDef_h__
#define __ImplAAFTypeDef_h__

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


#ifndef __ImplAAFMetaDefinition_h__
#include "ImplAAFMetaDefinition.h"
#endif

#ifndef OMTYPE_H
#include "OMType.h"
#endif



class ImplAAFPropertyValue;

class ImplAAFTypeDef : public ImplAAFMetaDefinition, public OMType
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


  //
  // non-published methods
  //

  // Returns true if property values of this type are of a fixed size.
  virtual aafBool IsFixedSize (void) const;

  // If this->IsFixedSize(), then will return the size of property
  // values of this type.  If not fixed size, will assert().
  virtual size_t PropValSize (void) const;

  // Returns true if offsets have been registered for this type def.
  virtual aafBool IsRegistered (void) const;

  // Will attempt to identify this as a built-in type; if it is, will
  // register the offsets.
  virtual void AttemptBuiltinRegistration (void);

  // If this->IsRegistered(), then will return the native in-memory
  // size of this type.  If not registered, will assert().
  virtual size_t NativeSize (void) const;

  // If this->IsRegistered(), then will return the native in-memory
  // size of this type.  If not registered, will PropValSize.
  virtual size_t ActualSize (void) const;

  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual void reorder(OMByte* bytes,
                       size_t bytesSize) const;

  virtual size_t externalSize(const OMByte* internalBytes,
                              size_t internalBytesSize) const;

  virtual void externalize(const OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(const OMByte* externalBytes,
                              size_t externalSize) const;

  virtual void internalize(const OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;

  // Allocate an OMProperty that can represent this type.  Implemented
  // by derived classes.
  virtual OMProperty *
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  // Allocate and initialize the correct subclass of ImplAAFPropertyValue 
  // for the given OMProperty.
  virtual AAFRESULT STDMETHODCALLTYPE
    CreatePropertyValue(OMProperty *property, 
                        ImplAAFPropertyValue ** pPropertyValue) const;


  //
  // Pure virtual methods to assist in the composition of types.  Each
  // type must implement these, to assist other types in determining
  // if particular aggregations are legal.
  //

  // Returns true if aggregates (that is, Record types) may be made
  // using this type as an element.
  virtual bool IsAggregatable () const;

  // Returns true if this type may be used as an element of a stream.
  virtual bool IsStreamable () const;

  // Returns true if this type may be used as an element of a
  // fixed-size array.
  virtual bool IsFixedArrayable () const;

  // Returns true if this type may be used as an element of a
  // variably-size array.
  virtual bool IsVariableArrayable () const;

  // Returns true if this type may be used as an element of a string.
  virtual bool IsStringable () const;


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

protected:
  // Helper function to return the raw type of UInt8Array (if
  // applicable for the derived type)
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtGetUInt8Array8Type
        (ImplAAFTypeDef ** ppRawTypeDef);
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDef> ImplAAFTypeDefSP;

#endif // ! __ImplAAFTypeDef_h__

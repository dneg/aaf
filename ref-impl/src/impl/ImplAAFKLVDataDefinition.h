//@doc
//@class    AAFKLVDataDefinition | Implementation class for AAFKLVDataDefinition
#ifndef __ImplAAFKLVDataDefinition_h__
#define __ImplAAFKLVDataDefinition_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=





class ImplAAFTypeDef;

class ImplAAFPropertyDef;

template <class T> class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFPropertyDef> ImplEnumAAFPropertyDefs;


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#include "OMWeakRefSetProperty.h"
#include "OMWeakRefProperty.h"

class ImplAAFKLVDataDefinition : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFKLVDataDefinition ();

protected:
  virtual ~ImplAAFKLVDataDefinition ();

public:


  //****************
  // AddParentProperty()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddParentProperty
        // @parm [in] Property definition to add.
        (ImplAAFPropertyDef * pParentProperty);

  //****************
  // GetParentProperties()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParentProperties
        // @parm [out] Property definition enumerator.
        (ImplEnumAAFPropertyDefs ** ppEnum);

  //****************
  // CountParentProperties()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountParentProperties
        // @parm [out] Number of property definitions.
        (aafUInt32*  pNumProperties);

  //****************
  // RemoveParentProperty()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveParentProperty
        // @parm [in] Property to remove.
        (ImplAAFPropertyDef * pParentProperty);

  //****************
  // GetKLVDataType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetKLVDataType
        // @parm [out] Returned data definition object
        (ImplAAFTypeDef ** ppTypeDef);


  //****************
  // SetKLVDataType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetKLVDataType
        // @parm [in] Data definition object
        (ImplAAFTypeDef * pTypeDef);

 private:
  OMWeakReferenceSetProperty<ImplAAFPropertyDef> _parentProperties;
  OMWeakReferenceProperty<ImplAAFTypeDef> _klvDataTypeDef;
    
};

#endif // ! __ImplAAFKLVDataDefinition_h__



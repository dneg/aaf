//@doc
//@class    AAFRefValue | Implementation class for AAFPropertyValue
#ifndef __ImplAAFRefValue_h__
#define __ImplAAFRefValue_h__


//=---------------------------------------------------------------------=
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


class OMObject;
class OMStorable;
class ImplAAFRoot;
class ImplAAFStorable;

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif


class ImplAAFTypeDefObjectRef;

class OMProperty;
class OMReferenceProperty; // The base class for singleton references


class ImplAAFRefValue : public ImplAAFPropertyValue
{
public:

protected:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRefValue ();
  virtual ~ImplAAFRefValue ();


  // non-published method to initialize this object.
  // Initialize an instance from a type definition. This is the "old-style"
  // "non-direct" access initialization method. 
  AAFRESULT Initialize (const ImplAAFTypeDefObjectRef *referenceType);

  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefObjectRef *referenceType,
                        OMProperty *property);
  
  // Return the propertyType as a referenceType.
  const ImplAAFTypeDefObjectRef *referenceType(void) const;
  
  // Return the instance's property as a reference property.
  OMReferenceProperty * referenceProperty(void) const;
  
  void SetLocalObject(ImplAAFStorable * localObject); // reference counted
  ImplAAFStorable * GetLocalObject(void) const; // not reference counted
  
public:
  virtual AAFRESULT STDMETHODCALLTYPE GetObject(ImplAAFStorable **ppObject) const = 0;
  virtual AAFRESULT STDMETHODCALLTYPE SetObject(ImplAAFStorable *pObject) = 0;

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);
  
  // Conversion utilities
  static ImplAAFStorable * ConvertOMObjectToRoot(OMObject *object);
  static ImplAAFStorable * ConvertRootToOMStorable(ImplAAFRoot *object);

private:
  ImplAAFStorable *_localObject; // Should be NULL if there is an associated property.

};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFRefValue> ImplAAFRefValueSP;

#endif // ! __ImplAAFRefValue_h__

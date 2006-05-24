//@doc
//@class    AAFStrongRefArrayValue | Implementation class for AAFStrongRefArrayValue
#ifndef __ImplAAFStrongRefArrayValue_h__
#define __ImplAAFStrongRefArrayValue_h__


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



#ifndef __ImplAAFRefArrayValue_h__
#include "ImplAAFRefArrayValue.h"
#endif

class ImplAAFTypeDefArray;

class ImplAAFStrongRefArrayValue : public ImplAAFRefArrayValue
{
public:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFStrongRefArrayValue ();
  
protected:
  virtual ~ImplAAFStrongRefArrayValue ();

  
  //
  // ImplAAFRefContainer overrides
  //  
public:
  // Return the type of object references in the container.
  virtual ImplAAFTypeDefObjectRef * GetElementType(void) const; // the result is NOT reference counted.
  
protected:
  // Perform specialized validation for an object before it is added
  // to a container.
  virtual AAFRESULT ValidateNewObject(ImplAAFStorable *pNewObject) const;
  
  // Perform any specialized cleanup of any object after it has been removed
  // from the container.
  virtual bool usesReferenceCounting(void) const;
  
public:

  // non-published method to initialize this object.
  // Initialize an instance from a type definition. This is the "old-style"
  // "non-direct" access initialization method. 
  AAFRESULT Initialize (const ImplAAFTypeDefArray *containerType,
                        bool fixedSize);
  
  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefArray *referenceType,
                        OMProperty *property,
                        bool fixedSize);

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFStrongRefArrayValue> ImplAAFStrongRefArrayValueSP;

#endif // ! __ImplAAFStrongRefArrayValue_h__

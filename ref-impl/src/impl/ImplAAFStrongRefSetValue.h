//@doc
//@class    AAFStrongRefSetValue | Implementation class for AAFStrongRefSetValue
#ifndef __ImplAAFStrongRefSetValue_h__
#define __ImplAAFStrongRefSetValue_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __ImplAAFRefSetValue_h__
#include "ImplAAFRefSetValue.h"
#endif

class ImplAAFTypeDefSet;

class ImplAAFStrongRefSetValue : public ImplAAFRefSetValue
{
public:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFStrongRefSetValue ();
  
protected:
  virtual ~ImplAAFStrongRefSetValue ();

  
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
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefSet *referenceType,
                        OMProperty *property);

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFStrongRefSetValue> ImplAAFStrongRefSetValueSP;

#endif // ! __ImplAAFStrongRefSetValue_h__

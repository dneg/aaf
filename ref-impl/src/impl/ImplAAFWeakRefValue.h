//@doc
//@class    AAFStrongRefValue | Implementation class for AAFStrongRefValue
#ifndef __ImplAAFWeakRefValue_h__
#define __ImplAAFWeakRefValue_h__


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



#ifndef __ImplAAFRefValue_h__
#include "ImplAAFRefValue.h"
#endif

class ImplAAFTypeDefWeakObjRef;


class ImplAAFWeakRefValue : public ImplAAFRefValue
{
public:

  //
  // Constructor/destructor
  //
  //********
  ImplAAFWeakRefValue ();
  
protected:
  virtual ~ImplAAFWeakRefValue ();
  
public:
  // non-published method to initialize this object.
  // Initialize an instance from a type definition. This is the "old-style"
  // "non-direct" access initialization method. 
  AAFRESULT Initialize (const ImplAAFTypeDefWeakObjRef *referenceType);

  // non-published method to initialize this object.
  // NOTE: The given property's type must be a reference type.
  AAFRESULT Initialize (const ImplAAFTypeDefWeakObjRef *referenceType,
                        OMProperty *property);

  virtual AAFRESULT STDMETHODCALLTYPE GetObject(ImplAAFStorable **ppObject) const;
  virtual AAFRESULT STDMETHODCALLTYPE SetObject(ImplAAFStorable *pObject);

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFWeakRefValue> ImplAAFWeakRefValueSP;

#endif // ! __ImplAAFWeakRefValue_h__

//@doc
//@class    AAFTypeDefWeakObjRef | Implementation class for AAFTypeDefWeakObjRef
#ifndef __CAAFTypeDefWeakObjRef_h__
#define __CAAFTypeDefWeakObjRef_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#ifndef __AAF_h__
#include "AAF.h"
#endif







#ifndef __CAAFTypeDefObjectRef_h__
#include "CAAFTypeDefObjectRef.h"
#endif


class CAAFTypeDefWeakObjRef
  : public IAAFTypeDefWeakObjRef,
    public CAAFTypeDefObjectRef
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTypeDefWeakObjRef (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTypeDefWeakObjRef ();

public:


  //***********************************************************
  //
  // Initialize()
  //
  // Initializes this type def to be a reference to objects of
  // the given type, and assigns this object the given AUID.  pObjType
  // points to the class definition of the least-derived class which
  // is possible to be contained in property values of this type.
  // 
  // pTargetSet points to an array of Property Def AUIDs which
  // indicate the location of the property containing the target of
  // this weak reference.  The AUIDs indicate the containment path to
  // the target property, starting at the root Header object.  For
  // example, if the containment hierarchy is:
  //
  // Header->A->B->C
  //
  // then the first AUID corresponds to the Header's property which
  // contains Object A; the second AUID corresponds to Object A's
  // property which contains Object B; and the third AUID corresponds
  // to Object B's property C which contans the target of this weak
  // reference.  If any intermediate property is a vector or set,
  // *all* objects in that vector or set are searched for the target.
  // Note that the final AUID *must* correspond to a Set property.
  // 
  // The number of AUIDs in pTargetSet is noted by ids.
  // 
  // This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  // - pObjType is a valid pointer.
  // - pTargetSet is a valid pointer.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - This object has already had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pObjType, pTypeName, or pTargetSet arg is NULL.
  //
  STDMETHOD (Initialize) (
    // auid to be used to identify this type
    /*[in, ref]*/ aafUID_constref  id,

    // class def of objects permitted to be referenced
    /*[in]*/ IAAFClassDef * pObjType,

    // friendly name of this type definition
    /*[in, string]*/ aafCharacter_constptr  pTypeName,

    // Number of property def IDs in pTargetSet
    /*[in]*/ aafUInt32  ids,

    // List of property definition IDs indicating the property where
    // the target is to be found.
    /*[in, size_is(ids)]*/ aafUID_constptr  pTargetSet);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFTypeDefWeakObjRef_h__



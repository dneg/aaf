#ifndef __AAFSmartPointer_h__
#define __AAFSmartPointer_h__
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

// Define smart pointer assertions before including the base header.
#ifndef AAF_SMART_POINTER_ASSERT
#define AAF_SMART_POINTER_ASSERT(condition) \
  if (! (condition)) throw #condition
#endif // ! AAF_SMART_POINTER_ASSERT

#ifndef __AAFSmartPointerBase_h__
#include "AAFSmartPointerBase.h"
#endif

//
// Smart pointer class for use with COM interfaces.  See
// AAFSmartPointerBase.h for details.
//
// Usage example:
//
// // Handy typedef:
// typedef IAAFSmartPointer<IAAFSequence> IAAFSequenceSP;
//
// // Instantiate a smart pointer to sequence interfaces:
// IAAFSequenceSP seqSP;
//
// // assume GetSequence(IAAFSequence**) already declared
// // Get a sequence interface pointer, just as you would if it was
// // declared as IAAFSequence**:
// GetSequence (&seqSP);
//
// // use the smart pointer as if it was declared as IAAFSequence**:
// AAFRESULT hr = seqSP->GetNumComponents(&count);
//
// // No need to call seqSP->Release(); when seqSP goes out of scope
// // the referenced sequence interface is automatically released.
//
// One detail repeated from AAFSmartPointerBase.h: clients may define
// the macro AAF_SMART_POINTER_ASSERT(condition) which is used by the
// smart pointer implementation to test pointer values.  If not
// client-defined, it will be set to assert(condition) by default.
//

//
// And now, the guts.
//

#ifndef AAF_SMART_POINTER_ASSERT
#error - AAF_SMART_POINTER_ASSERT should have been defined
// (at least in AAFSmartPointerBase.h)
#endif

//
// Implementation of the AAFCountedReference abstract base class for
// use in servicing reference counts to COM interfaces.
//
struct AAFRefCountedCOMReference : public AAFCountedReference<IUnknown>
{
protected:
  aafUInt32 acquire (IUnknown * pObj)
  {
	AAF_SMART_POINTER_ASSERT (pObj);
	return pObj->AddRef ();
  }

  aafUInt32 release (IUnknown * pObj)
  {
	AAF_SMART_POINTER_ASSERT (pObj);
	return pObj->Release ();
  }
};


//
// The smart pointer class.
//
template <typename ReferencedType>
struct IAAFSmartPointer
  : public AAFSmartPointerBase <ReferencedType, AAFRefCountedCOMReference>
{
};


#endif // ! __AAFSmartPointer_h__

#ifndef __AAFSmartPointer_h__
#define __AAFSmartPointer_h__
/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/

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
  void acquire (IUnknown * pObj)
  {
	AAF_SMART_POINTER_ASSERT (pObj);
	pObj->AddRef ();
  }

  void release (IUnknown * pObj)
  {
	AAF_SMART_POINTER_ASSERT (pObj);
	pObj->Release ();
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

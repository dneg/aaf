#ifndef __ImplAAFSmartPointer_h__
#define __ImplAAFSmartPointer_h__
/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*                                               *
\***********************************************/

// Define smart pointer assertions before including the base header.
#ifndef AAF_SMART_POINTER_ASSERT
#define AAF_SMART_POINTER_ASSERT(condition) \
  if (! (condition)) throw #condition
#endif // ! AAF_SMART_POINTER_ASSERT

#ifndef __AAFSmartPointerBase_h__
#include "AAFSmartPointerBase.h"
#endif

//
// Smart pointer class for use with AAF Impl objects.  See
// AAFSmartPointerBase.h for details.  The basics:
//
// Usage example:
//
// // Handy typedef:
// typedef ImplAAFSmartPointer<ImplAAFSequence> ImplAAFSequenceSP;
//
// // Instantiate a smart pointer to sequences:
// ImplAAFSequenceSP seqSP;
//
// // assume GetSequence(ImplAAFSequence**) already declared
// // Get a sequence pointer, just as you would if it was declared as
// // ImplAAFSequence**:
// GetSequence (&seqSP);
//
// // use the smart pointer as if it was declared as ImplAAFSequence**:
// AAFRESULT hr = seqSP->GetNumComponents(&count);
//
// // No need to call seqSP->ReleaseReference(); when seqSP goes out
// // of scope the referenced sequence interface is automatically
// // released.
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
// use in servicing reference counts to AAF Impl objects.
//
class ImplAAFRoot;
//
struct AAFCountedImplReference : public AAFCountedReference<ImplAAFRoot>
{
protected:
  void acquire (ImplAAFRoot * pObj)
  {
	AAF_SMART_POINTER_ASSERT (pObj);
	pObj->AcquireReference ();
  }

  void release (ImplAAFRoot * pObj)
  {
	AAF_SMART_POINTER_ASSERT (pObj);
	pObj->ReleaseReference ();
  }
};


//
// The smart pointer class.
//
template <typename ReferencedType>
struct ImplAAFSmartPointer
  : public AAFSmartPointerBase <ReferencedType, AAFCountedImplReference>
{
  // operator =
  ImplAAFSmartPointer<ReferencedType> & operator =
  (ReferencedType * src) ;
};


template <class ReferencedType>
ImplAAFSmartPointer<ReferencedType> &
ImplAAFSmartPointer<ReferencedType>::operator = (ReferencedType * ptr)
{
  // Hack! get a pointer to the rep of this object
  ReferencedType ** ppRep = this->operator&();
  AAF_SMART_POINTER_ASSERT (ppRep);
  
  // set the pointer
  *ppRep = ptr;

  if (*ppRep)
	acquire(*ppRep);

  return *this;
}


#endif // ! __ImplAAFSmartPointer_h__

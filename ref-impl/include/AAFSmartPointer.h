#ifndef __AAFSmartPointer_h__
#define __AAFSmartPointer_h__
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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

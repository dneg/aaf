#ifndef __AAFSmartPointerBase_h__
#define __AAFSmartPointerBase_h__
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

//
// This is a smart pointer template class for use as a reference to
// objects which support reference counting.  A helper class is also
// specified which defines a generalized reference counting interface
// used by the smart pointer template.  Clients of this SP template
// are expected to create two classes:
//
// - A class inherited from the AAFCountedReference class, providing
//   implementations for the acquire() and release() methods
//   appropriate for the referenced objects of interest.
//
// - A template instantiation of AAFSmartPointerBase, specifying the
//   referenced type of interest and the appropriate class derived
//   from AAFCountedReference which implements reference counting for
//   the referenced type.
//
// For example, clients interested in COM reference counting can
// supply a class derived from AAFCountedReference implementing its
// acquire() and release() methods.  Since we're talking about COM
// interfaces, they'd have to be implemented in terms of the IUnknown
// methods AddRef() and Release().  For the sake of argument, let's
// call this class ComCountedReference.
//
// In this example, the client may also wish to provide his own
// template, instantiating AAFSmartPointerBase with the appropriate
// template arguments:
//
// template <typename T>
// ComSmartPointer<T, ComCountedReference>
// {}
//
// This will declare a template class, called ComSmartPointer, which
// can be used with any type T which is derived from IUnknown.
//
// This resulting smart pointer template will have the following
// properties.
//
// Clients may define the macro AAF_SMART_POINTER_ASSERT(condition)
// which is used by the smart pointer implementation to test pointer
// values.  If not client-defined, it will be set to assert(condition)
// by default.
//
// Smart pointer instances of this template are designed for use with
// functions which return pointers to objects by reference as a
// function's argument.  For example:
//
// void GetObject (IAAFObject ** ppObj);
//
// where the object pointer is returned from GetObject() through the
// ppObj argument.  This template handles that by allowing itself to
// be converted to a T** pointer through the '&' operator.  In the
// case of the GetObject function above, you'd use it thusly:
//
// // Handy typedef
// typedef ComSmartPointer<IAAFObject> ObjectSP;
// ObjectSP myObjSmartPointer;
// GetObject (& myObjSmartPointer);
//
// This sp class will note that its internal representation was
// changed and do the appropriate acquire/release references.
//
// Note that this arrangement opens the door to some abuse by
// permitting clients to convert a sp to a T* or T** and mangling the
// contents directly; however IMO that liabililty is less than the
// liability of having reference counts going awry.
//
// The other usual smart pointer benefits apply.  When an instance of
// this smart pointer class is destroyed (e.g. when it goes out of
// scope) the reference count of the referenced object will
// automatically be decremented.
//

//
// And now, here are the guts.
//

//
// Abstract base class defining an interface to objects which maintain
// reference-counted references to other objects.  For example, a
// smart pointer class may derive from this interface in order to
// service reference counts to objects to which the smart pointer
// refers.
template <typename ReferencedObject>
struct AAFCountedReference
{
protected:
  virtual aafUInt32 acquire (ReferencedObject * pObj) = 0;
  virtual aafUInt32 release (ReferencedObject * pObj) = 0;
};


//
// Smart pointer template.  Template arguments:
//
// ReferencedType: the kind of object to which this pointer will
// point.
//
// RefCountType: a (derived) implementation of the AAFCountedReference
// class which implements reference counting for the type of object
// specified in ReferencedType.
//
template <typename ReferencedType, typename RefCountType>
struct AAFSmartPointerBase : public RefCountType
{
  // ctor to create 
  inline AAFSmartPointerBase ();

  // ctor that takes ownership of ReferencedType pointer
  inline AAFSmartPointerBase (ReferencedType* p);

  // copy ctor
  AAFSmartPointerBase (const AAFSmartPointerBase<ReferencedType, RefCountType> & src);
  
  // dtor
  virtual ~AAFSmartPointerBase ();

  // assignment operator
  AAFSmartPointerBase<ReferencedType, RefCountType> & operator=
    (const AAFSmartPointerBase<ReferencedType, RefCountType> & src);
  
  // Allows passing this smart ptr as argument to methods which expect
  // a ReferencedType**, in order to fill it in.
  ReferencedType ** operator & ();
 
  // Allows passing this smart ptr as argument to methods which expect
  // a ReferencedType*
  inline operator ReferencedType * () const;
 
  // member access operators (non-const and const)
  inline ReferencedType * operator-> ();
  inline const ReferencedType * operator-> () const;

private:

  // Current referenced object
  ReferencedType * _rep;
};


//
// Implementations
//

// Clients may define AAF_SMART_POINTER_ASSERT(condition).  If not
// client-defined, will use assert().
//
#ifndef AAF_SMART_POINTER_ASSERT
#include <assert.h>
#define AAF_SMART_POINTER_ASSERT(condition) assert(condition)
#endif // ! AAF_SMART_POINTER_ASSERT

template <typename ReferencedType, typename RefCountType>
inline AAFSmartPointerBase<ReferencedType, RefCountType>::
AAFSmartPointerBase ()
  : _rep (0)
{}

template <typename ReferencedType, typename RefCountType>
inline AAFSmartPointerBase<ReferencedType, RefCountType>::
AAFSmartPointerBase (ReferencedType* p)
  : _rep (p)
{
  if (_rep)
    acquire(_rep);
};

template <typename ReferencedType, typename RefCountType>
AAFSmartPointerBase<ReferencedType, RefCountType>::
AAFSmartPointerBase
  (const AAFSmartPointerBase<ReferencedType, RefCountType> & src)
	: _rep (src._rep)
{
  if (_rep)
	acquire(_rep);
}  


template <typename ReferencedType, typename RefCountType>
AAFSmartPointerBase<ReferencedType, RefCountType>::
~AAFSmartPointerBase ()
{
  if (_rep)
	{
	  aafUInt32 refCnt =
	  release (_rep);
	  _rep = 0;
	}
}


template <typename ReferencedType, typename RefCountType>
AAFSmartPointerBase<ReferencedType, RefCountType> &
AAFSmartPointerBase<ReferencedType, RefCountType>::
operator=
  (const AAFSmartPointerBase<ReferencedType, RefCountType> & src)
{
  if (_rep)
	{
	  release (_rep);
	  _rep = 0;
	}

  _rep = src._rep;
  if (_rep)
	acquire(_rep);

  return *this;
}
  

template <typename ReferencedType, typename RefCountType>
ReferencedType** AAFSmartPointerBase<ReferencedType, RefCountType>::
operator & ()
{
  if (_rep)
	{
	  release (_rep);
	  _rep = 0;
	}
  return &_rep;
}


template <typename ReferencedType, typename RefCountType>
inline AAFSmartPointerBase<ReferencedType, RefCountType>::
operator ReferencedType* () const
{
  // in case something changed since last time

  // ((AAFSmartPointerBase<ReferencedType, RefCountType>*)this)->updateRefCounts ();
  return _rep;
}


template <typename ReferencedType, typename RefCountType>
inline ReferencedType* AAFSmartPointerBase<ReferencedType, RefCountType>::
operator-> ()
{
  AAF_SMART_POINTER_ASSERT (_rep);
  return _rep;
}

template <typename ReferencedType, typename RefCountType>
inline const ReferencedType *
AAFSmartPointerBase<ReferencedType, RefCountType>::
operator-> () const
{
  AAF_SMART_POINTER_ASSERT (_rep);
  return _rep;
}


#endif // ! __AAFSmartPointerBase_h__

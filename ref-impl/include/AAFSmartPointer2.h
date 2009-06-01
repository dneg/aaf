#ifndef __AAFSmartPointer2_h__
#define __AAFSmartPointer2_h__
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
// DiskStream Inc.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAFSmartPointer.h"

// IAAFSmartPointer2 is a replacement for IAAFSmartPointer in
// situations where the IAAFSmartPointer::operator&() clashes with code
// that expects the "&" operator to always return a pointer of a type
// that mathces its operand.
//
// Said another way, certain code expects the following to always be
// work: "T* px; T** ppx = &px;". If that is not true, that code breaks.
//
// IAAFSmartPointer is implemented such that the follow is okay:
//
//   "IAAFSmartPointer<T> px; T** ppx = &px;"  // okay
//
// .. but this is not:
//       
//   "IAAFSmartPointer<T> px; IAAFSmartPointer<T>* ppx = &x;"  // compile error,
//                                                             // type mismatch,
//                                                             // &x returns T**  
//
// This causes problems with generic code (read stl) that expects the
// address-of operator to work "normally".
//
// The gnu stdc++ library, version 6, is an example of a library that
// fails. With its stl implementation the following is okay:
//
//    "vector<IAAFSmartPointer2<IAAFWhatever> >"    // okay
//
//  ... but this is not:
//
//    "vector<IAAFSmartPointer<IAAFWhatever> >"     // fails due to type mismatch,
//                                                  // IAAFSmartPointer<T>::operator&
//                                                  // returns T** where
//                                                  // IAAFSmartPointer<T>* is expected.
//
// IAAFSmartPointer2 and IAAFSmartPointer are highly compatible. They
// can mixed and matched at will. The only significant interface
// difference is that IAAFSmartPointer2::GetAddrOf() method replaces
// IAAFSmartPointer::operator&().
//       

template <typename ReferencedType>
class IAAFSmartPointer2
{
 public:

  // dtor
  ~IAAFSmartPointer2()
  {}

  // default ctor
  IAAFSmartPointer2()
  {}

  // ctor that takes ownership of ReferencedType pointer
  explicit IAAFSmartPointer2( ReferencedType* p )
    : _sp( p )
  {}

  // copy ctor
  IAAFSmartPointer2( const IAAFSmartPointer2& src )
    : _sp( src._sp )
  {}

  // assignment operator, IAAFSmartPonter2 = IAAFSmartPointer
  IAAFSmartPointer2<ReferencedType>& operator=
    (const IAAFSmartPointer<ReferencedType>& lhs)
  {
    _sp = lhs;
    return *this;
  }

  // assignment operator IAAFSmartPointer2 = IAAFSmartPointer2
  IAAFSmartPointer2<ReferencedType>& operator=
    (const IAAFSmartPointer2<ReferencedType>& lhs)
  {
    if ( &lhs != this )
    {
      _sp = lhs._sp;
    }
    return *this;
  }

  // Allows passing this smart ptr as argument to methods which expect
  // a ReferencedType**, in order to fill it in.
  ReferencedType** GetAddrOf()
  {
    return _sp.operator&();
  }

#if 0
  // Allows passing this smart ptr as argument to methods which expect
  // a ReferencedType**, in order to fill it in.
  ReferencedType** GetAddrOf()
  {
    return _sp.operator&();
  }
#endif
  // Allows passing this smart ptr as argument to methods which expect
  // a ReferencedType*
  operator ReferencedType * () const
  {
    return static_cast<ReferencedType*>( _sp );
  }
 
  // Cast to old smart pointer
  operator IAAFSmartPointer<ReferencedType> ()
  {
    return _sp;
  }

  // member access operators (non-const and const)
  ReferencedType * operator-> ()
  {
    return _sp.operator->();
  }

  const ReferencedType * operator-> () const
  {
    return _sp.operator->();
  }

 private:
  IAAFSmartPointer<ReferencedType> _sp;
};

#endif

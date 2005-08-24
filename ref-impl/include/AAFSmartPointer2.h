#ifndef __AAFSmartPointer2_h__
#define __AAFSmartPointer2_h__
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
// Licensor of the AAF Association is DiskStream Inc.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAFSmartPointer.h"

template <typename ReferencedType>
class IAAFSmartPointer2
{
 public:

  // dtor
  ~IAAFSmartPointer2()
  {}

  //
  // This part of the interface matches IAAFSmartPointer
  //

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

  //
  // This part of the interfaces matches IAAFSmartPointerBase
  //

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
    return _sp->operator->();
  }

  const ReferencedType * operator-> () const
  {
    return _sp->operator->();
  }

 private:
  IAAFSmartPointer<ReferencedType> _sp;
};

#endif

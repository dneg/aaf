//=---------------------------------------------------------------------=
//
// $Id$
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
//=---------------------------------------------------------------------=

#ifndef __TYPEDNODEFACTORYIMPL_h__
#define __TYPEDNODEFACTORYIMPL_h__

//project files
#include "AAFTypedObjNode.h"
#include "TypedNodeFactory.h"

//AX files
#include <AxSmartPointer.h>
#include <AxObject.h>

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

template<typename AAFObjType>
class TypedNodeFactoryImpl : public TypedNodeFactory
{
 public:
  TypedNodeFactoryImpl()
  {}

  ~TypedNodeFactoryImpl()
  {}

  boost::shared_ptr<Node> CreateNode(IAAFObjectSP spObj)
  {
    boost::shared_ptr<Node> spNode(new AAFTypedObjNode<AAFObjType>(spObj, AxObject(spObj).GetClassName()));
    return spNode;
  }

 private:

  // prohibited
  TypedNodeFactoryImpl( const TypedNodeFactoryImpl& );
  TypedNodeFactoryImpl& operator=( const TypedNodeFactoryImpl& );
};

} // end of namespace diskstream

#endif/*__TypedNodeFactoryImpl_h__*/

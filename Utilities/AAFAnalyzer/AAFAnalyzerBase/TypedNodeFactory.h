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

#ifndef __TYPEDNODEFACTORY_h__
#define __TYPEDNODEFACTORY_h__

//Ax files
#include <AxSmartPointer.h>

//Boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace boost;

class Node;
class AAFObjNode;

class TypedNodeFactory
{
 public:
  TypedNodeFactory();
  virtual ~TypedNodeFactory();

  virtual shared_ptr<Node> CreateNode(IAAFObjectSP spObj) = 0;
  virtual shared_ptr<Node> CreateNodeFrom( shared_ptr<AAFObjNode> spNode ) =0;

 private:

  // prohibited
  TypedNodeFactory( const TypedNodeFactory& );
  TypedNodeFactory& operator=( const TypedNodeFactory& );
};

} // end of namespace diskstream

#endif/*__TYPEDNODEFACTORY_h__*/

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

#ifndef __NODEFACTORYIMPL_h__
#define __NODEFACTORYIMPL_h__

//AAF Analyzer Base files
#include "NodeFactory.h"

//AX files
#include <AxSmartPointer.h>

namespace aafanalyzer {

using namespace boost;

class Node;

class NodeFactoryImpl : public NodeFactory
{
 public:
  NodeFactoryImpl();
  virtual ~NodeFactoryImpl();

  virtual shared_ptr<Node> CreateNode(IAAFObjectSP spObj);
  
 private:

  // prohibited
  NodeFactoryImpl( const NodeFactoryImpl& );
  NodeFactoryImpl& operator=( const NodeFactoryImpl& );
};

} // end of namespace diskstream

#endif/*__NODEFACTORYIMPL_h__*/

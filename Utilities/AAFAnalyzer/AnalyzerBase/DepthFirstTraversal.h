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

#ifndef __DEPTHFIRSTTRAVERSAL_h__
#define __DEPTHFIRSTTRAVERSAL_h__

//Boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace boost;

class Node;
class Visitor;
class EdgeMap;
class Edge;

class DepthFirstTraversal
{
 public:

  DepthFirstTraversal(shared_ptr<EdgeMap> spEdgeMap, shared_ptr<Node> spStartNode);
  ~DepthFirstTraversal();

  // Depricated (there is no need to pass a start node here).
  void TraverseDown(shared_ptr<Visitor> spVisitor, shared_ptr<Node> spNode);
  void TraverseUp(shared_ptr<Visitor> spVisitor, shared_ptr<Node> spNode);

  void TraverseDown(shared_ptr<Visitor> spVisitor );
  void TraverseUp(shared_ptr<Visitor> spVisitor );

 private:
  shared_ptr<EdgeMap> _spEdgeMap;
  shared_ptr<Node> _spStartNode;  

  // prohibited
  DepthFirstTraversal();
  DepthFirstTraversal( const DepthFirstTraversal& );
  DepthFirstTraversal& operator=( const DepthFirstTraversal& );
};

} // end of namespace diskstream

#endif

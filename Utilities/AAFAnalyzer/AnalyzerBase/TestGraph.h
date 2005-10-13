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

#ifndef __TESTGRAPH_h__
#define __TESTGRAPH_h__

//Boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace boost;

class Node;
class EdgeMap;

class TestGraph
{
 public:
   
  // The default copy constructor is okay.
  TestGraph(shared_ptr<EdgeMap> spEdgeMap, shared_ptr<Node> spRootNode);
  ~TestGraph();

  shared_ptr<EdgeMap> GetEdgeMap() const;
  shared_ptr<Node> GetRootNode() const;
  void Decorate( shared_ptr<Node> decoratedNode ) const;

 private:
  shared_ptr<EdgeMap> _spEdgeMap;
  
  //Using a double pointer so the root node can be decorated while keeping this
  //object constant.
  shared_ptr<shared_ptr<Node> > _spRootNode;

  shared_ptr<shared_ptr<Node> > InitializeRoot( shared_ptr<Node> spRoot );

  // prohibited
  TestGraph();
  TestGraph& operator=(const TestGraph& graph);
};

} // end of namespace diskstream

#endif

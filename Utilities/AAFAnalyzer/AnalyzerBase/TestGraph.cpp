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
//=---------------------------------------------------------------------=

//Analyzer Base files
#include "TestGraph.h"
#include "Node.h"
#include "EdgeMap.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer
{
    
using namespace boost;

TestGraph::TestGraph(shared_ptr<EdgeMap> spEdgeMap, shared_ptr<Node> spRootNode)
  : _spEdgeMap( spEdgeMap ),
    _spRootNode( spRootNode )
{
}

/*TestGraph::TestGraph& operator=(const TestGraph& graph)
{
  if(this != &graph)
  {
    _spEdgeMap = graph.GetEdgeMap();
    _spRootNode = graph.GetRootNode();
  }

  return *this;
}*/

TestGraph::~TestGraph()
{
}

shared_ptr<EdgeMap> TestGraph::GetEdgeMap() const
{
  return _spEdgeMap;
}

shared_ptr<Node> TestGraph::GetRootNode() const
{
  return _spRootNode;
}

//Update the node with the decorated node in all maps.
void TestGraph::Decorate( shared_ptr<Node> decoratedNode ) const
{
  //1. If the decorated node is the root of the tree
  if ( _spRootNode->GetLID() == decoratedNode->GetLID() )
  {
    _spRootNode = decoratedNode;
  }
  
  //2. Do any decorating based upon the type of the node.
  decoratedNode->Decorate( decoratedNode );
   
  //3. Update the EdgeMap
  _spEdgeMap->DecorateEdge( decoratedNode );
}

}//end of namespace

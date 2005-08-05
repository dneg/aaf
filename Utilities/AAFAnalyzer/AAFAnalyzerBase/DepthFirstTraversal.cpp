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

#include "DepthFirstTraversal.h"

#include "Node.h"
#include "Visitor.h"
#include "EdgeMap.h"
#include "Edge.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

DepthFirstTraversal::DepthFirstTraversal(boost::shared_ptr<EdgeMap> spEdgeMap, boost::shared_ptr<Node> spStartNode)
  : _spEdgeMap( spEdgeMap ),
    _spStartNode( spStartNode )
{
}

DepthFirstTraversal::~DepthFirstTraversal()
{
}

void DepthFirstTraversal::TraverseDown(boost::shared_ptr<Visitor> spVisitor, boost::shared_ptr<Node> spNode)
{
  EdgeMap::EdgeVectorSP theChildren = _spEdgeMap->GetChildren(spNode);  
  if(!spNode->PreOrderVisit(spVisitor))
  { //method failed, do not proceed further with tests
    return;
  }
    
  for(unsigned int i = 0; i < theChildren->size(); i++)
  {
    if(theChildren->at(i)->Visit(spVisitor))
    {
      TraverseDown(spVisitor, theChildren->at(i)->GetChildNode());
    }
  }

  spNode->PostOrderVisit(spVisitor);
}

void DepthFirstTraversal::TraverseUp(boost::shared_ptr<Visitor> spVisitor, boost::shared_ptr<Node> spNode)
{
  EdgeMap::EdgeVectorSP theParents = _spEdgeMap->GetParents(spNode);  
  if(!spNode->PreOrderVisit(spVisitor))
  { //method failed, do not proceed further with tests
    return;
  }
  
  for(unsigned int i = 0; i < theParents->size(); i++)
  {
    if(theParents->at(i)->Visit(spVisitor))
    {
      TraverseUp(spVisitor, theParents->at(i)->GetParentNode());
    }
  }

  spNode->PostOrderVisit(spVisitor);
}


} // end of namespace diskstream

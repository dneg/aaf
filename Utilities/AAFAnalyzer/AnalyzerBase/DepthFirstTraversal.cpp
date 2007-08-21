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

//Analyzer Base files
#include "DepthFirstTraversal.h"
#include "Node.h"
#include "Visitor.h"
#include "EdgeMap.h"
#include "Edge.h"

#include <iostream>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace boost;

DepthFirstTraversal::DepthFirstTraversal(shared_ptr<EdgeMap> spEdgeMap, shared_ptr<Node> spStartNode)
  : _spEdgeMap( spEdgeMap ),
    _spStartNode( spStartNode )
{}

DepthFirstTraversal::~DepthFirstTraversal()
{}

void DepthFirstTraversal::TraverseDown(shared_ptr<Visitor> spVisitor, shared_ptr<Node> spNode)
{
  if( !spNode->PreOrderVisit(spVisitor) )
  {
    spVisitor->TraversalStopVisit(*spNode);
    return;
  }
    
  EdgeMap::ConstEdgeVectorSP theChildren = _spEdgeMap->GetChildren(spNode);  
  for(unsigned int i = 0; i < theChildren->size(); i++)
  {
    if ( spVisitor->TraversalPreEdgeVisit( *theChildren->at(i) ) )
    {
      if(theChildren->at(i)->Visit(spVisitor))
      {
        TraverseDown( spVisitor, theChildren->at(i)->GetChildNode() );
      }
    }
    spVisitor->TraversalPostEdgeVisit( *theChildren->at(i) );
  }

  spNode->PostOrderVisit(spVisitor);
}

void DepthFirstTraversal::TraverseDown(shared_ptr<Visitor> spVisitor )
{
  TraverseDown( spVisitor, _spStartNode );
}

void DepthFirstTraversal::TraverseUp(shared_ptr<Visitor> spVisitor, shared_ptr<Node> spNode)
{
  if( !spNode->PreOrderVisit(spVisitor) )
  {
    spVisitor->TraversalStopVisit(*spNode);
    return;
  }
  
  EdgeMap::ConstEdgeVectorSP theParents = _spEdgeMap->GetParents(spNode);  
  for(unsigned int i = 0; i < theParents->size(); i++)
  {
    if ( spVisitor->TraversalPreEdgeVisit( *theParents->at(i) ) )
    {
      if(theParents->at(i)->Visit(spVisitor))
      {
        TraverseUp(spVisitor, theParents->at(i)->GetParentNode());
      }
    }
    spVisitor->TraversalPostEdgeVisit( *theParents->at(i) );
  }

  spNode->PostOrderVisit(spVisitor);
}

void DepthFirstTraversal::TraverseUp(shared_ptr<Visitor> spVisitor )
{
  TraverseUp( spVisitor, _spStartNode );
}


} // end of namespace diskstream

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
#include "Visitor.h"
#include "Edge.h"

#include <iostream>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================
namespace aafanalyzer {

Visitor::Visitor()
  : _visitedNodes(),
    _follow( FOLLOW_CONTAINED ),
    _edgeDetailsStack(),
    _sourceTag(false,0)
{
  _edgeDetailsStack.push_back( make_pair(Edge::EDGE_KIND_CONTAINMENT, 0) );
}

Visitor::Visitor( Follow_e follow )
  : _visitedNodes(),
    _follow( follow ),
    _edgeDetailsStack(),
    _sourceTag(false,0)
{
  _edgeDetailsStack.push_back( make_pair(Edge::EDGE_KIND_CONTAINMENT, 0) );
}

Visitor::~Visitor()
{}

bool Visitor::PreOrderVisit(Node& node)
{
  return true;
}

bool Visitor::PostOrderVisit(Node& node)
{
  return true;
}

bool Visitor::EdgeVisit(Edge& edge)
{
  return true;
}

unsigned int Visitor::GetLevel() const
{
  return static_cast<unsigned int>(_edgeDetailsStack.size());
}

Edge::EdgeKind_e Visitor::GetLastEdgeKind() const
{
  return _edgeDetailsStack.back().first;
}

bool Visitor::TraversalPreEdgeVisit(const Edge& edge)
{
  _edgeDetailsStack.push_back( make_pair(edge.GetKind(), edge.GetTag()) );

  if ( FOLLOW_ALL == _follow )
  {
    return true;
  }
  else if ( FOLLOW_CONTAINED == _follow )
  {
    return FollowContained( edge );
  }
  else 
  {	
    assert( FOLLOW_REFERENCES == _follow );
    return FollowReferences( edge );
  }
}

void Visitor::TraversalPostEdgeVisit(const Edge& edge)
{
  assert( edge.GetTag() == _edgeDetailsStack.back().second );
  _edgeDetailsStack.pop_back();
}

void Visitor::TraversalStopVisit( const Node& )
{
  // This means that a PreOrderVisit(node) call returned false and the
  // traversal is stopping. The _sourceTag has to be reset in this
  // because it may have been mid-way through traversing a reference
  // and failure to reset its state will leave the parser implemented
  // in FollowReferences in an invalid state.
  _sourceTag.first = false;
}

void Visitor::RecordVisit( Node::LID id )
{
  _visitedNodes.insert( id );
}

bool Visitor::HaveVisited( Node::LID id )
{
  return ( _visitedNodes.find( id ) != _visitedNodes.end() );
}

bool Visitor::FollowContained( const Edge& edge )
{
  if ( Edge::EDGE_KIND_CONTAINMENT == edge.GetKind() )
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool Visitor::FollowReferences( const Edge& edge )
{
#if 0
  // This is occasionally useful for debug.
  std::wcout << L"Follow: [" << edge.GetTypeName() << L"," << edge.GetTag() << L"] " 
	     << edge.GetParentNode()->GetName() << L"," << edge.GetParentNode()->GetLID() 
	     << L" -> " << edge.GetChildNode()->GetName() << L"," << edge.GetChildNode()->GetLID();
#endif

  bool rc = RealFollowReferences( edge );

#if 0
  // This is occasionally useful for debug.
  if ( rc )
    std::wcout << " YES" << std::endl;
  else
    std::wcout << " NO" << std::endl;
#endif

  return rc;
}

bool Visitor::RealFollowReferences( const Edge& edge )
{
  // Undo the generic push_back call that happened in edge visit. We
  // need to access the prior value on the back of the stack.
  _edgeDetailsStack.pop_back();

  const Edge::EdgeKind_e backKind = _edgeDetailsStack.back().first;
  const Edge::EdgeKind_e kind     = edge.GetKind();
  const Node::LID tag             = edge.GetTag();

  _edgeDetailsStack.push_back( make_pair(kind,tag) );

  // If this is normal containment.
  if ( backKind == Edge::EDGE_KIND_CONTAINMENT &&
           kind == Edge::EDGE_KIND_CONTAINMENT )
  {
    assert( !_sourceTag.first );
    return true;
  }

  // If the last edge was a containmnet edge, but this is a reference,
  // then we initialize the _sourceTag so that subsequent edge
  // traversals know which reference to follow.
  else if ( backKind == Edge::EDGE_KIND_CONTAINMENT &&
                kind == Edge::EDGE_KIND_REFERENCE )
  {
    assert( !_sourceTag.first );
    _sourceTag = make_pair(true,tag);
    return true;
  }

  // If the last edge was a reference, and this edge is a reference,
  // then we are mid-traveral along a multi node path created as a
  // result of resolving a source clip (i.e. source clip -> mob ->
  // slot -> component).  It is possible for multiple distinct source
  // objects (nodes) to reference a single mob therefore we make sure
  // follow the reference by comparing the tag.
  else if (backKind == Edge::EDGE_KIND_REFERENCE &&
               kind == Edge::EDGE_KIND_REFERENCE &&
                tag == _sourceTag.second )
  {
    assert( _sourceTag.first );
    return true;
  }

  // If we are traversing the last reference in series of reference
  // edges and this matches the reference tag then proceed.
  else if ( backKind == Edge::EDGE_KIND_REFERENCE &&
                kind == Edge::EDGE_KIND_LAST_REFERENCE &&
                 tag == _sourceTag.second )
  {
    assert( _sourceTag.first );
    _sourceTag.first = false;
    return true;
  }

  // If this last edge was the last reference (in a sequence of
  // references leading to a component) and the next is a containment
  // edge then we've arrived we want to invalidate the tag (because we
  // are nolonger traversing the reference) and follow the edge it to
  // the contained object.
  else if ( backKind == Edge::EDGE_KIND_LAST_REFERENCE &&
                kind == Edge::EDGE_KIND_CONTAINMENT )
  {
    assert( !_sourceTag.first );
    _sourceTag.first = false;
    return true;
  }
  
  // If the last edge was the last reference of a sequence and we have traversed
  // directly to a new reference enge then reset the tag and keep going. This
  // happens with a reference directly to a SourceClip.
  else if ( backKind == Edge::EDGE_KIND_LAST_REFERENCE &&
                kind == Edge::EDGE_KIND_REFERENCE )
  {
    assert( !_sourceTag.first );
    _sourceTag = make_pair(true,tag);
    return true;        
  }

  // Else, don't follow this edge.
  else
  {
    return false;
  }

}

} // end of namespace aafanalyzer


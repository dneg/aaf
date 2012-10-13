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

#ifndef __NODEREFCOUNTVISITOR_h__
#define __NODEREFCOUNTVISITOR_h__

//Edit Protocol Test files
#include <CompMobDependency.h>

//Test/Result files
#include <TestRegistry.h>

//AAF Analyzer Base files
#include <TypedVisitor.h>

//Analyzer Base files
#include <Node.h>

//STL files
#include <vector>
#include <map>

//Standard C files
#include <assert.h>

namespace aafanalyzer {

using namespace std;

// NodeRefCountVisitor is used to references to any single type of
// node in a graph. Simply instantiate it on the desired node type and
// execute a traversal over the graph.
//
// This was originally created to count references to composition
// mobs.
//
// To do that, instantiate:
//
//        NodeRefCountVisitor<IAAFCompsitionMob>
//
// .. and execute a traversal over the graph.
//

template <class AAFObjType>
class NodeRefCountVisitor : public TypedVisitor
{
 public:

  // Map node instance pointer to a reference count for that node.
  typedef map<boost::shared_ptr<Node>, int> NodeMap;

  // Map a reference count value to a vector of all nodes with that
  // reference count.
  typedef map<int, vector<Node::LID> > RefCountMap;

  typedef AAFTypedObjNode<AAFObjType> ReferencedNode;
  typedef boost::shared_ptr<ReferencedNode> ReferencedNodeSP;
  typedef vector<ReferencedNodeSP> ReferencedNodeVector;
  typedef boost::shared_ptr<ReferencedNodeVector> ReferencedNodeVectorSP;

  NodeRefCountVisitor( wostream& os )
    : TypedVisitor( Visitor::FOLLOW_REFERENCES ),
    _os(os)
  {}

  virtual ~NodeRefCountVisitor()
  {}

  bool PreOrderVisit( AAFTypedObjNode<AAFObjType>& node )
  {
    boost::shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    
    if ( IsPresent( spNode ) )
    {
      _nodeMap[spNode] = _nodeMap[spNode] + 1;
    }
    else
    {
      // We start the count at zero even though, strictly speaking,
      // this map entry is created as a result of the first entry not
      // the zeroeth. A ref count of zero implies that the object is
      // only referenced via a containment edge which implies it has
      // not external references. From that perspective zero makes
      // sense.
      _nodeMap[spNode] = 0;
    }

    return true;
  }
  
  // Returns a map that maps ref counts to all nodes with that ref count.
  // i.e. map[1] = vector<Node::LID> // a vector of unreferenced nodes
  // i.e. map[2] = vector<Node::LID> // a vector of nodes with one reference
  // i.e. map[3] = vector<Node::LID> // a vector of nodes with three references
  boost::shared_ptr<RefCountMap> GetRefCountMap()
  {
    boost::shared_ptr<RefCountMap> spRefCountMap( new RefCountMap );
    
    NodeMap::const_iterator iter;
    for( iter = _nodeMap.begin(); iter != _nodeMap.end(); ++iter )
    {
      (*spRefCountMap)[ iter->first->GetLID() ].push_back( iter->second );
    }
    
    return spRefCountMap;
  }

  static bool CompareEqual( int rhs, int lhs )
  {
    return rhs == lhs;
  }

  static bool CompareGreater( int rhs, int lhs )
  {
    return rhs > lhs;
  }

  ReferencedNodeVectorSP GetNodesComparedToCount( int refCount, bool (*CompareFunc)(int,int) )
  {
    ReferencedNodeVectorSP spNodes( new ReferencedNodeVector );

    // For each node, check if the ref count matches what we are
    // looking for if yes, then push a pointer to the node onto the
    // spNodes vector.
    NodeMap::const_iterator iter;
    for( iter = _nodeMap.begin(); iter != _nodeMap.end(); ++iter )
    {
      if ( CompareFunc(iter->second, refCount) )
      {
	boost::shared_ptr<Node> spNode = iter->first;
    
	ReferencedNodeSP spRefNode = boost::dynamic_pointer_cast<ReferencedNode>( spNode );
	assert( spRefNode );
	spNodes->push_back( spRefNode );
      }
    }
    
    return spNodes;
  }

  // Return a vector of all nodes with a particular reference count.

  ReferencedNodeVectorSP GetNodesWithCount( int refCount )
  {
    return GetNodesComparedToCount( refCount, CompareEqual );
  }

  // Return a vector of all nodes with a count greater than or equal to minRefCount.

  ReferencedNodeVectorSP GetNodesWithCountGreater( int refCount )
  {
    return GetNodesComparedToCount( refCount, CompareGreater );
  }

 private:

  // prohibited
  NodeRefCountVisitor();
  NodeRefCountVisitor( const NodeRefCountVisitor& );
  NodeRefCountVisitor& operator=( const NodeRefCountVisitor& );

  bool IsPresent(boost::shared_ptr<Node>& spNode)
  {
    NodeMap::const_iterator iter = _nodeMap.find(spNode);
    return iter != _nodeMap.end();
  }

  wostream& _os;
  NodeMap _nodeMap;
};

} // end of namespace diskstream

#endif

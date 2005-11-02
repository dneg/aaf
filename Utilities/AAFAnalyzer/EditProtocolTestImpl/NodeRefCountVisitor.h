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

#ifndef __NODEREFCOUNTVISITOR_h__
#define __NODEREFCOUNTVISITOR_h__

//Edit Protocol Test files
#include "CompMobDependency.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <Requirement.h>

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
using namespace boost;

// NodeRefCountVisitor is used to references to any single type of
// node in a graph. Simply instantiate it on the desired node type and
// execute a traversal over the graph.
//
// This was originally created to count references to composition mobs.
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

  // Map Node::LID to a reference count for that node.
  typedef map<shared_ptr<Node>, int> NodeMap;

  // Map a reference count value to a vector of all nodes with that
  // reference count.
  typedef map<int, vector<Node::LID> > RefCountMap;

  typedef AAFTypedObjNode<AAFObjType> ReferencedNode;
  typedef shared_ptr<ReferencedNode> ReferencedNodeSP;
  typedef vector<ReferencedNodeSP> ReferencedNodeVector;
  typedef shared_ptr<ReferencedNodeVector> ReferencedNodeVectorSP;

  NodeRefCountVisitor(wostream& os)
    : _os(os),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Contained Track Visitor",
                                            L"Visit derivation chain mateiral and ensure they contain the required tracks.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( CompMobDependency::GetTestInfo().GetName() )
               )                          )
  {}

  virtual ~NodeRefCountVisitor()
  {}

  bool PreOrderVisit(AAFTypedObjNode<AAFObjType>& node)
  {
    
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    
    if ( IsPresent( spNode ) )
    {
      _nodeMap[spNode] = _nodeMap[spNode] + 1;
    }
    else
    {
      _nodeMap[spNode] = 0;
    }

    return true;
  }

  // Returns a map that maps ref counts to all nodes with that ref count.
  // i.e. map[0] = vector<Node::LID> // a vector of unreferenced nodes
  // i.e. map[1] = vector<Node::LID> // a vector of nodes with one reference
  // i.e. map[2] = vector<Node::LID> // a vector of nodes with three references
  shared_ptr<RefCountMap> GetRefCountMap()
  {
    shared_ptr<RefCountMap> spRefCountMap( new RefCountMap );
    
    NodeMap::const_iterator iter;
    for( iter = _nodeMap.begin(); iter != _nodeMap.end(); ++iter )
    {
      (*spRefCountMap)[ iter->first->GetLID() ].push_back( iter->second );
    }
    
    return spRefCountMap;
  }

  // Return a vector of all nodes with a particular reference count.

  ReferencedNodeVectorSP GetNodesWithCount( int refCount )
  {
    ReferencedNodeVectorSP spNodes( new ReferencedNodeVector );

    // For each node, check if the ref count matches what we are
    // looking for if yes, then push a pointer to the node onto the
    // spNodes vector.
    NodeMap::const_iterator iter;
    for( iter = _nodeMap.begin(); iter != _nodeMap.end(); ++iter )
    {
      if ( iter->second == refCount )
      {
    shared_ptr<Node> spNode = iter->first;
    
	ReferencedNodeSP spRefNode = dynamic_pointer_cast<ReferencedNode>( spNode );
	assert( spRefNode );
	spNodes->push_back( spRefNode );
      }
    }
    
    return spNodes;
  }

  shared_ptr<DetailLevelTestResult> GetTestResult() const
  {
    return _spResult;
  }

 private:

  // prohibited
  NodeRefCountVisitor();
  NodeRefCountVisitor( const NodeRefCountVisitor& );
  NodeRefCountVisitor& operator=( const NodeRefCountVisitor& );

  bool IsPresent(shared_ptr<Node>& spNode)
  {
    NodeMap::const_iterator iter = _nodeMap.find(spNode);  
    return iter != _nodeMap.end();
  }

  wostream& _os;
  shared_ptr<DetailLevelTestResult> _spResult;
  NodeMap _nodeMap;   //<LID, ref count>
};

} // end of namespace diskstream

#endif

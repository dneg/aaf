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

#include "TypedVisitor.h"
#include "Node.h"

#include <TestResult.h>

#include <boost/shared_ptr.hpp>

//stl files
#include <iostream>
#include <vector>
#include <map>

namespace aafanalyzer {

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
  typedef std::map<Node::LID, int> NodeMap;

  // Map a reference count value to a vector of all nodes with that
  // reference count.
  typedef std::map<int, std::vector<Node::LID> > RefCountMap;

  NodeRefCountVisitor(std::ostream& os, TestResult& result)
    : _os(os),
     _result(result)
  {}

  virtual ~NodeRefCountVisitor()
  {}

  bool PreOrderVisit(AAFTypedObjNode<AAFObjType>& node)
  {
    Node::LID lid = node.GetLID();
    
    if(IsPresent(lid))
    {
      _nodeMap[lid] = _nodeMap[lid]++;
    }
    else
    {
      _nodeMap[lid] = 0;
    }

    return true;
  }

  boost::shared_ptr<RefCountMap> GetRefCountMap()
  {
    boost::shared_ptr<RefCountMap> spRefCountMap( new RefCountMap );
    
    NodeMap::const_iterator iter;
    for( iter = _nodeMap.begin(); iter != _nodeMap.end(); ++iter )
    {
      (*spRefCountMap)[ iter->first ].push_back( iter->second );
    }
    
    return spRefCountMap;
  }

 private:

  // prohibited
  NodeRefCountVisitor( const NodeRefCountVisitor& );
  NodeRefCountVisitor& operator=( const NodeRefCountVisitor& );

  bool IsPresent(unsigned int lid)
  {
    NodeMap::const_iterator iter = _nodeMap.find(lid);  
    return iter != _nodeMap.end();
  }

  std::ostream& _os;
  TestResult& _result;
  NodeMap _nodeMap;   //<LID, ref count>
};

} // end of namespace diskstream

#endif

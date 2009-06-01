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

#ifndef __VISITOR_h__
#define __VISITOR_h__

//Analyzer Base files
#include <Node.h>
#include <Edge.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <set>
#include <deque>

namespace aafanalyzer {

class Visitor
{
 public:
 
  enum Follow_e { FOLLOW_CONTAINED,
		  FOLLOW_REFERENCES,
		  FOLLOW_ALL,
		  FOLLOW_UNDEFINED };

  // Default ctor uses FOLLOW_CONTAINED
  Visitor();

  Visitor( Follow_e follow );

  virtual ~Visitor();

  virtual bool PreOrderVisit(Node& node);
  virtual bool PostOrderVisit(Node& node);
  virtual bool EdgeVisit(Edge& edge);

  unsigned int GetLevel() const;
  Edge::EdgeKind_e GetLastEdgeKind() const;
  
 protected:
 
  void RecordVisit( Node::LID id );
  bool HaveVisited( Node::LID id );

 private:

  // Non virtual pre/post order visit that must be called by traversal
  // implementations in order to support reference following.  There
  // will never me many traversal implementations. It could simple be
  // caused by a derived classes PostOrderVisit override but that just
  // one more thing to remember. This works too and simpler for
  // visitor implementors.

  friend class DepthFirstTraversal;
  bool TraversalPreEdgeVisit(const Edge& edge);
  void TraversalPostEdgeVisit(const Edge& edge);
  void TraversalStopVisit( const Node& node );

  // prohibited
  Visitor( const Visitor& );
  Visitor& operator=( const Visitor& );

  bool FollowContained( const Edge& edge );
  bool FollowReferences( const Edge &edge );

  bool RealFollowReferences( const Edge &edge );

  set<Node::LID> _visitedNodes;

  // What to do when following edges.
  const Follow_e _follow;

  typedef pair<Edge::EdgeKind_e, Node::LID> EdgeDetailsType;
  std::deque<EdgeDetailsType>  _edgeDetailsStack;

  pair<bool,Node::LID> _sourceTag;
};

} // end of namespace diskstream

#endif

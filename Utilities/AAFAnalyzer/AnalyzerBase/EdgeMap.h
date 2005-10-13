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

#ifndef __EDGEMAP_h__
#define __EDGEMAP_h__

//Analyzer Base files
#include "Node.h"

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <map>
#include <vector>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class Edge;

class EdgeMap
{
 public:

  typedef unsigned int LID;
  typedef vector<shared_ptr<Edge> > EdgeVector;
  typedef shared_ptr<EdgeVector> EdgeVectorSP;
  typedef map<Node::LID, EdgeVectorSP> Map;
  typedef shared_ptr<const EdgeVector> ConstEdgeVectorSP;

  EdgeMap();
  ~EdgeMap();

  ConstEdgeVectorSP GetParents(shared_ptr<Node> spNode) const;
  ConstEdgeVectorSP GetChildren(shared_ptr<Node> spNode) const;

  void AddEdge(shared_ptr<Edge> spEdge);
  void DecorateEdge(shared_ptr<Node> decoratedNode);

 private:
 
  bool IsInMap(Node::LID lid, const Map& map) const;

  mutable Map _parents;
  mutable Map _children;

  // prohibited
  EdgeMap( const EdgeMap& );
  EdgeMap& operator=( const EdgeMap& );
};

} // end of namespace diskstream

#endif

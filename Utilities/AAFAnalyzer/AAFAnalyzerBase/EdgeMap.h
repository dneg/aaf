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

//project files
#include "Node.h"

//boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <map>
#include <vector>

namespace aafanalyzer {

class Edge;

class EdgeMap
{
 public:

  typedef unsigned int LID;
  typedef std::vector<boost::shared_ptr<Edge> > EdgeVector;
  typedef boost::shared_ptr<EdgeVector> EdgeVectorSP;
  typedef std::map<Node::LID, EdgeVectorSP> Map;

  EdgeMap();
  ~EdgeMap();

  // Warning: A pointer to a vector stored in this class is returned
  // directly for performance reasons (rather than a copy). Modifying
  // the vector will invalidate the EdgeMap.
  // FIXME - Find way to declare the returned shared pointer as const.
  EdgeVectorSP GetParents(boost::shared_ptr<Node> spNode) const;
  EdgeVectorSP GetChildren(boost::shared_ptr<Node> spNode) const;

  void AddEdge(boost::shared_ptr<Edge> spEdge);

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

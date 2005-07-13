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

#ifndef __EDGE_h__
#define __EDGE_h__

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

class Visitor;
class Node;

class Edge
{
 public:
  Edge(boost::shared_ptr<Node> spParent, boost::shared_ptr<Node> spChild);
  virtual ~Edge();

  virtual bool Visit(boost::shared_ptr<Visitor> spVisitor);
  boost::shared_ptr<Node> GetParentNode() const;
  boost::shared_ptr<Node> GetChildNode() const;

 private:
  boost::shared_ptr<Node> _spParentNode;
  boost::shared_ptr<Node> _spChildNode;

  // prohibited
  Edge();
  Edge( const Edge& );
  Edge& operator=( const Edge& );
};

} // end of namespace diskstream

#endif

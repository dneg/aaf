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

#ifndef __EDGE_h__
#define __EDGE_h__

#include <Node.h>

#include <string>

//Boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace boost;

class Visitor;

class Edge
{
 public:

  enum EdgeKind_e { EDGE_KIND_CONTAINMENT,
                    EDGE_KIND_REFERENCE,
                    EDGE_KIND_LAST_REFERENCE,
                    EDGE_KIND_UNDEFINED };

  // containment kind and tag value equal to parent lid
  Edge( shared_ptr<Node> spParent, shared_ptr<Node> spChild );

  Edge( shared_ptr<Node> spParent, shared_ptr<Node> spChild,
        EdgeKind_e kind, Node::LID tag );

  virtual ~Edge();

  virtual bool Visit(shared_ptr<Visitor> spVisitor);
  shared_ptr<Node> GetParentNode() const;
  shared_ptr<Node> GetChildNode() const;

  // virtual constructor
  // uses the existing kind and tag values
  virtual shared_ptr<Edge> CreateNewEdge( shared_ptr<Node> spParent, shared_ptr<Node> spChild ) const;

  // custom type information
  virtual const std::wstring& GetTypeName() const;

  EdgeKind_e GetKind() const;
  Node::LID GetTag() const;

 private:

  // prohibited
  Edge();
  Edge( const Edge& );
  Edge& operator=( const Edge& );

  shared_ptr<Node> _spParentNode;
  shared_ptr<Node> _spChildNode;
  EdgeKind_e _kind;
  Node::LID _tag;
};

} // end of namespace diskstream

#endif

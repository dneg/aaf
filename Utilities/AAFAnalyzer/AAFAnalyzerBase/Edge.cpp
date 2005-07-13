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

#include "Edge.h"

#include "Node.h"
#include "Visitor.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

Edge::Edge(boost::shared_ptr<Node> spParent, boost::shared_ptr<Node> spChild)
  : _spParentNode( spParent ),
    _spChildNode( spChild )
{
}

Edge::~Edge()
{
}

bool Edge::Visit(boost::shared_ptr<Visitor> spVisitor)
{
  return spVisitor->EdgeVisit(*this);
}

boost::shared_ptr<Node> Edge::GetParentNode() const
{
  return _spParentNode;
}

boost::shared_ptr<Node> Edge::GetChildNode() const
{
  return _spChildNode;
}

} // end of namespace diskstream



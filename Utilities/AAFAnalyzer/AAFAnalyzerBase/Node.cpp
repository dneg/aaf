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

#include "Node.h"
#include "Visitor.h"

namespace aafanalyzer{

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{
Node::LID Node::_counter = 1; //set initial value

Node::Node()
  : _name( L"" ),
    _lid( _counter++ )
{
}

Node::Node(const std::basic_string<wchar_t>& name)
  : _name( name ),
    _lid( _counter++ )
{
}

Node::~Node()
{
}

bool Node::PreOrderVisit(boost::shared_ptr<Visitor> spVisitor)
{
  return spVisitor->PreOrderVisit(*this);
}

bool Node::PostOrderVisit(boost::shared_ptr<Visitor> spVisitor)
{
  return spVisitor->PostOrderVisit(*this);
}

Node::LID Node::GetLID() const
{
  return _lid;
}

const std::basic_string<wchar_t>& Node::GetName() const
{
  return _name;
}

}// end of namespace

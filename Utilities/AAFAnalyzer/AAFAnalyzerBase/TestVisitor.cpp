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

#include "TestVisitor.h"
#include "Node.h"
#include "Edge.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

TestVisitor::TestVisitor()
  : _level(0)
{
}

TestVisitor::~TestVisitor()
{
}

bool TestVisitor::PreOrderVisit(Node& node)
{
  _level++;
  std::wcout << "Pre: " << LevelToIndent(_level) << node.GetLID() << " " << node.GetName() << std::endl;
  return true;
}

bool TestVisitor::PostOrderVisit(Node& node)
{
  std::wcout << "Post:" << LevelToIndent(_level) << node.GetLID() << " " << node.GetName() << std::endl;
  _level--;
  return true;
}

bool TestVisitor::EdgeVisit(Edge& edge)
{
  std::wcout << "Edge:" << LevelToIndent(_level) << edge.GetParentNode()->GetLID()
    << "->"
    << edge.GetChildNode()->GetLID() << std::endl;

  return true;
}

std::basic_string<wchar_t> TestVisitor::LevelToIndent(unsigned int l)
{
  std::basic_string<wchar_t> indent;

  for (; l > 0 ; l-- ) 
  {
    indent += L" ";
  }
  return indent;
}


} // end of namespace diskstream

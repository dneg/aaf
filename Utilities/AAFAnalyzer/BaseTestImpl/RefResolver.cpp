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

#include "RefResolver.h"

#include <DepthFirstTraversal.h>
#include <ResolveRefVisitor.h>

//boost files
#include <boost/shared_ptr.hpp>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

RefResolver::RefResolver(std::ostream& os, boost::shared_ptr<TestGraph> spTestGraph)
: Test(os)
{
  SetTestGraph(spTestGraph);
}

RefResolver::~RefResolver()
{
}

TestResult RefResolver::Execute()
{
  TestResult result;
  boost::shared_ptr<ResolveRefVisitor> spVisitor(new ResolveRefVisitor(GetOutStream(), GetTestGraph()->GetEdgeMap(), result));
  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  //output to screen
  //GetOutStream() << GetName() << std::endl << GetDescription() << std::endl << std::endl;

  //set result properties
  result.SetName(GetName());
  result.SetDescription(GetDescription());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode());
  return result;
}

std::string RefResolver::GetName()
{
  std::string name = "--- Reference Resolver Test ---";
  return name;
}

std::string RefResolver::GetDescription()
{
  std::string description = "Test Description: Resolve all source clip references.";
  return description;
}


} // end of namespace diskstream

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

#include "CompMobDependency.h"

#include <NodeRefCountVisitor.h>
#include <DepthFirstTraversal.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

CompMobDependency::CompMobDependency(std::ostream& os, boost::shared_ptr<TestGraph> spTestGraph)
: Test(os)
{
  SetTestGraph(spTestGraph);
}

CompMobDependency::~CompMobDependency()
{
}

TestResult CompMobDependency::Execute()
{
  TestResult result;

  boost::shared_ptr<NodeRefCountVisitor<IAAFCompositionMob> > spVisitor(
       new NodeRefCountVisitor<IAAFCompositionMob>( GetOutStream(), result) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  //output to screen
  //GetOutStream() << GetName() << std::endl << GetDescription() << std::endl << std::endl;

  //set result properties
  result.SetName(GetName());
  result.SetDescription(GetDescription());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode()); 
  return result;
}

std::string CompMobDependency::GetName()
{
  std::string name = "--- CompositionMob Dependency Test ---";
  return name;
}

std::string CompMobDependency::GetDescription()
{
  std::string description = "Test Description: Traverse the directed graph and count composition mob references.";
  return description;
}


} // end of namespace diskstream

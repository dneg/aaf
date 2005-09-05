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

CompMobDependency::CompMobDependency(std::wostream& os, boost::shared_ptr<TestGraph> spTestGraph)
: Test(os)
{
  SetTestGraph(spTestGraph);
}

CompMobDependency::~CompMobDependency()
{
}

boost::shared_ptr<TestResult> CompMobDependency::Execute()
{

  boost::shared_ptr<TestResult> spVisitorResult(new TestResult());
  boost::shared_ptr<NodeRefCountVisitor<IAAFCompositionMob> > spVisitor(
       new NodeRefCountVisitor<IAAFCompositionMob>( GetOutStream(), *spVisitorResult) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  //output to screen
  //GetOutStream() << GetName() << std::endl << GetDescription() << std::endl << std::endl;

  //set result properties
  boost::shared_ptr<TestResult> spResult(new TestResult());
  spResult->SetName(GetName());
  spResult->SetDescription(GetDescription());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode()); 
  
  spResult->AppendSubtestResult(spVisitorResult);
  spResult->SetResult(spResult->GetAggregateResult());

  _spRootCompMobs = spVisitor->GetNodesWithCount(0);

  return spResult;
}

AxString CompMobDependency::GetName() const
{
  AxString name = L"CompositionMob Dependency Test";
  return name;
}

AxString CompMobDependency::GetDescription() const
{
  AxString description = L"Traverse the directed graph and count composition mob references.";
  return description;
}

CompMobDependency::CompMobNodeVectorSP CompMobDependency::GetRootCompMobNodes()
{
  return _spRootCompMobs;
}

} // end of namespace diskstream

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
#include <DetailLevelTestResult.h>

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

RefResolver::RefResolver(std::wostream& os, boost::shared_ptr<TestGraph> spTestGraph)
: Test(os, GetTestInfo())
{
  SetTestGraph(spTestGraph);
}

RefResolver::~RefResolver()
{
}

boost::shared_ptr<TestLevelTestResult> RefResolver::Execute()
{
  //TestResult visitorResult;
  boost::shared_ptr<ResolveRefVisitor> spVisitor(new ResolveRefVisitor(GetOutStream(), GetTestGraph()->GetEdgeMap()));
  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode());

  const boost::shared_ptr<const Test> me = this->shared_from_this();
  Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(this->GetCoveredRequirements()));
  boost::shared_ptr<TestLevelTestResult> spResult(
            new TestLevelTestResult( L"ReferenceResolver",
                                     L"Resolves source references in an AAF file.",
                                     spVisitor->GetTestResult()->GetExplanation(),
                                     L"-", //DOCREF
                                     spVisitor->GetTestResult()->GetResult(),
                                     me, 
                                     spMyReqs ) ); 
  boost::shared_ptr<const DetailLevelTestResult> spVisitorResult( spVisitor->GetTestResult() );
  spResult->AppendSubtestResult(spVisitorResult);

  return spResult;
}

AxString RefResolver::GetName() const
{
  AxString name = L"--- Reference Resolver Test ---";
  return name;
}

AxString RefResolver::GetDescription() const
{
  AxString description = L"Test Description: Resolve all source clip references.";
  return description;
}

const TestInfo RefResolver::GetTestInfo()
{
    boost::shared_ptr<std::vector<AxString> > spReqIds(new std::vector<AxString>);
//    spReqIds->push_back(L"Requirement Id");
    return TestInfo(L"RefResolver", spReqIds);
}

} // end of namespace diskstream

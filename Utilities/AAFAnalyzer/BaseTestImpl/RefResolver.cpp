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

//Base Test files
#include "RefResolver.h"
#include "ResolveRefVisitor.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>
#include <TestGraph.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

RefResolver::RefResolver(wostream& os, boost::shared_ptr<const TestGraph> spGraph)
: Test(os, GetTestInfo())
{
  SetTestGraph(spGraph);
}

RefResolver::~RefResolver()
{
}

boost::shared_ptr<TestLevelTestResult> RefResolver::Execute()
{
  boost::shared_ptr<TestLevelTestResult> spTestResult = this->CreateTestResult();

  boost::shared_ptr<ResolveRefVisitor>
    spVisitor(new ResolveRefVisitor(GetOutStream(),
                                    GetTestGraph()->GetEdgeMap(),
                                    spTestResult ));

  DepthFirstTraversal dfs( GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode() );

  // Traverse the data structure resolving source clip references as
  // they're found. The visitor adds detailed test results as along
  // the way.
  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode());

  return spTestResult;
}

AxString RefResolver::GetName() const
{
  AxString name = L"Reference Resolver Test";
  return name;
}

AxString RefResolver::GetDescription() const
{
  AxString description = L"Resolve source clip references.";
  return description;
}

const TestInfo RefResolver::GetTestInfo()
{
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    // Export Mobs for Entire Derivation Chain
    // (Warn if source references exists for mobs that are not in the
    // file.)
    spReqIds->push_back( L"REQ_EP_016" );

    // Slot Must Exist In Referenced Mob
    // (Fail if a source reference does indentfy a mob in the file but
    // identifies a slot that does not exist in that mob.)
    spReqIds->push_back( L"REQ_EP_257" );

    return TestInfo(L"RefResolver", spReqIds);
}

} // end of namespace diskstream

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

//Edit Protocol Test files
#include "CompMobDependency.h"
#include "NodeRefCountVisitor.h"

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>

//AAF Analyzer Base files
#include <TestGraph.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

#include <sstream>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

CompMobDependency::CompMobDependency(wostream& os, boost::shared_ptr<const TestGraph> spGraph)
  : Test(os, GetTestInfo() ),
    _spRootCompMobs(),
    _spNonRootCompMobs()
{
  SetTestGraph(spGraph);
}

CompMobDependency::~CompMobDependency()
{
}

boost::shared_ptr<TestLevelTestResult> CompMobDependency::Execute()
{
  // Execute NodeRefCountVisitor<IAAFCompositionMob> to the number of
  // references to composition mobs that exist in the graph.

  boost::shared_ptr<NodeRefCountVisitor<IAAFCompositionMob> >
    spVisitor( new NodeRefCountVisitor<IAAFCompositionMob>( GetOutStream() ) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), GetTestGraph()->GetRootNode());

  dfs.TraverseDown(spVisitor, GetTestGraph()->GetRootNode());
  
  // This currently can't fail. We create a result object to append
  // some details that are useful and because we have to return
  // through this interface.
  boost::shared_ptr<TestLevelTestResult> spTestLevelResult = CreateTestResult( L"", // explain
									TestResult::PASS );

  // Pull out, and save, the unreferenced composition mobs.
  _spRootCompMobs = spVisitor->GetNodesWithCount(0);

  // Record the result as test detail for reference purposes.
  wstringstream ss;
  // (static_cast is there to quiet compiler warning)
  ss << L"found " << static_cast<unsigned int>(_spRootCompMobs->size())
	 << L" unreferenced composition mob";
  if ( _spRootCompMobs->size() > 1 )
  {
    ss << L"s";
  }

  spTestLevelResult->AddDetail( ss.str() );

  // And the non root comp mobs.
  _spNonRootCompMobs = spVisitor->GetNodesWithCountGreater(0);

  return spTestLevelResult;
}

AxString CompMobDependency::GetName() const
{
  AxString name = L"CompositionMob Dependency Analysis";
  return name;
}

AxString CompMobDependency::GetDescription() const
{
  AxString description = L"Traverse the directed graph, count composition mob references, "
                         L"and identify the unreferenced composition mobs.";
  return description;
}

CompMobDependency::CompMobNodeVectorSP CompMobDependency::GetRootCompMobNodes()
{
  return _spRootCompMobs;
}

CompMobDependency::CompMobNodeVectorSP CompMobDependency::GetNonRootCompMobNodes()
{
  return _spNonRootCompMobs;
}

const TestInfo CompMobDependency::GetTestInfo()
{
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    // No requirements are actually tested here. This test is simply
    // used within the load phase to isolate the graph roots for a
    // later mob dependency test.

    return TestInfo(L"CompMobDependency", spReqIds);
}

} // end of namespace diskstream

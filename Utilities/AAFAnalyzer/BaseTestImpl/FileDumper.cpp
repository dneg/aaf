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
#include "FileDumper.h"
#include "DumpVisitor.h"

//Test/Result files
#include <TestLevelTestResult.h>

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
using namespace boost;

FileDumper::FileDumper( wostream& os,
			shared_ptr<const TestGraph> spGraph,
			shared_ptr<Node> spRoot,
			bool followReferences )
  : Test(os, GetTestInfo()),
    _spRoot(spRoot),
    _followReferences(followReferences)
{
  SetTestGraph(spGraph);
}

FileDumper::~FileDumper()
{}

shared_ptr<TestLevelTestResult> FileDumper::Execute()
{
  shared_ptr<DumpVisitor>
    spVisitor( new DumpVisitor( _followReferences ? Visitor::FOLLOW_REFERENCES : Visitor::FOLLOW_CONTAINED ) );

  DepthFirstTraversal dfs(GetTestGraph()->GetEdgeMap(), _spRoot);

  dfs.TraverseDown(spVisitor);

  // This is just a file dump. It can't fail. We don't really even
  // care about the return, since it is implemented as a test it is
  // necessary to return a result.
  shared_ptr<TestLevelTestResult>
    spResult( new TestLevelTestResult( GetName(),
				       GetDescription(),
				       L"",
				       TestResult::PASS,
				       this->shared_from_this() ) );
  return spResult;
}

AxString FileDumper::GetName() const
{
  return L"File Dump";
}

AxString FileDumper::GetDescription() const
{
  return L"Dump the analyzer\'s internal data structure.";
}

const TestInfo FileDumper::GetTestInfo()
{
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    // No requirements to register here. This is just a debug utility
    // that happens to be wrapped up as a test.

    return TestInfo(L"FileDumper", spReqIds);
}

} // end of namespace diskstream

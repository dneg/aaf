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

//Test/Result files
#include "TestLevelTestResult.h"
#include "DetailLevelTestResult.h"
#include "Test.h"
#include "TestRegistry.h"

//Requirement files
#include <Requirement.h>
#include <RequirementMismatchException.h>

#include <iostream>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

TestLevelTestResult::TestLevelTestResult( const boost::shared_ptr<const Test> associatedTest )
  : LowLevelTestResult( associatedTest )
{
  ProtectedSetResult(PASS);
}

TestLevelTestResult::TestLevelTestResult( const wstring& name,
                                          const wstring& desc,
                                          const wstring& explain,
                                          Result result,
                                          const boost::shared_ptr<const Test> associatedTest )
  : LowLevelTestResult( associatedTest, name, desc, explain )
{
  ProtectedSetResult(result);
}

TestLevelTestResult::TestLevelTestResult( const wstring& name,
                                          const wstring& desc,
                                          const boost::shared_ptr<const Test> associatedTest )
  : LowLevelTestResult( associatedTest, name, desc, L"" )
{
  ProtectedSetResult(PASS);
}

TestLevelTestResult::~TestLevelTestResult()
{}

const enum TestResult::ResultLevel TestLevelTestResult::GetResultType() const
{
  return TestResult::TEST;
}


boost::shared_ptr<DetailLevelTestResult> TestLevelTestResult::AddSingleResult( const wstring& reqId,
                                                                        const wstring& explain,
                                                                        Result result,
									Node& node )
{
  boost::shared_ptr<DetailLevelTestResult>
    spResult( new DetailLevelTestResult( this->GetAssociatedTest(),
                                         explain,
                                         reqId,
                                         result,
					 node.GetSharedPointerToNode() ) );

  this->AddSubtestResult( spResult );

  return spResult;
}

boost::shared_ptr<DetailLevelTestResult> TestLevelTestResult::AddUnassociatedSingleResult( const wstring& reqId,
										    const wstring& explain,
										    Result result )
{
  boost::shared_ptr<DetailLevelTestResult>
    spResult( new DetailLevelTestResult( this->GetAssociatedTest(),
                                         explain,
                                         reqId,
                                         result,
					 boost::shared_ptr<Node>() ) );

  this->AddSubtestResult( spResult );

  return spResult;
}

void TestLevelTestResult::InitConsolidateResults()
{
  const Requirement::RequirementMap& coveredReqs = GetAssociatedTest()->GetCoveredRequirements();

  // We start by with each test requirement assigned a PASS result.
  // If/when the individual test add PASS detail results explicitely
  // then this will have to be removed.
  for( Requirement::RequirementMap::const_iterator iter = coveredReqs.begin();
       iter != coveredReqs.end();
       ++iter )
  {
    this->AddRequirement( COVERED, iter->second );
  }

  // And set the overall result to pass. If any test add WARN, or
  // FAIL, results this will be reset.
  ProtectedSetResult( PASS );
}

} // end of namespace diskstream

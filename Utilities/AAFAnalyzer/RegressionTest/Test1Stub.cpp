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

#include "Test1Stub.h"

#include <vector>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

Test1Stub::Test1Stub(std::wostream& os)
: Test(os, GetTestInfo())
{
}

Test1Stub::~Test1Stub()
{
}

boost::shared_ptr<TestLevelTestResult> Test1Stub::Execute()
{
  const boost::shared_ptr<const Test> me = this->shared_from_this();
  Requirement::RequirementMapSP spMyReqs(new Requirement::RequirementMap(this->GetCoveredRequirements()));
  boost::shared_ptr<TestLevelTestResult> spResult(new TestLevelTestResult(me, spMyReqs ));
  spResult->SetName(GetName());
  spResult->SetDescription(GetDescription());

  return spResult;
}

AxString Test1Stub::GetName() const
{
  AxString name = L"Test 1";
  return name;
}

AxString Test1Stub::GetDescription() const
{
  AxString description = L"For Regression Testing.";
  return description;
}

const TestInfo Test1Stub::GetTestInfo()
{
    boost::shared_ptr<std::vector<AxString> > spReqIds(new std::vector<AxString>);
    spReqIds->push_back(L"TEST08");
    spReqIds->push_back(L"TEST01");
    spReqIds->push_back(L"TEST05");
    return TestInfo(L"Test1Stub", spReqIds);
}

} // end of namespace aafanalyzer

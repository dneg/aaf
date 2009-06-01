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

//Regression Test Structure files
#include "Test1Stub.h"

//Test/Result files
#include <TestLevelTestResult.h>

//STL files
#include <vector>

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

Test1Stub::Test1Stub(wostream& os)
: Test(os, GetTestInfo())
{
}

Test1Stub::~Test1Stub()
{
}

shared_ptr<TestLevelTestResult> Test1Stub::Execute()
{
  const shared_ptr<const Test> me = this->shared_from_this();
  shared_ptr<TestLevelTestResult> spResult(new TestLevelTestResult(me));
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
    shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);
    spReqIds->push_back(L"TEST08");
    spReqIds->push_back(L"TEST01");
    spReqIds->push_back(L"TEST05");
    return TestInfo(L"Test1Stub", spReqIds);
}

} // end of namespace aafanalyzer

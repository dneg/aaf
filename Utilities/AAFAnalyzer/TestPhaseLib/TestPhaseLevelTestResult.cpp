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

//Test/Result files
#include "TestPhaseLevelTestResult.h"
#include "TestLevelTestResult.h"

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

TestPhaseLevelTestResult::TestPhaseLevelTestResult()
  : HighLevelTestResult()
{}

TestPhaseLevelTestResult:: TestPhaseLevelTestResult( const wstring& name,
                                                     const wstring& desc,
                                                     const wstring& explain )
  : HighLevelTestResult( name, desc, explain )
{}

TestPhaseLevelTestResult::~TestPhaseLevelTestResult()
{}

void TestPhaseLevelTestResult::AppendSubtestResult( shared_ptr<TestLevelTestResult> subtestResult )
{
  this->AddSubtestResult( subtestResult );
}

const enum TestResult::ResultLevel TestPhaseLevelTestResult::GetResultType() const
{
  return TestResult::PHASE;
}

} // end of namespace diskstream

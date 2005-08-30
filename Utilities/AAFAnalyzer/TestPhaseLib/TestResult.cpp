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

#include "TestResult.h"
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

// Default result is FAIL.
// Test implementations must explicity indicate success.
// Default result of subtests is PASS, it is updated whenever a new subtest is
// attached to the worst result.  It does not affect the result unless it is
// done explicitly outside of this file.
TestResult::TestResult()
  : _result( FAIL ), 
    _spSubtestResults(new SubtestResultVector()),
    _aggregateEnumResult( PASS )
{}

TestResult::TestResult( const AxString& name,
			const AxString& desc,
			const AxString& explain,
			const AxString& docRef,
			Result defaultResult )
  : _name( name ),
    _desc( desc ),
    _expl( explain ),
    _docRef( docRef ),
    _result( defaultResult ),
    _spSubtestResults(new SubtestResultVector()),
    _aggregateEnumResult( PASS )
{}

TestResult::~TestResult()
{}

TestResult& TestResult::operator=(const TestResult& test)
{
  if(this != &test)
  {
    _expl = test._expl;
    _name = test._name;
    _desc = test._desc;
    _result = test._result;
    _spSubtestResults = test._spSubtestResults;
  }

  return *this;
}

const AxString& TestResult::GetExplanation() const
{
  return _expl;
}

const AxString& TestResult::GetDocumentRef() const
{
  return _docRef;
}

const AxString& TestResult::GetName() const
{
  return _name;
}

const AxString& TestResult::GetDescription() const
{
  return _desc;
}

enum TestResult::Result TestResult::GetResult() const
{
  return _result;
}

void TestResult::SetExplanation(const AxString& exp)
{
  _expl = exp;
}

void TestResult::SetName(const AxString& name)
{
  _name = name;
}

void TestResult::SetDescription(const AxString& desc)
{
  _desc = desc;
}

void TestResult::SetResult(Result result)
{
  //can only be set to success, warning, or failure
  _result = result;
}

TestResult::SubtestResultsSP TestResult::GetSubtestResults() const {
    return _spSubtestResults;
}

void TestResult::AppendSubtestResult( boost::shared_ptr<const TestResult> subtestResult ) {

    _spSubtestResults->push_back( subtestResult );

    //If the result of the appended test is worse than any other subtest then
    //update the aggregate result.
    if ( subtestResult->GetResult() > _aggregateEnumResult ) {
        _aggregateEnumResult = subtestResult->GetResult();
    }

}

enum TestResult::Result TestResult::GetAggregateResult() const {
    return _aggregateEnumResult;
}

bool TestResult::ContainsSubtests() const {
    return !_spSubtestResults->empty();
}

} // end of namespace diskstream

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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

#ifndef __TEST_h__
#define __TEST_h__

//Test/Result files
#include <TestInfo.h>
#include <TestLevelTestResult.h>

//Requirement files
#include <Requirement.h>

//Boost files
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

//STL files
#include <string>
#include <iostream>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class TestGraph;

class Test : public enable_shared_from_this<Test>
{
 public:
  Test(wostream& os, const TestInfo& info);
  virtual ~Test();

  virtual shared_ptr<TestLevelTestResult> Execute() = 0;
  virtual wstring GetName() const;
  virtual wstring GetDescription() const;
  wostream& GetOutStream() const;
  shared_ptr<const TestGraph> GetTestGraph();

  // Get the requirements registered against this test.
  const Requirement::RequirementMap& GetCoveredRequirements() const;

  // Get a single requirement that is registered against this test.
  // The call asserts that reqId is registered.
  shared_ptr<const Requirement> GetRequirement( const wstring& reqId ) const;

  // Is reqId registered against this test.
  bool IsRequirementRegistered( const wstring& reqId ) const;

protected:

  void SetTestGraph(shared_ptr<const TestGraph> spGraph);

  // Create a result for the purpose of add sub results.
  // The result name and description come from this test.
  // The default result value is that provided by the TestResult
  // constructor.
  shared_ptr<TestLevelTestResult> CreateTestResult() const;

  // Create a result for tests that don't add sub results (hence must
  // provide the test result value and an explanation).
  // The result name and description come from this test.
  shared_ptr<TestLevelTestResult> CreateTestResult( const wstring& explain,
                                                    TestResult::Result result ) const;

  // Same as above, but the result is PASS and has no explanation.
  // This is common for test that are simply used to wrap up some processing
  // in a standard interface, can't fail on (and don't register any requirements),
  // either pass or fail fatally (throw an exception).
  shared_ptr<TestLevelTestResult> CreatePassTestResult() const;

 private:
  // prohibited
  Test();
  Test(const Test&);
  Test& operator=( const Test& );

  wostream& _os;
  shared_ptr<const TestGraph> _spGraph;
  
  typedef shared_ptr<const Requirement::RequirementMap> ConstRequirementMapSP;
  const ConstRequirementMapSP _spCoveredRequirements;

  const ConstRequirementMapSP InitializeRequirements(const wstring& name);
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

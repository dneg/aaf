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

#ifndef __TESTRESULT_h__
#define __TESTRESULT_h__

//Requirement files
#include <Requirement.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <string>
#include <vector>

#include <iosfwd>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class TestResult
{
 public:

  typedef vector< shared_ptr<TestResult> > SubtestResultVector;
  typedef shared_ptr<SubtestResultVector> SubtestResultsSP;

  // The actual test result.  UNDEFINED is the default result when no
  // result value is provided by the constructor.  The left most
  // (minimum value) should always be UNDEFINED, and the rightmost
  // (maxium value) should alwasy be FAIL.
  enum Result      {UNDEFINED, COVERED, NOTED, PASS, WARN, FAIL};

  // The specialized type of this result.
  enum ResultLevel {TOP, PHASE, TEST, DETAIL};

  virtual ~TestResult();

  // The name of this test result (short string).
  const wstring& GetName() const;
  void SetName(const wstring& name);

  // A verbose description of this test result.
  const wstring& GetDescription() const;
  void SetDescription(const wstring& desc);

  // An detailed explanation of the result.
  const wstring& GetExplanation() const;
  void SetExplanation(const wstring& exp);
  
  // The test result. If this is result has children then
  // this is the agreggate result. (i.e. the worst result 
  // amount all the children.
  Result GetResult() const;

  // This test's child results.
  const SubtestResultVector& GetSubtestResults() const;

  // True if children exist.
  bool ContainsSubtests() const;
  
  // Get the PASS, WARN, or FAIL requirements.
  const Requirement::RequirementMap& GetRequirements( Result type ) const;

  // Get the kind of specialized result this is.
  virtual const enum ResultLevel GetResultType() const =0;
  
  // Add an abitrary detail to a result. Any number of details can
  // be added.
  void AddDetail( const wstring& detail );
  const vector<wstring>& GetDetails() const;

  // Consolidate test result for this node and all children. This
  // propogates the test results, and related requirements, from leaf
  // results up to the this result. Typically this is called once from
  // the top level result after analysis completes but before
  // reporting the results.
  void ConsolidateResults();

  // Dump the pass/warn/fail requirements. This is primarily.
  // intended for debug purposes.
  void Dump( const wstring& prefix, wostream& os ) const;

  // Does reqId have status "result" in this test result instance?
  bool HasResult( const wstring& reqId, Result result ) const;

 protected:
 
  TestResult();

  TestResult( const wstring& name,
              const wstring& desc,
              const wstring& explain );
              
  // A child of this test. The specialized TestResult implementations
  // have interfaces that ensure only child of the correct type
  // can be added to a particular specialized type.
  void AddSubtestResult( shared_ptr<TestResult> subtestResult);

  // Returns true if reqID (e.g. REQ_EP_123) is contain in any one of
  // the result maps.
  bool ContainsRequirement( const wstring& reqId, Result& outContainedIn );

  const Requirement::RequirementMapSP& GetMyRequirements( Result type );

  // Called by specialized TestResult instances that provide a
  // SetResult interface.
  void ProtectedSetResult( Result result );

  // Clear/Add/Remove - used to implement consolidate.
  // Used to implement consolidate.
  // JPT REVIEW - Remove when the deprecated LowLevelResult::SetRequirementStatus is removed.
  void AddRequirement( Result type, const shared_ptr<const Requirement>& req );
  void RemoveRequirement( const wstring& id );

 private:

  // prohibited
  TestResult( const TestResult& );
  TestResult& operator=( const TestResult& );

  // Used by consolidate.
  void ClearRequirements();

  // Add requirement if it doesn't exist in the status maps. If it
  // does exist then replace it with the new status.
  void SetRequirementStatus( Result type, const shared_ptr<const Requirement>& req );

  
  // This called before sub results are consildated. It gives them a
  // chance to do what ever inititialization is appropriated. For
  // example, and specifically, it is used by the TestLevelResult to
  // initialize all its associated test requirements as passed.
  virtual void InitConsolidateResults();
              
  wstring _name;
  wstring _desc;
  wstring _expl;

  enum Result _result;
 
  SubtestResultsSP _spSubtestResults; 
  
  const Requirement::RequirementMapSP _spCoveredRequirements;
  const Requirement::RequirementMapSP _spNotedRequirements;
  const Requirement::RequirementMapSP _spPassedRequirements;
  const Requirement::RequirementMapSP _spWarnedRequirements;
  const Requirement::RequirementMapSP _spFailedRequirements;
  const Requirement::RequirementMapSP _spUndefinedRequirements;

  // Arbitrary comments that can be added to any result.
  vector<wstring> _details;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

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

namespace aafanalyzer {

using namespace std;
using namespace boost;

class TestResult
{
 public:

  enum Result      {PASS, WARN, FAIL};
  enum ResultLevel {TOP, PHASE, TEST, DETAIL};

  virtual ~TestResult();

  const wstring& GetExplanation() const;
  const wstring& GetDocumentRef() const;
  const wstring& GetName() const;
  const wstring& GetDescription() const;
  enum Result GetResult() const;

  void SetName(const wstring& name);
  void SetDescription(const wstring& desc);
  void SetExplanation(const wstring& exp);
  void SetResult(Result result);
  
  typedef vector< shared_ptr<const TestResult> > SubtestResultVector;
  typedef shared_ptr<SubtestResultVector> SubtestResultsSP;

  const SubtestResultVector& GetSubtestResults() const;
  enum Result GetAggregateResult() const;
  bool ContainsSubtests() const;
  
  const Requirement::RequirementMap& GetRequirements( Result type ) const;
  virtual const enum ResultLevel GetResultType() const =0;
  
  void AddDetail( const wstring& detail );
  const vector<wstring>& GetDetails() const;

 protected:
 
  TestResult();
  TestResult( const wstring& name, const wstring& desc,
              const wstring& explain, const wstring& docRef,
              Result defaultResult );  
  TestResult( const Requirement::RequirementMapSP& requirements );
  TestResult( const wstring& name, const wstring& desc,
              const wstring& explain, const wstring& docRef,
              Result defaultResult, const Requirement::RequirementMapSP& requirements);
              
  void AddSubtestResult( shared_ptr<const TestResult> subtestResult);
  void SetEnumResult( Result enumResult );
  bool ContainsRequirment( const wstring& id, Result& outContainedIn );
  void ClearRequirements();
  void AddRequirement( Result type, const shared_ptr<const Requirement>& req );
  void RemoveRequirement( const wstring& id );
  const Requirement::RequirementMapSP& GetMyRequirements( Result type );
              
 private:

  wstring _name;
  wstring _desc;
  wstring _expl;
  wstring _docRef;
  enum Result _result;
 
  SubtestResultsSP _spSubtestResults; 
  enum Result _aggregateEnumResult;
  
  const Requirement::RequirementMapSP _spPassedRequirements;
  const Requirement::RequirementMapSP _spWarnedRequirements;
  const Requirement::RequirementMapSP _spFailedRequirements;

  // Arbitrary comments that can be added to any result.
  vector<wstring> _details;

  // prohibited
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

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

//Project files
#include "Test.h"

//Ax files
#include <AxTypes.h>

//stl files
#include <string>
#include <vector>

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace std;

class TestResult
{
 public:

  enum Result      {PASS, WARN, FAIL};
  enum ResultLevel {TOP, PHASE, TEST, DETAIL};

  virtual ~TestResult();

  const AxString& GetExplanation() const;
  const AxString& GetDocumentRef() const;
  const AxString& GetName() const;
  const AxString& GetDescription() const;
  enum Result GetResult() const;

  void SetName(const AxString& name);
  void SetDescription(const AxString& desc);
  void SetExplanation(const AxString& exp);
  void SetResult(Result result);
  
  typedef std::vector< boost::shared_ptr<const TestResult> > SubtestResultVector;
  typedef boost::shared_ptr<SubtestResultVector> SubtestResultsSP;

  const SubtestResultVector& GetSubtestResults() const;
  enum Result GetAggregateResult() const;
  bool ContainsSubtests() const;
  
  const Requirement::RequirementMap& GetRequirements( Result type ) const;
  virtual const enum ResultLevel GetResultType() const =0;
  
  void AddDetail( const AxString& detail );
  const vector<AxString>& GetDetails() const;

 protected:
 
  TestResult();
  TestResult( const AxString& name, const AxString& desc,
              const AxString& explain, const AxString& docRef,
              Result defaultResult );  
  TestResult( const Requirement::RequirementMapSP& requirements );
  TestResult( const AxString& name, const AxString& desc,
              const AxString& explain, const AxString& docRef,
              Result defaultResult, const Requirement::RequirementMapSP& requirements);
              
  void AddSubtestResult( boost::shared_ptr<const TestResult> subtestResult);
  void SetEnumResult( Result enumResult );
  bool ContainsRequirment( const AxString& id, Result& outContainedIn );
  void ClearRequirements();
  void AddRequirement( Result type, const boost::shared_ptr<const Requirement>& req );
  void RemoveRequirement( const AxString& id );
  const Requirement::RequirementMapSP& GetMyRequirements( Result type );
              
 private:

  AxString _name;
  AxString _desc;
  AxString _expl;
  AxString _docRef;
  enum Result _result;
 
  SubtestResultsSP _spSubtestResults; 
  enum Result _aggregateEnumResult;
  
  const Requirement::RequirementMapSP _spPassedRequirements;
  const Requirement::RequirementMapSP _spWarnedRequirements;
  const Requirement::RequirementMapSP _spFailedRequirements;

  // Arbitrary comments that can be added to any result.
  vector<AxString> _details;

  // prohibited
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

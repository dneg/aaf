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

#ifndef __TESTLEVELTESTRESULT_h__
#define __TESTLEVELTESTRESULT_h__

//Test/Result files
#include "LowLevelTestResult.h"

namespace aafanalyzer {

using namespace std;
using namespace boost;

class DetailLevelTestResult;
class Test;

class TestLevelTestResult : public LowLevelTestResult
{
 public:

  TestLevelTestResult( const shared_ptr<const Test>& associatedTest,
                       const Requirement::RequirementMapSP& requirements );
  TestLevelTestResult( const wstring& name, const wstring& desc,
                       const wstring& explain, const wstring& docRef,
                       Result defaultResult,
                       const shared_ptr<const Test>& associatedTest,
                       const Requirement::RequirementMapSP& requirements );
  ~TestLevelTestResult();

  void AppendSubtestResult( const shared_ptr<const DetailLevelTestResult>& subtestResult );
  const enum ResultLevel GetResultType() const;
 private:
 
  const shared_ptr<const Test> _spAssociatedTest;
   
  void FindRequirementMismatch( const Requirement::RequirementMap& source,
                                const Requirement::RequirementMap& matchWith,
                                const wstring& sourceName,
                                const wstring& matchName ) const;
  void CheckDescendantRequirements( const shared_ptr<const TestResult>& subtestResult, const wstring& name );
   
  // prohibited
  TestLevelTestResult( const TestLevelTestResult& );
  TestLevelTestResult operator=( const TestLevelTestResult& );
  
};

} // end of namespace diskstream

#endif/*__TESTLEVELTESTRESULT_h__*/

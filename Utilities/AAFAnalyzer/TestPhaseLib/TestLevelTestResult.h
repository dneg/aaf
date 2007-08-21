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

  TestLevelTestResult( const shared_ptr<const Test> associatedTest );

  // Use this constructor to create a test result that is not intended
  // to parent a set of sub results.  If detailed results are added
  // then there aggregate test result will replace the result value
  // passed to the constructor.
  TestLevelTestResult( const wstring& name,
		       const wstring& desc,
                       const wstring& explain,
		       Result result,
                       const shared_ptr<const Test> associatedTest );

  // Use this constructor if you will be adding DetailLevelTestResult
  // objects (in which case this objects result is the aggregate of the 
  // sub results).
  TestLevelTestResult( const wstring& name,
		       const wstring& desc,
                       const shared_ptr<const Test> associatedTest );

  ~TestLevelTestResult();

  const enum ResultLevel GetResultType() const;

  // Add a single result for one requirement.
  shared_ptr<DetailLevelTestResult> AddSingleResult( const wstring& reqId,
						     Result result,
						     const wstring& resultName,
						     const wstring& resultDesc,
						     const wstring& resultExplain );


  // Add a single result for one requirement. Result name
  // and description are those of the requirment. Call must explain
  // the result.
  shared_ptr<DetailLevelTestResult> AddSingleResult( const wstring& reqId,
                                                     const wstring& resultExplain,
                                                     Result result );

 private:

  // Override consildate results so that the associated test results
  // can be used to initialized the passing requirements (i.e. assume
  // all tests pass to begin with.)
  virtual void InitConsolidateResults();
    
  // prohibited
  TestLevelTestResult( const TestLevelTestResult& );
  TestLevelTestResult operator=( const TestLevelTestResult& );
  
};

} // end of namespace diskstream

#endif/*__TESTLEVELTESTRESULT_h__*/

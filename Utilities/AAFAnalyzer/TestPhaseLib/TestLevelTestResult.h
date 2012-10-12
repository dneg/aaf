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

#ifndef __TESTLEVELTESTRESULT_h__
#define __TESTLEVELTESTRESULT_h__

//Test/Result files
#include "LowLevelTestResult.h"

//Base
#include <Node.h>

namespace aafanalyzer {

using namespace std;

class DetailLevelTestResult;
class Test;

class TestLevelTestResult : public LowLevelTestResult
{
 public:

  TestLevelTestResult( const boost::shared_ptr<const Test> associatedTest );

  // Use this constructor to create a test result that is not intended
  // to parent a set of sub results.  If detailed results are added
  // then there aggregate test result will replace the result value
  // passed to the constructor.
  TestLevelTestResult( const wstring& name,
		       const wstring& desc,
                       const wstring& explain,
		       Result result,
                       const boost::shared_ptr<const Test> associatedTest );

  // Use this constructor if you will be adding DetailLevelTestResult
  // objects (in which case this objects result is the aggregate of the 
  // sub results).
  TestLevelTestResult( const wstring& name,
		       const wstring& desc,
                       const boost::shared_ptr<const Test> associatedTest );

  ~TestLevelTestResult();

  const enum ResultLevel GetResultType() const;

  // Add a single result for one requirement. Result name and
  // description are those of the requirment. Caller must explain the
  // result.

  boost::shared_ptr<DetailLevelTestResult> AddSingleResult( const wstring& reqId,
                                                     const wstring& resultExplain,
                                                     Result result,
						     Node& node );

  // Same as above, but not associated with any particular node.
  boost::shared_ptr<DetailLevelTestResult> AddUnassociatedSingleResult( const wstring& reqId,
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

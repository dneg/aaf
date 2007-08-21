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

#ifndef __DETAILLEVELTESTRESULT_h__
#define __DETAILLEVELTESTRESULT_h__

//Test/Result files
#include "LowLevelTestResult.h"

namespace aafanalyzer {

using namespace std;
using namespace boost;

// A detail level test result stores a result for a single requirement
// (All others store a value that is aggregate result determine by
// take the worst sub result at consolidation time.) The requirement
// identfied by reqId must be present.  Note, while
// DetailLevelTestResult inherits pass/warn/fail result maps from the
// TestReult parent class only a single result is in those maps
// by. This is an important invariant in the DetailLevelTestResult
// implemenation.  As such, it cannot have chld results (i.e. is
// always a leaf node in the result tree).
//
// reqId must identify one of the requirements registered against the
// associatedTest.
//
// JPT REVIEW - This should be renamed to SingleTestResult so that its
// name more accurately represents it role.

class DetailLevelTestResult : public LowLevelTestResult
{
 public:

  // Result name and description are initialized to those of the
  // requirment.  Call must identify the requirment, provide a result,
  // and an explanation.
  DetailLevelTestResult( const shared_ptr<const Test> associatedTest,
                         const wstring& explain,
			 const wstring& reqId,
                         Result result );

  // Call provides result name and description rather than using the
  // requirements name and description.
  DetailLevelTestResult( const shared_ptr<const Test> associatedTest,
                         const wstring& name,
                         const wstring& desc,
                         const wstring& explain,
			 const wstring& reqId,
                         Result result );

  virtual ~DetailLevelTestResult();

  // Sets the result for the requirement identified by reqId. reqId
  // must be the same value passed to the constructor. This condition
  // is asserted. (This may seem redundant. It is done because passing
  // this value forces test implementors to be certain the correct
  // result is being updated and ensures complete clarity in the
  // code.)
  void SetResult( const wstring& reqId, Result result );

  virtual const enum ResultLevel GetResultType() const;

 private:

  // prohibited
  DetailLevelTestResult( const DetailLevelTestResult& );
  DetailLevelTestResult& operator=( const DetailLevelTestResult& );

  shared_ptr<const Requirement> GetRequirement() const;

  // This detail result is for the following requirement.
  // This is here primarily to aid debugging. We add test results
  // on at a time against requirements, but they are accumulated
  // in sets in the TestResult base class. This is less obtuse.
  const wstring& _reqId;
};

} // end of namespace diskstream

#endif/*__DETAILLEVELTESTRESULT_h__*/

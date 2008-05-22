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

#ifndef __LOWLEVELTESTRESULT_h__
#define __LOWLEVELTESTRESULT_h__

//Test/Result files
#include "TestResult.h"

namespace aafanalyzer {

class Test;

using namespace std;
using namespace boost;

// A LowLevelTestResult is test result that is associated with (i.e
// reported against) as particular test.  TestLevelResult is an
// instance of LowLevelTestResult the collects the results for a
// single test.  DetailLevelTestResult is a result against (generally)
// a single requirement registered for the assocaited test.

class LowLevelTestResult : public TestResult
{
 public:

  virtual ~LowLevelTestResult();

  virtual const enum ResultLevel GetResultType() const = 0;

  const shared_ptr<const Test> GetAssociatedTest() const;

 protected:

  LowLevelTestResult( const shared_ptr<const Test> associatedTest );

  LowLevelTestResult( const shared_ptr<const Test> associatedTest,
		      const wstring& name, const wstring& desc,
                      const wstring& explain ); 
 private:

  // prohibited
  LowLevelTestResult( const LowLevelTestResult& );
  LowLevelTestResult& operator=( const LowLevelTestResult& );

  void SetRequirementStatus( TestResult::Result level, const shared_ptr<const Requirement>& req);

  const shared_ptr<const Test> _spAssociatedTest;
};

} // end of namespace diskstream

#endif/*__LOWLEVELTESTRESULT_h__*/

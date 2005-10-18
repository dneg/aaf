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

#ifndef __HIGHLEVELTESTRESULT_h__
#define __HIGHLEVELTESTRESULT_h__

//Test/Result files
#include "TestResult.h"

namespace aafanalyzer {

using namespace std;

class HighLevelTestResult : public TestResult
{
 public:

  virtual ~HighLevelTestResult();

  //This function only needs to be called if direct child test results have
  //their requirement status modified after they are appended to this test.
  //Requirements are automatically updated when a subtest is appended,
  void UpdateRequirementStatus();
  
  virtual const enum ResultLevel GetResultType() const =0;

 protected:
  HighLevelTestResult();
  HighLevelTestResult( const wstring& name, const wstring& desc,
                       const wstring& explain, const wstring& docRef,
                       Result defaultResult );

  // prohibited
  HighLevelTestResult( const HighLevelTestResult& );
  HighLevelTestResult& operator=( const HighLevelTestResult& );
  
};

} // end of namespace diskstream

#endif/*__HIGHLEVELTESTRESULT_h__*/

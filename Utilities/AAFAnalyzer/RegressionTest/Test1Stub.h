//=---------------------------------------------------------------------=
//
// $$
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

#ifndef __TEST1STUB_h__
#define __TEST1STUB_h__

//project files
#include <Test.h>
#include <TestLevelTestResult.h>

namespace aafanalyzer {

class Test1Stub : public Test
{
 public:
  Test1Stub(std::wostream& os);
  ~Test1Stub();

  boost::shared_ptr<TestLevelTestResult> Execute();
  AxString GetName() const;
  AxString GetDescription() const;
  static const TestInfo GetTestInfo();

};

} // end of namespace diskstream

#endif/*__TEST1STUB_h__*/

//=---------------------------------------------------------------------=
//
// $Id$
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

#ifndef __EPEditRateTest_h_
#define __EPEditRateTest_h_

//Test/Result files
#include <Test.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

class TestLevelTestResult;

using namespace boost;

class EPEditRateTest : public Test
{
 public:

  EPEditRateTest( wostream& log,
                 shared_ptr<const TestGraph> spGraph );
  virtual ~EPEditRateTest();

  virtual shared_ptr<TestLevelTestResult> Execute();
  virtual AxString GetName() const;
  virtual AxString GetDescription() const;
  static const TestInfo GetTestInfo();

 private:

  // prohibited
  EPEditRateTest();
  EPEditRateTest( const EPEditRateTest& );
  EPEditRateTest& operator=( const EPEditRateTest& );

};

} // end of namespace diskstream

#endif /* __EPEditRateTest_h_*/

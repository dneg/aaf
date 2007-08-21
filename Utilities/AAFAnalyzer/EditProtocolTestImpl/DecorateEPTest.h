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

#ifndef __DecorateEPTest_h__
#define __DecorateEPTest_h__

//Test/Result files
#include <Test.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

using namespace boost;
using namespace std;

class TestGraph;

class DecorateEPTest : public Test
{
 public:

  DecorateEPTest( wostream& log,
		  shared_ptr<const TestGraph> spGraphInfo );
  virtual ~DecorateEPTest();

  virtual shared_ptr<TestLevelTestResult> Execute();
  virtual AxString GetName() const;
  virtual AxString GetDescription() const;
  static const TestInfo GetTestInfo();

 private:

  // prohibited
  DecorateEPTest();
  DecorateEPTest( const DecorateEPTest& );
  DecorateEPTest& operator=( const DecorateEPTest& );
};

} // end of namespace aafanalyzer

#endif

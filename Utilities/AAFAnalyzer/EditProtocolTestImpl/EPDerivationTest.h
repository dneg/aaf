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

#ifndef __EPDerivatinTest_h__
#define __EPDerivatinTest_h__

#include <CompMobDependency.h>

#include <Test.h>

namespace aafanalyzer {

using namespace boost;
using namespace std;

class EPDerivationTest : public Test
{
 public:

  EPDerivationTest( wostream& log,
		    shared_ptr<TestGraph> spGraph,
		    CompMobDependency::CompMobNodeVectorSP spTopLevelCompMobs );
  virtual ~EPDerivationTest();

  virtual shared_ptr<TestLevelTestResult> Execute();
  virtual AxString GetName() const;
  virtual AxString GetDescription() const;
  static const TestInfo GetTestInfo();

 private:

  // prohibited
  EPDerivationTest();
  EPDerivationTest( const EPDerivationTest& );
  EPDerivationTest& operator=( const EPDerivationTest& );

  shared_ptr<TestGraph> _spGraph;
  CompMobDependency::CompMobNodeVectorSP _spTopLevelCompMobs;
};

} // end of namespace aafanalyzer

#endif

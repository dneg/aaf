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

#ifndef __EPMobDepPhase_h__
#define __EPMobDepPhase_h__

//Edit Protocol Test files (included so the application only needs to include
//the Test Phase and not individual tests in order to register tests).
#include <DecorateEPTest.h>
#include <CompMobDependency.h>
#include <EPDerivationTest.h>
#include <EPNameTest.h>
#include <EPContainedTrackTest.h>
#include <EPTrackContentsTest.h>
#include <EPLocatorTest.h>
#include <EPEditRateTest.h>
#include <EPEffectTest.h>
#include <EPAnnotationTest.h>

//Test/Result files
#include <TestPhase.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

using namespace boost;
using namespace std;

class TestGraph;

class EPMobDepPhase : public TestPhase
{
 public:
  EPMobDepPhase( wostream& log, shared_ptr<const TestGraph> spGraph );
  virtual ~EPMobDepPhase();

  virtual AxString GetDescription() const;
  virtual AxString GetName() const;
  virtual shared_ptr<TestPhaseLevelTestResult> Execute();

 private:

  // prohibited
  EPMobDepPhase();
  EPMobDepPhase( const EPMobDepPhase& );
  EPMobDepPhase& operator=( const EPMobDepPhase& );

  wostream& _log;
  shared_ptr<const TestGraph> _spGraph;
};

} // end of namespace aafanalyzer

#endif

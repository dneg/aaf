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

#ifndef __EPMobDepPhase_h__
#define __EPMobDepPhase_h__

// Edit Protocol Test files (included so the application only needs to
// include the Test Phase and not individual tests in order to
// register tests).

#include <DecorateEPTest.h>
#include <EPDerivationTest.h>
#include <EPNameTest.h>
#include <EPContainedTrackTest.h>
#include <EPTrackContentsTest.h>
#include <EPLocatorTest.h>
#include <EPEditRateTest.h>
#include <EPEffectTest.h>
#include <EPAnnotationTest.h>
#include <EPDefinitionTest.h>
#include <EPHeaderTest.h>
#include <EPParameterTest.h>
#include <EPMultiChannelAudioTest.h>

//Base test files
#include <CompMobDependency.h>

//Test/Result files
#include <TestPhase.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

using namespace std;

class TestGraph;

class EPMobDepPhase : public TestPhase
{
 public:
  EPMobDepPhase( wostream& log,
		 boost::shared_ptr<const TestGraph> spGraph,
		 CompMobDependency::CompMobNodeVectorSP spCompMobRoots );
  virtual ~EPMobDepPhase();

  virtual AxString GetDescription() const;
  virtual AxString GetName() const;
  virtual boost::shared_ptr<TestPhaseLevelTestResult> Execute();

 private:

  // prohibited
  EPMobDepPhase();
  EPMobDepPhase( const EPMobDepPhase& );
  EPMobDepPhase& operator=( const EPMobDepPhase& );

  wostream& _log;
  boost::shared_ptr<const TestGraph> _spGraph;
  CompMobDependency::CompMobNodeVectorSP _spCompMobRoots;
};

} // end of namespace aafanalyzer

#endif

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

//Edit Protocol Test files
#include "EPMobDepPhase.h"

//Test/Result files
#include <TestPhaseLevelTestResult.h>

//STL files
#include <sstream>

namespace {

using namespace aafanalyzer;

const wchar_t* PHASE_NAME=L"Mob Dependency Analysis Phase";
const wchar_t* PHASE_DESC=L"Analyze mob references to ensure structure complies with the Edit Protocol.";


} // end of namespace

//======================================================================

namespace aafanalyzer {
    
using namespace boost;

EPMobDepPhase::EPMobDepPhase( wostream& log,
			      shared_ptr<const TestGraph> spGraph,
			      CompMobDependency::CompMobNodeVectorSP spCompMobRoots )
  : TestPhase( log ),
    _log( log ),
    _spGraph( spGraph ),
    _spCompMobRoots( spCompMobRoots )
{}

EPMobDepPhase::~EPMobDepPhase()
{}

AxString EPMobDepPhase::GetDescription() const
{
  return PHASE_DESC;
}

AxString EPMobDepPhase::GetName() const
{
  return PHASE_NAME;
}

shared_ptr<TestPhaseLevelTestResult> EPMobDepPhase::Execute()
{
  shared_ptr<TestPhaseLevelTestResult>
    spPhaseResult( new TestPhaseLevelTestResult( PHASE_NAME,        // name
						 PHASE_DESC,        // desc
						 L"" ) );           // explain
                                                                                    
  // First, decorate all mob nodes with an EPTypedObjNode decoration.
  shared_ptr<DecorateEPTest> decorator( new DecorateEPTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( decorator->Execute() );

  // Second, compute the composition mob dependencies.  NOTE - This is
  // necessary because when the nodes are decorated new objects are
  // inserted that wrap the old root nodes (amount others) in the
  // _spCompMobRoots vector. We need pointers to the new, decorated,
  // root nodes.  A faster implementation would pass _spCompMobRoots
  // to DecorateEPTest and it would map the nodes on the file as it
  // decorates them.  I (jpt) don't want to make such an intrusive
  // change right now.
  shared_ptr<CompMobDependency> spDepTest( new CompMobDependency(_log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( spDepTest->Execute() );

  // JPT REVIEW - A sanity check to ensure the spPostDecoratedRoots
  // and _spCompMobRoots identify the same underly node would be wise
  // at this point.

  // Third, run the dependency test to verify the chains starting
  // with the identified root compositions.
  shared_ptr<EPDerivationTest> derivationTest( new EPDerivationTest(_log, _spGraph, spDepTest) );
  spPhaseResult->AppendSubtestResult( derivationTest->Execute() );
  
  // Fourth, run the naming test
  shared_ptr<EPNameTest> nameTest( new EPNameTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( nameTest->Execute() );
  
  // Fifth, run the contained track test
  shared_ptr<EPContainedTrackTest> trackTest( new EPContainedTrackTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( trackTest->Execute() );
  
  // Sixth, run the track contents test
  shared_ptr<EPTrackContentsTest> trackContentsTest( new EPTrackContentsTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( trackContentsTest->Execute() );
  
  // Seventh, run the locator test
  // TODO: Uncomment this line when the test is running properly.
  // shared_ptr<EPLocatorTest> locatorTest( new EPLocatorTest( _log, _spGraph ) );
  // spPhaseResult->AppendSubtestResult( locatorTest->Execute() );

  // Eighth, run the edit rate test
  shared_ptr<EPEditRateTest> editRateTest( new EPEditRateTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( editRateTest->Execute() );
  
  // Ninth, run the effect test
  shared_ptr<EPEffectTest> effectTest( new EPEffectTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( effectTest->Execute() );
  
  // Tenth, run the annotation test
  shared_ptr<EPAnnotationTest> annotationTest( new EPAnnotationTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( annotationTest->Execute() );
  
  // Eleventh, run the definition test
  shared_ptr<EPDefinitionTest> definitionTest( new EPDefinitionTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( definitionTest->Execute() );
  
  // Twelevth, run the header test
  shared_ptr<EPHeaderTest> headerTest( new EPHeaderTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( headerTest->Execute() );
  
  // Thirteenth, run the parameter test
  shared_ptr<EPParameterTest> parameterTest( new EPParameterTest( _log, _spGraph ) );
  spPhaseResult->AppendSubtestResult( parameterTest->Execute() );
  
  // Fourteenth, run the multi-channel audio test
  CompMobDependency::CompMobNodeVectorSP spPostDecorateRoots = spDepTest->GetRootCompMobNodes();
  shared_ptr<EPMultiChannelAudioTest> mcaTest( new EPMultiChannelAudioTest( _log, _spGraph, spPostDecorateRoots ) );
  spPhaseResult->AppendSubtestResult( mcaTest->Execute() );

  return spPhaseResult;
}

} // end of namespace aafanalyzer

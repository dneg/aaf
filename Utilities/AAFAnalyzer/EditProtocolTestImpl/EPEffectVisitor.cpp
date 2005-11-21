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

//Edit Protocol Test files
#include "EPEffectVisitor.h"
#include "EPEffectTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Ax files
#include <AxComponent.h>
#include <AxParameter.h>
#include <AxEx.h>

//AAF files
#include <AAFClassDefUIDs.h>
#include <AAFParameterDefs.h>
#include <AAFResult.h>

//STL files
#include <sstream>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPEffectVisitor::EPEffectVisitor( wostream& log, shared_ptr<EdgeMap> spEdgeMap )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Effect Visitor",
                                            L"Visit effects and make sure they are valid.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPEffectTest::GetTestInfo().GetName() )
               )                          )
{
    _isParentTransition.push( false );
}
    
EPEffectVisitor::~EPEffectVisitor()
{}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPEffect>& node)
{
    _isParentTransition.push( false );
    return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoDissolveEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, true, L"REQ_EP_180", EPVideoDissolveEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPSMPTEVideoWipeEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, true, L"REQ_EP_183", EPSMPTEVideoWipeEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoSpeedControlEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_186", EPVideoSpeedControlEffect::GetName() );
    
    AxOperationGroup axOpGroup( node.GetAAFObjectOfType() );
    
    wstringstream ss;
    ss << EPVideoSpeedControlEffect::GetName() << L" in "
       << this->GetMobSlotName( _spEdgeMap, node );

    try
    {
        AxParameter axParam( axOpGroup.LookupParameter( kAAFParameterDef_SpeedRatio ) );
        AxClassDef clsDef( axParam.GetDefinition() );
        if ( this->IsType( clsDef, kAAFClassID_ConstantValue, kAAFClassID_Parameter ) )
        {
            AxConstantValue axConstParam( AxQueryInterface<IAAFParameter, IAAFConstantValue>( axParam ) );
            try
            {
                aafRational_t parameter;
                axConstParam.GetValue<aafRational_t>( parameter );
                if ( parameter.denominator == 0 )
                {
                    ss << L" has a Speed Ratio parameter with a 0 denominator ("
                       << parameter.numerator << L"/" << parameter.denominator
                       << L").";

                    _spResult->AddInformationResult( L"REQ_EP_187", ss.str().c_str(), TestResult::WARN );

                    testPassed = false;
                }
                else if ( parameter.numerator == 0 )
                {
                    ss << L" has a Speed Ratio parameter with a ratio value of 0 ("
                       << parameter.numerator << L"/" << parameter.denominator
                       << L").";

                    _spResult->AddInformationResult( L"REQ_EP_187", ss.str().c_str(), TestResult::FAIL );
                    
                    testPassed = false;
                }
            }
            catch ( const AxEx& ex )
            {
                ss << L" has a Speed Ratio parameter that does not have a rational value.";
                _spResult->AddInformationResult( L"REQ_EP_187", ss.str().c_str(), TestResult::FAIL );
                testPassed = false;
            }
        }
        else
        {
            ss << L" has a Speed Ratio parameter that is not constant";
            _spResult->AddInformationResult( L"REQ_EP_187", ss.str().c_str(), TestResult::FAIL );
            testPassed = false;
        }
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() != AAFRESULT_NO_MORE_OBJECTS )
        {
            throw ex;
        }

        ss << L" does not have a Speed Ratio parameter.";
        _spResult->AddInformationResult( L"REQ_EP_187", ss.str().c_str(), TestResult::FAIL );
        testPassed = false;
    }
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoRepeatEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_190", EPVideoRepeatEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoFlipEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_194", EPVideoFlipEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoFlopEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_197", EPVideoFlopEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoFlipFlopEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_200", EPVideoFlipFlopEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoPositionEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_203", EPVideoPositionEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoCropEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_206", EPVideoCropEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoScaleEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_209", EPVideoScaleEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoRotateEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_212", EPVideoRotateEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoCornerPinningEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_215", EPVideoCornerPinningEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPAlphaWithVideoKeyEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_220", EPAlphaWithVideoKeyEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPSeparateAlphaKeyEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_225", EPSeparateAlphaKeyEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPLuminanceKeyEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_228", EPLuminanceKeyEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPChromaKeyEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_231", EPChromaKeyEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPMonoAudioGainEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_234", EPMonoAudioGainEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPMonoAudioPanEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, false, L"REQ_EP_240", EPMonoAudioPanEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPSingleParameterAudioDissolveEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, true, L"REQ_EP_244", EPSingleParameterAudioDissolveEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPTwoParameterAudioDissolveEffect>& node)
{
    bool testPassed = true;
    testPassed = VeirfyTransitionRequirement( node, true, L"REQ_EP_247", EPTwoParameterAudioDissolveEffect::GetName() );
    _isParentTransition.push( false );
    return testPassed;
}

bool EPEffectVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTransition>& node )
{
    _isParentTransition.push( true );
    return true;
}
    
bool EPEffectVisitor::PostOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node )
{
    _isParentTransition.pop();
    return true;
}

bool EPEffectVisitor::PostOrderVisit( AAFTypedObjNode<IAAFTransition>& node )
{
    _isParentTransition.pop();
    return true;
}

shared_ptr<DetailLevelTestResult> EPEffectVisitor::GetResult()
{
    return _spResult;
}

bool EPEffectVisitor::VeirfyTransitionRequirement( AAFTypedObjNode<IAAFOperationGroup>& node, bool withinTransition, const AxString& reqId, const AxString& type )
{
    if ( withinTransition != _isParentTransition.top() )
    {
        AxString mobSlotName = this->GetMobSlotName( _spEdgeMap, node );
        AxString explain = type + L" in " + mobSlotName + L" is ";
        if ( withinTransition )
        {
            explain += L"not ";
        }
        explain += L"within a Transition object.";
        _spResult->AddInformationResult( reqId, explain, TestResult::FAIL );
        return false;
    }
    return true;
}
    
} // end of namespace aafanalyzer

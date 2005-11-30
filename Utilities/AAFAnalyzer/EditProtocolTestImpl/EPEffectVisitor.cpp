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

//Analyzer Base files
#include <DepthFirstTraversal.h>

//Ax files
#include <AxComponent.h>
#include <AxParameter.h>
#include <AxEx.h>
#include <AxIterator.h>

//AAF files
#include <AAFClassDefUIDs.h>
#include <AAFParameterDefs.h>
#include <AAFResult.h>

//STL files
#include <sstream>

namespace {

using namespace aafanalyzer;

class TransitionInputVisitor : public TypedVisitor
{
    public:
        TransitionInputVisitor()
        {}
        
        ~TransitionInputVisitor()
        {}

        bool PreOrderVisit( AAFTypedObjNode<IAAFTransition>& node )
        {
            //The traversal started at an operation group and should have
            //proceeded here immediatley.  Record the address of this transition
            //object and continue up to the parent sequence.
            _spIaafTransition = AxQueryInterface<IAAFTransition, IAAFComponent>( node.GetAAFObjectOfType() );
            return true;
        }
        
        bool PreOrderVisit( AAFTypedObjNode<IAAFSequence>& node )
        {
            AxSequence axSequence( node.GetAAFObjectOfType() );

            //The graph may not return edges in order, therefore, we need to
            //loop through and find the correct transition by address.  Use
            //this method as an iterator does not guarantee an orderd traversal.
            aafUInt32 index = 0;
            aafUInt32 numComponents = axSequence.CountComponents();

            for ( ; index < numComponents; index++ )
            {
                if ( axSequence.GetComponentAt( index ) == _spIaafTransition )
                {
                    break;
                }
            }
            
            if ( index >= numComponents )
            {
                //A fatal error has occured as the transition was not found.
                //Throw an exception.
            }
            
            //Get the preceeding and following components
            if ( index > 0 )
            {
                _spPreceeding = axSequence.GetComponentAt( index - 1);
            }
            if ( index < numComponents - 1 )
            {
                _spFollowing = axSequence.GetComponentAt( index + 1);
            }

            //Traversal should start at an operation group, traverse up to
            //the parent transition and then up to this sequence.  There is
            //no need to traverse any further.
            return false;
        }
        
        IAAFComponentSP GetPreceedingComponent()
        {
            return _spPreceeding;
        }
        
        IAAFComponentSP GetFollowingComponent()
        {
            return _spFollowing;
        }
      
        IAAFTransitionSP GetTransition()
        {
            return AxQueryInterface<IAAFComponent, IAAFTransition>( _spIaafTransition );
        }
        
    private:
    
        IAAFComponentSP _spIaafTransition;
        IAAFComponentSP _spPreceeding;
        IAAFComponentSP _spFollowing;
    
        // prohibited
        TransitionInputVisitor( const TransitionInputVisitor& );
        TransitionInputVisitor& operator=( const TransitionInputVisitor& );
};

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
    
    AxString name = EPTwoParameterAudioDissolveEffect::GetName() + L" in " + this->GetMobSlotName( _spEdgeMap, node );
    if ( !testPassed )
    {
        //Parent is not a transition, so, we can't check REQ_EP_249.
        AxString explain = name + L" is not within a Transition object.";
        _spResult->AddInformationResult( L"REQ_EP_248", explain, TestResult::FAIL );
        _spResult->AddInformationResult( L"REQ_EP_249", explain, TestResult::FAIL );
    }
    else
    {
        shared_ptr<TransitionInputVisitor> spVisitor( new TransitionInputVisitor() );
        shared_ptr<Node> spNode( node.GetSharedPointerToNode() );
        DepthFirstTraversal dft( _spEdgeMap, spNode );
        dft.TraverseUp( spVisitor );
        
        bool incomingPassed = true;
        bool outgoingPassed = true;
        
        aafLength_t transitionLen;
        AxTransition axTransition( spVisitor->GetTransition() );
        try
        {
            transitionLen = axTransition.GetLength();
        }
        catch ( const AxExHResult& ex )
        {
            if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
            {
                AxString explain = L"Transition object of " + name 
                                 + L" does not have a length property.";
                _spResult->AddInformationResult( L"REQ_EP_248", explain, TestResult::FAIL );
                _spResult->AddInformationResult( L"REQ_EP_249", explain, TestResult::FAIL );
                incomingPassed = false;
                outgoingPassed = false;
            }
            else
            {
                throw ex;
            }
        }
        
        if ( spVisitor->GetFollowingComponent() )
        {
            aafLength_t incomingLen;            
            AxComponent incoming( spVisitor->GetFollowingComponent() );
            try
            {
                incomingLen = incoming.GetLength();
            }
            catch ( const AxExHResult& ex )
            {
                if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
                {
                    AxString explain = L"Incoming segment of " + name 
                                     + L" does not have a length property.";
                    _spResult->AddInformationResult( L"REQ_EP_248", explain, TestResult::FAIL );
                    _spResult->AddInformationResult( L"REQ_EP_249", explain, TestResult::FAIL );
                    incomingPassed = false;
                }
                else
                {
                    throw ex;
                }
            }
            
            if ( incomingPassed && incomingLen < transitionLen )
            {
                wstringstream ss;
                
                ss << name << L" has an incoming segment with length = "
                   << incomingLen << L" and a transition with length = "
                   << transitionLen << L".";
                _spResult->AddInformationResult( L"REQ_EP_248", ss.str().c_str(), TestResult::FAIL );
                _spResult->AddInformationResult( L"REQ_EP_249", ss.str().c_str(), TestResult::FAIL );
                incomingPassed = false;
            }            
        }
        else
        {
            AxString explain = name + L" does not have an incoming segment.";
            _spResult->AddInformationResult( L"REQ_EP_248", explain, TestResult::FAIL );
            _spResult->AddInformationResult( L"REQ_EP_249", explain, TestResult::FAIL );
            testPassed = false;
        }
        
        if ( spVisitor->GetPreceedingComponent() )
        {
            aafLength_t outgoingLen;            
            AxComponent outgoing( spVisitor->GetPreceedingComponent() );
            try
            {
                outgoingLen = outgoing.GetLength();
            }
            catch ( const AxExHResult& ex )
            {
                if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
                {
                    AxString explain = L"Outgoing segment of " + name 
                                     + L" does not have a length property.";
                    _spResult->AddInformationResult( L"REQ_EP_249", explain, TestResult::FAIL );
                    outgoingPassed = false;
                }
                else
                {
                    throw ex;
                }
            }
            
            if ( outgoingPassed && outgoingLen < transitionLen )
            {
                wstringstream ss;
                
                ss << name << L" has an outgoing segment with length = "
                   << outgoingLen << L" and a transition with length = "
                   << transitionLen << L".";
                _spResult->AddInformationResult( L"REQ_EP_249", ss.str().c_str(), TestResult::FAIL );
                outgoingPassed = false;
            }            
        }
        else
        {
            AxString explain = name + L" does not have an outgoing segment.";
            _spResult->AddInformationResult( L"REQ_EP_249", explain, TestResult::FAIL );
            testPassed = false;
        }
        
        testPassed = incomingPassed && outgoingPassed;
        
    }
    
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

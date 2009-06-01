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
#include "EPEffectVisitor.h"
#include "EPEffectTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//AAF Analyzer Base files
#include <MobNodeMap.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>

//Ax files
#include <AxComponent.h>
#include <AxParameter.h>
#include <AxEx.h>
#include <AxEssence.h>

//AAF files
#include <AAFClassDefUIDs.h>
#include <AAFParameterDefs.h>
#include <AAFResult.h>

//STL files
#include <sstream>

namespace {

using namespace aafanalyzer;
using namespace boost;

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

	    // JPT REVIEW - The order issue has been resolved.  Review
	    // the code to determine the reordering code below can be
	    // removed.

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

template<typename EffectType>
class AlphaEffectVisitor : public EPTypedVisitor
{
    public:
        AlphaEffectVisitor( const AxString& descriptorReq,
			    const AxString& alphaReq,
			    const AxString& slotName,
			    shared_ptr<TestLevelTestResult> spTestResult )
            : _descriptorReq( descriptorReq ),
              _alphaReq( alphaReq ),
              _slotName( slotName ),
              _spTestResult( spTestResult ),
              _testPassed( true )
        {}

        ~AlphaEffectVisitor()
        {}

        //Note: All we want to accomplish is to visit the mob node, we do not
        //      need to traverse any further so all visit methods will return
        //      false.

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRGBAImageFileSource>& node )
        {
            //This is an acceptable input, so just test for the
            //AlphaTransparencyProperty.
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            AxString mobName = this->GetMobName( axMob, EPRGBAImageFileSource::GetName() );
            AxEssenceDescriptor axDescriptor( axMob.GetEssenceDescriptor() );
            AxDigitalImageDescriptor axImageDesc( AxQueryInterface<IAAFEssenceDescriptor,
						  IAAFDigitalImageDescriptor>( axDescriptor ) );
            CheckTransparency( axImageDesc, mobName, node );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPCDCIImageFileSource>& node )
        {
            //This is an acceptable input, so just test for the
            //AlphaTransparency property.
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            AxString mobName = this->GetMobName( axMob, EPCDCIImageFileSource::GetName() );
            AxEssenceDescriptor axDescriptor( axMob.GetEssenceDescriptor() );
            AxDigitalImageDescriptor axImageDesc( AxQueryInterface<IAAFEssenceDescriptor, IAAFDigitalImageDescriptor>( axDescriptor ) );
            CheckTransparency( axImageDesc, mobName, node );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImageFileSource>& node )
        {
            //This is not an acceptable input, so fail the descriptor
            //requirement.
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            AxString mobName = this->GetMobName( axMob, EPCDCIImageFileSource::GetName() );

            _spTestResult->AddSingleResult(
                _descriptorReq,
                L"Input " + mobName + L" to " + EffectType::GetName() + L" in " +
                     _slotName + L" does not have a RGBADescriptor or a CDCIDescriptor.",
                TestResult::FAIL,
		node );
            _testPassed = false;

            //Now check for the AlphaTransparency property.
            AxEssenceDescriptor axDescriptor( axMob.GetEssenceDescriptor() );
            AxDigitalImageDescriptor axImageDesc( AxQueryInterface<IAAFEssenceDescriptor, IAAFDigitalImageDescriptor>( axDescriptor ) );
            CheckTransparency( axImageDesc, mobName, node );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
        {
            //This is invalid input and does not have the AlphaTransparency
            //property.
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            AxString mobName = this->GetMobName( axMob, L"Composition Mob");
            FailBothRequirements( mobName, node );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
        {
            //This is invalid input and does not have the AlphaTransparency
            //property.
            AxMasterMob axMob( node.GetAAFObjectOfType() );
            AxString mobName = this->GetMobName( axMob, L"Master Mob");
            FailBothRequirements( mobName, node );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
        {
            //This is invalid input and does not have the AlphaTransparency
            //property.
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            AxString mobName = this->GetMobName( axMob, L"Source Mob");
            FailBothRequirements( mobName, node );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFMob>& node )
        {
            //This is invalid input and does not have the AlphaTransparency
            //property.
            AxMob axMob( node.GetAAFObjectOfType() );
            AxString mobName = this->GetMobName( axMob, L"Mob");
            FailBothRequirements( mobName, node );
            return false;
        }

        bool GetTestResult()
        {
            return _testPassed;
        }

    private:

        void CheckTransparency( AxDigitalImageDescriptor& axDescriptor, const AxString& inputName, Node& node )
        {
            try
            {
                axDescriptor.GetAlphaTransparency();
            }
            catch ( const AxExHResult& ex )
            {
                if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
                {
                    _spTestResult->AddSingleResult(
                        _alphaReq,
                        L"Input " + inputName + L" to " + EffectType::GetName() +
                             L" in " + _slotName +
                             L" does not have the DigitalImageDescriptor::AlphaTransparency property.",
                        TestResult::FAIL,
			node );
                    _testPassed = false;
                }
                else
                {
                    throw ex;
                }
            }
        }

        void FailBothRequirements( const AxString& inputName, Node& node )
        {
            AxString ident = L"Input " + inputName + L" to " + EffectType::GetName() + L" in " + _slotName;

            _spTestResult->AddSingleResult(
                _descriptorReq,
                ident + L" does not have a RGBADescriptor or a CDCIDescriptor.",
                TestResult::FAIL,
		node );
            _spTestResult->AddSingleResult(
                _alphaReq,
                ident + L" does not have the DigitalImageDescriptor::AlphaTransparency property.",
                TestResult::FAIL,
		node );
            _testPassed = false;
        }


        const AxString _descriptorReq;
        const AxString _alphaReq;
        const AxString _slotName;
        shared_ptr<TestLevelTestResult> _spTestResult;
        bool _testPassed;

        // prohibited
        AlphaEffectVisitor();
        AlphaEffectVisitor( const AlphaEffectVisitor& );
        AlphaEffectVisitor& operator=( const AlphaEffectVisitor& );
};

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;

EPEffectVisitor::EPEffectVisitor( wostream& log,
				  shared_ptr<EdgeMap> spEdgeMap,
				  shared_ptr<TestLevelTestResult> spTestResult )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spTestResult( spTestResult )
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
    VerifyTransitionRequirement( node, true, L"REQ_EP_180", EPVideoDissolveEffect::GetName() );
    _isParentTransition.push( false );
    return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPSMPTEVideoWipeEffect>& node)
{
    VerifyTransitionRequirement( node, true, L"REQ_EP_183", EPSMPTEVideoWipeEffect::GetName() );
    _isParentTransition.push( false );
    return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoSpeedControlEffect>& node)
{
    VerifyTransitionRequirement( node, false, L"REQ_EP_186", EPVideoSpeedControlEffect::GetName() );

    AxOperationGroup axOpGroup( node.GetAAFObjectOfType() );

    wstringstream ss;
    ss << EPVideoSpeedControlEffect::GetName() << L" in "
       << this->GetMobSlotName( _spEdgeMap, node );

    try
    {
        AxParameter axParam( axOpGroup.LookupParameter( kAAFParameterDef_SpeedRatio ) );
        IAAFConstantValueSP spConstVal;
        if ( AxIsA( axParam, spConstVal ) )
        {
            AxConstantValue axConstParam( spConstVal );
	    aafRational_t parameter;
	    axConstParam.GetValue<aafRational_t>( parameter );
	    if ( parameter.denominator == 0 )
	    {
	      ss << L" has a Speed Ratio parameter with a 0 denominator ("
		 << parameter.numerator << L"/" << parameter.denominator
		 << L").";

	      _spTestResult->AddSingleResult( L"REQ_EP_187", ss.str().c_str(), TestResult::WARN, node );
	    }
	    else if ( parameter.numerator == 0 )
	    {
	      ss << L" has a Speed Ratio parameter with a ratio value of 0 ("
		 << parameter.numerator << L"/" << parameter.denominator
		 << L").";
	      
	      _spTestResult->AddSingleResult( L"REQ_EP_187", ss.str().c_str(), TestResult::FAIL, node );
	    }
	}
        else
        {
            ss << L" has a Speed Ratio parameter that is not constant";
            _spTestResult->AddSingleResult( L"REQ_EP_187", ss.str().c_str(), TestResult::FAIL, node );
        }
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() != AAFRESULT_NO_MORE_OBJECTS )
        {
            throw ex;
        }

        ss << L" does not have a Speed Ratio parameter.";
        _spTestResult->AddSingleResult( L"REQ_EP_187", ss.str().c_str(), TestResult::FAIL, node );
    }

    _isParentTransition.push( false );

    return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoRepeatEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_190", EPVideoRepeatEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoFlipEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_194", EPVideoFlipEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoFlopEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_197", EPVideoFlopEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoFlipFlopEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_200", EPVideoFlipFlopEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoPositionEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_203", EPVideoPositionEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoCropEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_206", EPVideoCropEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoScaleEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_209", EPVideoScaleEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoRotateEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_212", EPVideoRotateEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPVideoCornerPinningEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_215", EPVideoCornerPinningEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPAlphaWithVideoKeyEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_220", EPAlphaWithVideoKeyEffect::GetName() );

  AxOperationGroup axOpGroup( node.GetAAFObjectOfType() );
  VerifyAlphaRequirements( node, axOpGroup, EPAlphaWithVideoKeyEffect::GetName(), 2, L"REQ_EP_218", L"REQ_EP_219", this->GetMobSlotName( _spEdgeMap, node ) );

  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPSeparateAlphaKeyEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_225", EPSeparateAlphaKeyEffect::GetName() );

  AxOperationGroup axOpGroup( node.GetAAFObjectOfType() );
  VerifyAlphaRequirements( node, axOpGroup, EPSeparateAlphaKeyEffect::GetName(), 3, L"REQ_EP_223", L"REQ_EP_224", this->GetMobSlotName( _spEdgeMap, node ) );

  _isParentTransition.push( false );
  
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPLuminanceKeyEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_228", EPLuminanceKeyEffect::GetName() );

  _isParentTransition.push( false );

  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPChromaKeyEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_231", EPChromaKeyEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPMonoAudioGainEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_234", EPMonoAudioGainEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPMonoAudioPanEffect>& node)
{
  VerifyTransitionRequirement( node, false, L"REQ_EP_240", EPMonoAudioPanEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPSingleParameterAudioDissolveEffect>& node)
{
  VerifyTransitionRequirement( node, true, L"REQ_EP_244", EPSingleParameterAudioDissolveEffect::GetName() );
  _isParentTransition.push( false );
  return true;
}

bool EPEffectVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationGroup, EPTwoParameterAudioDissolveEffect>& node)
{
  VerifyTransitionRequirement( node, true, L"REQ_EP_247", EPTwoParameterAudioDissolveEffect::GetName() );

  AxString name = EPTwoParameterAudioDissolveEffect::GetName() + L" in " + this->GetMobSlotName( _spEdgeMap, node );
  if ( !_isParentTransition.top() )
  {
    //Parent is not a transition, so, we can't check REQ_EP_249.
    AxString explain = name + L" is not within a Transition object.";
    _spTestResult->AddSingleResult( L"REQ_EP_248", explain, TestResult::FAIL, node );
    _spTestResult->AddSingleResult( L"REQ_EP_249", explain, TestResult::FAIL, node );
  }
  else
  {
    shared_ptr<TransitionInputVisitor> spVisitor( new TransitionInputVisitor() );
    shared_ptr<Node> spNode( node.GetSharedPointerToNode() );
    DepthFirstTraversal dft( _spEdgeMap, spNode );
    dft.TraverseUp( spVisitor );

    bool incomingPassed = true;
    bool outgoingPassed = true;
    
    AxTransition axTransition( spVisitor->GetTransition() );
    pair<bool,aafLength_t> transitionLen = axTransition.ExistsLength();
    if ( !transitionLen.first )
    {
      AxString explain = L"Transition object of " + name
	+ L" does not have a length property.";
      _spTestResult->AddSingleResult( L"REQ_EP_248", explain, TestResult::FAIL, node );
      _spTestResult->AddSingleResult( L"REQ_EP_249", explain, TestResult::FAIL, node );
      incomingPassed = false;
      outgoingPassed = false;
    }

    if ( spVisitor->GetFollowingComponent() )
    {
      AxComponent incoming( spVisitor->GetFollowingComponent() );
      pair<bool,aafLength_t> incomingLen = incoming.ExistsLength();
      if ( !incomingLen.first )
      {
	AxString explain = L"Incoming segment of " + name
	  + L" does not have a length property.";
	_spTestResult->AddSingleResult( L"REQ_EP_248", explain, TestResult::FAIL, node );
	_spTestResult->AddSingleResult( L"REQ_EP_249", explain, TestResult::FAIL, node );
	incomingPassed = false;
      }
      
      if ( incomingPassed && incomingLen.second < transitionLen.second )
      {
	wstringstream ss;
	
	ss << name << L" has an incoming segment with length = "
	   << incomingLen.second << L" and a transition with length = "
	   << transitionLen.second << L".";
	_spTestResult->AddSingleResult( L"REQ_EP_248", ss.str().c_str(), TestResult::FAIL, node );
	_spTestResult->AddSingleResult( L"REQ_EP_249", ss.str().c_str(), TestResult::FAIL, node );
	incomingPassed = false;
      }
    }
    else
    {
      AxString explain = name + L" does not have an incoming segment.";
      _spTestResult->AddSingleResult( L"REQ_EP_248", explain, TestResult::FAIL, node );
      _spTestResult->AddSingleResult( L"REQ_EP_249", explain, TestResult::FAIL, node );
    }
    
    if ( spVisitor->GetPreceedingComponent() )
    {
      AxComponent outgoing( spVisitor->GetPreceedingComponent() );
      pair<bool,aafLength_t> outgoingLen = outgoing.ExistsLength();
      if ( !outgoingLen.first )
      {
	AxString explain = L"Outgoing segment of " + name
	  + L" does not have a length property.";
	_spTestResult->AddSingleResult( L"REQ_EP_249", explain, TestResult::FAIL, node );
	outgoingPassed = false;
      }
      
      if ( outgoingPassed && outgoingLen.second < transitionLen.second )
      {
	wstringstream ss;
	
	ss << name << L" has an outgoing segment with length = "
	   << outgoingLen.second << L" and a transition with length = "
	   << transitionLen.second << L".";
	_spTestResult->AddSingleResult( L"REQ_EP_249", ss.str().c_str(), TestResult::FAIL, node );
	outgoingPassed = false;
      }
    }
    else
    {
      AxString explain = name + L" does not have an outgoing segment.";
      _spTestResult->AddSingleResult( L"REQ_EP_249", explain, TestResult::FAIL, node );
    }
  }
  
  _isParentTransition.push( false );
  
  return true;
}

bool EPEffectVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTransition>& node )
{
    _isParentTransition.push( true );
    return true;
}

bool EPEffectVisitor::PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node )
{
    // JPT REVIEW - Andrew's original code didn't include this because
    // it didn't anticipate effects other those above (the decorated
    // operation group objects.  This should generate a warning
    // because it indicates the presence of an unknown effect.


    _isParentTransition.push(false);
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

void EPEffectVisitor::VerifyTransitionRequirement( AAFTypedObjNode<IAAFOperationGroup>& node,
						   bool mustBeInTransition,
						   const AxString& reqId,
						   const AxString& type )
{
  TestResult::Result result = TestResult::PASS;

  // If mustBeInTrantion is true then _isParentTransition.top() must
  // be true (i.e. we are in a transtition).
  //
  // If mustBeInTrantion is false then _isParentTransition.top() must
  // be false (i.e. we are not in a transtition).

  if ( mustBeInTransition != _isParentTransition.top() )
  {
    result = TestResult::FAIL;
  }

  // Build the message.  e.g. "Dissolve is in transition", or
  // "Dissolve is not in transition".
  
  AxString mobSlotName = this->GetMobSlotName( _spEdgeMap, node );
  AxString explain = type + L" in " + mobSlotName + L" is ";

  
  // We need to insert the word "not" depending on whether this effect
  // must or must not be in a transition and whether it actually is or
  // is not. The four cases are:
  //
  // (mustBeInTransition && TestResult::Result == PASS)
  // It must and it is: "effect is in a transition" -> don't add not
  //
  // (mustBeInTransition && TestResult::Result == FAIL)
  // It must and it is not: "effect is not in a transition" -> do add not
  //
  // (!mustBeInTransition && TestResult::Result == PASS)
  // It must not and it is not: "effect is not in a transition" -> do add not
  //
  // (!mustBeInTransition && TestResult::Result == FAIL)
  // It must not and it is: "effect is in a transition" -> don't add not

  if ( ( mustBeInTransition && result == TestResult::FAIL) ||
       (!mustBeInTransition && result == TestResult::PASS) )
  {
    explain += L"not ";
  }

  explain += L"within a Transition object.";
  
  _spTestResult->AddSingleResult( reqId, explain, TestResult::FAIL, node );
}

void EPEffectVisitor::VerifyAlphaRequirements( Node& node, 
					       AxOperationGroup& axOpGroup,
					       const AxString& effectType,
					       aafUInt32 expectedInputs,
					       const AxString& descriptorReq,
					       const AxString& alphaReq,
					       const AxString& slotName )
{
  if ( axOpGroup.CountSourceSegments() == expectedInputs )
  {
    // The alpha input is always the final input

    AxSegment axSegment( axOpGroup.GetInputSegmentAt( expectedInputs - 1) );
    IAAFSourceReferenceSP spSrcRef;

    if ( AxIsA( axSegment, spSrcRef ) )
    {
      AxSourceReference axSourceRef( spSrcRef );
      aafMobID_t mobid = axSourceRef.GetSourceID();
      
      // Make sure the source reference leads to a mob.
      if ( AxConstants::NULL_MOBID == mobid )
      {
	// Not a valid mob so fail
	_spTestResult->AddSingleResult( descriptorReq,
					L"Input to " + effectType +  L" in " + slotName + L" is a null source reference.",
					TestResult::FAIL,
					node );
	_spTestResult->AddSingleResult( alphaReq,
					L"Input to " + effectType +  L" in " + slotName + L" is a null source reference.",
					TestResult::FAIL,
					node );
	return;
      }

      // Make sure the source reference leads to a mob in this file.
      //
      // JPT REVIEW - This is sufficient to determine the reference is
      // valid only because the prior reference resolution test
      // validated slot id of the reference.  and will have generated
      // an error (unnoticed here) it is not.
      //
      // A better way to do this would be to access that resolution
      // information, not repeat the test here. That can be done be
      // accessing thild nodes directly. They should be references and
      // they should be valid.

      shared_ptr<Node> spInputNode = MobNodeMap::GetInstance().GetMobNode(mobid);
      if ( !spInputNode )
      {
	// Out of File reference so warn.
	_spTestResult->AddSingleResult( descriptorReq,
					L"Input to " + effectType +  L" in " + slotName + L" is an out-of-file reference.",
					TestResult::WARN,
					node );
	_spTestResult->AddSingleResult( alphaReq,
					L"Input to " + effectType +  L" in " + slotName + L" is an out-of-file reference.",
					TestResult::WARN,
					node );
	return;
      }

      // Else, record pass result.
      _spTestResult->AddSingleResult( descriptorReq,
				      L"Input to " + effectType +  L" in " + slotName + L" is a valid reference.",
				      TestResult::PASS,
				      node );
      _spTestResult->AddSingleResult( alphaReq,
				      L"Input to " + effectType +  L" in " + slotName + L" is a valid reference.",
				      TestResult::PASS,
				      node );

      //Verify the Alpha Requirements.
      shared_ptr<AlphaEffectVisitor<EPAlphaWithVideoKeyEffect> >
	spVisitor( new AlphaEffectVisitor<EPAlphaWithVideoKeyEffect>( descriptorReq, alphaReq, slotName, _spTestResult ) );
      DepthFirstTraversal dft( _spEdgeMap, spInputNode );
      dft.TraverseDown( spVisitor );
      return;
    }
    else
    {
      //The input is not a source reference so fail
      _spTestResult->AddSingleResult( descriptorReq,
				      L"Input to " + effectType +  L" in " + slotName + L" is not a SourceReference.",
				      TestResult::FAIL,
				      node );
      _spTestResult->AddSingleResult( alphaReq,
				      L"Input to " + effectType +  L" in " + slotName + L" is not a SourceReference.",
				      TestResult::FAIL,
				      node );
      return;
    }
  }
  else
  {
    //Incorrect number of Input Segments - don't know which one to check so fail
    wstringstream ss;
    
    ss << expectedInputs << L" segments and found "
       << axOpGroup.CountSourceSegments() << L" segments.";
    
    _spTestResult->AddSingleResult( descriptorReq,
				    effectType +  L" in " + slotName + L" has an incorrect number of input segments. " +
				    + L"Expecting " + ss.str().c_str(),
				    TestResult::FAIL,
				    node );
    _spTestResult->AddSingleResult( alphaReq,
				    effectType +  L" in " + slotName + L" has an incorrect number of input segments. " +
				    + L"Expecting " + ss.str().c_str(),
				    TestResult::FAIL,
				    node );
    return;
  }
}

} // end of namespace aafanalyzer

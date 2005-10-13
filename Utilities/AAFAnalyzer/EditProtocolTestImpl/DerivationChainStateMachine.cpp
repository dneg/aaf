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
#include "DerivationChainStateMachine.h"
#include "StateMachineException.h"

//Requirement files
#include <RequirementRegistry.h>

//AAF files
#include <AAFExtEnum.h>
#include <AAFClassDefUIDs.h>

namespace {

using namespace aafanalyzer;

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;
using namespace boost;

DerivationChainStateMachine::DerivationChainStateMachine( wostream& log )
    : _log( log ), _reversing( false )
{
    
    this->SetTransitions();
    this->SetEvents();
    this->SetFailures();
    
    _currentState.push( INITIAL );
   
}

void DerivationChainStateMachine::SetTransitions()
{
    //Transitions out of the INITIAL state:
    _transitionMap[make_pair( INITIAL, kAAFUsage_TopLevel )] = TOP_LEVEL;
    
    //Transitions out of the TOP_LEVEL state:
    _transitionMap[make_pair( TOP_LEVEL, kAAFUsage_LowerLevel )] = LOWER_LEVEL;
    _transitionMap[make_pair( TOP_LEVEL, kAAFUsage_SubClip )] = SUB_CLIP;
    _transitionMap[make_pair( TOP_LEVEL, kAAFUsage_AdjustedClip )] = ADJUSTED_CLIP;
    _transitionMap[make_pair( TOP_LEVEL, kAAFUsage_Template )] = TEMPLATE_CLIP;
    _transitionMap[make_pair( TOP_LEVEL, kAAFClassID_MasterMob )] = CLIP;
    
    //Transitions out of the LOWER_LEVEL state:
    _transitionMap[make_pair( LOWER_LEVEL, kAAFUsage_LowerLevel )] = LOWER_LEVEL;
    _transitionMap[make_pair( LOWER_LEVEL, kAAFUsage_SubClip )] = SUB_CLIP;
    _transitionMap[make_pair( LOWER_LEVEL, kAAFUsage_AdjustedClip )] = ADJUSTED_CLIP;
    _transitionMap[make_pair( LOWER_LEVEL, kAAFUsage_Template )] = TEMPLATE_CLIP;
    _transitionMap[make_pair( LOWER_LEVEL, kAAFClassID_MasterMob )] = CLIP;
    
    //Transitions out of the SUB_CLIP state:
    _transitionMap[make_pair( SUB_CLIP, kAAFUsage_AdjustedClip )] = ADJUSTED_CLIP;
    _transitionMap[make_pair( SUB_CLIP, kAAFUsage_Template )] = TEMPLATE_CLIP;
    _transitionMap[make_pair( SUB_CLIP, kAAFClassID_MasterMob )] = CLIP;
    
    //Transitions out of the ADJUSTED_CLIP state:
    _transitionMap[make_pair( ADJUSTED_CLIP, kAAFUsage_Template )] = TEMPLATE_CLIP;
    _transitionMap[make_pair( ADJUSTED_CLIP, kAAFClassID_MasterMob )] = CLIP;
    
    //Transitions out of the TEMPLATE_CLIP state:
    _transitionMap[make_pair( TEMPLATE_CLIP, kAAFClassID_SourceClip )] = EOC;
    
    //Transitions out of the CLIP state:
    _transitionMap[make_pair( CLIP, kAAFClassID_FileDescriptor )] = FILE_SOURCE;
    _transitionMap[make_pair( CLIP, kAAFClassID_ImportDescriptor )] = IMPORT_SOURCE;
    _transitionMap[make_pair( CLIP, kAAFClassID_TapeDescriptor )] = TAPE_SOURCE;
    _transitionMap[make_pair( CLIP, kAAFClassID_FilmDescriptor )] = FILM_SOURCE;
    _transitionMap[make_pair( CLIP, kAAFClassID_SourceClip )] = EOC;
    _transitionMap[make_pair( CLIP, kAAFClassID_SourceMob )] = OOF;
    
    //Transitions out of the FILE_SOURCE state:
    _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_FileDescriptor )] = FILE_SOURCE;
    _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_RecordingDescriptor )] = RECORDING_SOURCE;
    _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_ImportDescriptor )] = IMPORT_SOURCE;
    _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_TapeDescriptor )] = TAPE_SOURCE;
    _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_FilmDescriptor )] = FILM_SOURCE;
    _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_SourceMob )] = OOF;
    
    //Transitions out of the RECORDING_SOURCE state:
    _transitionMap[make_pair( RECORDING_SOURCE, kAAFClassID_SourceClip )] = EOC;
    
    //Transitions out of the IMPORT_SOURCE state:
    _transitionMap[make_pair( IMPORT_SOURCE, kAAFClassID_TapeDescriptor )] = TAPE_SOURCE;
    _transitionMap[make_pair( IMPORT_SOURCE, kAAFClassID_FilmDescriptor )] = FILM_SOURCE;
    _transitionMap[make_pair( IMPORT_SOURCE, kAAFClassID_SourceClip )] = EOC;
    _transitionMap[make_pair( IMPORT_SOURCE, kAAFClassID_SourceMob )] = OOF;
    
    //Transitions out of the TAPE_SOURCE state:
    _transitionMap[make_pair( TAPE_SOURCE, kAAFClassID_FilmDescriptor )] = FILM_SOURCE;
    _transitionMap[make_pair( TAPE_SOURCE, kAAFClassID_SourceClip )] = EOC;
    _transitionMap[make_pair( TAPE_SOURCE, kAAFClassID_SourceMob )] = OOF;
    
    //Transitions out of the FILM_SOURCE state:
    _transitionMap[make_pair( FILM_SOURCE, kAAFClassID_SourceClip )] = EOC;
    
}

void DerivationChainStateMachine::SetEvents()
{
    _eventMap[kAAFUsage_AdjustedClip] = L"Adjusted Clip Composition"; //Encountered an Adjusted Clip Composition.
    _eventMap[kAAFUsage_LowerLevel] = L"Lower Level Composition";     //Encountered a Lower Level Composition.
    _eventMap[kAAFUsage_SubClip] = L"Sub-Clip Composition";           //Encountered a Sub-Clip Composition.
    _eventMap[kAAFUsage_Template] = L"Template Clip";                 //Encountered a Template Clip.
    _eventMap[kAAFUsage_TopLevel] = L"Top Level Composition";         //Encountered a Top Level Composition.
    _eventMap[kAAFClassID_MasterMob] = L"Clip";                       //Used to indicate a Clip was encountered.
    _eventMap[kAAFClassID_FileDescriptor] = L"File Source";           //Encountered a File Source.
    _eventMap[kAAFClassID_RecordingDescriptor] = L"Recording Source"; //Encountered a Recording Source.
    _eventMap[kAAFClassID_ImportDescriptor] = L"Import Source";       //Encountered an Import Source.
    _eventMap[kAAFClassID_TapeDescriptor] = L"Tape Source";           //Encountered a Tape Source.
    _eventMap[kAAFClassID_FilmDescriptor] = L"Film Source";           //Encountered a Film Source.
    _eventMap[kAAFClassID_SourceClip] = L"End of Derivation Chain";   //Used to indicate the End of Chain.
    _eventMap[kAAFClassID_SourceMob] = L"Out of File Reference";      //Used to indicate an out of file reference.

    //Include the EssenceDescriptor as a valid event.  EssenceDescriptor should
    //be passed to the state machine when an EssenceDescriptor that does not
    //belong in the derivation chain is encountered.  Therefore, there will
    //be no transition triggered by this event.
    _eventMap[kAAFClassID_EssenceDescriptor] = L"Unknown Essence Descriptor";
    
    //Similarily, this is here to ensure that every object has a parent object
    //that will be accepted by the state machine, however, no transitions will
    //be triggered by this event.
    _eventMap[kAAFClassID_InterchangeObject] = L"Unknown";
}

void DerivationChainStateMachine::SetFailures()
{
    
    //Set up the map of requirement ids to return when a transition fails
    //depending on the current state.
    
    //Note: REQ_EP_018, REQ_EP_019, REQ_EP_025 and REQ_EP_030 also
    //      can fail while testing the derivation chain, however, they depend
    //      on factors other than the current state and must be tested in
    //      the transition functions.
    //      REQ_EP_017 fails when TransitionBack fails, however, it is not
    //      returned, it is up to the calling test to set this.

    //Also, these failures are caused by transitions between different types
    //of nodes in the derivation chain.  References out of the file and to the
    //end of the derivation chain do not necessarily cause these errors, they
    //are dealt with seperatley.
         
    //INITIAL -> There is no clear requirement specifying this error.
        
    _failureMap[TOP_LEVEL].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_026" ) );
    _failureMap[LOWER_LEVEL].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_031" ) );
    _failureMap[SUB_CLIP].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_036" ) );
    _failureMap[ADJUSTED_CLIP].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_045" ) );
    _failureMap[TEMPLATE_CLIP].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_050" ) );
    _failureMap[CLIP].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_055" ) );
    _failureMap[FILE_SOURCE].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_063" ) );
    _failureMap[FILE_SOURCE].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_064" ) );
    _failureMap[RECORDING_SOURCE].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_072" ) );
    _failureMap[IMPORT_SOURCE].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_076" ) );
    _failureMap[TAPE_SOURCE].push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_083" ) );
    //FILM_SOURCE -> There is not clear requirement specifying this failure.

}

DerivationChainStateMachine::~DerivationChainStateMachine()
{
}
  
bool DerivationChainStateMachine::Transition( aafUID_t event, const AxString& nextName, AxString& detail, vector<shared_ptr<const Requirement> >& requirements )
{

    //Currently traversing down the tree.
    _reversing = false;

    EventMap::const_iterator nameIter = _eventMap.find( event );
    
    if ( nameIter == _eventMap.end() )
    {
        throw StateMachineException( L"Attempting to transition in the Derivation Chain State Machine with an unknown event." );
    }

    State currentState = _currentState.top();

    TransitionMap::const_iterator transIter = _transitionMap.find( make_pair( currentState, event ) );
    
    detail = Indent() + L"Transition to " + nameIter->second + L" (" + nextName + L")";

    if ( transIter != _transitionMap.end() )
    {
        _currentState.push( transIter->second );
        detail = L"OK   - " + detail;
    }
    else
    {
        detail = L"FAIL - " + detail;
        
        //Populate the failing requirement vector, End of Chain and Out of File
        //events require special processing.
        if ( event == kAAFClassID_SourceClip )
        {
            if ( currentState == FILE_SOURCE )
            {
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_064" ) );
            }
        }
        else if ( event == kAAFClassID_SourceMob )
        {
            if ( currentState == TOP_LEVEL || currentState == LOWER_LEVEL ||
                 currentState == SUB_CLIP || currentState == ADJUSTED_CLIP )
            {
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_018" ) );
            }
            else if ( currentState == TEMPLATE_CLIP )
            {
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_018" ) );
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_050" ) );
            }
            else if ( currentState == CLIP || currentState == FILE_SOURCE ||
                      currentState == IMPORT_SOURCE ||
                      currentState == TAPE_SOURCE || currentState == FILM_SOURCE )
            {
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_019" ) );
            }
            else if ( currentState == RECORDING_SOURCE )
            {
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_019" ) );
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_072" ) );
            }
        }
        else
        {
            for (unsigned int i=0; i < _failureMap[currentState].size(); i++)
            {
                requirements.push_back( _failureMap[currentState].at(i) );
            }
            if ( event == kAAFUsage_TopLevel )
            {
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_025" ) );
            }
            else if ( event == kAAFUsage_LowerLevel )
            {
                requirements.push_back( RequirementRegistry::GetInstance().GetRequirement( L"REQ_EP_030" ) );
            }
        }

        //This test failed, one of two things may happen:
        //(a) The current state may be poped off the stack in which case the
        //    state machine is reversing, or
        //(b) Another child of the current state will be visited, in which vase
        //    the reversing property will be reset
        //Therefore, set _reversing to true to prevent an invalid reversal from
        //being reported.
        _reversing = true;

        return false;
    }
    
    return true;

}

bool DerivationChainStateMachine::TransitionBack()
{
    
    bool successfulTransition = true;
    State current = _currentState.top();
    _currentState.pop();
    
    //If the direction of tree traversal has changed at a point that is not the
    //end of the derivation chain or a reference out of the file, report an
    //error.
    if ( !_reversing && (current != EOC || current != OOF) )
    {
        successfulTransition = false;
    }
    
    _reversing = true;
    return successfulTransition;
    
}

bool DerivationChainStateMachine::IsKnownEvent( const aafUID_t event ) const
{
    return !(_eventMap.find( event ) == _eventMap.end());
}

AxString DerivationChainStateMachine::Indent()
{

  unsigned int l = _currentState.size() - 1;
  AxString indent;

  for (; l > 0 ; l-- ) 
  {
    indent += L"  ";
  }
  return indent;

}

} // end of namespace aafanalyzer

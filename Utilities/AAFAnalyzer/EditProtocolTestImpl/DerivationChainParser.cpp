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
#include "DerivationChainParser.h"
#include "DerivationChainParserException.h"

//TestResult
#include <TestLevelTestResult.h>

//Requirement files
#include <RequirementRegistry.h>

//AAF files
#include <AAFExtEnum.h>
#include <AAFClassDefUIDs.h>

#include <sstream>

#include <iostream>

// set to 1 to activate debug output
#define DEBUG_OUT 0

namespace {

using namespace aafanalyzer;

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;
using namespace boost;

DerivationChainParser::DerivationChainParser( wostream& log,
					      shared_ptr<TestLevelTestResult> spTestResult )
  : _log( log ),
    _spTestResult( spTestResult )
{
  _currentState.push( INITIAL );
  _objNameStack.push( L"Initial" );
}


void DerivationChainParser::StaticInit()
{
  if ( _staticInitFlag )
  {
    _staticInitFlag = false;
    InitTransitions();
    InitEvents();
    InitStateToReq();
    InitStateNames();
  }
}

void DerivationChainParser::InitTransitions()
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
  _transitionMap[make_pair( TEMPLATE_CLIP, EOC_EVENT_ID )] = EOC;
  
  //Transitions out of the CLIP state:
  _transitionMap[make_pair( CLIP, kAAFClassID_FileDescriptor )] = FILE_SOURCE;
  _transitionMap[make_pair( CLIP, kAAFClassID_ImportDescriptor )] = IMPORT_SOURCE;
  _transitionMap[make_pair( CLIP, kAAFClassID_TapeDescriptor )] = TAPE_SOURCE;
  _transitionMap[make_pair( CLIP, kAAFClassID_FilmDescriptor )] = FILM_SOURCE;
  _transitionMap[make_pair( CLIP, OOF_EVENT_ID )] = OOF;
  
  //Transitions out of the FILE_SOURCE state:
  _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_FileDescriptor )] = FILE_SOURCE;
  _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_RecordingDescriptor )] = RECORDING_SOURCE;
  _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_ImportDescriptor )] = IMPORT_SOURCE;
  _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_TapeDescriptor )] = TAPE_SOURCE;
  _transitionMap[make_pair( FILE_SOURCE, kAAFClassID_FilmDescriptor )] = FILM_SOURCE;
  _transitionMap[make_pair( FILE_SOURCE, EOC_EVENT_ID )] = EOC;
  _transitionMap[make_pair( FILE_SOURCE, OOF_EVENT_ID )] = OOF;
  
  //Transitions out of the RECORDING_SOURCE state:
  _transitionMap[make_pair( RECORDING_SOURCE, EOC_EVENT_ID )] = EOC;
  
  //Transitions out of the IMPORT_SOURCE state:
  _transitionMap[make_pair( IMPORT_SOURCE, kAAFClassID_TapeDescriptor )] = TAPE_SOURCE;
  _transitionMap[make_pair( IMPORT_SOURCE, kAAFClassID_FilmDescriptor )] = FILM_SOURCE;
  _transitionMap[make_pair( IMPORT_SOURCE, EOC_EVENT_ID )] = EOC;
  _transitionMap[make_pair( IMPORT_SOURCE, OOF_EVENT_ID )] = OOF;
  
  //Transitions out of the TAPE_SOURCE state:
  _transitionMap[make_pair( TAPE_SOURCE, kAAFClassID_FilmDescriptor )] = FILM_SOURCE;
  _transitionMap[make_pair( TAPE_SOURCE, EOC_EVENT_ID )] = EOC;
  _transitionMap[make_pair( TAPE_SOURCE, OOF_EVENT_ID )] = OOF;
  
  //Transitions out of the FILM_SOURCE state:
  _transitionMap[make_pair( FILM_SOURCE, EOC_EVENT_ID )] = EOC;
}

void DerivationChainParser::InitEvents()
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
  _eventMap[EOC_EVENT_ID] = L"End of Derivation Chain";             //Used to indicate the End of Chain.
  _eventMap[OOF_EVENT_ID] = L"Out of File Reference";               //Used to indicate an out of file reference.
  
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

void DerivationChainParser::InitStateToReq()
{
  // Map the state to the requirement that should fail if a parse
  // error occurs while in that states (i.e. receive an event for
  // which there is no transition. A single requirement should specify
  // the valid transitions for each state.

  _stateToReqMap[INITIAL]          = L"REQ_EP_260";
  _stateToReqMap[TOP_LEVEL]        = L"REQ_EP_026";
  _stateToReqMap[LOWER_LEVEL]      = L"REQ_EP_031";
  _stateToReqMap[SUB_CLIP]         = L"REQ_EP_036";
  _stateToReqMap[ADJUSTED_CLIP]    = L"REQ_EP_045";
  _stateToReqMap[TEMPLATE_CLIP]    = L"REQ_EP_050";
  _stateToReqMap[CLIP]             = L"REQ_EP_055";
  _stateToReqMap[FILE_SOURCE]      = L"REQ_EP_063";
  _stateToReqMap[RECORDING_SOURCE] = L"REQ_EP_072";
  _stateToReqMap[IMPORT_SOURCE]    = L"REQ_EP_076";
  _stateToReqMap[TAPE_SOURCE]      = L"REQ_EP_083";
  _stateToReqMap[FILM_SOURCE]      = L"REQ_EP_259";
  _stateToReqMap[EOC]              = L"REQ_EP_017";
  _stateToReqMap[OOF]              = L"REQ_EP_261";
}

void DerivationChainParser::InitStateNames()
{
#define _LSTRING(X,Y) X##Y
#define TOLSTRING(S) _LSTRING(L,#S)
#define STATE_NAME( X ) _stateNameMap[ X ] = TOLSTRING(X) ;
#define STATE_NAME_XY( X, Y ) _stateNameMap[ X ] = Y ;

  STATE_NAME( INITIAL );
  STATE_NAME( TOP_LEVEL );
  STATE_NAME( LOWER_LEVEL );
  STATE_NAME( SUB_CLIP );
  STATE_NAME( ADJUSTED_CLIP );
  STATE_NAME( TEMPLATE_CLIP );
  STATE_NAME( CLIP );
  STATE_NAME( FILE_SOURCE );
  STATE_NAME( RECORDING_SOURCE );
  STATE_NAME( IMPORT_SOURCE );
  STATE_NAME( TAPE_SOURCE );
  STATE_NAME( FILM_SOURCE );
  STATE_NAME_XY( EOC, L"END OF CHAIN" );
  STATE_NAME_XY( OOF, L"OUT OF FILE"  );

#undef _LSTRING
#undef TOLSTRING
#undef STATE_NAME
#undef STATE_NAME_XY
}

DerivationChainParser::~DerivationChainParser()
{}
  
void DerivationChainParser::AddFinalNotationalResult()
{
  StateToReqMap::const_iterator stateToReqIter = _stateToReqMap.find( _currentState.top() );
  assert( stateToReqIter != _stateToReqMap.end() );

  _spTestResult->AddSingleResult( stateToReqIter->second,
				  L"The derivation chain parser reached a valid end state.",
				  TestResult::NOTED );
}

void DerivationChainParser::AddNotationalResult( const wstring& nextName,
						 State currentState )
{
  std::wstringstream explain;
  explain << nextName << L" following " << _stateNameMap[currentState] << L" is valid.";

  StateToReqMap::const_iterator stateToReqIter = _stateToReqMap.find(currentState);
  assert( stateToReqIter != _stateToReqMap.end() );

  _spTestResult->AddSingleResult( stateToReqIter->second,
				  explain.str(),
				  TestResult::NOTED );
}

void DerivationChainParser::AddParseErrorResults( const wstring& curName,
						  const wstring& nextName,
						  const EventID& event,
						  State currentState )
{
  // First the basic transition error that is generated for all failures.
  std::wstringstream explain;
  explain << nextName << L" following " << _stateNameMap[currentState] << L" is not a valid.";
  
  StateToReqMap::const_iterator stateToReqIter = _stateToReqMap.find(currentState);
  assert( stateToReqIter != _stateToReqMap.end() );
  
  _spTestResult->AddSingleResult( stateToReqIter->second,
				  explain.str(),
				  TestResult::FAIL );
}

bool DerivationChainParser::IsInEndState() const
{
  if ( _currentState.top() == EOC ||
       _currentState.top() == OOF  )
  {
    return true;
  }
  else
  {
    return false;
  }
}

void DerivationChainParser::CheckEndState()
{
  if ( !IsInEndState() )
  {
    wstringstream explain;
    explain << _objNameStack.top() << L" cannot be the last object in a derivation chain.";

    _spTestResult->AddSingleResult( L"REQ_EP_262",
				    explain.str(),
				    TestResult::FAIL );

    // Also add to to the tes result details because it makes
    // debugging easier (when it is see next to all the other details.
    AddDetail( false, explain.str() );
  }
}

bool DerivationChainParser::Transition( const EventID& event,
					const AxString& nextName )
{
  EventMap::const_iterator eventNameIter = _eventMap.find( event );
  
  // JPT - Review if this happens it is an implementation error. We
  // should never be sending an unknown event to this parser.
  if ( eventNameIter == _eventMap.end() )
  {
    throw DerivationChainParserException( L"Attempting to transition in the "
					  "Derivation Chain State Machine with an unknown event." );
  }

  State currentState = _currentState.top();

  TransitionMap::const_iterator transIter = _transitionMap.find( make_pair( currentState, event ) );
  
  // If we found a transition for the current state and event pair
  // then all is well, we haven't violated and EP derivation chain
  // rules. Set the current state to the next state we looked up in
  // the transition map, and generate a detail message.
  //
  // Add a nototional result indicating that the requirement that
  // defines the valid transitions out of the currentState has been
  // satisfied.

  if ( transIter != _transitionMap.end() )
  {
    std::wstringstream ss;
    ss << L"Valid object " << nextName 
       << L" transitioning from state " << _stateNameMap[currentState]
       << L" to state " << _stateNameMap[transIter->second];
    AddDetail( true, ss.str() );

    AddNotationalResult( nextName, currentState );

    // Update the current state.
    _currentState.push( transIter->second );
    _objNameStack.push( nextName );

    if ( IsInEndState() )
    {
      AddFinalNotationalResult();
    }

    return true;
  }

  // Else, we failed to find a transition given the current
  // state/event pair.  This means that the event (aka objct) that was
  // passed into to the parser has violated of the EP derivation chain
  // rules.  We report the falure against the requirement that is
  // associated with currentState because that is one that specifies
  // the next valid state - hence the one that has been violated.
  //
  // Determine the failing requirment and generate a detailed error
  // message. Since the transition failed we can't continue parsing.
  //
  // The failing requirements is determined by the current state.
  // e.g. if we are in the state CLIP and the trantiion lookup failed
  // then the event (i.e. next relevant object in the AAF graph) was
  // not a file source, import source, tape source, film source, or
  // out of file reference.
  
  else
  {
    std::wstringstream ss;
    ss << L"Invalid object " << nextName 
       << L" while in state " << _stateNameMap[currentState];
    AddDetail( false, ss.str() );

    AddParseErrorResults( _objNameStack.top(), nextName, event, currentState );

    return false;
  }        
}

bool DerivationChainParser::IsKnownEvent( const aafUID_t event )
{
    return !(_eventMap.find( event ) == _eventMap.end());
}

void DerivationChainParser::AddDetail( bool ok, const wstring& detail )
{
  std::wstringstream ss;
  if ( ok )
  {
    ss << L"OK   - ";
  }
  else
  {
    ss << L"FAIL  - ";
  }

  ss << Indent() << detail;

  _spTestResult->AddDetail( ss.str() );
}

AxString DerivationChainParser::Indent()
{

  size_t l = _currentState.size() - 1;
  AxString indent;

  for (; l > 0 ; l-- ) 
  {
    indent += L"  ";
  }
  return indent;

}

// static members declare

const DerivationChainParser::EventID
DerivationChainParser::EOC_EVENT_ID = {0x83468e99, 0x6f5c, 0x4e85,
				       {0xb8, 0x3f, 0x4d, 0xe6, 0x10, 0xf0, 0xbb, 0x04}};

const DerivationChainParser::EventID
DerivationChainParser::OOF_EVENT_ID = {0x805d47fb, 0x539f, 0x4b67,
				       {0x9b, 0xec, 0x4b, 0x24, 0x4f, 0x3d, 0x17, 0xd2}};

bool DerivationChainParser::_staticInitFlag = true;
DerivationChainParser::TransitionMap DerivationChainParser::_transitionMap;
DerivationChainParser::EventMap DerivationChainParser::_eventMap;
DerivationChainParser::StateToReqMap DerivationChainParser::_stateToReqMap;
DerivationChainParser::StateNameMap DerivationChainParser::_stateNameMap;


// Global static init of the derivation chain parser tables.
class DerivationChainParserGlobalInit
{
public:
  DerivationChainParserGlobalInit()
  {
    DerivationChainParser::StaticInit();
  }
} globalDerivationChainParserInit;

} // end of namespace aafanalyzer

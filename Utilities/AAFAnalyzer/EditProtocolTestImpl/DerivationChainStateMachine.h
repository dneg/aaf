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

#ifndef __DERIVATIONCHAINSTATEMACHINE_H_
#define __DERIVATIONCHAINSTATEMACHINE_H_

//Requirement files
#include <Requirement.h>

//Ax files
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <map>
#include <stack>
#include <vector>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class DerivationChainStateMachine
{
  typedef aafUID_t EventID;

 public:

  DerivationChainStateMachine( wostream& log );
  ~DerivationChainStateMachine();
  
  bool Transition( const EventID& event,
                   const AxString& nextName,
                   AxString& detail,
                   vector<shared_ptr<const Requirement> >& requirements );

  bool TransitionBack( AxString& detail );

  bool IsKnownEvent( const aafUID_t event ) const;

 private:
 
  enum State { INITIAL, TOP_LEVEL, LOWER_LEVEL, SUB_CLIP, ADJUSTED_CLIP,
               TEMPLATE_CLIP, CLIP, FILE_SOURCE, RECORDING_SOURCE, IMPORT_SOURCE,
               TAPE_SOURCE, FILM_SOURCE,
               EOC, // End Of Chain (zero valued source clip mob id)
               OOF  // Out Of File  (out of file reference)
             };
 
  void SetTransitions();
  void SetEvents();
  void SetFailures();
  void SetStateNames();
  AxString Indent();

  typedef map<pair<const State, const EventID>, State> TransitionMap;
  typedef map<EventID, AxString> EventMap;
  typedef map<State, vector<shared_ptr<const Requirement > > > FailureMap;
  typedef map<State, AxString> StateNameMap;

  TransitionMap _transitionMap;
  EventMap _eventMap;
  FailureMap _failureMap;
  StateNameMap _stateNameMap;
  stack<State> _currentState;
  wostream& _log;
  bool _reversing;

  // prohibited
  DerivationChainStateMachine();
  DerivationChainStateMachine( const DerivationChainStateMachine& );
  DerivationChainStateMachine& operator=( const DerivationChainStateMachine& );

};

} // end of namespace aafanalyzer

#endif /*__DERIVATIONCHAINSTATEMACHINE_H_*/

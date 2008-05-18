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

#ifndef __EPEditRateVisitor_h_
#define __EPEditRateVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <AAFTypedObjNode.h>
#include "AllowedEditRateTable.h"

//Test/Result files
#include <TestResult.h>

//Ax files
#include <AxTypes.h>

//STL files
#include <set>

//Not in the aafanalyzer namespace so the forward declaration must be made
//outside of the namespace.
class AxMobSlot;
class AxDataDef;

namespace aafanalyzer {

using namespace boost;

class TestLevelTestResult;
class EPAudioTrack;
class EPVideoTrack;

class EPEditRateVisitor : public EPTypedVisitor
{

  public:

    EPEditRateVisitor( wostream& log,
		       shared_ptr<EdgeMap> spEdgeMap,
		       shared_ptr<TestLevelTestResult> spTestResult );
    
    virtual ~EPEditRateVisitor();

    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEssenceTrack>& node );
    
    void CheckAudioSampleRates();
    
  private:
   
    // prohibited
    EPEditRateVisitor();
    EPEditRateVisitor( const EPEditRateVisitor& );
    EPEditRateVisitor& operator=( const EPEditRateVisitor& );
  
    typedef pair<aafInt32, aafInt32> RationalKey;
    typedef map<RationalKey, unsigned int> RateMap;
   
    wostream& _log;
    shared_ptr<EdgeMap> _spEdgeMap;
    shared_ptr<TestLevelTestResult> _spTestResult;
    AllowedEditRateTable _erTable;

    RateMap _audioEditRates;
    RateMap _audioSampleRates;

    unsigned int _unknownAudioTracks;

    bool TestEditRate( aafRational_t editRate, AxMobSlot& axMobSlot, const AxString& mobName, Node& node );
    void VisitAudioTrack( shared_ptr<EPAudioTrack> spTrack, aafRational_t editRate, Node& node );
    bool VisitVideoTrack( shared_ptr<EPVideoTrack> spTrack, aafRational_t editRate, Node& node );
};

} // end of namespace diskstream

#endif /*__EPEditRateVisitor_h_*/

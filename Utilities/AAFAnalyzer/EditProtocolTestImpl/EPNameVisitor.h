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

#ifndef __EPNameVisitor_h_
#define __EPNameVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//Ax files
#include <AxMob.h>

//STL files
#include <map>
#include <set>

class AxDefObject;

namespace aafanalyzer {

class DetailLevelTestResult;

using namespace boost;
using namespace std;

class EPNameVisitor : public EPTypedVisitor
{

  public:
  
    EPNameVisitor( wostream& log, shared_ptr<EdgeMap> spEdgeMap );

    virtual ~EPNameVisitor();

    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node ); 
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node );
    
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEssenceTrack>& node );
    
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFDataDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFParameterDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFPluginDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFContainerDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFInterpolationDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTaggedValueDefinition>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCodecDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFKLVDataDefinition>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFDefObject>& node );
    
    shared_ptr<DetailLevelTestResult> GetResult();
    
    void CheckForUniqueNames();

  private:
  
    typedef map<AxString, unsigned int> NameMap;
    typedef set<AxString> NameSet;
  
    wostream& _log;
    shared_ptr<EdgeMap> _spEdgeMap;
    
    shared_ptr<DetailLevelTestResult> _spResult;
    NameMap _compositionNames;
    NameMap _essenceTrackNames;
    NameSet _topLevelNames;
    NameSet _lowerLevelNames;
    
    bool VisitComposition( const AxString& type, const AxString& reqId, AxCompositionMob& axCompMob );
    bool VisitNonComposition( const AxString& type, const AxString& reqId, AxMob& axMob );
    void CheckForUniqueNames( NameSet& names, const AxString& reqId, const AxString& type );

    // prohibited
    EPNameVisitor();
    EPNameVisitor( const EPNameVisitor& );
    EPNameVisitor& operator=( const EPNameVisitor& );
  
};

} // end of namespace diskstream

#endif /*__EPNameVisitor_h_*/

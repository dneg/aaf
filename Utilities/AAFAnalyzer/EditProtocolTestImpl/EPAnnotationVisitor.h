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

#ifndef __EPAnnotationVisitor_h_
#define __EPAnnotationVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//STL files
#include <stack>
#include <set>

class AxComponent;

namespace aafanalyzer {

class TestLevelTestResult;

using namespace boost;
using namespace std;

class EPAnnotationVisitor : public EPTypedVisitor
{

  public:
  
    EPAnnotationVisitor( wostream& log,
			 shared_ptr<EdgeMap> spEdgeMap,
			 shared_ptr<TestLevelTestResult> _spTestResult );
    virtual ~EPAnnotationVisitor();

    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFComponent>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTransition>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSegment>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSequence>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFFiller>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceReference>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFEvent>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFGPITrigger>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCommentMarker>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFDescriptiveMarker>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFEdgecode>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTimecode>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTimecodeStream>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTimecodeStream12M>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFPulldown>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFNestedScope>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFScopeReference>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFEssenceGroup>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSelector>& node );
    
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTaggedValue>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFKLVData>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTaggedValueDefinition>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFKLVDataDefinition>& node );
    
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPTimecodeTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEdgecodeTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPNonEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot,   EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot,   EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot,   EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot,   EPTimecodeTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot,   EPEdgecodeTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot,   EPNonEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot,    EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot,    EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot,    EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot,    EPTimecodeTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot,    EPEdgecodeTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot,    EPNonEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot,         EPAudioTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot,         EPVideoTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot,         EPEssenceTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot,         EPTimecodeTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot,         EPEdgecodeTrack>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot,         EPNonEssenceTrack>& node );
    
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFComponent>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFTransition>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSegment>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSequence>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFFiller>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSourceReference>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFEvent>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFGPITrigger>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFCommentMarker>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFDescriptiveMarker>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFEdgecode>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFTimecode>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFTimecodeStream>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFTimecodeStream12M>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFPulldown>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFNestedScope>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFScopeReference>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFEssenceGroup>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSelector>& node );
    
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node );
    
    void CheckForTaggedValueDefinitions();
    void CheckForKLVValueDefinitions();
    
  private:
  
    wostream& _log;
    shared_ptr<EdgeMap> _spEdgeMap;
    shared_ptr<TestLevelTestResult> _spTestResult;
    
    stack<bool> _isAncestorEssenceTrack;
    stack<bool> _isAncestorEventMobSlot;
    stack<bool> _isParentMobSlot;
    
    set<AxString> _taggedValueNames;
    set<AxString> _taggedValueDefs;
    set<aafUID_t> _klvDataKeys;
    set<aafUID_t> _klvDataDefs;
    
    bool PopStacks();
       
    // prohibited
    EPAnnotationVisitor();
    EPAnnotationVisitor( const EPAnnotationVisitor& );
    EPAnnotationVisitor& operator=( const EPAnnotationVisitor& );
  
};

} // end of namespace diskstream

#endif /*__EPAnnotationVisitor_h_*/

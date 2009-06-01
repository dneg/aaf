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

//Edit Protocol Analyzer Base files
#include "EPTypedVisitor.h"

//Analyzer Base files
#include <DepthFirstTraversal.h>
#include <EdgeMap.h>

//Ax files
#include <AxMob.h>
#include <AxEx.h>
#include <AxMetaDef.h>
#include <AxMobSlot.h>
#include <AxDefObject.h>

//AAF files
#include <AAFResult.h>

//STL files
#include <sstream>

namespace {

using namespace aafanalyzer;

class ParentMobVisitor : public EPTypedVisitor
{
    public:
        ParentMobVisitor()
        {}

        ~ParentMobVisitor()
        {}

        bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPTopLevelComposition::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPLowerLevelComposition::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPSubClipComposition::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPAdjustedClipComposition::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
        {
            AxMasterMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPTemplateClip::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
        {
            AxMasterMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPClip::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRGBAImageFileSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPRGBAImageFileSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPCDCIImageFileSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPCDCIImageFileSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImageFileSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPImageFileSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMonoAudioFileSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPMonoAudioFileSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPMultiChannelAudioFileSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPFileSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPRecordingSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPImportSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPTapeSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPFilmSource::GetName() );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPAuxiliarySource>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, EPAuxiliarySource::GetName() );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
        {
            AxCompositionMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, L"Composition Mob" );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
        {
            AxMasterMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, L"Master Mob" );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
        {
            AxSourceMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, L"Source Mob" );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFMob>& node )
        {
            AxMob axMob( node.GetAAFObjectOfType() );
            _parentName = this->GetMobName( axMob, L"Mob" );
            return false;
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFTimelineMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFEventMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFStaticMobSlot>& node )
        {
            shared_ptr<AAFTypedObjNode<IAAFMobSlot> > spGeneric( node.DownCastToAAF<IAAFMobSlot>() );
            return this->PreOrderVisit( *spGeneric );
        }

        bool PreOrderVisit( AAFTypedObjNode<IAAFMobSlot>& node )
        {
            AxMobSlot axMobSlot( node.GetAAFObjectOfType() );
            aafSlotID_t slotId = axMobSlot.GetSlotID();

            wstringstream ss;
            ss << L"Mob slot with ID = " << slotId << " of ";
            _slotName = ss.str().c_str();
            return true;
        }

        const AxString GetParentMobName()
        {
            return _parentName;
        }

        const AxString GetParentSlotName()
        {
            return _slotName + _parentName;
        }

    private:

        AxString _parentName;
        AxString _slotName;

        // prohibited
        ParentMobVisitor( const ParentMobVisitor& );
        ParentMobVisitor& operator=( const ParentMobVisitor& );
};

class ChildMobSlotVisitor : public EPTypedVisitor
{
    public:
        ChildMobSlotVisitor()
            : _spEssenceTracks( new EPTypedVisitor::MobSlotNodeSet ),
              _spAudioTracks( new EPTypedVisitor::MobSlotNodeSet ),
              _spVideoTracks( new EPTypedVisitor::MobSlotNodeSet ),
              _spTimecodeTracks( new EPTypedVisitor::MobSlotNodeSet ),
              _spEdgecodeTracks( new EPTypedVisitor::MobSlotNodeSet )
        {}

        ~ChildMobSlotVisitor()
        {}

        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEssenceTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPAudioTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            _spAudioTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPVideoTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            _spVideoTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPTimecodeTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spTimecodeTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFTimelineMobSlot, EPEdgecodeTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEdgecodeTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEssenceTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPAudioTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            _spAudioTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPVideoTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            _spVideoTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPTimecodeTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spTimecodeTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFStaticMobSlot, EPEdgecodeTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEdgecodeTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEssenceTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPAudioTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            _spAudioTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPVideoTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            _spVideoTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPTimecodeTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spTimecodeTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFEventMobSlot, EPEdgecodeTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEdgecodeTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEssenceTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPAudioTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            _spAudioTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPVideoTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEssenceTracks->insert( spAAFNode );
            _spVideoTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPTimecodeTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spTimecodeTracks->insert( spAAFNode );
            return false;
        }

        bool PreOrderVisit( EPTypedObjNode<IAAFMobSlot, EPEdgecodeTrack>& node )
        {
            shared_ptr<Node> spNode = node.GetSharedPointerToNode();
            shared_ptr<AAFObjNode> spAAFNode = dynamic_pointer_cast<AAFObjNode>( spNode );
            _spEdgecodeTracks->insert( spAAFNode );
            return false;
        }

        shared_ptr<EPTypedVisitor::MobSlotNodeSet> GetEssenceTracks()
        {
            return _spEssenceTracks;
        }

        shared_ptr<EPTypedVisitor::MobSlotNodeSet> GetAudioTracks()
        {
            return _spAudioTracks;
        }

        shared_ptr<EPTypedVisitor::MobSlotNodeSet> GetVideoTracks()
        {
            return _spVideoTracks;
        }

        shared_ptr<EPTypedVisitor::MobSlotNodeSet> GetTimecodeTracks()
        {
            return _spTimecodeTracks;
        }

        shared_ptr<EPTypedVisitor::MobSlotNodeSet> GetEdgecodeTracks()
        {
            return _spEdgecodeTracks;
        }

    private:

        shared_ptr<EPTypedVisitor::MobSlotNodeSet> _spEssenceTracks;
        shared_ptr<EPTypedVisitor::MobSlotNodeSet> _spAudioTracks;
        shared_ptr<EPTypedVisitor::MobSlotNodeSet> _spVideoTracks;
        shared_ptr<EPTypedVisitor::MobSlotNodeSet> _spTimecodeTracks;
        shared_ptr<EPTypedVisitor::MobSlotNodeSet> _spEdgecodeTracks;

        // prohibited
        ChildMobSlotVisitor( const ChildMobSlotVisitor& );
        ChildMobSlotVisitor& operator=( const ChildMobSlotVisitor& );
};

} // end of namespace

//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer
{

using namespace std;
using namespace boost;

EPTypedVisitor::EPTypedVisitor()
  : TypedVisitor()
{
}

EPTypedVisitor::EPTypedVisitor( Visitor::Follow_e follow )
  : TypedVisitor( follow )
{
}

EPTypedVisitor::~EPTypedVisitor()
{
}

AxString EPTypedVisitor::GetMobName( AxMob& axMob, const AxString& type )
{
    //Get the name of the mob
    AxString nodeName;
    try
    {
        nodeName = type + L" \"" + axMob.GetName() + L"\"";
    }
    catch ( const AxExHResult& ex )
    {
      if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
      {
        nodeName = L"Unnamed " + type;
      }
      else
      {
        throw ex;
      }
    }

    return nodeName;

}

AxString EPTypedVisitor::GetMobName( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ParentMobVisitor> spVisitor( new ParentMobVisitor );

    dfs.TraverseUp( spVisitor );
    return spVisitor->GetParentMobName();
}

AxString EPTypedVisitor::GetMobSlotName( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ParentMobVisitor> spVisitor( new ParentMobVisitor );

    dfs.TraverseUp( spVisitor );
    return spVisitor->GetParentSlotName();
}

shared_ptr<EPTypedVisitor::MobSlotNodeSet> EPTypedVisitor::GetEssenceTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );

    dfs.TraverseDown( spVisitor );
    return spVisitor->GetEssenceTracks();
}

shared_ptr<EPTypedVisitor::MobSlotNodeSet> EPTypedVisitor::GetAudioTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );

    dfs.TraverseDown( spVisitor );
    return spVisitor->GetAudioTracks();
}

shared_ptr<EPTypedVisitor::MobSlotNodeSet> EPTypedVisitor::GetVideoTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );

    dfs.TraverseDown( spVisitor );
    return spVisitor->GetVideoTracks();
}

shared_ptr<EPTypedVisitor::MobSlotNodeSet> EPTypedVisitor::GetTimecodeTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );

    dfs.TraverseDown( spVisitor );
    return spVisitor->GetTimecodeTracks();
}

shared_ptr<EPTypedVisitor::MobSlotNodeSet> EPTypedVisitor::GetEdgecodeTracks( shared_ptr<EdgeMap> spEdgeMap, Node& node )
{
    shared_ptr<Node> spNode = dynamic_pointer_cast<Node>( node.GetSharedPointerToNode() );
    DepthFirstTraversal dfs( spEdgeMap, spNode );
    shared_ptr<ChildMobSlotVisitor> spVisitor( new ChildMobSlotVisitor );

    dfs.TraverseDown( spVisitor );
    return spVisitor->GetEdgecodeTracks();
}


} // end of namespace diskstream


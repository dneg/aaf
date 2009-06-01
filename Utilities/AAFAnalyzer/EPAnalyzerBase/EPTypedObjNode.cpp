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
#include "EPTypedObjNode.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

using namespace boost;

template<typename AAFObjType, typename EPObjType>
EPTypedObjNode<AAFObjType, EPObjType>::EPTypedObjNode( const shared_ptr<AAFTypedObjNode<AAFObjType> > spNode, shared_ptr<EPObjType> spEPObject )
  : AAFTypedObjNodeDecorator<AAFObjType>( spNode ),
    _spEPObject( spEPObject )
{}

template<typename AAFObjType, typename EPObjType>
EPTypedObjNode<AAFObjType, EPObjType>::~EPTypedObjNode()
{}

template<typename AAFObjType, typename EPObjType>
bool EPTypedObjNode<AAFObjType, EPObjType>::PreOrderVisit(shared_ptr<Visitor> spVisitor)
{
  shared_ptr<EPTypedVisitor> spEPTypedVis = dynamic_pointer_cast<EPTypedVisitor>(spVisitor);
  if (spEPTypedVis)
  {
    return spEPTypedVis->PreOrderVisit(*this);
  }

  shared_ptr<TypedVisitor> spTypedVis = dynamic_pointer_cast<TypedVisitor>(spVisitor);
  if(spTypedVis)
  {
    return spTypedVis->PreOrderVisit(*this);
  }

  return spVisitor->PreOrderVisit(*this);
}

template<typename AAFObjType, typename EPObjType>
bool EPTypedObjNode<AAFObjType, EPObjType>::PostOrderVisit(shared_ptr<Visitor> spVisitor)
{
  shared_ptr<EPTypedVisitor> spEPTypedVis = dynamic_pointer_cast<EPTypedVisitor>(spVisitor);
  if (spEPTypedVis)
  {
    return spEPTypedVis->PostOrderVisit(*this);
  }
  shared_ptr<TypedVisitor> spTypedVis = dynamic_pointer_cast<TypedVisitor>(spVisitor);
  if(spTypedVis)
  {
    return spTypedVis->PostOrderVisit(*this);
  }

  return spVisitor->PostOrderVisit(*this);
}

template<typename AAFObjType, typename EPObjType>
shared_ptr<EPObjType> EPTypedObjNode<AAFObjType, EPObjType>::GetEPObject()
{
    return _spEPObject;
}

//Derivation Chain Object Nodes
template class EPTypedObjNode<IAAFCompositionMob,   EPTopLevelComposition>;
template class EPTypedObjNode<IAAFCompositionMob,   EPLowerLevelComposition>;
template class EPTypedObjNode<IAAFCompositionMob,   EPSubClipComposition>;
template class EPTypedObjNode<IAAFCompositionMob,   EPAdjustedClipComposition>;
template class EPTypedObjNode<IAAFMasterMob,        EPTemplateClip>;
template class EPTypedObjNode<IAAFMasterMob,        EPClip>;
template class EPTypedObjNode<IAAFSourceMob,        EPFileSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPMonoAudioFileSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPMultiChannelAudioFileSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPImageFileSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPRGBAImageFileSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPCDCIImageFileSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPRecordingSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPImportSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPTapeSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPFilmSource>;
template class EPTypedObjNode<IAAFSourceMob,        EPAuxiliarySource>;

//Mob Slot Object Nodes
template class EPTypedObjNode<IAAFTimelineMobSlot,  EPEssenceTrack>;
template class EPTypedObjNode<IAAFTimelineMobSlot,  EPAudioTrack>;
template class EPTypedObjNode<IAAFTimelineMobSlot,  EPVideoTrack>;
template class EPTypedObjNode<IAAFTimelineMobSlot,  EPNonEssenceTrack>;
template class EPTypedObjNode<IAAFTimelineMobSlot,  EPTimecodeTrack>;
template class EPTypedObjNode<IAAFTimelineMobSlot,  EPEdgecodeTrack>;
template class EPTypedObjNode<IAAFStaticMobSlot,    EPEssenceTrack>;
template class EPTypedObjNode<IAAFStaticMobSlot,    EPAudioTrack>;
template class EPTypedObjNode<IAAFStaticMobSlot,    EPVideoTrack>;
template class EPTypedObjNode<IAAFStaticMobSlot,    EPNonEssenceTrack>;
template class EPTypedObjNode<IAAFStaticMobSlot,    EPTimecodeTrack>;
template class EPTypedObjNode<IAAFStaticMobSlot,    EPEdgecodeTrack>;
template class EPTypedObjNode<IAAFEventMobSlot,     EPEssenceTrack>;
template class EPTypedObjNode<IAAFEventMobSlot,     EPAudioTrack>;
template class EPTypedObjNode<IAAFEventMobSlot,     EPVideoTrack>;
template class EPTypedObjNode<IAAFEventMobSlot,     EPNonEssenceTrack>;
template class EPTypedObjNode<IAAFEventMobSlot,     EPTimecodeTrack>;
template class EPTypedObjNode<IAAFEventMobSlot,     EPEdgecodeTrack>;
template class EPTypedObjNode<IAAFMobSlot,          EPEssenceTrack>;
template class EPTypedObjNode<IAAFMobSlot,          EPAudioTrack>;
template class EPTypedObjNode<IAAFMobSlot,          EPVideoTrack>;
template class EPTypedObjNode<IAAFMobSlot,          EPNonEssenceTrack>;
template class EPTypedObjNode<IAAFMobSlot,          EPTimecodeTrack>;
template class EPTypedObjNode<IAAFMobSlot,          EPEdgecodeTrack>;

//Effect Object Nodes
template class EPTypedObjNode<IAAFOperationGroup,   EPEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoDissolveEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPSMPTEVideoWipeEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoSpeedControlEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoRepeatEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoFlipEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoFlopEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoFlipFlopEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoPositionEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoCropEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoScaleEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoRotateEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPVideoCornerPinningEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPAlphaWithVideoKeyEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPSeparateAlphaKeyEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPLuminanceKeyEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPChromaKeyEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPMonoAudioGainEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPMonoAudioPanEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPSingleParameterAudioDissolveEffect>;
template class EPTypedObjNode<IAAFOperationGroup,   EPTwoParameterAudioDissolveEffect>;

//Effect Data Definition Nodes
template class EPTypedObjNode<IAAFOperationDef,     EPEffect>;


} // end of namespace diskstream

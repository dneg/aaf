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
EPTypedObjNode<AAFObjType, EPObjType>::EPTypedObjNode( const shared_ptr<AAFTypedObjNode<AAFObjType> > spNode )
  : AAFTypedObjNodeDecorator<AAFObjType>( spNode )
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

template class EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>;
template class EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>;
template class EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>;
template class EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>;
template class EPTypedObjNode<IAAFMasterMob, EPTemplateClip>;
template class EPTypedObjNode<IAAFMasterMob, EPClip>;
template class EPTypedObjNode<IAAFSourceMob, EPFileSource>;
template class EPTypedObjNode<IAAFSourceMob, EPRecordingSource>;
template class EPTypedObjNode<IAAFSourceMob, EPImportSource>;
template class EPTypedObjNode<IAAFSourceMob, EPTapeSource>;
template class EPTypedObjNode<IAAFSourceMob, EPFilmSource>;

} // end of namespace diskstream

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

//AAF Analyzer Base files
#include "AAFTypedObjNodeDecorator.h"
#include "TypedVisitor.h"

//AAF files
#include <AAF.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

using namespace boost;

template<typename AAFObjType>
AAFTypedObjNodeDecorator<AAFObjType>::AAFTypedObjNodeDecorator( const shared_ptr<AAFTypedObjNode<AAFObjType> > spNode )
  : AAFTypedObjNode<AAFObjType>( spNode->GetAAFObjectOfType(), spNode ),
    _spNode( spNode )
{}

template<typename AAFObjType>
AAFTypedObjNodeDecorator<AAFObjType>::~AAFTypedObjNodeDecorator()
{}

template<typename AAFObjType>
IAAFSmartPointer<AAFObjType> AAFTypedObjNodeDecorator<AAFObjType>::GetAAFObjectOfType() const
{
  return _spNode->GetAAFObjectOfType();
}

template<typename AAFObjType>
void AAFTypedObjNodeDecorator<AAFObjType>::Decorate( shared_ptr<Node> decoratedNode )
{
  return _spNode->Decorate( decoratedNode );
}

template class AAFTypedObjNodeDecorator<IAAFCompositionMob>;
template class AAFTypedObjNodeDecorator<IAAFMasterMob>;
template class AAFTypedObjNodeDecorator<IAAFSourceMob>;

template class AAFTypedObjNodeDecorator<IAAFTimelineMobSlot>;
template class AAFTypedObjNodeDecorator<IAAFStaticMobSlot>;
template class AAFTypedObjNodeDecorator<IAAFEventMobSlot>;
template class AAFTypedObjNodeDecorator<IAAFMobSlot>;

template class AAFTypedObjNodeDecorator<IAAFOperationGroup>;

template class AAFTypedObjNodeDecorator<IAAFOperationDef>;

} // end of namespace diskstream

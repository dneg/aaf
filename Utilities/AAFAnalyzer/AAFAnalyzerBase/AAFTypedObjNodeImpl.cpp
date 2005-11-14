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
#include "AAFTypedObjNodeImpl.h"
#include "TypedVisitor.h"
#include "MobNodeMap.h"

//Ax files
#include <AxMob.h>

//AAF files
#include <AAF.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

using namespace std;
using namespace boost;

template<typename AAFObjType>
AAFTypedObjNodeImpl<AAFObjType>::AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> spObject)
  : AAFTypedObjNode<AAFObjType>( spObject, L"" ),
    _spTypedObj( spObject )
{}

template<typename AAFObjType>
AAFTypedObjNodeImpl<AAFObjType>::AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> spObject,
					     const basic_string<wchar_t>& name)
  : AAFTypedObjNode<AAFObjType>( spObject, name ),
    _spTypedObj( spObject )
{}

template<typename AAFObjType>
AAFTypedObjNodeImpl<AAFObjType>::AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> spObject,
                         shared_ptr<Node> spNode )
  : AAFTypedObjNode<AAFObjType>( spObject, spNode ),
    _spTypedObj( spObject )
{}

template<typename AAFObjType>
AAFTypedObjNodeImpl<AAFObjType>::~AAFTypedObjNodeImpl()
{}

template<typename AAFObjType>
bool AAFTypedObjNodeImpl<AAFObjType>::PreOrderVisit(shared_ptr<Visitor> spVisitor)
{
  shared_ptr<TypedVisitor> spTypedVis = dynamic_pointer_cast<TypedVisitor>(spVisitor);
  if(spTypedVis)
  {
    return spTypedVis->PreOrderVisit(*this);
  }

  return spVisitor->PreOrderVisit(*this);
}

template<typename AAFObjType>
bool AAFTypedObjNodeImpl<AAFObjType>::PostOrderVisit(shared_ptr<Visitor> spVisitor)
{
  shared_ptr<TypedVisitor> spTypedVis = dynamic_pointer_cast<TypedVisitor>(spVisitor);
  if(spTypedVis)
  {
    return spTypedVis->PostOrderVisit(*this);
  }

  return spVisitor->PostOrderVisit(*this);
}

template<typename AAFObjType>
IAAFSmartPointer<AAFObjType> AAFTypedObjNodeImpl<AAFObjType>::GetAAFObjectOfType() const
{
  return _spTypedObj;
}

template<typename AAFObjType>
void AAFTypedObjNodeImpl<AAFObjType>::Decorate( shared_ptr<Node> decoratedNode )
{
    shared_ptr<AAFTypedObjNode<IAAFMob> > spMob = dynamic_pointer_cast<AAFTypedObjNode<IAAFMob> >( this->GetSharedPointerToNode() );
    if ( spMob )
    {
        AxMob axMob( spMob->GetAAFObjectOfType() );
        aafMobID_t mobid = axMob.GetMobID();
        MobNodeMap::GetInstance().DecorateMobNode( mobid, decoratedNode );
        return;
    }
}

#include "TypedNodeImplTemplate.cpp.gen"

} // end of namespace diskstream

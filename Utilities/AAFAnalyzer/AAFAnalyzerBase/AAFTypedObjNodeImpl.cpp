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

//AAF Analyzer Base files
#include "AAFTypedObjNodeImpl.h"
#include "TypedVisitor.h"
#include "MobNodeMap.h"

//Ax files
#include <AxMob.h>
#include <AxSmartPointer.h>

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
                         boost::shared_ptr<Node> spNode )
  : AAFTypedObjNode<AAFObjType>( spObject, spNode ),
    _spTypedObj( spObject )
{}

template<typename AAFObjType>
AAFTypedObjNodeImpl<AAFObjType>::~AAFTypedObjNodeImpl()
{}

template<typename AAFObjType>
bool AAFTypedObjNodeImpl<AAFObjType>::PreOrderVisit(boost::shared_ptr<Visitor> spVisitor)
{
  boost::shared_ptr<TypedVisitor> spTypedVis = boost::dynamic_pointer_cast<TypedVisitor>(spVisitor);
  if(spTypedVis)
  {
    return spTypedVis->PreOrderVisit(*this);
  }

  return spVisitor->PreOrderVisit(*this);
}

template<typename AAFObjType>
bool AAFTypedObjNodeImpl<AAFObjType>::PostOrderVisit(boost::shared_ptr<Visitor> spVisitor)
{
  boost::shared_ptr<TypedVisitor> spTypedVis = boost::dynamic_pointer_cast<TypedVisitor>(spVisitor);
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
void AAFTypedObjNodeImpl<AAFObjType>::Decorate( boost::shared_ptr<Node> decoratedNode )
{
    boost::shared_ptr<AAFObjNode> spMob = boost::dynamic_pointer_cast<AAFObjNode>( this->GetSharedPointerToNode() );
    if ( spMob )
    {
        AxObject axObject( spMob->GetAAFObject() );
        IAAFMobSP spMob;

        if ( AxIsA(axObject, spMob) )
        {
            AxMob axMob( spMob );
            aafMobID_t mobid = axMob.GetMobID();
            MobNodeMap::GetInstance().DecorateMobNode( mobid, decoratedNode );
        }
        return;
    }
}

#include "TypedNodeImplTemplate.cpp.gen"

} // end of namespace diskstream

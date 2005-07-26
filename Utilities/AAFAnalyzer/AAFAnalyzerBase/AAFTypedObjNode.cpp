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

#include "AAFTypedObjNode.h"

#include "TypedVisitor.h"
#include <AAF.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {


template<typename AAFObjType>
AAFTypedObjNode<AAFObjType>::AAFTypedObjNode(IAAFSmartPointer<AAFObjType> spObject)
  : Node( L"" ),
    _spTypedObj( spObject )
{}

template<typename AAFObjType>
AAFTypedObjNode<AAFObjType>::AAFTypedObjNode(IAAFSmartPointer<AAFObjType> spObject,
					     const std::basic_string<wchar_t>& name)
  : Node( name ),
    _spTypedObj( spObject )
{}

template<typename AAFObjType>
AAFTypedObjNode<AAFObjType>::~AAFTypedObjNode()
{
}

template<typename AAFObjType>
bool AAFTypedObjNode<AAFObjType>::PreOrderVisit(boost::shared_ptr<Visitor> spVisitor)
{
  boost::shared_ptr<TypedVisitor> spTypedVis = boost::dynamic_pointer_cast<TypedVisitor>(spVisitor);
  if(spTypedVis)
  {
    return spTypedVis->PreOrderVisit(*this);
  }

  return spVisitor->PreOrderVisit(*this);
}

template<typename AAFObjType>
bool AAFTypedObjNode<AAFObjType>::PostOrderVisit(boost::shared_ptr<Visitor> spVisitor)
{
  boost::shared_ptr<TypedVisitor> spTypedVis = boost::dynamic_pointer_cast<TypedVisitor>(spVisitor);
  if(spTypedVis)
  {
    return spTypedVis->PostOrderVisit(*this);
  }

  return spVisitor->PostOrderVisit(*this);
}

template<typename AAFObjType>
IAAFSmartPointer<AAFObjType> AAFTypedObjNode<AAFObjType>::GetAAFObjectType() const
{
  return _spTypedObj;
}

#include "TypedNodeTemplate.cpp.gen"

} // end of namespace diskstream

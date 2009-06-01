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
#include "AAFContainment.h"
#include "TypedVisitor.h"

#include <assert.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace boost;

AAFContainment::AAFContainment(shared_ptr<Node> spParent, shared_ptr<Node> spChild)
  : Edge(spParent, spChild, Edge::EDGE_KIND_CONTAINMENT, spParent->GetLID() )
{}

AAFContainment::~AAFContainment()
{}

bool AAFContainment::Visit(shared_ptr<Visitor> spVisitor)
{
  shared_ptr<TypedVisitor> spTypedVis = dynamic_pointer_cast<TypedVisitor>(spVisitor);
  if(spTypedVis)
  {
    return spTypedVis->EdgeVisit(*this);
  }

  return spVisitor->EdgeVisit(*this);
}

shared_ptr<Edge> AAFContainment::CreateNewEdge( shared_ptr<Node> spParent, shared_ptr<Node> spChild ) const
{
  shared_ptr<Edge> spNewEdge( new AAFContainment( spParent, spChild ) );
  return spNewEdge;
}

const std::wstring& AAFContainment::GetTypeName() const
{
  return typeName;
}

const std::wstring AAFContainment::typeName = L"containment";

} // end of namespace diskstream

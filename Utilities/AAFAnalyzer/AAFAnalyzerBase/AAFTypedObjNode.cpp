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
#include "AAFTypedObjNode.h"

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
AAFTypedObjNode<AAFObjType>::AAFTypedObjNode(IAAFSmartPointer<AAFObjType> spObject,
					     const basic_string<wchar_t>& name)
  : AAFObjNode(AxQueryInterface<AAFObjType,IAAFObject>( spObject ), name )
{}

template<typename AAFObjType>
AAFTypedObjNode<AAFObjType>::AAFTypedObjNode( shared_ptr<AAFTypedObjNode<AAFObjType> > spExistingNode )
  : AAFObjNode( spExistingNode->GetAAFObject(),
                spExistingNode->GetName(),
                spExistingNode->GetLID() )
{}

template<typename AAFObjType>
AAFTypedObjNode<AAFObjType>::~AAFTypedObjNode()
{}

#include "TypedNodeTemplate.cpp.gen"

} // end of namespace diskstream

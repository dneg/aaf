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


#include "NodeFactoryImpl.h"

#include "TypedNodeFactoryImpl.h"
#include "TypedNodeFactoryRegistry.h"
#include <AxObject.h>
#include <AxMetaDef.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

NodeFactoryImpl::NodeFactoryImpl()
{
}

NodeFactoryImpl::~NodeFactoryImpl()
{
}

boost::shared_ptr<Node> NodeFactoryImpl::CreateNode(IAAFObjectSP spObj)
{
  AxObject axObj(spObj);
  AxClassDef clsDef(axObj.GetDefinition());
  boost::shared_ptr<TypedNodeFactory> spNodeFactory;

  spNodeFactory = TypedNodeFactoryRegistry::GetInstance().LookUp(clsDef);

  return spNodeFactory->CreateNode(spObj);
}

} // end of namespace diskstream

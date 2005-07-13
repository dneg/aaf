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

#include "TypedNodeFactoryRegistry.h"

#include "TypedNodeFactory.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer {

//static variable
TypedNodeFactoryRegistry* TypedNodeFactoryRegistry::_pFactory = NULL;

TypedNodeFactoryRegistry::TypedNodeFactoryRegistry()
{
}

TypedNodeFactoryRegistry::~TypedNodeFactoryRegistry()
{
}

bool TypedNodeFactoryRegistry::IsPresent(aafUID_t AUID)
{
  return false;
}

bool TypedNodeFactoryRegistry::Deregister(aafUID_t AUID)
{
  return false;
}

TypedNodeFactoryRegistry& TypedNodeFactoryRegistry::GetInstance()
{
  //using Singleton pattern, create object iff it does not exist
  if(_pFactory == NULL)
  {
    _pFactory = new TypedNodeFactoryRegistry();
  }

  return *_pFactory;
}

boost::shared_ptr<TypedNodeFactory> TypedNodeFactoryRegistry::LookUp(aafUID_t AUID)
{
  // TO BE COMPLETED

  boost::shared_ptr<TypedNodeFactory> NOVAL;
  return NOVAL;
}

void TypedNodeFactoryRegistry::Register(aafUID_t AUID, boost::shared_ptr<TypedNodeFactory> spFactory)
{
  // TO BE COMPLETED
}

} // end of namespace diskstream

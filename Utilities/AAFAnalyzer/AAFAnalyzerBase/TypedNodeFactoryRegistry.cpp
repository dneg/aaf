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

#include "TypedNodeFactoryImpl.h"
#include <AAFMetaDictionary.h>
#include <iostream>

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
  //while() parse thru input file and register classes
  Register(AUID_AAFInterchangeObject, boost::shared_ptr<TypedNodeFactory>(new TypedNodeFactoryImpl<IAAFObject>()));
}

TypedNodeFactoryRegistry::~TypedNodeFactoryRegistry()
{
  if(_pFactory != NULL)
  {
    delete _pFactory;
    _pFactory = NULL;
  }
}

bool TypedNodeFactoryRegistry::IsPresent(aafUID_t AUID)
{
  Map::const_iterator iter = _Map.find(AUID);
  return iter != _Map.end();
}

bool TypedNodeFactoryRegistry::Deregister(aafUID_t AUID)
{
  // TO BE COMPLETED
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

boost::shared_ptr<TypedNodeFactory> TypedNodeFactoryRegistry::LookUp(AxClassDef& clsDef)
{
  //Find the Auid for the IAAFObject in question, if none exists look to its parent and continue
  //upwards until an Auid is found.  An exception will be thrown if no Auid is found at the
  //topmost level (i.e. no parent exists for IAAFObject).  This is not handled because it should 
  //not happen in a correct file.
  boost::shared_ptr<TypedNodeFactory> spNodeFactory;
  aafUID_t Auid = clsDef.GetAUID();

  if(IsPresent(Auid))//Base Case:
  {
    spNodeFactory = _Map[Auid];
  }
  else//Recursive Case:
  {
    AxClassDef clsParent(clsDef.GetParent());
    spNodeFactory = LookUp(clsParent);         
  }

  return spNodeFactory;
}

void TypedNodeFactoryRegistry::Register(aafUID_t AUID, boost::shared_ptr<TypedNodeFactory> spFactory)
{
  //only add a factory into Map if it is not already present
  if(!IsPresent(AUID))
  {
    _Map[AUID] = spFactory;
  }
}

} // end of namespace diskstream

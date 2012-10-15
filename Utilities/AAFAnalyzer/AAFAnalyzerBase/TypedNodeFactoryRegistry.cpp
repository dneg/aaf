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
#include "TypedNodeFactoryRegistry.h"
#include "TypedNodeFactoryImpl.h"

//Ax files
#include <AxMetaDef.h>
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

//static variable
TypedNodeFactoryRegistry* TypedNodeFactoryRegistry::_pFactory = NULL;

TypedNodeFactoryRegistry::TypedNodeFactoryRegistry()
{
  RegisterAAFObjects();
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
  aafUID_t Auid = clsDef.GetAUID();

  //Base Case:
  if(IsPresent(Auid))
  {
    return _Map[Auid];
  }  
  //Recursive Case:
  AxClassDef clsParent(clsDef.GetParent());

  return LookUp(clsParent);//Use Tail Recursion to avoid potential problem of stack overflow
}

void TypedNodeFactoryRegistry::Register(aafUID_t AUID, boost::shared_ptr<TypedNodeFactory> spFactory)
{
  //only add a factory into Map if it is not already present
  if(!IsPresent(AUID))
  {
    _Map[AUID] = spFactory;
  }
}

void TypedNodeFactoryRegistry::RegisterAAFObjects()
{
#include "RegistrationCode.cpp.gen"
}

} // end of namespace diskstream

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

#ifndef __TYPEDNODEFACTORYREGISTRY_h__
#define __TYPEDNODEFACTORYREGISTRY_h__

//AAF files
#include <AAFTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <map>

class AxClassDef;

namespace aafanalyzer {

using namespace std;

class TypedNodeFactory;

class TypedNodeFactoryRegistry
{
 public:

  typedef map<aafUID_t, boost::shared_ptr<TypedNodeFactory> > Map;

  // dtor must be public so that shared_ptr class can access it
  ~TypedNodeFactoryRegistry();

  boost::shared_ptr<TypedNodeFactory> LookUp(AxClassDef& clsDef);
  static TypedNodeFactoryRegistry& GetInstance();

 private:

  TypedNodeFactoryRegistry();//using Singleton pattern to allow only one object
  bool IsPresent(aafUID_t AUID);
  bool Deregister(aafUID_t AUID);
  void Register(aafUID_t AUID, boost::shared_ptr<TypedNodeFactory> spFactory);
  void RegisterAAFObjects();

  static TypedNodeFactoryRegistry* _pFactory;
  Map _Map;  

  // prohibited
  TypedNodeFactoryRegistry( const TypedNodeFactoryRegistry& );
  TypedNodeFactoryRegistry& operator=( const TypedNodeFactoryRegistry& );
};

} // end of namespace diskstream

#endif/*__TYPEDNODEFACTORYREGISTRY_h__*/

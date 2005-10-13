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

#ifndef __MOBNODEMAP_h__
#define __MOBNODEMAP_h__

//Ax files
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <map>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class Node;

class MobNodeMap
{
 public:

  typedef map<aafMobID_t, shared_ptr<Node> > Map;

  bool IsPresent(aafMobID_t mobid);
  static MobNodeMap& GetInstance();
  void AddMobNode(aafMobID_t mobid, shared_ptr<Node> spNode);
  void DecorateMobNode(aafMobID_t mobid, shared_ptr<Node> decoratedNode);
  shared_ptr<Node> GetMobNode(aafMobID_t mobid);

 private:

  MobNodeMap();
  virtual ~MobNodeMap();

  // prohibited
  MobNodeMap( const MobNodeMap& );
  MobNodeMap& operator=( const MobNodeMap& );

  static MobNodeMap* _pMobNodeMap;
  Map _Map;
};

} // end of namespace diskstream

#endif

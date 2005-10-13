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
#include "MobNodeMap.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================
namespace aafanalyzer {

using namespace boost;

//static variable
MobNodeMap* MobNodeMap::_pMobNodeMap = NULL;

MobNodeMap::MobNodeMap()
{
}

MobNodeMap::~MobNodeMap()
{
  if(_pMobNodeMap != NULL)
  {
    delete _pMobNodeMap;
    _pMobNodeMap = NULL;
  }
}

MobNodeMap& MobNodeMap::GetInstance()
{
  //using Singleton pattern, create object iff it does not exist
  if(_pMobNodeMap == NULL)
  {
    _pMobNodeMap = new MobNodeMap();
  }

  return *_pMobNodeMap;
}

bool MobNodeMap::IsPresent(aafMobID_t mobid)
{
  Map::const_iterator iter = _Map.find(mobid);
  return iter != _Map.end();
}

void MobNodeMap::AddMobNode(aafMobID_t mobid, shared_ptr<Node> spNode)
{
  //add mob to map if it is not already present
  if(!IsPresent(mobid))
  {
    _Map[mobid] = spNode;
  }
}

void MobNodeMap::DecorateMobNode(aafMobID_t mobid, shared_ptr<Node> decoratedNode)
{
  if(IsPresent(mobid))
  {
    _Map[mobid] = decoratedNode;
  }
}

shared_ptr<Node> MobNodeMap::GetMobNode(aafMobID_t mobid)
{ 
  shared_ptr<Node> spMobNode;

  if(IsPresent(mobid))
  {
    spMobNode = _Map[mobid];
  }

  return spMobNode;
}

}

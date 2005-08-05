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

#include "TempAllNodeMap.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================
namespace aafanalyzer {

//static variable
TempAllNodeMap* TempAllNodeMap::_pTempAllNodeMap = NULL;

TempAllNodeMap::TempAllNodeMap()
{
}

TempAllNodeMap::~TempAllNodeMap()
{
  if(_pTempAllNodeMap != NULL)
  {
    delete _pTempAllNodeMap;
    _pTempAllNodeMap = NULL;
  }
}

TempAllNodeMap& TempAllNodeMap::GetInstance()
{
  //using Singleton pattern, create object iff it does not exist
  if(_pTempAllNodeMap == NULL)
  {
    _pTempAllNodeMap = new TempAllNodeMap();
  }

  return *_pTempAllNodeMap;
}

void TempAllNodeMap::AddNode(unsigned int id, boost::shared_ptr<Node> spNode)
{
  _Map[id] = spNode;
}

boost::shared_ptr<Node> TempAllNodeMap::GetNode(unsigned int id)
{
  return _Map[id];
}

}

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

#include "EdgeMap.h"
#include "Edge.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================
namespace aafanalyzer
{

EdgeMap::EdgeMap()
{
}

EdgeMap::~EdgeMap()
{
}


EdgeMap::EdgeVectorSP EdgeMap::GetParents(boost::shared_ptr<Node> spNode) const
{
  if(!IsInMap(spNode->GetLID(), _children))
  {
    return EdgeVectorSP(new EdgeVector);//has no parents, size() == 0
  }

  return _children[spNode->GetLID()];   
}

EdgeMap::EdgeVectorSP EdgeMap::GetChildren(boost::shared_ptr<Node> spNode) const
{
  if(!IsInMap(spNode->GetLID(), _parents))
  {
    return EdgeVectorSP(new EdgeVector);//has no children, size() == 0
  }

  return _parents[spNode->GetLID()];   
}

bool EdgeMap::IsInMap(LID lid, const Map& map) const
{
  Map::const_iterator iter = map.find(lid);  
  return iter != map.end();
}

void EdgeMap::AddEdge(boost::shared_ptr<Edge> spEdge)
{
  LID lidParent = spEdge->GetParentNode()->GetLID();
  LID lidChild = spEdge->GetChildNode()->GetLID();

  if (!IsInMap(lidParent, _parents))
  {
     EdgeVectorSP spVector(new EdgeVector);
    _parents[lidParent] = spVector;
  }

  if (!IsInMap(lidChild, _children))
  {
     EdgeVectorSP spVector(new EdgeVector);
    _children[lidChild] = spVector;
  }

  _parents[lidParent]->push_back(spEdge);
  _children[lidChild]->push_back(spEdge);
}


}

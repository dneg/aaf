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

//Analyzer Base files
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
    
using namespace boost;

EdgeMap::EdgeMap()
{
}

EdgeMap::~EdgeMap()
{
}


EdgeMap::ConstEdgeVectorSP EdgeMap::GetParents(shared_ptr<Node> spNode) const
{
  if(!IsInMap(spNode->GetLID(), _children))
  {
    return EdgeVectorSP(new EdgeVector);//has no parents, size() == 0
  }

  return _children[spNode->GetLID()];   
}

EdgeMap::ConstEdgeVectorSP EdgeMap::GetChildren(shared_ptr<Node> spNode) const
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

void EdgeMap::AddEdge(shared_ptr<Edge> spEdge)
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

void EdgeMap::DecorateEdge(shared_ptr<Node> decoratedNode)
{

    LID id = decoratedNode->GetLID();
    
    //Get all edges where it is the parent node
    if ( IsInMap( id, _parents ) )
    {
        for (unsigned int i = 0; i < _parents[id]->size(); i++)
        {
            shared_ptr<Node> childNode = _parents[id]->at(i)->GetChildNode();
            LID cid = childNode->GetLID();
            for (unsigned int j = 0; j < _children[cid]->size(); j++)
            {
                if ( _children[cid]->at(j)->GetParentNode()->GetLID() == id )
                {
                    shared_ptr<Edge> decoratedChildEdge( new Edge( decoratedNode, childNode ) );
                    _children[cid]->at(j) = decoratedChildEdge;
                    break;
                }
            }
            shared_ptr<Edge> decoratedEdge( new Edge( decoratedNode, childNode ) );
            _parents[id]->at(i) = decoratedEdge;
        }

    }
    
    //Get all edges where it is the child node
    if ( IsInMap( id, _children ) )
    {
        for (unsigned int i = 0; i < _children[id]->size(); i++)
        {
            shared_ptr<Node> parentNode = _children[id]->at(i)->GetParentNode();
            LID pid = parentNode->GetLID();
            for (unsigned int j = 0; j < _parents[pid]->size(); j++)
            {
                if ( _parents[pid]->at(j)->GetChildNode()->GetLID() == id )
                {
                    shared_ptr<Edge> decoratedParentEdge( new Edge( parentNode, decoratedNode ) );
                    _parents[pid]->at(j) = decoratedParentEdge;
                    break;
                }
            }
            shared_ptr<Edge> decoratedEdge( new Edge( parentNode, decoratedNode ) );
            _children[id]->at(i) = decoratedEdge;
        }
    }

}

}

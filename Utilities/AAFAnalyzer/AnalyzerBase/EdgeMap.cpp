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

EdgeMap::EdgeMap()
{
}

EdgeMap::~EdgeMap()
{
}


EdgeMap::ConstEdgeVectorSP EdgeMap::GetParents(boost::shared_ptr<Node> spNode) const
{
  if(!IsInMap(spNode->GetLID(), _children))
  {
    return EdgeVectorSP(new EdgeVector);//has no parents, size() == 0
  }

  return _children[spNode->GetLID()];   
}

EdgeMap::ConstEdgeVectorSP EdgeMap::GetChildren(boost::shared_ptr<Node> spNode) const
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

void EdgeMap::DecorateEdge(boost::shared_ptr<Node> decoratedNode)
{

    LID id = decoratedNode->GetLID();
    
    //Get all edges where it is the parent node
    if ( IsInMap( id, _parents ) )
    {
        for (unsigned int i = 0; i < _parents[id]->size(); i++)
        {
            boost::shared_ptr<Node> childNode = _parents[id]->at(i)->GetChildNode();
            LID cid = childNode->GetLID();
            for (unsigned int j = 0; j < _children[cid]->size(); j++)
            {
                if ( _children[cid]->at(j)->GetParentNode()->GetLID() == id )
                {
                    boost::shared_ptr<Edge> decoratedChildEdge( _children[cid]->at(j)->CreateNewEdge( decoratedNode, childNode ) );
                    _children[cid]->at(j) = decoratedChildEdge;
                    break;
                }
            }
            boost::shared_ptr<Edge> decoratedEdge( _parents[id]->at(i)->CreateNewEdge( decoratedNode, childNode ) );
            _parents[id]->at(i) = decoratedEdge;
        }

    }
    
    //Get all edges where it is the child node
    if ( IsInMap( id, _children ) )
    {
        for (unsigned int i = 0; i < _children[id]->size(); i++)
        {
            boost::shared_ptr<Node> parentNode = _children[id]->at(i)->GetParentNode();
            LID pid = parentNode->GetLID();
            for (unsigned int j = 0; j < _parents[pid]->size(); j++)
            {
                if ( _parents[pid]->at(j)->GetChildNode()->GetLID() == id )
                {
                      boost::shared_ptr<Edge> decoratedParentEdge( _parents[pid]->at(j)->CreateNewEdge( parentNode, decoratedNode ) );
                    _parents[pid]->at(j) = decoratedParentEdge;
                    break;
                }
            }
              boost::shared_ptr<Edge> decoratedEdge( _children[id]->at(i)->CreateNewEdge( parentNode, decoratedNode ) );
            _children[id]->at(i) = decoratedEdge;
        }
    }

}

}

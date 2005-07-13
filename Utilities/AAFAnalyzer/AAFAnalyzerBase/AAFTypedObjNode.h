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

#ifndef __AAFTYPEDOBJNODE_h__
#define __AAFTYPEDOBJNODE_h__

//project files
#include "Node.h"
#include "Visitor.h"

//Ax files
#include <AxSmartPointer.h>

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

template<typename AAFObjType>
class AAFTypedObjNode : public Node
{
 public:
  AAFTypedObjNode(IAAFObjectSP ObjectType );
  AAFTypedObjNode(IAAFObjectSP ObjectType, const std::basic_string<wchar_t>& name );
  ~AAFTypedObjNode();
  
  bool PreOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  bool PostOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  boost::shared_ptr<AAFObjType> GetAAFObjectType() const;

 private:

  //prohibited
  AAFTypedObjNode();
  AAFTypedObjNode( const AAFTypedObjNode& );
  AAFTypedObjNode& operator=( const AAFTypedObjNode& );

  IAAFSmartPointer<AAFObjType> _spTypedObj;
};

template<typename AAFObjType>
AAFTypedObjNode<AAFObjType>::AAFTypedObjNode(IAAFObjectSP spObject )
  : _spTypedObj( spObject ),
    Node( L"" )
{}

template<typename AAFObjType>
AAFTypedObjNode<AAFObjType>::AAFTypedObjNode(IAAFObjectSP spObject, const std::basic_string<wchar_t>& name)
  : Node( name ),
    _spTypedObj( spObject )
{}

template<typename AAFObjType>
AAFTypedObjNode<AAFObjType>::~AAFTypedObjNode()
{
}

template<typename AAFObjType>
bool AAFTypedObjNode<AAFObjType>::PreOrderVisit(boost::shared_ptr<Visitor> spVisitor)
{
  return spVisitor->PreOrderVisit(*this);
}

template<typename AAFObjType>
bool AAFTypedObjNode<AAFObjType>::PostOrderVisit(boost::shared_ptr<Visitor> spVisitor)
{
  return spVisitor->PostOrderVisit(*this);
}

template<typename AAFObjType>
boost::shared_ptr<AAFObjType> AAFTypedObjNode<AAFObjType>::GetAAFObjectType() const
{
  return _spTypedObj;
}

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

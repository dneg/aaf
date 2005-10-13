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

#ifndef __AAFTYPEDOBJNODEDECORATOR_h__
#define __AAFTYPEDOBJNODEDECORATOR_h__

//AAF Analyzer Base files
#include "AAFTypedObjNode.h"

namespace aafanalyzer {

using namespace boost;

template<typename AAFObjType>
class AAFTypedObjNodeDecorator : public AAFTypedObjNode<AAFObjType>
{
 public:
  ~AAFTypedObjNodeDecorator();
  
  //Do not implement PreOrderVisit() or PostOrderVisit().  Forwarding these
  //requests to the contained node will cause the visitor to incorrectly
  //reslove the type of the node.
  IAAFSmartPointer<AAFObjType> GetAAFObjectOfType() const;
  void Decorate( shared_ptr<Node> decoratedNode );

 protected:
  AAFTypedObjNodeDecorator( const shared_ptr<AAFTypedObjNode<AAFObjType> > spNode );

 private:

  const shared_ptr<AAFTypedObjNode<AAFObjType> > _spNode;

  //prohibited
  AAFTypedObjNodeDecorator();
  AAFTypedObjNodeDecorator( const AAFTypedObjNodeDecorator& );
  AAFTypedObjNodeDecorator& operator=( const AAFTypedObjNodeDecorator& );

};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

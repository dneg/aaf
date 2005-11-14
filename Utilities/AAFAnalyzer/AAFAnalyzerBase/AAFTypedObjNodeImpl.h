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
#define __AAFTYPEDOBJNODEIMPL_h__

//AAF Analyzer Base files
#include "AAFTypedObjNode.h"

namespace aafanalyzer {

using namespace std;
using namespace boost;

template<typename AAFObjType>
class AAFTypedObjNodeImpl : public AAFTypedObjNode<AAFObjType>
{
 public:
  AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> ObjectType );
  AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> ObjectType,
		  const basic_string<wchar_t>& name );
  AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> ObjectType,
          shared_ptr<Node> spNode );
  ~AAFTypedObjNodeImpl();
  
  bool PreOrderVisit(shared_ptr<Visitor> spVisitor);
  bool PostOrderVisit(shared_ptr<Visitor> spVisitor);
  IAAFSmartPointer<AAFObjType> GetAAFObjectOfType() const;
  virtual void Decorate( shared_ptr<Node> decoratedNode );

 private:

  //prohibited
  AAFTypedObjNodeImpl();
  AAFTypedObjNodeImpl( const AAFTypedObjNodeImpl& );
  AAFTypedObjNodeImpl& operator=( const AAFTypedObjNodeImpl& );

  IAAFSmartPointer<AAFObjType> _spTypedObj;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

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

//Ax files
#include <AxSmartPointer.h>

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

template<typename AAFObjType>
class AAFTypedObjNode : public Node
{
 public:
  AAFTypedObjNode(IAAFSmartPointer<AAFObjType> ObjectType );
  AAFTypedObjNode(IAAFSmartPointer<AAFObjType> ObjectType,
		  const std::basic_string<wchar_t>& name );
  ~AAFTypedObjNode();
  
  bool PreOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  bool PostOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  IAAFSmartPointer<AAFObjType> GetAAFObjectType() const;

 private:

  //prohibited
  AAFTypedObjNode();
  AAFTypedObjNode( const AAFTypedObjNode& );
  AAFTypedObjNode& operator=( const AAFTypedObjNode& );

  IAAFSmartPointer<AAFObjType> _spTypedObj;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

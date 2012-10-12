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

#ifndef __AAFTYPEDOBJNODEDECORATOR_h__
#define __AAFTYPEDOBJNODEIMPL_h__

//AAF Analyzer Base files
#include "AAFTypedObjNode.h"

namespace aafanalyzer {

using namespace std;

template<typename AAFObjType>
class AAFTypedObjNodeImpl : public AAFTypedObjNode<AAFObjType>
{
 public:
  AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> ObjectType );
  AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> ObjectType,
		  const basic_string<wchar_t>& name );
  AAFTypedObjNodeImpl(IAAFSmartPointer<AAFObjType> ObjectType,
          boost::shared_ptr<Node> spNode );
  ~AAFTypedObjNodeImpl();
  
  bool PreOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  bool PostOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  IAAFSmartPointer<AAFObjType> GetAAFObjectOfType() const;
  virtual void Decorate( boost::shared_ptr<Node> decoratedNode );

 private:

  //prohibited
  AAFTypedObjNodeImpl();
  AAFTypedObjNodeImpl( const AAFTypedObjNodeImpl& );
  AAFTypedObjNodeImpl& operator=( const AAFTypedObjNodeImpl& );

  IAAFSmartPointer<AAFObjType> _spTypedObj;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

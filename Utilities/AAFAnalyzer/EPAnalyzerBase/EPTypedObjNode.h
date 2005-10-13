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

#ifndef __EPTYPEDOBJNODE_h__
#define __EPTYPEDOBJNODE_h__

//AAF Analyzer Base files
#include "AAFTypedObjNodeDecorator.h"

namespace aafanalyzer {

using namespace boost;

template<typename AAFObjType, typename EPObjType>
class EPTypedObjNode : public AAFTypedObjNodeDecorator<AAFObjType>
{
 public:
  EPTypedObjNode( const shared_ptr<AAFTypedObjNode<AAFObjType> > spNode );
  ~EPTypedObjNode();
  bool PreOrderVisit(shared_ptr<Visitor> spVisitor);
  bool PostOrderVisit(shared_ptr<Visitor> spVisitor);

 private:

  //prohibited
  EPTypedObjNode();
  EPTypedObjNode( const EPTypedObjNode& );
  EPTypedObjNode& operator=( const EPTypedObjNode& );

};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

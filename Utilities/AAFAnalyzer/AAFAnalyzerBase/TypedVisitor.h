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

#ifndef __TYPEDVISITOR_h__
#define __TYPEDVISITOR_h__

//project files
#include "Visitor.h"
#include "AAFTypedObjNode.h"

//AAF files
#include <AAF.h>

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

class AAFContainment;
class AAFComponentReference;
class AAFMobReference;
class AAFSlotReference;

class TypedVisitor : public Visitor
{
 public:
  TypedVisitor();
  ~TypedVisitor();

  bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFHeader> >);
  bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFHeader> >);
  bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFContentStorage> >);
  bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFContentStorage> >);
  bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCompositionMob> >);
  bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFCompositionMob> >);
  bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMasterMob> >);
  bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFMasterMob> >);
  bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceMob> >);
  bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceMob> >);
  bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSequence> >);
  bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSequence> >);
  bool PreOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceClip> >);
  bool PostOrderVisit(boost::shared_ptr<AAFTypedObjNode<IAAFSourceClip> >);
  /*
  .
  .
  */
  bool PreOrderVisit(boost::shared_ptr<IAAFObject>);
  bool PostOrderVisit(boost::shared_ptr<IAAFObject>);

  bool EdgeVisit(boost::shared_ptr<AAFContainment> pEdge);
  bool EdgeVisit(boost::shared_ptr<AAFComponentReference> pEdge);
  bool EdgeVisit(boost::shared_ptr<AAFMobReference> pEdge);
  bool EdgeVisit(boost::shared_ptr<AAFSlotReference> pEdge);  

 private:

  // prohibited
  TypedVisitor( const TypedVisitor& );
  TypedVisitor& operator=( const TypedVisitor& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/


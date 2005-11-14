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

#ifndef __AAFMOBREFERENCE_h__
#define __AAFMOBREFERENCE_h__

//Analyzer Base files
#include <Edge.h>

namespace aafanalyzer {

using namespace boost;

class AAFMobReference : public Edge
{
 public:
  AAFMobReference(shared_ptr<Node> spParent, shared_ptr<Node> spChild);
  ~AAFMobReference();

  bool Visit(shared_ptr<Visitor> spVisitor);
  virtual shared_ptr<Edge> CreateNewEdge( shared_ptr<Node> spParent, shared_ptr<Node> spChild ) const;

 private:

  // prohibited
  AAFMobReference();
  AAFMobReference( const AAFMobReference& );
  AAFMobReference& operator=( const AAFMobReference& );
};

} // end of namespace diskstream

#endif

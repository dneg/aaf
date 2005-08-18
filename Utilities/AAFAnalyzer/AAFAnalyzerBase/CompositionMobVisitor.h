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

#ifndef __COMPOSITIONMOBVISITOR_h__
#define __COMPOSITIONMOBVISITOR_h__

#include "TypedVisitor.h"

#include <TestResult.h>

//stl files
#include <iostream>
#include <vector>
#include <map>

namespace aafanalyzer {

class Edge;
class EdgeMap;

class CompositionMobVisitor : public TypedVisitor
{
 public:
  typedef std::map<unsigned int, int> Map;
  typedef std::vector<unsigned int> Vector;

  CompositionMobVisitor(std::ostream& os, TestResult& result);
  virtual ~CompositionMobVisitor();

  bool PreOrderVisit(AAFTypedObjNode<IAAFCompositionMob>& node);
  Vector GetCompMobsWithCount(int count);

 private:
  bool IsPresent(unsigned int lid);

  std::ostream& _os;
  TestResult& _Result;
  Map _Map;//<LID, ref count>

  // prohibited
  CompositionMobVisitor( const CompositionMobVisitor& );
  CompositionMobVisitor& operator=( const CompositionMobVisitor& );
};

} // end of namespace diskstream

#endif

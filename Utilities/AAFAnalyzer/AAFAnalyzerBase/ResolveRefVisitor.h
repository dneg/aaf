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

#ifndef __RESOLVEREFVISITOR_h__
#define __RESOLVEREFVISITOR_h__

#include "TypedVisitor.h"

#include <TestResult.h>

//stl files
#include <iostream>

namespace aafanalyzer {

class Edge;
class EdgeMap;

class ResolveRefVisitor : public TypedVisitor
{
 public:
  ResolveRefVisitor(std::ostream& os, boost::shared_ptr<EdgeMap> spEdgeMap);
  virtual ~ResolveRefVisitor();

  virtual bool PostOrderVisit(AAFTypedObjNode<IAAFSourceClip>& node);
  virtual bool EdgeVisit(Edge& edge);

  boost::shared_ptr<const TestResult> GetTestResult() const;

 private:
  std::ostream& _os;
  boost::shared_ptr<EdgeMap> _spEdgeMap;
  boost::shared_ptr< TestResult > _spResult;

  // prohibited
  ResolveRefVisitor( const ResolveRefVisitor& );
  ResolveRefVisitor& operator=( const ResolveRefVisitor& );
};

} // end of namespace diskstream

#endif

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

//AAF Analyzer Base files
#include <TypedVisitor.h>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class Edge;
class EdgeMap;
class DetailLevelTestResult;

class ResolveRefVisitor : public TypedVisitor
{
 public:
  ResolveRefVisitor(wostream& os, shared_ptr<EdgeMap> spEdgeMap);
  virtual ~ResolveRefVisitor();

  virtual bool PostOrderVisit(AAFTypedObjNode<IAAFSourceClip>& node);

  shared_ptr<const DetailLevelTestResult> GetTestResult() const;

 private:
  wostream& _os;
  shared_ptr<EdgeMap> _spEdgeMap;
  shared_ptr< DetailLevelTestResult > _spResult;

  // prohibited
  ResolveRefVisitor( const ResolveRefVisitor& );
  ResolveRefVisitor& operator=( const ResolveRefVisitor& );
};

} // end of namespace diskstream

#endif

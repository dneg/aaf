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

#ifndef __ACYCLICVISITOR_h__
#define __ACYCLICVISITOR_h__

//Analyzer Base files
#include <Visitor.h>
#include <Node.h>

//STL files
#include <vector>
#include <set>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class DetailLevelTestResult;
class TestLevelTestResult;

class AcyclicVisitor : public Visitor
{
 public:

  AcyclicVisitor(wostream& os,
		 shared_ptr<TestLevelTestResult> spTestResult);

  virtual ~AcyclicVisitor();

  virtual bool PreOrderVisit(Node& node);
  virtual bool PostOrderVisit(Node& node);

  bool IsCycleDetected() const;

 private:
 
  // prohibited
  AcyclicVisitor();
  AcyclicVisitor( const AcyclicVisitor& );
  AcyclicVisitor& operator=( const AcyclicVisitor& );

  void CreateCycleTestResult();

  //typedef vector<Node::LID> Vector;
  typedef vector<shared_ptr<Node> > NodeVector;
  typedef set<Node::LID> NodeSet;
 
  NodeVector _nodeVector;
  NodeSet _nodeSet;

  wostream& _os;
  shared_ptr<TestLevelTestResult> _spTestResult;

  int _level;
  bool _cycleDetected;
};

} // end of namespace diskstream

#endif

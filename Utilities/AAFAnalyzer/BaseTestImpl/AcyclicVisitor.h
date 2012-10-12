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

class DetailLevelTestResult;
class TestLevelTestResult;

class AcyclicVisitor : public Visitor
{
 public:

  AcyclicVisitor(wostream& os,
		 boost::shared_ptr<TestLevelTestResult> spTestResult);

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
  typedef vector<boost::shared_ptr<Node> > NodeVector;
  typedef set<Node::LID> NodeSet;
 
  NodeVector _nodeVector;
  NodeSet _nodeSet;

  wostream& _os;
  boost::shared_ptr<TestLevelTestResult> _spTestResult;

  int _level;
  bool _cycleDetected;
};

} // end of namespace diskstream

#endif

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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

#ifndef __CompMobDependency_h__
#define __CompMobDependency_h__

//Test/Result files
#include <Test.h>

//AAF Analyzer Base files
#include <AAFTypedObjNode.h>

//STL files
#include <vector>

namespace aafanalyzer {

class TestLevelTestResult;

using namespace boost;
using namespace std;

class CompMobDependency : public Test
{
 public:

  typedef AAFTypedObjNode<IAAFCompositionMob> CompMobNode;
  typedef shared_ptr<CompMobNode> CompMobNodeSP;
  typedef vector<CompMobNodeSP> CompMobNodeVector;
  typedef shared_ptr<CompMobNodeVector> CompMobNodeVectorSP;

  CompMobDependency(wostream& os, shared_ptr<const TestGraph> spGraph);
  virtual ~CompMobDependency();

  virtual shared_ptr<TestLevelTestResult> Execute();
  virtual AxString GetName() const;
  virtual AxString GetDescription() const;
  static const TestInfo GetTestInfo();

  CompMobNodeVectorSP GetRootCompMobNodes();

  CompMobNodeVectorSP GetNonRootCompMobNodes();

 private:

  // prohibited
  CompMobDependency();
  CompMobDependency(const CompMobDependency&);
  CompMobDependency& operator=( const CompMobDependency& );

  CompMobNodeVectorSP _spRootCompMobs;
  CompMobNodeVectorSP _spNonRootCompMobs;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

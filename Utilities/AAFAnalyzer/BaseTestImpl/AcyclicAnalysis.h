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

#ifndef __ACYCLICANALYSIS_h__
#define __ACYCLICANALYSIS_h__

//Test/Result files
#include <Test.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

using namespace std;
using namespace boost;
    
class TestLevelTestResult;
class TestGraph;

class AcyclicAnalysis : public Test
{
 public:
  AcyclicAnalysis(wostream& os, shared_ptr<const TestGraph> spGraph);
  ~AcyclicAnalysis();

  shared_ptr<TestLevelTestResult> Execute();
  AxString GetName() const;
  AxString GetDescription() const;
  static const TestInfo GetTestInfo();

  bool IsCyclic() const;

 private:

  // prohibited
  AcyclicAnalysis();
  AcyclicAnalysis(const AcyclicAnalysis&);
  AcyclicAnalysis& operator=( const AcyclicAnalysis& );

  bool _isCyclic;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

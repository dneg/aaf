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

#ifndef __DUMPPHASE_h__
#define __DUMPPHASE_h__

//Base Test files (included so the application only needs to include the Test
//Phase and not individual tests in order to register tests).
#include "FileDumper.h"

//Test/Result files
#include <TestPhase.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {
    
using namespace std;
using namespace boost;

class TestGraph;

class DumpPhase : public TestPhase
{
 public:
  DumpPhase(wostream& os, shared_ptr<const TestGraph> spGraph);
  ~DumpPhase();

  shared_ptr<const TestGraph> GetTestGraph();
  shared_ptr<TestPhaseLevelTestResult> Execute();

 private:
  shared_ptr<const TestGraph> _spGraph;

  // prohibited
  DumpPhase();
  DumpPhase( const DumpPhase& );
  DumpPhase& operator=( const DumpPhase& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

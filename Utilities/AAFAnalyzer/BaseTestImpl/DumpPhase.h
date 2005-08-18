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

//test files
#include <TestPhase.h>

namespace aafanalyzer {

class TestGraph;

class DumpPhase : public TestPhase
{
 public:
  DumpPhase(std::ostream& os, boost::shared_ptr<TestGraph> spTestGraph);
  ~DumpPhase();

  boost::shared_ptr<TestGraph> GetTestGraph();
  std::vector<TestResult> Execute();  

 private:
  boost::shared_ptr<TestGraph> _spTestGraph;

  // prohibited
  DumpPhase();
  DumpPhase( const DumpPhase& );
  DumpPhase& operator=( const DumpPhase& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

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

#ifndef __LOADPHASE_h__
#define __LOADPHASE_h__

//test files
#include "TestPhase.h"
#include "FileLoad.h"
#include "RefResolver.h"
#include "AcyclicAnalysis.h"

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

class TestGraph;

class LoadPhase : public TestPhase
{
 public:
  LoadPhase(std::ostream& os, const std::basic_string<wchar_t> AAFFile);
  ~LoadPhase();

  boost::shared_ptr<TestGraph> GetTestGraph();
  std::vector<TestResult> Execute();  

 private:
  boost::shared_ptr<TestGraph> _spTestGraph;
  const std::basic_string<wchar_t> _FileName;

  // prohibited
  LoadPhase();
  LoadPhase( const LoadPhase& );
  LoadPhase& operator=( const LoadPhase& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

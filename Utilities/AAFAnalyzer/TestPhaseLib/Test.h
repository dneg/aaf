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

#ifndef __TEST_h__
#define __TEST_h__

//Ax files
#include <AxTypes.h>

//project files 
#include <TestGraph.h>
#include <Requirement.h>
#include "TestInfo.h"

//stl files
#include <iostream>
#include <string>

//boost files
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace aafanalyzer {

class TestLevelTestResult;

class Test : public boost::enable_shared_from_this<Test>
{
 public:
  Test(std::wostream& os, const TestInfo& info);
  virtual ~Test();

  virtual boost::shared_ptr<TestLevelTestResult> Execute() = 0;
  virtual AxString GetName() const;
  virtual AxString GetDescription() const;
  std::wostream& GetOutStream() const;
  boost::shared_ptr<TestGraph> GetTestGraph();

  const Requirement::RequirementMap& GetCoveredRequirements() const;

protected:
  void SetTestGraph(boost::shared_ptr<TestGraph> spTestGraph);

 private:
  std::wostream& _os;
  boost::shared_ptr<TestGraph> _spTestGraph;
  
  typedef boost::shared_ptr<const Requirement::RequirementMap> ConstRequirementMapSP;
  const ConstRequirementMapSP _spCoveredRequirements;

  const ConstRequirementMapSP InitializeRequirements(const AxString& name);

  // prohibited
  Test(const Test&);
  Test& operator=( const Test& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

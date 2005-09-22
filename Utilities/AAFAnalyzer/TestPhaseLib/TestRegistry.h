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

#ifndef __TESTREGISTRY_H_
#define __TESTREGISTRY_H_

//Ax Files
#include <AxTypes.h>

//Boost Files
#include <boost/shared_ptr.hpp>

//Project Files
#include "Test.h"
#include "TopLevelTestResult.h"
#include "TestInfo.h"
#include <Requirement.h>

//STL files
#include <map>
#include <vector>

namespace aafanalyzer {

class TestRegistry
{
 public:

  // dtor must be public so that shared_ptr class can access it
  ~TestRegistry();

  static TestRegistry& GetInstance();
  void Register( const TestInfo& info );
  const boost::shared_ptr<const Requirement::RequirementMap> GetRequirementsForTest( const AxString& name ) const;
  const Requirement::RequirementMap& GetRequirementCoverage() const;
  bool VerifyTestResultCoverage(const boost::shared_ptr<TopLevelTestResult> results) const;
  void UseUnsafeRequirements();
  bool IsUnsafeRequirements();

 private:

  typedef std::map< const AxString, boost::shared_ptr<const Requirement::RequirementMap> > Map;

  TestRegistry();//using Singleton pattern to allow only one object
  void VerifyTestResultCoverage(const boost::shared_ptr<const TestResult> result, Requirement::RequirementMap& outstandingReqs) const;

  static TestRegistry* _pTestRegistry;
  Map _testSet;
  Requirement::RequirementMap _coveredRequirements;
  bool _useUnsafeRequirements;

  // prohibited

};

} // end of namespace diskstream

#endif /*__TESTREGISTRY_H_*/

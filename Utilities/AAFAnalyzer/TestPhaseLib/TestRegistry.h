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

//Requirement files
#include <Requirement.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <string>
#include <map>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class TestResult;
class TopLevelTestResult;
class TestInfo;

class TestRegistry
{
 public:

  // dtor must be public so that shared_ptr class can access it
  ~TestRegistry();

  static TestRegistry& GetInstance();
  void Register( const TestInfo& info );
  const shared_ptr<Requirement::RequirementMap> GetRequirementsForTest( const wstring& name ) const;
  const shared_ptr<const Requirement::RequirementMap> GetConstRequirementsForTest( const wstring& name ) const;
  const Requirement::RequirementMap& GetRequirementCoverage() const;
  bool VerifyTestResultCoverage(const shared_ptr<TopLevelTestResult> results) const;

 private:

  typedef map< const wstring, shared_ptr<const Requirement::RequirementMap> > Map;

  TestRegistry();//using Singleton pattern to allow only one object
  void VerifyTestResultCoverage(const shared_ptr<const TestResult> result, Requirement::RequirementMap& outstandingReqs) const;

  static TestRegistry* _pTestRegistry;
  Map _testSet;
  Requirement::RequirementMap _coveredRequirements;

  // prohibited
  TestRegistry( const TestRegistry& );
  TestRegistry& operator=( const TestRegistry& );

};

} // end of namespace diskstream

#endif /*__TESTREGISTRY_H_*/

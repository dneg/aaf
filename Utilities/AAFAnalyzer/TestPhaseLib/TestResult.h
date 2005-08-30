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

#ifndef __TESTRESULT_h__
#define __TESTRESULT_h__

//Ax files
#include <AxTypes.h>

//stl files
#include <string>
#include <vector>

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

using namespace std;

class TestResult
{
 public:

  enum Result {PASS, WARN, FAIL};

  TestResult();
  TestResult( const AxString& name,
	      const AxString& desc,
	      const AxString& explain,
	      const AxString& docref,
	      Result defaultResult );
  TestResult& operator=(const TestResult& test);
  ~TestResult();

  const AxString& GetExplanation() const;
  const AxString& GetDocumentRef() const;
  const AxString& GetName() const;
  const AxString& GetDescription() const;
  enum Result GetResult() const;

  void SetName(const AxString& name);
  void SetDescription(const AxString& desc);
  void SetExplanation(const AxString& exp);
  void SetResult(Result result);
  
  typedef std::vector< boost::shared_ptr<const TestResult> > SubtestResultVector;
  typedef boost::shared_ptr<SubtestResultVector> SubtestResultsSP;

  SubtestResultsSP GetSubtestResults() const;
  void AppendSubtestResult( boost::shared_ptr<const TestResult> subtestResult );
  enum Result GetAggregateResult() const;
  bool ContainsSubtests() const;

 private:
  AxString _name;
  AxString _desc;
  AxString _expl;
  AxString _docRef;
  enum Result _result;
  
  SubtestResultsSP _spSubtestResults; 
  enum Result _aggregateEnumResult;

  // prohibited
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

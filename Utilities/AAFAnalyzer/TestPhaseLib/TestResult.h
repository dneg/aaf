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

//stl files
#include <string>

namespace aafanalyzer {

using namespace std;

class TestResult
{
 public:

  enum Result {PASS, WARN, FAIL};

  TestResult();
  TestResult( const string& name,
	      const string& desc,
	      const string& explain,
	      const string& docref,
	      Result defaultResult );
  TestResult& operator=(const TestResult& test);
  ~TestResult();

  const string& GetExplanation() const;
  const string& GetDocumentRef() const;
  const string& GetName() const;
  const string& GetDescription() const;
  enum Result GetResult() const;

  void SetName(const string& name);
  void SetDescription(const string& desc);
  void SetExplanation(const string& exp);
  void SetResult(Result result);

 private:
  string _name;
  string _desc;
  string _expl;
  string _docRef;
  enum Result _result;

  // prohibited
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

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

class TestResult
{
 public:
  TestResult();
  TestResult(const TestResult& test);
  TestResult& operator=(const TestResult& test);
  ~TestResult();

  std::string GetExplanation();
  std::string GetDocumentRef();
  std::string GetName();
  std::string GetDescription();

 private:

  // prohibited
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

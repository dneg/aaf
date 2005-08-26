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

#include "TestResult.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

// Default result is FAIL.
// Test implementations must explicity indicate success.
TestResult::TestResult()
  : _result( FAIL )
{}

TestResult::TestResult( const string& name,
			const string& desc,
			const string& explain,
			const string& docRef,
			Result defaultResult )
  : _name( name ),
    _desc( desc ),
    _expl( explain ),
    _docRef( docRef ),
    _result( defaultResult )
{}

TestResult::~TestResult()
{}

TestResult& TestResult::operator=(const TestResult& test)
{
  if(this != &test)
  {
    _expl = test._expl;
    _name = test._name;
    _desc = test._desc;
    _result = test._result;
  }

  return *this;
}

const string& TestResult::GetExplanation()
{
  return _expl;
}

const string& TestResult::GetDocumentRef()
{
  return _docRef;
}

const string& TestResult::GetName()
{
  return _name;
}

const string& TestResult::GetDescription()
{
  return _desc;
}

void TestResult::SetExplanation(const string& exp)
{
  _expl = exp;
}

void TestResult::SetName(const string& name)
{
  _name = name;
}

void TestResult::SetDescription(const string& desc)
{
  _desc = desc;
}

void TestResult::SetResult(Result result)
{
  //can only be set to success, warning, or failure
  _result = result;
}

enum TestResult::Result TestResult::GetResult()
{
  return _result;
}

} // end of namespace diskstream

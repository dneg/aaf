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

TestResult::TestResult()
{
  //TestResult is successful unless found otherwise by a visitor
  _enum_result = success;
}

TestResult::~TestResult()
{
}

TestResult& TestResult::operator=(const TestResult& test)
{
  if(this != &test)
  {
    _Expl = test._Expl;
    _Name = test._Name;
    _Desc = test._Desc;
    _enum_result = test._enum_result;
  }

  return *this;
}

std::string TestResult::GetExplanation()
{
  return _Expl;
}

std::string TestResult::GetDocumentRef()
{
  std::string null = "";
  return null;
}

std::string TestResult::GetName()
{
  return _Name;
}

std::string TestResult::GetDescription()
{
  return _Desc;
}

void TestResult::SetExplanation(std::string exp)
{
  _Expl = exp;
}

void TestResult::SetName(std::string name)
{
  _Name = name;
}

void TestResult::SetDescription(std::string desc)
{
  _Desc = desc;
}

void TestResult::SetResult(Result result)
{
  //can only be set to success, warning, or failure
  _enum_result = result;
}

enum TestResult::Result TestResult::GetResult()
{
  return _enum_result;
}

} // end of namespace diskstream

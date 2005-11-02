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

//Test/Result files
#include "Test.h"
#include "TestRegistry.h"

//Analyzer Base files
#include <TestGraph.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;
using namespace boost;

Test::Test(wostream& os, const TestInfo& info)
: _os(os), _spCoveredRequirements(InitializeRequirements(info.GetName()))
{
}

const Test::ConstRequirementMapSP Test::InitializeRequirements(const wstring& name)
{
    TestRegistry& reg = TestRegistry::GetInstance();
    return reg.GetConstRequirementsForTest(name);
}

Test::~Test()
{
}

wstring Test::GetName() const
{
  wstring null;
  return null;
}

wstring Test::GetDescription() const
{
  wstring null;
  return null;
}

wostream& Test::GetOutStream() const
{
  return _os;
}

shared_ptr<const TestGraph> Test::GetTestGraph()
{
  return _spGraph;
}

void Test::SetTestGraph(shared_ptr<const TestGraph> spGraph)
{
  _spGraph = spGraph;
}

const Requirement::RequirementMap& Test::GetCoveredRequirements() const
{
    return *_spCoveredRequirements;
}

} // end of namespace diskstream

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

//Base Test files
#include <LoadPhase.h>

#include <CompMobDependency.h>

//Test/Result files
#include <TestGraph.h>
#include <TestPhaseLevelTestResult.h>
#include <TestLevelTestResult.h>

//AAF Analyzer Base files
#include <AAFGraphInfo.h>

#include <sstream>

namespace {

using namespace aafanalyzer;

const wchar_t* PHASE_NAME = L"Load Phase";
const wchar_t* PHASE_DESC = L"Load an AAF file, resolve references, "
                            L"and ensure the graph is acyclic.";

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 	
{
    
using namespace std;

LoadPhase::LoadPhase(wostream& os, const basic_string<wchar_t> AAFFile) 
  : TestPhase(os),
    _FileName(AAFFile),
    _spRootsVector( new CompMobDependency::CompMobNodeVector ),
    _isCyclic(true)
{}

LoadPhase::~LoadPhase()
{}

boost::shared_ptr<const AAFGraphInfo> LoadPhase::GetTestGraphInfo()
{
  return _spGraphInfo;
}

boost::shared_ptr<TestPhaseLevelTestResult> LoadPhase::Execute() 
{
  boost::shared_ptr<TestPhaseLevelTestResult> spLoadTest(
                            new TestPhaseLevelTestResult( PHASE_NAME,
                                                          PHASE_DESC,
                                                          L"" ));   // explain

  // Load the AAF file and create the graph.
  boost::shared_ptr<FileLoad> load(new FileLoad(GetOutStream(), _FileName));
  boost::shared_ptr<TestLevelTestResult> spTestResult( load->Execute() );
  spLoadTest->AppendSubtestResult(spTestResult);

  // Get the TestGraph object we need for other tests.
  _spGraphInfo = load->GetTestGraphInfo();

  // Resolve all the references in the AAF graph.
  boost::shared_ptr<RefResolver> ref(new RefResolver(GetOutStream(), _spGraphInfo->GetGraph()));
  spTestResult = ref->Execute();
  spLoadTest->AppendSubtestResult(spTestResult);

  // Ensure the AAF file graph is acyclic.
  //

  // If it fails then there is a cycle and further processing cannot
  // continue (because subsequent visitor implementations are not
  // cycle aware. They will end up in an infinite loop.)
  boost::shared_ptr<AcyclicAnalysis> acy(new AcyclicAnalysis(GetOutStream(), _spGraphInfo->GetGraph()));
  spTestResult = acy->Execute();
  spLoadTest->AppendSubtestResult(spTestResult);

  _isCyclic = acy->IsCyclic();

  if ( _isCyclic )
  {
    return spLoadTest;
  }

  // Perform dependency analysis to identify unreferenced (root)
  // composition mobs.
  boost::shared_ptr<CompMobDependency> depTest( new CompMobDependency(GetOutStream(), _spGraphInfo->GetGraph() ) );
  spLoadTest->AppendSubtestResult( depTest->Execute() );

  _spRootsVector = depTest->GetRootCompMobNodes();

  return spLoadTest;
}

vector<boost::shared_ptr<Node> > LoadPhase::GetRoots() const
{
  vector<boost::shared_ptr<Node> > roots;

  for( size_t i = 0; i < _spRootsVector->size(); ++i )
  {
    roots.push_back( (*_spRootsVector)[i] );
  }

  return roots;
}

CompMobDependency::CompMobNodeVectorSP LoadPhase::GetCompMobRoots()
{
  return _spRootsVector;
}

bool LoadPhase::IsCyclic() const
{
  return _isCyclic;
}

} // end of namespace diskstream

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

#include "FileLoad.h"

#include <GraphBuilder.h>
#include <NodeFactoryImpl.h>

//boost files
#include <boost/shared_ptr.hpp>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

FileLoad::FileLoad(std::ostream& os, const std::basic_string<wchar_t> AAFFile)
: Test(os),
  _File(AAFFile)
{
}

FileLoad::~FileLoad()
{
}

TestResult FileLoad::Execute()
{
  GraphBuilder graphBuild;
  boost::shared_ptr<NodeFactory> spFactory(new NodeFactoryImpl());

  //output to screen
  //GetOutStream() << GetName() << std::endl << GetDescription() << std::endl << std::endl;
    
  //build the graph and initialize TestGraph 
  boost::shared_ptr<TestGraph> spTestGraph(new TestGraph(graphBuild.CreateGraph(_File, spFactory)));
  SetTestGraph(spTestGraph);
  
  // Set result properties.
  // If the graph builder did not through an exception, then
  // it succeeded, therefore, result is PASS.
  TestResult result( "FileLoad",
		     "Loads and AAF file and builds the test graph.",
		     "File loaded correctly.",
		     "-", // DOCREF
		     TestResult::PASS );
  return result;
}

std::string FileLoad::GetName()
{
  std::string name = "--- File Load Test ---";
  return name;
}

std::string FileLoad::GetDescription()
{
  std::string description = "Test Description: Load an AAF file and build a graph of contained AAF objects.";
  return description;
}


} // end of namespace diskstream

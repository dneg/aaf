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

//project files
#include <LoadPhase.h>
#include <DumpPhase.h>
#include <EPMobDepPhase.h>

//Ax files
#include <AxInit.h>
#include <AxUtil.h>
#include <AxEx.h>

//STL files
#include <string>

namespace {

using namespace aafanalyzer;

std::basic_string<wchar_t> LevelToIndent(unsigned int l)
{
  std::basic_string<wchar_t> indent;

  for (; l > 0 ; l-- ) 
  {
    indent += L"    ";
  }
  return indent;
}

void OutputResultMsgs(boost::shared_ptr<const TestResult> res, unsigned int level)
{
  std::wcout << LevelToIndent(level) << res->GetName() << std::endl;
  std::wcout << LevelToIndent(level) << res->GetDescription() << std::endl;
  
  if(res->GetResult() == TestResult::PASS) {
    std::wcout << LevelToIndent(level) << L"Test Passed" << std::endl << std::endl;
  }

  else if(res->GetResult() == TestResult::WARN)
  {
    std::wcout << LevelToIndent(level) << L"Test Passed, but with warnings." << std::endl;
    std::wcout << LevelToIndent(level) << res->GetExplanation() << std::endl << std::endl;
  }

  else if(res->GetResult() == TestResult::FAIL)
  {
    std::wcout << LevelToIndent(level) << L"Test Failed" << std::endl;
    std::wcout << LevelToIndent(level) << res->GetExplanation() << std::endl << std::endl;
  }
  else
  {
    assert(0);
  }
  
  if ( res->ContainsSubtests() )
  {
    TestResult::SubtestResultsSP spSubResults = res->GetSubtestResults();
    for (unsigned int i = 0; i < spSubResults->size(); i++) {
        OutputResultMsgs(spSubResults->at(i), level + 1);
    }
  }
 
}

//======================================================================

class Usage
{};

std::ostream& operator<<( std::ostream& os, const Usage& )
{
  os << "AAFAnalyzer: [options] filename.aaf" << std::endl;
  return os;
};

//======================================================================

} // end of namespace


int main( int argc, char** argv )
{
  using namespace boost;
  using namespace std;
  using namespace aafanalyzer;

  try
  {
    //
    // Process the command line arguments.
    //

    AxCmdLineArgs args( argc, argv );

    // Dump option
    pair<bool,int> dumpArg = args.get( "-dump" );

    // Filename is the last argument.
    pair<bool,const char*> fileNameArg = args.get( argc-1, 1 );

    if ( !fileNameArg.first )
    {
      throw Usage();
    }

    //
    // Execute the tests.
    //

    AxInit axInitObj;
    
    const basic_string<wchar_t> fileName = AxStringUtil::mbtowc( fileNameArg.second );

    //Create the root result object.
    boost::shared_ptr<TestResult> spSubResult;
    boost::shared_ptr<TestResult> spResult( new TestResult() );
    spResult->SetName( L"AAF Analyzer" );
    spResult->SetDescription( L"Run a suite of tests on an AAF file." );
    
    // First phase: load the aaf file, and build the object graph.
    LoadPhase load( wcout, fileName );
    shared_ptr<TestGraph> spGraph = load.GetTestGraph();
    spSubResult = load.Execute();
    spResult->AppendSubtestResult(spSubResult);
    
    // Second phase: execute the edit protocol mob dependency tests.
    EPMobDepPhase mobDepPhase( wcout, load.GetTestGraph() );
    spResult->AppendSubtestResult( mobDepPhase.Execute() );

    // More test phases go here...
    
    // optional dump phase
    if ( dumpArg.first )
    {
      DumpPhase dump(wcout, load.GetTestGraph());
      spSubResult = dump.Execute();
      spResult->AppendSubtestResult(spSubResult);
    }

    spResult->SetResult(spResult->GetAggregateResult());      
    OutputResultMsgs(spResult, 0);
  }
  catch ( const Usage& ex )
  {
    cout << ex << endl;
  }
  catch ( const AxExHResult& ex )
  {
    wcout << L"Error: " << ex.widewhat() << endl;
  }
  catch ( const exception& ex )
  {
    cout << "Error: " << ex.what() << endl;
  }
  catch ( ... )
  {
    cout << "Error: unhandled exeption" << endl;
  }
  
  return 0;
}

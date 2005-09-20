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
#include <TopLevelTestResult.h>
#include <TestPhaseLevelTestResult.h>
#include <EPMobDepPhase.h>
#include <Requirement.h>
#include <RequirementLoader.h>
#include <RequirementMismatchException.h>
#include <RequirementRegistryException.h>
#include <RequirementXMLException.h>
#include <TestRegistryException.h>
#include <TestInfoRegistrar.h>
#include <TestRegistry.h>
#include <RequirementRegistry.h>

//Ax files
#include <AxInit.h>
#include <AxUtil.h>
#include <AxEx.h>

//STL files
#include <string>
#include <algorithm>

//Boost files
#include <boost/shared_ptr.hpp>

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

class OutputDetail
{
public:
  OutputDetail( unsigned int level )
    :  _level( level )
  {}
  void operator() ( const AxString& detail )
  {
      std::wcout << LevelToIndent( _level ) << L"Detail:\t" << detail << endl;
  }
private:
  unsigned int _level;
};

void ListRequirements( const AxString& title, const Requirement::RequirementMap& reqs, const int level)
{
  std::wcout << LevelToIndent( level ) << title << ": ";
  if (reqs.size() == 0)
  {
    std::wcout << "None" << std::endl;
  }
  else
  {
    Requirement::RequirementMap::const_iterator iter;
    iter = reqs.begin();
    while ( true ) 
    {
        std::wcout << iter->first;
        iter++;
        if (iter != reqs.end() )
        {
          std::wcout << "; ";
        }
        else
        {
          break;
        }
    }
    std::wcout << std::endl;
  }
}

void OutputResultMsgs(boost::shared_ptr<const TestResult> res, unsigned int level)
{
  std::wcout << LevelToIndent(level) << "Name:\t" << res->GetName() << std::endl;
  std::wcout << LevelToIndent(level) << "Desc:\t" << res->GetDescription() << std::endl;
  
  const vector<AxString>& details = res->GetDetails();
  if ( !details.empty() )
  {
    for_each( details.begin(), details.end(), OutputDetail( level ) );
  }

  std::wcout << LevelToIndent(level) << "Result:\t";
  if(res->GetResult() == TestResult::PASS) {
    std::wcout << L"Passed" << std::endl;
  }

  else if(res->GetResult() == TestResult::WARN)
  {
    std::wcout << L"Passed, but with warnings." << std::endl;
    std::wcout << LevelToIndent(level) << "Reason:\t" << res->GetExplanation() << std::endl;
  }

  else if(res->GetResult() == TestResult::FAIL)
  {
    std::wcout << L"Failed" << std::endl;
    std::wcout << LevelToIndent(level) << "Reason:\t" << res->GetExplanation() << std::endl;
  }
  else
  {
    assert(0);
  }

  std::wcout << std::endl;
  
  ListRequirements( L"Passing Requirements", res->GetRequirements(TestResult::PASS), level);
  ListRequirements( L"Warning Requirements", res->GetRequirements(TestResult::WARN), level);
  ListRequirements( L"Failing Requirements", res->GetRequirements(TestResult::FAIL), level);

  std::wcout << std::endl;
  
  if ( res->ContainsSubtests() )
  {
    TestResult::SubtestResultVector subResults = res->GetSubtestResults();
    for (unsigned int i = 0; i < subResults.size(); i++) {
        OutputResultMsgs(subResults.at(i), level + 1);
    }
  }
 
}

void RegisterTests()
{
    //Register Load Phase tests.
    TestInfoRegistrar<FileLoad> fileLoad;
    TestInfoRegistrar<RefResolver> refResolver;
    TestInfoRegistrar<AcyclicAnalysis> acyclicAnalysis;

    //Register Edit Protocol Mob Dependency Phase tests.
    TestInfoRegistrar<CompMobDependency> compMobDependency;
    TestInfoRegistrar<EPDerivationTest> epDerivationTest;

    //Register Dump Phase tests.
    TestInfoRegistrar<FileDumper> fileDumper;   
}

void PrintRequirement( std::pair<const AxString, boost::shared_ptr<const Requirement> > entry )
{
    std::wcout << entry.first << L": " << entry.second->GetName() << std::endl;
}

void OutputRequirements( const AxString& title, const Requirement::RequirementMap& reqs)
{
  std::wcout << std::endl << title << std::endl;
  if ( !reqs.empty() )
  {
    for_each( reqs.begin(), reqs.end(), PrintRequirement );
  }
  else 
  {
     std::wcout << L"None" << std::endl << std::endl;
  }
}

//======================================================================

class Usage
{};

std::ostream& operator<<( std::ostream& os, const Usage& )
{
  os << "Valid usages are: " << std::endl;
  os << "AAFAnalyzer [options] filename.aaf requirement_filename.xml" << std::endl;
  os << "AAFAnalyzer [requirement output] requirement_filename.xml" << std::endl;
  os << "[options] = -dump | -uncheckedrequirements | [requirement output]" << std::endl;
  os << "[requirement output] = -allreqs | -coverage" << std::endl;
  os << std::endl;
  os << "-dump: Output graph of AAF file" << std::endl;
  os << "-uncheckedrequirements: Allow tests to use unloaded requirements and test" << std::endl;
  os << "                        results to use unassociated requirements" << std::endl;
  os << "-allreqs: Output all loaded requirements" << std::endl;
  os << "-coverage: Output all requirements covered by the test suite." << std::endl;
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
    
    // List All Requirements option
    pair<bool, int> allReqsArg = args.get( "-allreqs" );
    
    // List All Covered Requirements option
    pair<bool, int> coverageArg = args.get( "-coverage" );
    
    // Allow test to register with unregistered requirements
    pair<bool, int> unsafeRegistryArg = args.get ( "-uncheckedrequirements" );

    // Filename is the second last argument.
    pair<bool,const char*> fileNameArg = args.get( argc-2, 1 );
    
    // Requirements Filename is last argument.
    pair<bool, const char*> requirementsFile = args.get( argc-1, 2 );
    
    int numFlags = dumpArg.first + allReqsArg.first + coverageArg.first + unsafeRegistryArg.first;
    
    bool runTests;
    
    if ( dumpArg.first || unsafeRegistryArg.first || numFlags == 0 )
    {
      //Test should be run if the user specifies to dump results or to use
      //unchecked requirements.
      runTests = true;
    }
    else if ( numFlags == argc - 3 )
    {
      //allreques, coverage or both were specified, but, two files were
      //provided so run tests
      runTests = true;
    }
    else
    {
      runTests = false;
    }
    
    if ( runTests && numFlags != argc - 3 )
    {
      //If tests are being run, there should be 3 more arguments than flags:
      //AAFAnalyzer, the AAF file and the XML file.
      throw Usage();
    }
    else if ( !runTests && numFlags != argc - 2 )
    {
      //If tests are not being run, there should be 2 more arguments than flags:
      //AAFAnalyzer and the XML file.
      throw Usage();
    }

    //
    //Load all of the requirements in the XML file
    //
    RequirementLoader loader;
    loader.ParseXML( requirementsFile.second );
    
    if ( allReqsArg.first )
    {
      OutputRequirements(L"All Loaded Requirements", RequirementRegistry::GetInstance().GetAllRequirements());
      if ( !runTests && !coverageArg.first )
      {
        return 0;
      }
    }

    //
    // Register the tests.
    //

    //Allow unregistered or non-existant requirements to be used when necessary.
    if ( unsafeRegistryArg.first )
    {
        TestRegistry::GetInstance().UseUnsafeRequirements();
    }
    RegisterTests();
    
    if ( coverageArg.first )
    {
      OutputRequirements(L"Requirements Covered By Tests", TestRegistry::GetInstance().GetRequirementCoverage());
      if ( !runTests )
      {
        return 0;
      }
    }

    //
    // Execute the tests.
    //

    AxInit axInitObj;
    
    const basic_string<wchar_t> fileName = AxStringUtil::mbtowc( fileNameArg.second );

    //Create the result object.
    boost::shared_ptr<TopLevelTestResult> spResult(new TopLevelTestResult());
    spResult->SetName( L"AAF Analyzer" );
    spResult->SetDescription( L"AAF Edit Protocol compliance test." );

    //first phase
    LoadPhase load( wcout, fileName );
    boost::shared_ptr<const TestPhaseLevelTestResult> spSubResult( load.Execute() );
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
  catch ( const RequirementXMLException& ex )
  {
    wcout << ex.widewhat() << endl;
  }
  catch ( const RequirementMismatchException& ex )
  {
    wcout << ex.widewhat() << endl;
  }
  catch ( const RequirementRegistryException& ex )
  {
    wcout << ex.widewhat() << endl;
  }
  catch ( const TestRegistryException& ex )
  {
    wcout << ex.widewhat() << endl;
  }
  catch ( const exception& ex )
  {
    cout << L"Error: " << ex.what() << endl;
  }
  catch ( ... )
  {
    wcout << L"Error: unhandled exeption" << endl;
  }
  
  return 0;
}

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

//Edit Protocol Test files
#include <EPMobDepPhase.h>

//Base Test files
#include <LoadPhase.h>
#include <DumpPhase.h>

//Test/Result Structure files
#include <TopLevelTestResult.h>
#include <TestPhaseLevelTestResult.h>
#include <TestInfoRegistrar.h>
#include <TestRegistry.h>
#include <TestRegistryException.h>

//Requirement files
#include <RequirementLoader.h>
#include <RequirementXMLException.h>
#include <RequirementRegistry.h>
#include <RequirementMismatchException.h>
#include <RequirementRegistryException.h>
#include <Requirement.h>

//Edit Protocol Analyzer Base files
#include <EPCastException.h>

//AAF Analyzer Base files
#include <AAFGraphInfo.h>

//Analyzer Base files
#include <AnalyzerException.h>

//Ax files
#include <AxInit.h>
#include <AxUtil.h>
#include <AxEx.h>

//STL files
#include <string>
#include <algorithm>
#include <fstream>

//Boost files
#include <boost/shared_ptr.hpp>

#if defined(OS_WINDOWS)
#pragma warning( push )
#pragma warning( disable: 4996 )
#endif
#include <boost/format.hpp>

#if defined(OS_WINDOWS)
#pragma warning( pop )
#endif

namespace {

using namespace aafanalyzer;
using namespace std;
using namespace boost;

basic_string<wchar_t> LevelToIndent(unsigned int l)
{
  basic_string<wchar_t> indent;

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
      wcout << LevelToIndent( _level ) << L"Detail: " << detail << endl;
  }
private:
  unsigned int _level;
};

void ListRequirements( const AxString& title, const Requirement::RequirementMap& reqs, const int level)
{
  wcout << LevelToIndent( level ) << title << ": ";
  if (reqs.size() == 0)
  {
    wcout << "None" << endl;
  }
  else
  {
    Requirement::RequirementMap::const_iterator iter;
    iter = reqs.begin();
    while ( true ) 
    {
        wcout << iter->first;
        iter++;
        if (iter != reqs.end() )
        {
          wcout << "; ";
        }
        else
        {
          break;
        }
    }
    wcout << endl;
  }
}

//======================================================================

// Recursively output result messages if active is true.  If the
// result name is equal to "filter" then force active to true.

void OutputVerboseResultMsgs( shared_ptr<const TestResult> res,
                              unsigned int level,
                              const wstring& filter,
                              bool active )
{
  if ( filter == res->GetName() )
  {
    active = true;
  }
  
  if ( active )
  {
    wcout << LevelToIndent(level) << "Name:   " << res->GetName() << endl;
    wcout << LevelToIndent(level) << "Desc:   " << res->GetDescription() << endl;
  
    const vector<AxString>& details = res->GetDetails();
    if ( !details.empty() )
    {
      for_each( details.begin(), details.end(), OutputDetail( level ) );
    }

    wcout << LevelToIndent(level) << "Result: ";
    if(res->GetResult() == TestResult::PASS) {
      wcout << L"Passed" << endl;
    }

    else if(res->GetResult() == TestResult::WARN)
    {
      wcout << L"Passed, but with warnings." << endl;
      wcout << LevelToIndent(level) << "Reason: " << res->GetExplanation() << endl;
    }

    else if(res->GetResult() == TestResult::FAIL)
    {
      wcout << L"Failed" << endl;
      wcout << LevelToIndent(level) << "Reason: " << res->GetExplanation() << endl;
    }
    else
    {
      assert(0);
    }
  
    ListRequirements( L"Passing Requirements", res->GetRequirements(TestResult::PASS), level);
    ListRequirements( L"Warning Requirements", res->GetRequirements(TestResult::WARN), level);
    ListRequirements( L"Failing Requirements", res->GetRequirements(TestResult::FAIL), level);

    wcout << endl;
  }

  if ( res->ContainsSubtests() )
  {
    TestResult::SubtestResultVector subResults = res->GetSubtestResults();
    for (unsigned int i = 0; i < subResults.size(); i++) {
        OutputVerboseResultMsgs(subResults.at(i), level + 1, filter, active);
    }
  }
}

void OutputVerboseResultMsgs( shared_ptr<const TestResult> res,
                              unsigned int level )
{
  wstring empty;
  OutputVerboseResultMsgs( res,level, empty, true );
}

void OutputVerboseResultMsgs( shared_ptr<const TestResult> res,
                              unsigned int level,
                              wstring filter )
{
  OutputVerboseResultMsgs( res,level, filter, false );
}

//======================================================================

// Escape single quotes.
wstring Esc( const wstring& in )
{
  wstring out;
  for( wstring::const_iterator i = in.begin();
       i != in.end();
       ++i )
  {
    if ( L'\'' == *i )
    {
      out.push_back( L'\\' );
      out.push_back( L'\'' );
    }
    else
    {
      out.push_back( *i );
    }
  }
  return out;
}

void OutputFileCoverage(shared_ptr<const TestResult> res, const basic_string<wchar_t> fileName, unsigned int level)
{
  wcout  << "File Name: " << fileName << endl; 
  wcout  << "Name:      " << res->GetName() << endl;
  wcout  << "Desc:      " << res->GetDescription() << endl;
  
  const vector<AxString>& details = res->GetDetails();
  if ( !details.empty() )
  {
    for_each( details.begin(), details.end(), OutputDetail( level ) );
  }

  wcout  << "Result:    ";
  if(res->GetResult() == TestResult::PASS) {
    wcout << L"Passed" << endl;
  }

  else if(res->GetResult() == TestResult::WARN)
  {
    wcout << L"Passed, but with warnings." << endl;
    wcout << "Reason:    " << res->GetExplanation() << endl;
  }

  else if(res->GetResult() == TestResult::FAIL)
  {
    wcout << L"Failed" << endl;
    wcout << "Reason:    " << res->GetExplanation() << endl;
  }
  else
  {
    assert(0);
  }
  
  ListRequirements( L"Passing Requirements", res->GetRequirements(TestResult::PASS), level);
  ListRequirements( L"Warning Requirements", res->GetRequirements(TestResult::WARN), level);
  ListRequirements( L"Failing Requirements", res->GetRequirements(TestResult::FAIL), level);

  wcout << endl;
}

void OutputSimpleResultMsgs( shared_ptr<const TestResult> res )
{
  Requirement::RequirementMap::const_iterator iter;

  const Requirement::RequirementMap& failures = res->GetRequirements( TestResult::FAIL );
  for ( iter = failures.begin(); iter != failures.end(); iter++ )
  {
    shared_ptr<const Requirement> req = iter->second;
    wcout << req->GetId() << L": \"" << req->GetName() << L"\" caused a failure." << endl;
    wcout << L"Description: " << req->GetDescription() << endl;
    wcout << L"See " << req->GetDocument() << L"(" << req->GetVersion()
          << L") Section " << req->GetSection() << endl;
        wcout << endl;
  }
    
  const Requirement::RequirementMap& warnings = res->GetRequirements( TestResult::WARN );
  for ( iter = warnings.begin(); iter != warnings.end(); iter++ )
  {
    shared_ptr<const Requirement> req = iter->second;
    wcout << req->GetId() << L": \"" << req->GetName() << L"\" caused a warning." << endl;
    wcout << L"Description: " << req->GetDescription() << endl;
    wcout << L"See " << req->GetDocument() << L"(" << req->GetVersion()
          << L") Section " << req->GetSection() << endl;
    wcout << endl;
  }   
}

void RegisterTests()
{
  //Register Load Phase tests.
  TestInfoRegistrar<FileLoad> fileLoad;
  TestInfoRegistrar<RefResolver> refResolver;
  TestInfoRegistrar<AcyclicAnalysis> acyclicAnalysis;
  
  //Register Edit Protocol Mob Dependency Phase tests.
  TestInfoRegistrar<DecorateEPTest> decorateEPTest;
  TestInfoRegistrar<CompMobDependency> compMobDependency;
  TestInfoRegistrar<EPDerivationTest> epDerivationTest;
  TestInfoRegistrar<EPNameTest> epNameTest;
  TestInfoRegistrar<EPContainedTrackTest> epTrackTest;
  TestInfoRegistrar<EPTrackContentsTest> epTrackContentsTest;

  //TODO: Uncomment when Locator Test is being run.
  //TestInfoRegistrar<EPLocatorTest> epLocatorTest;
  TestInfoRegistrar<EPEditRateTest> epEditRateTest;
  TestInfoRegistrar<EPEffectTest> epEffectTest;
  TestInfoRegistrar<EPAnnotationTest> epAnnotationTest;
  TestInfoRegistrar<EPDefinitionTest> epDefinitionTest;
  TestInfoRegistrar<EPHeaderTest> epHeaderTest;
  TestInfoRegistrar<EPParameterTest> epParameterTest;
  TestInfoRegistrar<EPMultiChannelAudioTest> epMultiChannelAudioTest;
  
  //Register Dump Phase tests.
  TestInfoRegistrar<FileDumper> fileDumper;   
}

class PrintRequirement
{
public:
  PrintRequirement( const string& type )
    : _type( AxStringUtil::mbtowc(type.c_str()) )
  {}

  ~PrintRequirement()
  {}

  void operator()( pair<const AxString, shared_ptr<const Requirement> > entry )
  {
    if ( _type == L"all" ||
             _type == entry.second->GetRequirementTypeAsString() )
    {
      // The boost format library below 1.34.0 doesn't support wformat
      // therefore these strings conversions are neccessary. I
      // (jptrainor) wrote this originally using 1.34.0 where wformat
      // was supported. It's awful but harmless.
      boost::format fmt( "%1%:%|14t|%2%%|22t|%3%" );
      fmt % AxStringUtil::wctomb( entry.first );
      fmt % AxStringUtil::wctomb( entry.second->GetRequirementTypeAsString() );
      fmt % AxStringUtil::wctomb( entry.second->GetName() );
      wcout << AxStringUtil::mbtowc( fmt.str().c_str() ) << endl;
    }
  }

private:
  wstring _type;
};

class PrintRequirementDetails
{
public:
  PrintRequirementDetails( const string& type )
    : _type( AxStringUtil::mbtowc(type.c_str()) )
  {}
  
  ~PrintRequirementDetails()
  {}

  void operator()( pair<const AxString, shared_ptr<const Requirement> > entry )
  {
    if ( _type == L"all" ||
         _type == entry.second->GetRequirementTypeAsString() )
    {
      wcout << L"ID:          " << entry.first << endl;
      wcout << L"Name:        " << entry.second->GetName() << endl;
      wcout << L"Type:        " << entry.second->GetRequirementTypeAsString() << endl;
      wcout << L"Category:    " << entry.second->GetCategoryAsString() << endl;
      wcout << L"Description: " << entry.second->GetDescription()<<endl;
      wcout << L"Document:    " << entry.second->GetDocument()<<endl;
      wcout << L"Version:     " << entry.second->GetVersion()<<endl;
      wcout << L"Section:     " << entry.second->GetSection()<<endl<<endl;
    }
  }

private:
  wstring _type;
};

void OutputRequirements( const AxString& title,
                         const Requirement::RequirementMap& reqs,
                         bool details,
                         const string& type )
{
  wcout << endl << title << endl;
  if ( !reqs.empty() )
  {
    if (details)
    {
      PrintRequirementDetails func( type );
      for_each( reqs.begin(), reqs.end(), func );
    }
    else
    {
      PrintRequirement func( type );
      for_each( reqs.begin(), reqs.end(), func );
    }
  }
  else 
  {
    wcout << L"None" << endl << endl;
  }
}

//======================================================================

class Usage
{
public:
  Usage()
    : _what( false, "" )
  {}

  Usage( const string& what )
    : _what( true, what )
  {}

  ~Usage()
  {}

  const pair<bool,string>& GetWhat() const
  {
    return _what;
  }

private:

  pair<bool,string> _what;
};

ostream& operator<<( ostream& os, const Usage& usage )
{
  os << "AAFAnalyzer -analyze -reqs file.xml [analysis options] filename.aaf" << endl;
  os << endl;

  os << "AAFAnalyzer -report -reqs file.xml [requirement options]" << endl;
  os << endl;

  os << "[analysis options]    = -uncheckedrequirements"    << endl;
  os << "                      | -verbose"                  << endl;
  os << "                      | -filter <result name>"     << endl;
  os << "                      | -filecoverage"             << endl;
  os << "                      | -dump"                     << endl;
  os << endl;

  os << "[requirement options] = -type all|file|app|def"    << endl;
  os << "                      | -detail"                   << endl;
  os << "                      | -testcoverage"             << endl;
  os << endl;

  os << "-dump:                  output graph of AAF file"                                        << endl;
  os << "-filecoverage:          output the file's requirement coverage data"                     << endl;
  os << "-uncheckedrequirements: allow tests to use unloaded requirements and test"               << endl;
  os << "                        results to use unassociated requirements"                        << endl;
  os << "-reqs:                  specify XML file that describes the requirements"                << endl;
  os << "-type:                  report only to specified requirement types, default is \"all\"." << endl;
  os << "-detail:                output requirement details"                                      << endl;
  os << "-testcoverage:          output all requirements covered by the test suite"               << endl;
  os << "-filter <arg>           limit verbose result output to result with name \"arg\""         << endl;
  os << "-verbose:               output details of how warnings and failures were discovered"     << endl;
  os << endl;
  os << "return values:" << endl;
  os << "\t0  - all tests passed" << endl;
  os << "\t1  - one or more tests failed" << endl;
  os << "\t2  - all tests passed, but with warnings" << endl;
  os << "\t3  - all tests did not run because an exception occured" << endl;
  os << endl;

  pair<bool,string> what = usage.GetWhat();
  if ( what.first )
  {
    os << endl;
    os << "error: " << what.second << endl;
  }

  os << endl;
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

    if ( argc == 1 )
    {
      throw Usage();
    }

    AxCmdLineArgs args( argc, argv );

    // Analyze option
    pair<bool,int> analyzeArg = args.get( "-analyze" );

    // Dump option
    pair<bool,int> dumpArg = args.get( "-dump" );
    
    // List All Requirements option
    pair<bool, int> reportArg = args.get( "-report" );
    string reqType("all");

    // List All Requirements option
    pair<bool, int> reqTypeArg = args.get( "-type" );
    if ( reqTypeArg.first )
    {
      pair<bool, const char*> which = args.get( reqTypeArg.second + 1 );

      if ( !which.first )
      {
        throw Usage( "missing type argument" );
      }

      string s( which.second );
      if ( ! (s == "file" || s == "app" || s == "def" || s == "all" ) )
      {
        throw Usage( "unsupported type arg \"" + s + "\"" );
      }

      reqType = s;
    }

    // List All Requirements option
    pair<bool, int> detailArg = args.get( "-detail" );
    
    // List All Covered Requirements option
    pair<bool, int> testCoverageArg = args.get( "-testcoverage" );
    
    // Print file coverage
    pair<bool, int> fileCoverageArg = args.get( "-filecoverage" );
    
    // Allow test to register with unregistered requirements
    pair<bool, int> unsafeRegistryArg = args.get ( "-uncheckedrequirements" );
    
    // Show verbose output
    pair<bool, int> verboseOutput = args.get( "-verbose" );

    // Filter output by result name.
    pair<bool, int> filterByNameOpt = args.get( "-filter" );
    pair<bool, const char*> filterByNameArg( false, 0 );
    if ( filterByNameOpt.first )
    {
      filterByNameArg = args.get( filterByNameOpt.second + 1 );
      if ( !filterByNameArg.first )
      {
        throw Usage( "missing filter name" );
      }
    }
        
    // Requirements Filename is last argument.
    pair<bool,int> reqsArg = args.get( "-reqs" );
    pair<bool, const char*> requirementsFile( false, 0 );
    if ( reqsArg.first )
    {
      requirementsFile = args.get( reqsArg.second+1 );
      if ( !requirementsFile.first )
      {
        throw Usage( "missing requirements file name argument" );
      }
    }

    //
    // validate arguments (that have not been validated above) before
        // accessing the position file parameter.
    //

    if ( !(reportArg.first || analyzeArg.first) )
    {
      throw Usage( "must specify either -report or -analyze" );
    }

    if ( !requirementsFile.first )
    {
      throw Usage( "no requirements file specified." );
    }

    // AAF Filename is the last argument.
    pair<bool,const char*> fileNameArg(false,0);
    if ( analyzeArg.first )
    {
      if ( args.IsFetched( argc-1, 1 ) )
      {
	throw Usage( "last argument should be an AAF file" );
      }
      fileNameArg = args.get( argc-1, 1 );
      args.MarkFetched( argc-1 );
    }

    //
    // load the requirements
    //
    
    RequirementLoader loader;
    loader.ParseXML( requirementsFile.second );
    
    // If a report is requested, but it is not a test coverage report,
    // then do it before registering the tests.
    if ( reportArg.first && !testCoverageArg.first )
    {
      OutputRequirements( L"All Loaded Requirements",
                          RequirementRegistry::GetInstance().GetAllRequirements(),
                          detailArg.first,
                          reqType );
      return 0;
    }
    
    //
    // Register the tests.
    //

    // Allow unregistered or non-existant requirements to be used when necessary.
    if ( unsafeRegistryArg.first )
    {
        TestRegistry::GetInstance().UseUnsafeRequirements();
    }
    RegisterTests();
    
    // Now that the tests are registered we can report coverage.
    if ( testCoverageArg.first )
    {
      OutputRequirements( L"Requirements Covered By Tests",
                          TestRegistry::GetInstance().GetRequirementCoverage(),
                          false,
                          reqType );
      return 0;
    }

    //
    // Execute the tests.
    //

    AxInit axInitObj;
    
    const basic_string<wchar_t> fileName = AxStringUtil::mbtowc( fileNameArg.second );

    // Create the result object.
    shared_ptr<TopLevelTestResult> spResult(new TopLevelTestResult());
    spResult->SetName( L"AAF Analyzer" );
    spResult->SetDescription( L"AAF Edit Protocol compliance test." );

    // first phase
    LoadPhase load( wcout, fileName );
    shared_ptr<const TestPhaseLevelTestResult> spSubResult( load.Execute() );
    spResult->AppendSubtestResult(spSubResult);
    if (spSubResult->GetResult()==TestResult::FAIL)
    {
      if ( verboseOutput.first )
      {
        if ( filterByNameArg.first )
        {
          OutputVerboseResultMsgs( spResult, 0, AxStringUtil::mbtowc(filterByNameArg.second) );
        }
        else
        {
          OutputVerboseResultMsgs( spResult, 0 );
        }
      }
      else if (fileCoverageArg.first)
      {
        OutputFileCoverage(spResult, fileName, 0);
      }
      else
      {
        OutputSimpleResultMsgs( spResult );
      }

      throw AnalyzerException(L"Load phase failed. Further tests aborted.");
    }
    
    //Store the test graph info so the AAF file is not inadvertantley closed.
    shared_ptr<const AAFGraphInfo> graphInfo = load.GetTestGraphInfo();

    // Second phase: execute the edit protocol mob dependency tests.
    EPMobDepPhase mobDepPhase( wcout, graphInfo->GetGraph() );
    spResult->AppendSubtestResult( mobDepPhase.Execute() );

    // More test phases go here...
    
    // optional dump phase
    if ( dumpArg.first )
    {
      DumpPhase dump( wcout, graphInfo->GetGraph() );
      spSubResult = dump.Execute();
      spResult->AppendSubtestResult(spSubResult);
    }

    spResult->SetResult(spResult->GetAggregateResult());
    if ( verboseOutput.first )
    {
      if ( filterByNameArg.first )
      {
        OutputVerboseResultMsgs( spResult, 0, AxStringUtil::mbtowc(filterByNameArg.second) );
      }
      else
      {
        OutputVerboseResultMsgs( spResult, 0 );
      }
    }
    else if (fileCoverageArg.first)
    {
        OutputFileCoverage(spResult, fileName, 0);
    }
    else
    {
        OutputSimpleResultMsgs( spResult );
    }
    
    TestResult::Result exitCode = spResult->GetResult();
    if ( exitCode == TestResult::FAIL )
    {
        //Failures occured
      return 1;
    }
    else if ( exitCode == TestResult::WARN )
    {
      //Warnings, but no failures occured
      return 2;
    }
    else
    {
      //All tests passed
      return 0;
    }
    
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
  catch ( const EPCastException& ex )
  {
    wcout << ex.widewhat() << endl;
  }
  catch ( const AxEx& ex )
  {
    wcout << L"Error: " << ex.widewhat() << endl;
  }
  catch ( const AnalyzerException& ex )
  {
    wcout << L"Error: " << ex.widewhat() << endl;
  }
  catch ( const exception& ex )
  {
    cout << "Error: " << ex.what() << endl;
  }
  catch ( ... )
  {
    wcout << L"Error: unhandled exeption" << endl;
  }
  
  //An exception occured and all tests were not run.
  return 3;
}

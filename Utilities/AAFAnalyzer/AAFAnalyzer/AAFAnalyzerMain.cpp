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

#define AAFANALYZER_VERSION "0.9.6"

//Edit Protocol Test files
#include <EPMobDepPhase.h>

//Base Test files
#include <LoadPhase.h>
#include <DumpPhase.h>

//Test/Result Structure files
#include <TopLevelTestResult.h>
#include <TestPhaseLevelTestResult.h>
#include <DetailLevelTestResult.h>
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
#include <set>
#include <sstream>

//Boost files
#include <boost/shared_ptr.hpp>

#if defined(OS_WINDOWS)
#pragma warning( push )
#pragma warning( disable: 4996 )
#endif
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>

#if defined(OS_WINDOWS)
#pragma warning( pop )
#endif

namespace {

using namespace aafanalyzer;
using namespace std;
using namespace boost;
namespace fs = boost::filesystem;

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

  if ( reqs.empty() )
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
// result name is equal to "filter" then force active to true.  The
// caller is responsible for consolidating results before calling this
// function.

void RecursiveOutputVerboseResultMsgs( shared_ptr<TestResult> res,
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

    if ( res->GetResultType() == TestResult::DETAIL )
    {
      shared_ptr<DetailLevelTestResult> spDetailResult = dynamic_pointer_cast<DetailLevelTestResult>(res);
      assert( spDetailResult );
      shared_ptr<Node> spNode = spDetailResult->GetAssociatedNode();
      if ( spNode )
      {
	wcout << LevelToIndent(level) << "Node:   " << spNode->GetLID() << L" (" << spNode->GetName() << L")" << endl;
      }
    }
  
    wcout << LevelToIndent(level) << "Result: ";
    wstring result;

    if (res->GetResult() == TestResult::COVERED) {
      result = L"Covered";
    }
    else if (res->GetResult() == TestResult::NOTED) {
      result = L"Noted";
    }
    else if (res->GetResult() == TestResult::PASS) {
      result = L"Passed";
    }
    else if (res->GetResult() == TestResult::INFO)
    {
      result = L"Info";
    }
    else if (res->GetResult() == TestResult::WARN)
    {
      result = L"Passed, but with warnings.";
    }
    else if (res->GetResult() == TestResult::FAIL)
    {
      result = L"Failed";
    }
    else
    {
      assert(res->GetResult() == TestResult::UNDEFINED);
      result = L"Undefined";
    }

    wcout << result << endl;

    wcout << LevelToIndent(level) << "Reason: " << res->GetExplanation() << endl;

    ListRequirements( L"Covered Requirements", res->GetRequirements(TestResult::COVERED), level);
    ListRequirements( L"Noted Requirements",   res->GetRequirements(TestResult::NOTED),   level);
    ListRequirements( L"Passing Requirements", res->GetRequirements(TestResult::PASS),    level);
    ListRequirements( L"Info Requirements",    res->GetRequirements(TestResult::INFO),    level);
    ListRequirements( L"Warning Requirements", res->GetRequirements(TestResult::WARN),    level);
    ListRequirements( L"Failing Requirements", res->GetRequirements(TestResult::FAIL),    level);

    const vector<AxString>& details = res->GetDetails();
    if ( !details.empty() )
    {
      for_each( details.begin(), details.end(), OutputDetail( level ) );
    }

    wcout << endl;
  }

  if ( res->ContainsSubtests() )
  {
    TestResult::SubtestResultVector subResults = res->GetSubtestResults();
    for (unsigned int i = 0; i < subResults.size(); i++) {
        RecursiveOutputVerboseResultMsgs(subResults.at(i), level + 1, filter, active);
    }
  }
}


void OutputVerboseResultMsgs( shared_ptr<TestResult> res,
                              unsigned int level,
                              const wstring& filter,
                              bool active )
{
  RecursiveOutputVerboseResultMsgs( res, level, filter, active );
}

void OutputVerboseResultMsgs( shared_ptr<TestResult> res,
                              unsigned int level )
{
  wstring empty;
  RecursiveOutputVerboseResultMsgs( res,level, empty, true );
}

void OutputVerboseResultMsgs( shared_ptr<TestResult> res,
                              unsigned int level,
                              wstring filter )
{
  RecursiveOutputVerboseResultMsgs( res,level, filter, false );
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

  if (res->GetResult() == TestResult::COVERED)
  {
    wcout << L"Covered" << endl;
  }
  else if (res->GetResult() == TestResult::NOTED)
  {
    wcout << L"Noted" << endl;
  }
  else if (res->GetResult() == TestResult::PASS)
  {
    wcout << L"Passed" << endl;
  }
  else if (res->GetResult() == TestResult::WARN)
  {
    wcout << L"Info" << endl;
  }
  else if (res->GetResult() == TestResult::INFO)
  {
    wcout << L"Warned" << endl;
  }
  else if (res->GetResult() == TestResult::FAIL)
  {
    wcout << L"Failed" << endl;
  }
  else
  {
    assert(0);
  }

  wcout << "Reason:    " << res->GetExplanation() << endl;
  
  ListRequirements( L"Covered Requirements", res->GetRequirements(TestResult::COVERED), level );
  ListRequirements( L"Noted Requirements",   res->GetRequirements(TestResult::NOTED),   level );
  ListRequirements( L"Passing Requirements", res->GetRequirements(TestResult::PASS),    level );
  ListRequirements( L"Info Requirements",    res->GetRequirements(TestResult::INFO),    level );
  ListRequirements( L"Warning Requirements", res->GetRequirements(TestResult::WARN),    level );
  ListRequirements( L"Failing Requirements", res->GetRequirements(TestResult::FAIL),    level );

  wcout << endl;
}

void CollectReasons( shared_ptr<const TestResult> res,
                     const wstring& reqId,
                     TestResult::Result result,
                     set<wstring>& reasonSet )
{
  // Recurse down to leaf (ie. single) results to find the detailed
  // result explanations. The higher level
  // agregate results do not contain useful explanations.
  if ( res->ContainsSubtests() )
  {
    const TestResult::SubtestResultVector&
      subResults = res->GetSubtestResults();
    for( size_t i = 0; i < subResults.size(); ++i )
    {
      CollectReasons( subResults[i], reqId, result, reasonSet );
    }
        return;
  }

  // This is a leaf result. If it matches the reqId and result we are
  // looking for then output the explanation.
  if ( res->HasResult( reqId, result ) )
  {
        reasonSet.insert( res->GetExplanation() );
  }
}

void OutputReasons( shared_ptr<const TestResult> res,
                    const wstring& reqId,
                    TestResult::Result result )
{
        set<wstring> reasonSet;
        CollectReasons( res, reqId, result, reasonSet );
        for( set<wstring>::const_iterator iter = reasonSet.begin();
                 iter != reasonSet.end();
                 ++iter )
        {
                wcout << "Reason : " << *iter << endl;
        }
}

void OutputSimpleResultMsgs( shared_ptr<const TestResult> res )
{
  Requirement::RequirementMap::const_iterator iter;

  const Requirement::RequirementMap& failures = res->GetRequirements( TestResult::FAIL );
  for ( iter = failures.begin(); iter != failures.end(); iter++ )
  {
    shared_ptr<const Requirement> req = iter->second;
    wcout << "FAIL   : " << req->GetId() << L", " << req->GetName() << endl;
    wcout << L"Desc   : " << req->GetDescription() << endl;

    if ( req->GetDocument().size() > 0 )
    {
      wcout << L"Doc    : " << req->GetDocument() << L" (" << req->GetVersion()
            << L") Section " << req->GetSection() << endl;
    }

    OutputReasons( res, req->GetId(), TestResult::FAIL );

    wcout << endl;
  }
    
  const Requirement::RequirementMap& warnings = res->GetRequirements( TestResult::WARN );
  for ( iter = warnings.begin(); iter != warnings.end(); iter++ )
  {
    shared_ptr<const Requirement> req = iter->second;
    wcout << "WARN   : " << req->GetId() << L", " << req->GetName() << endl;
        wcout << L"Desc   : " << req->GetDescription() << endl;

    if ( req->GetDocument().size() > 0 )
    {
      wcout << L"Doc    : " << req->GetDocument() << L" (" << req->GetVersion()
            << L") Section " << req->GetSection() << endl;
    }

    OutputReasons( res, req->GetId(), TestResult::WARN );

    wcout << endl;
  }   
}


//======================================================================

void WriteXMLResult(shared_ptr<const DetailLevelTestResult> res, wostream& os)
{
  os << "\t<result>" << endl;
  os << "\t\t<id>" <<  res->GetId() << "</id>" << endl;
  os << "\t\t<status>" << res->GetResultAsString() << "</status>" << endl;
  os << "\t\t<explain><![CDATA[" << res->GetExplanation() << "]]></explain>" << endl;
  os << "\t</result>" << endl;
}

void RecursiveOutputXMLResults( shared_ptr<const TestResult> res, wostream& os )
{
  if (res->GetResultType() == TestResult::DETAIL)
  {
    shared_ptr<const DetailLevelTestResult> detailResult = dynamic_pointer_cast<const DetailLevelTestResult>(res);
    WriteXMLResult(detailResult, os);
  }

  TestResult::SubtestResultVector subResults = res->GetSubtestResults();
  for (TestResult::SubtestResultVector::const_iterator iter = subResults.begin();
       iter != subResults.end();
       ++iter)
  {
    RecursiveOutputXMLResults(*iter, os);
  }
}

void OutputXMLResults(shared_ptr<const TestResult> res, wostream& os)
{
  os << "<aafanalyzer_results version=\"" << AAFANALYZER_VERSION << "\">" << endl;
  RecursiveOutputXMLResults(res,os);
  os << "</aafanalyzer_results>" << endl;
}

//======================================================================

void RegisterTests()
{
  // Register Load Phase tests.
  TestInfoRegistrar<FileLoad> fileLoad;
  TestInfoRegistrar<RefResolver> refResolver;
  TestInfoRegistrar<AcyclicAnalysis> acyclicAnalysis;
  TestInfoRegistrar<CompMobDependency> compMobDependency;

  // Register file dumper. It is not really a test but is implemented
  // using the test interface so should registered.
  TestInfoRegistrar<FileDumper> fileDumper;   
  
  //Register Edit Protocol Mob Dependency Phase tests.
  TestInfoRegistrar<DecorateEPTest> decorateEPTest;
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
      wcout << L"ID:          " << entry.first                                << endl;
      wcout << L"Name:        " << entry.second->GetName()                    << endl;
      wcout << L"Type:        " << entry.second->GetRequirementTypeAsString() << endl;
      wcout << L"Category:    " << entry.second->GetCategoryAsString()        << endl;
      wcout << L"Action:      " << entry.second->GetAction()                  << endl;
      wcout << L"Description: " << entry.second->GetDescription()             << endl;
      wcout << L"Annotation:  " << entry.second->GetAnnotation()              << endl;
      wcout << L"Note: "        << entry.second->GetNote()                    << endl;
      wcout << L"Document:    " << entry.second->GetDocument()                << endl;
      wcout << L"Version:     " << entry.second->GetVersion()                 << endl;
      wcout << L"Section:     " << entry.second->GetSection()                 << endl;
      wcout << endl;
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

class GenerateRequirementSQLInsert
{
public:
  GenerateRequirementSQLInsert()
  {}
  
  ~GenerateRequirementSQLInsert()
  {}

  wstring Escape( const wstring& s )
  {
    wstring result;

    for( wstring::const_iterator c = s.begin();
	 c != s.end();
	 ++c )
    {
      if ( *c == L'\'' )
      {
	result.push_back( L'\\' );
      }
      result.push_back( *c );
    }

    return result;
  }

  wstring Q( const wstring& s, bool comma = true )
  {
    wstring result( L"'" + Escape(s) + L"'" );

    if ( comma )
    {
      result.append( L", " );
    }

    return result;
  }

  void operator()( pair<const AxString, shared_ptr<const Requirement> > entry )
  {
    static wstring stnd_stmnt
      = L"INSERT INTO Standard (doc_title, doc_version) VALUES ";
    static wstring req_stmnt
      = L"INSERT INTO Requirement (req_formal_id, req_name, description, req_type, category, annotate, note, standardid, doc_section, report, status) VALUES ";

    wstring report( L"FALSE" );
    if ( entry.second->GetRequirementType() == Requirement::FILE )
    {
      report = L"TRUE";
    }

    // Build the Standard insert statement.
    if ( !(entry.second->GetDocument().empty() && entry.second->GetVersion().empty()) )
    {
      wstringstream stnd_stmnt_os;
      stnd_stmnt_os << stnd_stmnt;
      stnd_stmnt_os << L"(";
      stnd_stmnt_os << Q( entry.second->GetDocument() );
      stnd_stmnt_os << Q( entry.second->GetVersion(), false );
      stnd_stmnt_os << L");";
      stnd_stmnt_os << endl;
      _insert_standard_stmnts.insert( stnd_stmnt_os.str() );
    }

    // Build the Requirement insert statement.
    wstringstream req_stmnt_os;
    req_stmnt_os << req_stmnt;
    req_stmnt_os << L"(";
    req_stmnt_os << Q( entry.second->GetId() );
    req_stmnt_os << Q( entry.second->GetName() );
    req_stmnt_os << Q( entry.second->GetDescription() );
    req_stmnt_os << Q( entry.second->GetRequirementTypeAsString() );
    req_stmnt_os << Q( entry.second->GetCategoryAsString() );
    req_stmnt_os << Q( entry.second->GetAnnotation() );
    req_stmnt_os << Q( entry.second->GetNote() );

    // Query to get the Standard.standarid of the standard document.
    if ( !(entry.second->GetDocument().empty() && entry.second->GetVersion().empty()) )
    {
      req_stmnt_os << L"(SELECT standardid FROM Standard"
		   << L" WHERE"
		   << L" doc_title=" << Q(entry.second->GetDocument(),false)
		   << L" AND"
		   << L" doc_version=" << Q(entry.second->GetVersion(),false)
		   << L"), ";
      req_stmnt_os << Q( entry.second->GetSection() );
    }
    else
    {
      // Else null standardid and doc_section because they aren't
      // specified in the xml file.
      req_stmnt_os << L" NULL, NULL, ";
    }

    req_stmnt_os << report << L", ";
    req_stmnt_os << Q( L"active", false );
    req_stmnt_os << L");";
    req_stmnt_os << endl;
    _insert_requirement_stmnts.insert( req_stmnt_os.str() );
  }

  void Dump( wostream& os )
  {
    for( set<wstring>::const_iterator iter = _insert_standard_stmnts.begin();
	 iter != _insert_standard_stmnts.end();
	 ++iter )
    {
      wcout << *iter << endl;
    }

    for( set<wstring>::const_iterator iter = _insert_requirement_stmnts.begin();
	 iter != _insert_requirement_stmnts.end();
	 ++iter )
    {
      wcout << *iter << endl;
    }
  }

private:
  set<wstring> _insert_standard_stmnts;
  set<wstring> _insert_requirement_stmnts;
};

void OutputRequirementSQLUpdate( const Requirement::RequirementMap& reqs )
{
  GenerateRequirementSQLInsert result = 
    for_each( reqs.begin(), reqs.end(), GenerateRequirementSQLInsert() );
  result.Dump( wcout );
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
  os << "AAFAnalyzer [-reqs file.xml] [analysis options] filename.aaf" << endl;
  os << endl;

  os << "AAFAnalyzer -report [-sqlinsert] [-reqs file.xml] [requirement options]" << endl;
  os << endl;

  os << "[analysis options]    = -verbose"                  << endl;
  os << "                      | -filter <result name>"     << endl;
  os << "                      | -filecoverage"             << endl;
  os << "                      | -xmlresults"               << endl;
  os << "                      | -dump {header | comp}"     << endl;
  os << endl;

  os << "[requirement options] = -type all|file|app|def"    << endl;
  os << "                      | -detail"                   << endl;
  os << "                      | -testcoverage"             << endl;
  os << endl;

  os << "-dump header:           dump contained objects beginning with header"                    << endl;
  os << "-dump comp:             dump the top level composition, including resolved references"   << endl;
  os << "-filecoverage:          output the file's requirement coverage data"                     << endl;
  os << "-xmlresults:            dump results in xml format"                                      << endl;
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
    // Figure out the install location.
    fs::path argvzero( argv[0], fs::native );
    fs::path installPath = argvzero.branch_path();

    //
    // Process the command line arguments.
    //

    if ( argc == 1 )
    {
      throw Usage();
    }

    AxCmdLineArgs args( argc, argv );

    // Dump option
    pair<bool,int> dumpArg = args.get( "-dump" );
    pair<bool,const char*> dumpOption( false, 0 );
    if ( dumpArg.first )
    {
      dumpOption = args.get( dumpArg.second + 1 );
      if ( !dumpOption.first )
      {
        throw Usage( "-dump must be followed by an argument specifying what to dump" );
      }

      if ( !(dumpOption.second == string("header") ||
             dumpOption.second == string("comp") ) )
      {
        boost::format fmt( "unkown -dump option: \"%1%\"" );
        fmt % dumpOption.second;
        throw Usage( fmt.str() );
      }
   }
    
    // Generate sql insert statements.
    pair<bool, int> sqlInsertArg = args.get( "-sqlinsert" );

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

    // Show requirement detail options
    pair<bool, int> detailArg = args.get( "-detail" );
    
    // List all requirements covered by tests
    pair<bool, int> testCoverageArg = args.get( "-testcoverage" );
    
    // Print file coverage
    pair<bool, int> fileCoverageArg = args.get( "-filecoverage" );

    // Dump full results in xml format
    pair<bool, int> xmlResultArg = args.get( "-xmlresults" );
    
    // Show verbose result output
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
        
    // Requirements filename
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

    // AAF Filename is the last argument if requirements report not requested.
    pair<bool,const char*> fileNameArg(false,0);
    if ( !reportArg.first )
    {
      if ( args.IsFetched( argc-1, 1 ) )
      {
        throw Usage( "last argument should be an AAF file" );
      }
      fileNameArg = args.get( argc-1, 1 );
      args.MarkFetched( argc-1 );
    }

    // Finally, check for unprocessed options rather than blindly
    // allowing something that a user entered in error to pass.
    for( int i = 1; i < argc; ++i )
    {
      if ( !args.IsFetched( i ) )
      {
        boost::format fmt( "unkown argument \"%1%\"" );
        fmt % argv[i];
        throw Usage( fmt.str() );
      }
    }

    //
    // load the requirements
    //
    
    RequirementLoader loader;
    if ( requirementsFile.first )
    {
      loader.ParseXML( requirementsFile.second );
    }
    else
    {
      // Assume it is the same directory where the program is      
      // executing from.
      fs::path path = installPath / "AAFRequirements.xml";
          loader.ParseXML( path.string().c_str() );
    }


    // sql insert generate
    if ( sqlInsertArg.first )
    {
      OutputRequirementSQLUpdate( RequirementRegistry::GetInstance().GetAllRequirements() );
      return 0;
    }

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

    // First phase - load all objects and resolve references.
    LoadPhase load( wcout, fileName );
    shared_ptr<TestPhaseLevelTestResult> spSubResult( load.Execute() );
    spResult->AppendSubtestResult(spSubResult);
    
    // Store the test graph info so the AAF file is not inadvertantley
    // closed.
    shared_ptr<const AAFGraphInfo> graphInfo = load.GetTestGraphInfo();

    // The source references are resolved as part of the LoadPhase
    // test. We can dump the composition after that test runs.  This
    // tells us what the data structure looks like going into the more
    // detailed edit protocol (e.g. mob dependency) tests.
    vector<shared_ptr<Node> > roots = load.GetRoots();
    if ( dumpArg.first )
    {
      if ( dumpOption.second == string("header") )
      {
        wcout << L"header dump:" << endl;
        DumpPhase dump( wcout, graphInfo->GetGraph() );
        spSubResult = dump.Execute();
        spResult->AppendSubtestResult(spSubResult);
      }
      else
      {
        assert( dumpOption.second == string( "comp" ) );
        wcout << "composition dump:" << endl;

        if ( load.IsCyclic() )
        {
          assert( roots.empty() );
          wcout << L"Note: The reference graph is cyclic. No composotion root was determined." << endl;
        }

        wcout << L"found " <<  static_cast<unsigned int>(roots.size()) << L" unreferenced object";
        if ( roots.size() > 1 )
        {
          wcout << "s";
        }
        wcout << endl;

        for( size_t i = 0; i < roots.size(); ++i )
        {
          wcout << L"root object " << static_cast<unsigned int>(i) << L": " << roots[i]->GetName() << endl;
          DumpPhase dump( wcout, graphInfo->GetGraph(), roots[i] );
          spSubResult = dump.Execute();
          spResult->AppendSubtestResult(spSubResult);
        }
      }
    }

    // Second phase: determine the edit protocol types of the loaded
    // objects, analyze EP dependencies, and numerous finer grain
    // tests.
    if ( !load.IsCyclic() )
    {
      EPMobDepPhase mobDepPhase( wcout, graphInfo->GetGraph(), load.GetCompMobRoots() );
      spResult->AppendSubtestResult( mobDepPhase.Execute() );
    }

    // Consildate the results before reporting or else the top level
    // status, and requirement sets, will not reflect the low level
    // results.
    spResult->ConsolidateResults();

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
    else if (xmlResultArg.first)
    {
      OutputXMLResults(spResult, wcout);
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

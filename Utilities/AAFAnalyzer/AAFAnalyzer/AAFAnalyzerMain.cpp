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

void OutputVerboseResultMsgs(shared_ptr<const TestResult> res, unsigned int level)
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
  
  if ( res->ContainsSubtests() )
  {
    TestResult::SubtestResultVector subResults = res->GetSubtestResults();
    for (unsigned int i = 0; i < subResults.size(); i++) {
        OutputVerboseResultMsgs(subResults.at(i), level + 1);
    }
  }
 
}


void OutputPythonErrors(shared_ptr<const TestResult> res, int level, int breadth)
{
	wstring s;
  	if(res->GetResult()==TestResult::PASS)
  	{
  	}
	else
  	{			
  		wcerr<< "\terr" <<level<<breadth<<"= parse.Error('"<<res->GetName() <<"')"<< endl;
  		wcerr<< "\terr" <<level<<breadth<<".set_description('"<<res->GetDescription() <<"')"<< endl;
  		//wcout<< "Test Name:   " << res->GetName() << endl;
  		//wcout<< "Desc:   " << res->GetDescription() << endl;
  
  		
		const vector<AxString>& details = res->GetDetails();
		for (unsigned int i = 0; i<details.size(); i++ )
    	{
    		wcerr<< "\terr" <<level<<breadth<<".set_detail('"<<details.at(i) <<"')"<< endl;
    	}
		
		// Making sure 'reason' will be written on one line in the python file
			wstring reason=res->GetExplanation();
			wstring s=reason;
			while (s.rfind(L"\n")!=wstring::npos)
			{	
				int index=s.rfind(L"\n");
				reason.replace(index, 2, L"");
				reason.insert(index, L"\\n");
				s=s.substr(0, index);
			}
		//wcout  << "Result: ";
		if(res->GetResult() == TestResult::WARN)
		{
			wcerr<< "\terr" <<level<<breadth<<".set_result('Passed, but with warnings')"<< endl;
			
			wstring reason=res->GetExplanation();
			wstring s=reason;
			
			wcerr<< "\terr" <<level<<breadth<<".set_reason('"<<reason <<"')"<< endl;
			const Requirement::RequirementMap& reqs = res->GetRequirements(TestResult::WARN);
			if (reqs.size() == 0)
  			{
   				 wcerr<< "\terr" <<level<<breadth<<".set_warn('None')"<< endl;
  			}
  			else
  			{
    			Requirement::RequirementMap::const_iterator iter;
    			iter = reqs.begin();
    			while ( true ) 
    			{
    				wcerr<< "\terr" <<level<<breadth<<".set_warn('"<<iter->first <<"')"<< endl;
        			iter++;
        			if (iter == reqs.end() )
        			{
          				break;
        			}
    			}
  			
   			}
			//wcout << L"Passed, but with warnings." << endl;
			//ListRequirements( L"Warning Requirements", res->GetRequirements(TestResult::WARN), 0);
		    //wcout << "Reason: " << res->GetExplanation() << endl;
		}
		
		else if(res->GetResult() == TestResult::FAIL)
		{
			//wcout << L"Failed" << endl;
			wcerr<< "\terr" <<level<<breadth<<".set_result('Failed')"<< endl;
			wcerr<< "\terr" <<level<<breadth<<".set_reason('"<<reason <<"')" << endl;
			
			const Requirement::RequirementMap& failreqs = res->GetRequirements(TestResult::FAIL);
			if (failreqs.size() == 0)
  			{
   				 wcerr<< "\terr" <<level<<breadth<<".set_fail('None')"<< endl;
  			}
  			else
  			{
    			Requirement::RequirementMap::const_iterator iter;
    			iter = failreqs.begin();
    			while ( true ) 
    			{
    				wcerr<< "\terr" <<level<<breadth<<".set_fail('"<<iter->first <<"')"<< endl;
        			iter++;
        			if (iter == failreqs.end() )
        			{
          				break;
        			}
    			}
  			
   			}
			
			const Requirement::RequirementMap& warnreqs = res->GetRequirements(TestResult::WARN);
			if (warnreqs.size() == 0)
  			{
   				 wcerr<< "\terr" <<level<<breadth<<".set_warn('None')"<< endl;
  			}
  			else
  			{
    			Requirement::RequirementMap::const_iterator iter;
    			iter = warnreqs.begin();
    			while ( true ) 
    			{
    				wcerr<< "\terr" <<level<<breadth<<".set_warn('"<<iter->first <<"')"<< endl;
        			iter++;
        			if (iter == warnreqs.end() )
        			{
          				break;
        			}
    			}
  			
   			}
			
		}
		else
		{
			assert(0);
		}
		
	
	}
	
	if ( res->ContainsSubtests() )
  	{
    	TestResult::SubtestResultVector subResults = res->GetSubtestResults();
    	for (unsigned int i = 0; i < subResults.size(); i++) {
    		if (subResults.at(i)->GetResult()==TestResult::FAIL || subResults.at(i)->GetResult()==TestResult::WARN)
    		{	
    			OutputPythonErrors(subResults.at(i), level+1, i);
    			wcerr<< "\terr" <<level<<breadth<<".set_suberror(err"<<level+1<<i <<")"<< endl;
    		}
    	}
  	}
 
}

void WritePythonErrors(shared_ptr<const TestResult> res)
{			
			
			//freopen(fileName,"w",stdout);
    		OutputPythonErrors( res, 0, 0 );
    		wcerr<<"\terrors.append(err00)"<<endl<<endl;
    		//fclose (stdout);
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

void PrintRequirement( pair<const AxString, shared_ptr<const Requirement> > entry )
{
    wcout << entry.first << L": " << entry.second->GetName() << endl;

}
void PrintRequirementDetails( pair<const AxString, shared_ptr<const Requirement> > entry )
{
    wcout <<L"ID:          "<<entry.first << endl;
    wcout <<L"Name:        "<< entry.second->GetName() << endl;
    wcout <<L"Type:        "<< entry.second->GetRequirementType() << endl;
    wcout <<L"Category:    "<< entry.second->GetCategory() << endl;
	wcout <<L"Description: "<<entry.second->GetDescription()<<endl;
	wcout <<L"Document:    "<<entry.second->GetDocument()<<endl;
	wcout <<L"Version:     "<<entry.second->GetVersion()<<endl;
	wcout <<L"Section:     "<<entry.second->GetSection()<<endl<<endl;
}

void OutputRequirements( const AxString& title, const Requirement::RequirementMap& reqs, bool details)
{
  wcout << endl << title << endl;
  if ( !reqs.empty() )
  {
  	if (details)
  	{
    	for_each( reqs.begin(), reqs.end(), PrintRequirementDetails );
  	}
  	else
  	{
  		for_each( reqs.begin(), reqs.end(), PrintRequirement ); 
  	}
  }
  else 
  {
     wcout << L"None" << endl << endl;
  }
}

//======================================================================

class Usage
{};

ostream& operator<<( ostream& os, const Usage& )
{
  os << "Valid usages are: " << endl;
  os << "AAFAnalyzer [options] filename.aaf requirement_filename.xml" << endl;
  os << "AAFAnalyzer [requirement output] requirement_filename.xml" << endl;
  os << "[options] = -dump | -uncheckedrequirements | -verbose | [requirement output]" << endl;
  os << "[requirement output] = -allreqs | -coverage" << endl;
  os << endl;
  os << "-dump: Output graph of AAF file" << endl;
  os << "-uncheckedrequirements: Allow tests to use unloaded requirements and test" << endl;
  os << "                        results to use unassociated requirements" << endl;
  os << "-allreqs: Output all loaded requirements" << endl;
  os << "-coverage: Output all requirements covered by the test suite." << endl;
  os << "-verbose: Output details of how warnings and failures were discovered." << endl;
  return os;
};

//======================================================================

} // end of namespace


int main( int argc, char** argv )
{
  using namespace boost;
  using namespace std;
  using namespace aafanalyzer;

  int regressionTestModifier = 1;

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
    
    // List All Requirements option
    pair<bool, int> allDetReqsArg = args.get( "-alldetailedreqs" );
    
    // List All Covered Requirements option
    pair<bool, int> coverageArg = args.get( "-coverage" );
    
    // Print file coverage
    pair<bool, int> fileCoverageArg = args.get( "-filecoverage" );
    
    // Output Python code consisting of TestResult errors in a Python data structure
    pair<bool, int> pythonDataArg = args.get( "-python" );
    
    // Allow test to register with unregistered requirements
    pair<bool, int> unsafeRegistryArg = args.get ( "-uncheckedrequirements" );
    
    // Show verbose output
    pair<bool, int> verboseOutput = args.get( "-verbose" );

    // Undocumented parameter used to return 0 when the regression tests run
    // even if there are failures
    pair<bool, int> regressionTest = args.get( "-regressiontest" );
    if ( regressionTest.first )
    {
        regressionTestModifier = 0;
    }
	
	
    // Filename is the second last argument.
    pair<bool,const char*> fileNameArg = args.get( argc-2, 1 );
    
    // Requirements Filename is last argument.
    pair<bool, const char*> requirementsFile = args.get( argc-1, 2 );
    
    int numFlags = dumpArg.first + allReqsArg.first + allDetReqsArg.first + coverageArg.first + 
                   unsafeRegistryArg.first + verboseOutput.first + 
                   regressionTest.first + fileCoverageArg.first + pythonDataArg.first;
    
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
      //AAFAnalyzer, the AAF file and the XML file 
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
      OutputRequirements(L"All Loaded Requirements", RequirementRegistry::GetInstance().GetAllRequirements(), false);
      if ( !runTests && !coverageArg.first )
      {
        //No tests were run so report a success.
        return 0;
      }
    }
    
    if ( allDetReqsArg.first )
    {
      OutputRequirements(L"All Loaded Requirements", RequirementRegistry::GetInstance().GetAllRequirements(), true);
      if ( !runTests && !coverageArg.first )
      {
        //No tests were run so report a success.
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
      OutputRequirements(L"Requirements Covered By Tests", TestRegistry::GetInstance().GetRequirementCoverage(), false);
      if ( !runTests )
      {
        //No tests were run so report a success.
        return 0;
      }
    }

    //
    // Execute the tests.
    //

    AxInit axInitObj;
    
    const basic_string<wchar_t> fileName = AxStringUtil::mbtowc( fileNameArg.second );

    //Create the result object.
    shared_ptr<TopLevelTestResult> spResult(new TopLevelTestResult());
    spResult->SetName( L"AAF Analyzer" );
    spResult->SetDescription( L"AAF Edit Protocol compliance test." );

    //first phase
    LoadPhase load( wcout, fileName );
    shared_ptr<const TestPhaseLevelTestResult> spSubResult( load.Execute() );
    spResult->AppendSubtestResult(spSubResult);
	if (spSubResult->GetResult()==TestResult::FAIL){
		if ( verboseOutput.first )
    	{
        	OutputVerboseResultMsgs(spResult, 0);
    	}
    	else if (fileCoverageArg.first && pythonDataArg.first)
    	{
    		OutputFileCoverage(spResult, fileName, 0);
    		WritePythonErrors(spResult);

    	}
    	else if (fileCoverageArg.first && !pythonDataArg.first)
    	{
    		OutputFileCoverage(spResult, fileName, 0);

    	}
    	else
    	{
        	OutputSimpleResultMsgs( spResult );
    	}
		throw AnalyzerException(L"Load phase failed.  Further tests aborted.");
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
        OutputVerboseResultMsgs(spResult, 0);
    }
    else if (fileCoverageArg.first && pythonDataArg.first)
    {
    	OutputFileCoverage(spResult, fileName, 0);
    	WritePythonErrors(spResult);
    }
    else if (fileCoverageArg.first && !pythonDataArg.first)
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
        return 1 * regressionTestModifier;
    }
    else if ( exitCode == TestResult::WARN )
    {
        //Warnings, but no failures occured
        return 2 * regressionTestModifier;
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
  return 3 * regressionTestModifier;
}

//project files
#include <LoadPhase.h>
#include <DumpPhase.h>

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
    std::wcout << LevelToIndent(level) << L"Test Passed!" << std::endl << std::endl;
  }

  else if(res->GetResult() == TestResult::WARN)
  {
    std::wcout << LevelToIndent(level) << L"Test Passed, but with warnings!" << std::endl;
    std::wcout << LevelToIndent(level) << res->GetExplanation() << std::endl << std::endl;
  }

  else if(res->GetResult() == TestResult::FAIL)
  {
    std::wcout << LevelToIndent(level) << L"Test Failed!" << std::endl;
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

} // end of namespace

int main( int argc, char** argv )
{
  try
  {
    //
    // Process the command line arguments.
    //

    AxCmdLineArgs args( argc, argv );

    // Dump option
    std::pair<bool,int> dumpArg = args.get( "-dump" );

    // Filename is the last argument.
    std::pair<bool,const char*> fileNameArg = args.get( argc-1, 1 );

    if ( !fileNameArg.first )
    {
      throw Usage();
    }

    //
    // Execute the tests.
    //

    AxInit axInitObj;
    
    const std::basic_string<wchar_t> fileName = AxStringUtil::mbtowc( fileNameArg.second );

    //Create the result object.
    boost::shared_ptr<TestResult> spSubResult;
    boost::shared_ptr<TestResult> spResult(new TestResult());
    spResult->SetName(L"AAF Analyzer");
    spResult->SetDescription(L"Run a suite of tests on an AAF file.");
    
    //first phase
    LoadPhase load(std::cout, fileName); 
    spSubResult = load.Execute();
    spResult->AppendSubtestResult(spSubResult);
    
    // More test phases go here...
    
    // optional dump phase
    if ( dumpArg.first )
    {
	  DumpPhase dump(std::cout, load.GetTestGraph());
      spSubResult = dump.Execute();
      spResult->AppendSubtestResult(spSubResult);
    }

    spResult->SetResult(spResult->GetAggregateResult());      
    OutputResultMsgs(spResult, 0);
  }
  catch ( const Usage& ex )
  {
    std::cout << ex << std::endl;
  }
  catch ( const AxExHResult& ex )
  {
    std::wcout << L"Error: " << ex.widewhat() << std::endl;
  }
  catch ( const std::exception& ex )
  {
    std::cout << "Error: " << ex.what() << std::endl;
  }
  catch ( ... )
  {
    std::cout << "Error: unhandled exeption" << std::endl;
  }
  
  return 0;
}

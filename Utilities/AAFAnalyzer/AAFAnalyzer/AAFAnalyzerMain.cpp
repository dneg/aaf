//project files
#include <LoadPhase.h>
#include <DumpPhase.h>

//Ax files
#include <AxInit.h>
#include <AxUtil.h>
#include <AxEx.h>

namespace {

using namespace aafanalyzer;

void OutputResultMsgs(std::vector<TestResult>& vec)
{
  for(unsigned int i = 0; i < vec.size(); i++)
  {
    std::cout << vec.at(i).GetName() << std::endl;
    std::cout << vec.at(i).GetDescription() << std::endl;

    if(vec.at(i).GetResult() == TestResult::success)
      std::cout << "Test Passed!" << std::endl << std::endl;

    else if(vec.at(i).GetResult() == TestResult::warning)
    {
      std::cout << "Test Passed, but with warnings!" << std::endl;
      std::cout << vec.at(i).GetExplanation() << std::endl << std::endl;
    }

    else if(vec.at(i).GetResult() == TestResult::error)
    {
      std::cout << "Test Failed!" << std::endl;
      std::cout << vec.at(i).GetExplanation() << std::endl << std::endl;
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

    std::vector<TestResult> results;
    
    //first phase
    LoadPhase load(std::cout, fileName); 
    results = load.Execute();
    OutputResultMsgs(results);
    
    // More test phases go here...
    
    // optional dump phase
    if ( dumpArg.first )
      {
	DumpPhase dump(std::cout, load.GetTestGraph());
	results = dump.Execute();
	OutputResultMsgs(results);
      }
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

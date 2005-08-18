//project files
#include <LoadPhase.h>
#include <DumpPhase.h>

//Ax files
#include <AxEx.h>

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

int main()
{
  try
  {
    const std::basic_string<wchar_t> FileName = L"test.aaf";  
    std::vector<TestResult> results;

    if(AAFLoad(getenv("AX_AAF_COMAPI")) == 0)
    {
      //first phase
      LoadPhase load(std::cout, FileName); 
      results = load.Execute();
      OutputResultMsgs(results);

      //dump phase
      DumpPhase dump(std::cout, load.GetTestGraph());
      results = dump.Execute();
      OutputResultMsgs(results);

      AAFUnload();
    }
    else
      std::cout << "Failed to load library!" << std::endl;
  }
  catch ( const AxExHResult& ex )
  {
    std::wcout << L"Error: " << ex.what() << std::endl;
  }
  catch ( ... )
  {
    std::cout << "Error: unhandled exeption" << std::endl;
  }
  std::cout << "Completed Graph Testing!" << std::endl << std::endl;

  return 0;
}

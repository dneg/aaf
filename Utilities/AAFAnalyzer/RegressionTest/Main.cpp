//project files
#include "LoadPhase.h"
#include "DumpPhase.h"

//Ax files
#include <AxEx.h>

using namespace aafanalyzer;

int main()
{
  try
  {
    const std::basic_string<wchar_t> FileName = L"test.aaf";  

    if(AAFLoad(getenv("AX_AAF_COMAPI")) == 0)
    {
      LoadPhase load(std::cout, FileName); 
      DumpPhase dump(std::cout, load.GetTestGraph());
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

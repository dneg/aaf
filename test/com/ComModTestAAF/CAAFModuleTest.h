#ifndef __CAAFModuleTest_h__
#define __CAAFModuleTest_h__

#include "AAFTypes.h"


class CAAFModuleTest
{
public:
	CAAFModuleTest();
	~CAAFModuleTest();
    

  //
	// Call  Module test functions.
	//
  HRESULT Test(unsigned char *pClassName = NULL);

};


#endif // __CAAFModuleTest_h__

// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "CAAFEssenceFormat.h"
#include "CAAFEssenceFormat.h"
#ifndef __CAAFEssenceFormat_h__
#error - improperly defined include guard
#endif


#include <iostream.h>

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFEssenceFormat; // generated


HRESULT CAAFEssenceFormat::test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  IAAFEssenceFormat *pObject = NULL;

  try
    {
      // Attempt to create an AAFEssenceFormat.
      hr =  CoCreateInstance(
                             CLSID_AAFEssenceFormat,
                             NULL, 
                             CLSCTX_INPROC_SERVER, 
                             IID_IAAFEssenceFormat, (void **)&pObject);
      if (FAILED(hr))
        {
          cerr << "CAAFEssenceFormat::test...FAILED!";
          cerr << hr;
          cerr << "\tCoCreateInstance(&CLSID_AAFEssenceFormat, NULL,"
            " CLSCTX_INPROC_SERVER, &IID_IAAFEssenceFormat, ...);" <<
              endl;
          return hr;
        }

      // module-specific tests go here

      if (pObject)
        pObject->Release();
      return AAFRESULT_NOT_IMPLEMENTED;

    }
  catch (...)
    {
      cerr << "CAAFEssenceFormat::test...Caught general C++"
        " exception!" << endl; 
    }

  // Cleanup our object if it exists.
  if (pObject)
    pObject->Release();

  return hr;
}



			//@comm The value data is passed in as a void * through the "value"
			// argument.  The size of the value must be passed through the
			// valueSize argument.	


/****/

			//@comm The actual number of bytes read is returned in
			// bytesRead.  If the buffer is not big enough to return the entire
			// value, an error is returned.
/****/

/****/




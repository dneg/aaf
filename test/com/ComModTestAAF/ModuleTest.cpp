//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=
//
// File: ModuleTest.cpp
// 
// Implementation for types and utilities shared by two or more module
// tests.
//

#include "ModuleTest.h"
#include "AAFResult.h"


//
// Feature support
//

// DR1 -> {1, 0, 0, 1, kAAFVersionBeta};
// DR2 -> {1, 0, 0, 2, kAAFVersionBeta};
// DR3 -> {1, 0, 0, 3, kAAFVersionBeta};
// DR4 -> {1, 0, 0, 4, kAAFVersionBeta};


bool DR4TestSupported(aafProductVersion_constref toolkitVersion)
{
  bool result = true;
   
  if (kAAFVersionBeta == toolkitVersion.type)
  {
    if (1 == toolkitVersion.major && 
        0 == toolkitVersion.minor && 
        0 == toolkitVersion.tertiary && 
        3 >= toolkitVersion.patchLevel)
    {
      result = false;
    }
  }
  
  return result;

}


bool RC1TestSupported(aafProductVersion_constref toolkitVersion)
{
  bool result = true;
   
  if (kAAFVersionBeta == toolkitVersion.type)
  {
    if (1 == toolkitVersion.major && 
        0 == toolkitVersion.minor && 
        0 == toolkitVersion.tertiary && 
        4 >= toolkitVersion.patchLevel)
    {
      result = false;
    }
  }
  
  return result;

}



// Support for Sets (strong reference sets) added in DR4
bool StrongReferenceSetsSupported(aafProductVersion_constref toolkitVersion)
{
  return DR4TestSupported(toolkitVersion);
} 


// Support for weak references was added after DR4
bool WeakReferencesSupported(aafProductVersion_constref toolkitVersion)
{
  return RC1TestSupported(toolkitVersion);
} 


// Support for weak reference sets was added after DR4
bool WeakReferenceSetsSupported(aafProductVersion_constref toolkitVersion)
{
  return WeakReferencesSupported(toolkitVersion);
} 


// Support for extending AAFObject was added after DR4
bool ExtendingAAFObjectSupported(aafProductVersion_constref toolkitVersion)
{
  return RC1TestSupported(toolkitVersion);
} 


// Return the current version of the AAF toolkit shared library and
// the version of the toolkit used to create the file.
HRESULT GetAAFVersions(IAAFHeader * pHeader, 
                       aafProductVersion_t * currentAAFVersion, 
                       aafProductVersion_t * fileAAFVersion)
{
  HRESULT result = S_OK;
  if (NULL == pHeader || NULL == currentAAFVersion)
    return AAFRESULT_NULL_PARAM;
  
  result = pHeader->GetRefImplVersion(currentAAFVersion);
  if (SUCCEEDED(result) && NULL != fileAAFVersion)
  {
    IAAFIdentification * pFirstIdentification = NULL;
    result = pHeader->GetIdentificationAt(0, &pFirstIdentification);
    if (SUCCEEDED(result))
    {
      result = pFirstIdentification->GetRefImplVersion(fileAAFVersion);
      pFirstIdentification->Release();
    } 
  }
  
  return result;
}

/***********************************************************************
 *
 *              Copyright (c) 2001 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/
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

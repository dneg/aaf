//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
// Portions created by British Broadcasting Corporation are
// Copyright 2004, British Broadcasting Corporation.  All rights reserved.
//
//=---------------------------------------------------------------------=


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Include the AAF interface declarations.
#include "AAF.h"
#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFFileKinds.h"
#include "AAFSmartPointer.h"
#include "AAFFileMode.h"
#include "AAFPropertyDefs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFDataDefs.h"

typedef IAAFSmartPointer<IAAFClassDef> IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFDefObject> IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFDictionary> IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFMetaDefinition> IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFObject> IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef> IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue> IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef> IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefInt> IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IEnumAAFDataDefs> IEnumAAFDataDefsSP;
typedef IAAFSmartPointer<IEnumAAFTypeDefs> IEnumAAFTypeDefsSP;

static void     FatalErrorCode(HRESULT errcode, int line, char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
  // we don't need to exit on failure
  //exit(1); 
}

static HRESULT moduleErrorTmp = S_OK;/* note usage in macro */
#define check(a)  \
{  moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     FatalErrorCode(moduleErrorTmp, __LINE__, __FILE__);\
}

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}


static void convert(wchar_t* wcName, size_t length, const char* name)
{
  assert((name /* && *name */), "Valid input name");
  assert(wcName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");
  
  size_t status = mbstowcs(wcName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, "Error : Failed to convert'%s' to a wide character string.\n\n", name);
    exit(1);  
  }
}

static void convert(char* cName, size_t length, const wchar_t* name)
{
  assert((name /* && *name */), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, ": Error : Conversion failed.\n\n");
    exit(1);  
  }
}

static void convert(char* cName, size_t length, const char* name)
{
  assert((name /* && *name */), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t sourceLength = strlen(name);
  if (sourceLength < length - 1) {
    strncpy(cName, name, length);
  } else {
    fprintf(stderr, "Error : Failed to copy '%s'.\n\n", name);
    exit(1);  
  }
}

static void convert(wchar_t* wName, size_t length, const wchar_t* name)
{
  assert((name /* && *name */), "Valid input name");
  assert(wName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t sourceLength = 0;
  while (*name)
    ++sourceLength;
  if (sourceLength < length - 1) {
    // Copy the string if there is enough room in the destinition buffer.
    while (*wName++ = *name++)
      ;
  } else {
    fprintf(stderr, "Error : Failed to copy '%s'.\n\n", name);
    exit(1);  
  }
}

static void printProductVersion(aafProductVersion_t* pProductVersion)
{
  printf("%d.%d.%d-%d", pProductVersion->major,
                        pProductVersion->minor,
                        pProductVersion->tertiary,
                        pProductVersion->patchLevel);
  char* releaseType;
  switch (pProductVersion->type) {
    case kAAFVersionUnknown:
      releaseType = "Unknown";
      break;
    case kAAFVersionReleased:
      releaseType = "Released";
      break;
    case kAAFVersionDebug:
      releaseType = "Debug";
      break;
    case kAAFVersionPatched:
      releaseType = "Patched";
      break;
    case kAAFVersionBeta:
      releaseType = "Beta";
      break;
    case kAAFVersionPrivateBuild:
      releaseType = "PrivateBuild";
      break;
    default:
      releaseType = "Not Recognized";
      break;
  }
  printf(" (%s)", releaseType);
}

#if defined( OS_UNIX )

aafUInt8 HostByteOrder(void)
{
  aafUInt16 word = 0x1234;
  aafUInt8 byte = *((aafUInt8*)&word);
  aafUInt8 result;

  if (byte == 0x12) {
    result = 'B';
  } else {
    result = 'L';
  }
  return result;
}

static const unsigned char idMapLittle[] =
{ 3, 2, 1, 0, '-', 5, 4, '-', 7, 6, '-', 8, 9, '-', 10, 11, 12, 13, 14, 15 }; 
static const unsigned char idMapBig[] =
{ 0, 1, 2, 3, '-', 4, 5, '-', 6, 7, '-', 8, 9, '-', 10, 11, 12, 13, 14, 15 }; 
static const unsigned char* guidMap;
static const wchar_t digits[] = L"0123456789ABCDEF"; 

#define GUIDSTRMAX 38 

typedef OLECHAR OMCHAR;

int StringFromGUID2(const GUID& guid, OMCHAR* buffer, int bufferSize) 
{
  if (HostByteOrder() == 'L') {
    guidMap = &idMapLittle[0];
  } else {
    guidMap = &idMapBig[0];
  }
  const unsigned char* ip = (const unsigned char*) &guid; // input pointer
  OMCHAR* op = buffer;                                    // output pointer

  *op++ = L'{'; 
 
  for (size_t i = 0; i < sizeof(idMapLittle); i++) { 

    if (guidMap[i] == '-') { 
      *op++ = L'-'; 
    } else { 
      *op++ = digits[ (ip[guidMap[i]] & 0xF0) >> 4 ]; 
      *op++ = digits[ (ip[guidMap[i]] & 0x0F) ]; 
    } 
  } 
  *op++ = L'}'; 
  *op = L'\0'; 
 
  return GUIDSTRMAX; 
} 

#endif

// The maximum number of characters in the formated GUID.
// (as returned by StringFromGUID2).
const size_t MAX_GUID_BUFFER = 40;

static void formatGUID(char *cBuffer, size_t length, aafUID_t *pGUID)
{
  assert(pGUID, "Valid input GUID");
  assert(cBuffer != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  int bytesCopied = 0;
  OLECHAR wGUID[MAX_GUID_BUFFER];

  bytesCopied = StringFromGUID2(*((GUID*)pGUID), wGUID, MAX_GUID_BUFFER);
  if (0 < bytesCopied) {
    convert(cBuffer, length, wGUID);
  } else {
    fprintf(stderr, "\nError : formatGUID failed.\n\n");
    exit(1);  
  }
}

static void printDate(aafDateStruct_t *pDate)
{
  printf("%04d-%02d-%02d", pDate->year, pDate->month, pDate->day);
}

static void printTime(aafTimeStruct_t *pTime)
{
  printf("%02d:%02d:%02d.%02d", pTime->hour, pTime->minute, pTime->second, pTime->fraction);
}

static void printTimeStamp(aafTimeStamp_t *pTimeStamp)
{
  printDate(&pTimeStamp->date);
  printf(" ");
  printTime(&pTimeStamp->time);
}

static void printIdentification(IAAFIdentification* pIdent)
{
  aafWChar wchName[500];
  char chName[1000];
    
    
  check(pIdent->GetCompanyName(wchName, sizeof (wchName)));
  convert(chName, sizeof(chName), wchName);
  printf("CompanyName          = \"%s\"\n", chName);

  check(pIdent->GetProductName(wchName, sizeof (wchName)));
  convert(chName, sizeof(chName), wchName);
  printf("ProductName          = \"%s\"\n", chName);

  check(pIdent->GetProductVersionString(wchName, sizeof (wchName)));
  convert(chName, sizeof(chName), wchName);
  printf("ProductVersionString = \"%s\"\n", chName);

  aafProductVersion_t version;
  check(pIdent->GetProductVersion(&version));
  printf("ProductVersion       = ");
  printProductVersion(&version);
  printf("\n");

  aafUID_t productID;
  check(pIdent->GetProductID(&productID));
  formatGUID(chName, sizeof(chName), &productID);
  printf("ProductID            = %s\n", chName);

  aafTimeStamp_t timeStamp;
  check(pIdent->GetDate(&timeStamp));
  printf("Date                 = ");
  printTimeStamp(&timeStamp);
  printf("\n");

  check(pIdent->GetRefImplVersion(&version));
  printf("RefImplVersion       = ");
  printProductVersion(&version);
  printf("\n");

  check(pIdent->GetPlatform(wchName, sizeof (wchName)));
  convert(chName, sizeof(chName), wchName);
  printf("Platform             = \"%s\"\n", chName);

  aafUID_t generationID;
  check(pIdent->GetGenerationID(&generationID));
  formatGUID(chName, sizeof(chName), &generationID);
  printf("GenerationID         = %s\n", chName);
}


static void ReadAAFFile(aafWChar * pFileName)
{
  HRESULT hr = S_OK;
  IAAFFile * pFile = NULL;

  hr = AAFFileOpenExistingRead (pFileName, AAF_FILE_MODE_LAZY_LOADING, &pFile);
  if (SUCCEEDED(hr))
  {
    IAAFHeader * pHeader = NULL;

    hr = pFile->GetHeader(&pHeader);
    check(hr); // display error message
    if (SUCCEEDED(hr))
    {
      IAAFIdentification *    pIdent = NULL;

      hr = pHeader->GetLastIdentification(&pIdent);
      check(hr); // display error message
      if (SUCCEEDED(hr))
      {
        printIdentification(pIdent);

        pIdent->Release();
        pIdent = NULL;

        // count Mobs
        aafNumSlots_t n;
        hr = pHeader->CountMobs(kAAFAllMob, &n);
        check(hr);
        printf("\nNumber of Mobs       = %d\n", n);

        // Header::Version, Header::ObjectModelVersion
        aafVersionType_t version = {0};
        check(pHeader->GetFileRevision (&version) );
        printf("\nHeader::Version      = %d.%d\n", version.major, version.minor);

        // Public API doesn't have a method to access Header::ObjectModelVersion,
        // so get it with property direct.
        printf("Header::ObjectModelVersion = ");
        IAAFObjectSP pObj;
        IAAFClassDefSP pClassDef;
        IAAFPropertyDefSP pPropertyDef;
        IAAFPropertyValueSP pPropValue;
        IAAFTypeDefSP pObjVerTypeDef;
        IAAFTypeDefIntSP pTypeDefInt;
        aafUInt32 val;
        check(pHeader->QueryInterface(IID_IAAFObject, (void **)&pObj));
        check(pObj->GetDefinition (&pClassDef));
        check(pClassDef->LookupPropertyDef(kAAFPropID_Header_ObjectModelVersion, &pPropertyDef));
        aafBoolean_t present;
        check(pObj->IsPropertyPresent(pPropertyDef, &present));
        aafUInt32 objectModelVersion = 0;
        if (present)
        {
          check(pObj->GetPropertyValue (pPropertyDef, &pPropValue));
          check(pPropValue->GetType(&pObjVerTypeDef));
          check(pObjVerTypeDef->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTypeDefInt));
          check(pTypeDefInt->GetInteger(pPropValue, (aafMemPtr_t) &val, sizeof (val)));
          objectModelVersion = val;
          printf("%d\n", objectModelVersion);
        }
        else
          printf("0 (implied by property not being present)\n");

        // Show datadefs, with version number
        IEnumAAFDataDefsSP pEnumDataDef;
        IAAFDictionarySP pDictionary;
        check(pHeader->GetDictionary(&pDictionary));
        check(pDictionary->GetDataDefs(&pEnumDataDef));
        IAAFDataDef* pDataDef;

        printf("\nDatadefs             = ");
        while (SUCCEEDED(pEnumDataDef->NextOne(&pDataDef)))
        {
          IAAFDefObjectSP pDefObject;
          check(pDataDef->QueryInterface(IID_IAAFDefObject, (void**)&pDefObject));
          pDataDef->Release();
          pDataDef = NULL;

          aafUID_t id = {0};
          check(pDefObject->GetAUID(&id));

          aafWChar wchName[500];
          char chName[1000];
          check( pDefObject->GetName(wchName, sizeof (wchName)) );
          convert(chName, sizeof(chName), wchName);

          if (memcmp( &id, &DDEF_Picture, sizeof(id)) == 0)
            printf("\"%s\" (recognized as LegacyPicture)\n", chName);
          else if (memcmp( &id, &kAAFDataDef_Picture, sizeof(id)) == 0)
            printf("\"%s\" (recognized as Picture)\n", chName);
          else if (memcmp( &id, &DDEF_Sound, sizeof(id)) == 0)
            printf("\"%s\" (recognized as LegacySound)\n", chName);
          else if (memcmp( &id, &kAAFDataDef_Sound, sizeof(id)) == 0)
            printf("\"%s\" (recognized as Sound)\n", chName);
          else if (memcmp( &id, &DDEF_Timecode, sizeof(id)) == 0)
            printf("\"%s\" (recognized as LegacyTimecode)\n", chName);
          else if (memcmp( &id, &kAAFDataDef_Timecode, sizeof(id)) == 0)
            printf("\"%s\" (recognized as Timecode)\n", chName);
          else if (memcmp( &id, &kAAFDataDef_PictureWithMatte, sizeof(id)) == 0)
            printf("\"%s\" (recognized as PictureWithMatte)\n", chName);
          else if (memcmp( &id, &kAAFDataDef_Edgecode, sizeof(id)) == 0)
            printf("\"%s\" (recognized as Edgecode)\n", chName);
          else if (memcmp( &id, &kAAFDataDef_Auxiliary, sizeof(id)) == 0)
            printf("\"%s\" (recognized as Auxiliary)\n", chName);
          else if (memcmp( &id, &kAAFDataDef_Descriptive, sizeof(id)) == 0)
            printf("\"%s\" (recognized as Descriptive)\n", chName);
          else if (memcmp( &id, &kAAFDataDef_Matte, sizeof(id)) == 0)
            printf("\"%s\" (recognized as Matte)\n", chName);
          else
            printf("\"%s\"\n", chName);
          printf("                       ");
        }

        // Check if file contains TypeDefs known to cause a v1.0 reader to assert.
        // Known instances of this are UInt32Set and AUIDSet added to the v1.1 SDK.
        // Cannot use Dictionary::LookupTypeDef to check for them, because this
        // has the side effect of registering the typedef we are checking for
        // from the built-in model. Instead, iterate through typedefs (in file)
        // and check for the known instances.
        printf("\nTypes incompatible with SDK v1.0.x =");
        IEnumAAFTypeDefsSP pEnumTypeDef;
        check(pDictionary->GetTypeDefs(&pEnumTypeDef));
        IAAFTypeDef* pTypeDef;
        bool foundToxic = false;
        while (SUCCEEDED(pEnumTypeDef->NextOne(&pTypeDef)))
        {
          IAAFMetaDefinitionSP pMetaDef;
          check(pTypeDef->QueryInterface(IID_IAAFMetaDefinition, (void**)&pMetaDef));
          pTypeDef->Release();
          pTypeDef = NULL;

          aafUID_t typeUID;
          check(pMetaDef->GetAUID(&typeUID));

          aafWChar wchName[500];
          char chName[1000];
          check( pMetaDef->GetName(wchName, sizeof (wchName)) );
          convert(chName, sizeof(chName), wchName);

          if ((memcmp( &typeUID, &kAAFTypeID_AUIDSet, sizeof(typeUID)) == 0)
            || (memcmp( &typeUID, &kAAFTypeID_UInt32Set, sizeof(typeUID)) == 0))
          {
            printf(" %s", chName);
            foundToxic = true;
          }
        }
        if (!foundToxic)
          printf(" (none)");
        printf("\n\n");

      }
      pHeader->Release();
      pHeader = NULL;
    }

    hr = pFile->Close();
    check(hr);

    pFile->Release();
    pFile = NULL;

    // Get file kind.
    // Since AAF SDK v1.0.2, the file kind actually identifies the implementation
    // of the file kind, from which the file kind is inferred.
    aafUID_t fileKind = {0};
    aafBool isAAFFile = kAAFFalse;

    check(AAFFileIsAAFFile(pFileName, &fileKind, &isAAFFile));
    if (isAAFFile)
    {
      if (memcmp( &fileKind, &kAAFFileKind_AafM512Binary, sizeof(fileKind)) == 0)
        printf("Filekind             = 512-byte SS (reading with Microsoft)\n");
      else if (memcmp( &fileKind, &kAAFFileKind_AafS512Binary, sizeof(fileKind)) == 0)
        printf("Filekind             = 512-byte SS (reading with Schemasoft)\n");
      else if (memcmp( &fileKind, &kAAFFileKind_AafG512Binary, sizeof(fileKind)) == 0)
        printf("Filekind             = 512-byte SS (reading with GSF)\n");
      else if (memcmp( &fileKind, &kAAFFileKind_AafM4KBinary, sizeof(fileKind)) == 0)
        printf("Filekind             = 4096-byte SS (reading with Microsoft)\n");
      else if (memcmp( &fileKind, &kAAFFileKind_AafS4KBinary, sizeof(fileKind)) == 0)
        printf("Filekind             = 4096-byte SS (reading with Schemasoft)\n");
      else if (memcmp( &fileKind, &kAAFFileKind_AafG4KBinary, sizeof(fileKind)) == 0)
        printf("Filekind             = 4096-byte SS (reading with GSF)\n");
      else
        printf("Filekind             = Recognized by SDK but unknown to AAFInfo\n");
    }
  }
  else
  {
    fprintf(stderr, "Error : Failed to open file (result = %0x).\n", hr);
  }
}

int main(int argumentCount, char* argumentVector[])
{
  if (argumentCount != 2) {
    fprintf(stderr, "Error : wrong number of arguments\n");
    fprintf(stderr, "Usage : AAFInfo <file>\n");
    return(1);
  }

  char* inputFileName = argumentVector[1];

  wchar_t wInputFileName[256];
  convert(wInputFileName, 256, inputFileName);

  HRESULT hr = AAFLoad(0);
  if (!AAFRESULT_SUCCEEDED(hr))
  {
    fprintf(stderr, "Error : Failed to load the AAF library, ");
    fprintf(stderr, "check environment variables -\n");
    fprintf(stderr, "  Windows    - $PATH\n");
    fprintf(stderr, "  Unix/Linux - $LD_LIBRARY_PATH\n");
    exit(hr);
  }

  ReadAAFFile(wInputFileName);

  AAFUnload();

  return(0);
}


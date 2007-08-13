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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "XMLStoredFormatTest.h"


struct CAAFInitialize
{
    CAAFInitialize(const char *dllname = NULL)
    {
        HRESULT hr = AAFLoad(dllname);
        if (!AAFRESULT_SUCCEEDED(hr))
        {
            fprintf(stderr, "Error : Failed to load the AAF library, ");
            fprintf(stderr, "check environment variables -\n");
            fprintf(stderr, "  Windows    - $PATH\n");
            fprintf(stderr, "  Unix/Linux - $LD_LIBRARY_PATH\n");
            exit(hr);
        }
    }

    ~CAAFInitialize()
    {
        AAFUnload();
    }
};

void report(bool passed)
{
    if (passed)
    {
        printf("--> passed\n");
    }
    else
    {
        printf("--> FAILED\n");
    }
}

IAAFFile* openFileForReading(const aafCharacter* fileName)
{
    IAAFFile* file = 0;
    checkResult(AAFFileOpenExistingRead(fileName, 0, &file));
    return file;
}

void replaceCharacter(wchar_t* str, wchar_t c, wchar_t r)
{
    wchar_t* tmp = wcschr(str, c);
    *tmp = r;
}

static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"XMLStoredFormatTest";

void saveCopy(const aafCharacter* inFileName, const aafCharacter* outFileName)
{
    char tmp[256];
    wcstombs(tmp, outFileName, 256);
    remove(tmp);

    IAAFFile* inFile = 0;
    IAAFFile* outFile = 0;

    checkResult(AAFFileOpenExistingRead(inFileName, 0, &inFile));

    aafProductVersion_t v;
    v.major = 0;
    v.minor = 1;
    v.tertiary = 0;
    v.patchLevel = 0;
    v.type = kAAFVersionUnknown;

    const aafUID_t productUID = 
        {0xd3ec8680, 0x1e79, 0x48f9, {0x99, 0x41, 0x0a, 0x10, 0xa1, 0x68, 0x86, 0xaf}};
    aafProductIdentification_t  productInfo;
    productInfo.companyName = companyName;
    productInfo.productName = productName;
    productInfo.productVersion = &v;
    productInfo.productVersionString = 0;
    productInfo.productID = productUID;
    productInfo.platform = 0;

    checkResult(AAFFileOpenNewModifyEx(outFileName, &kAAFFileKind_AafXmlText, 0, &productInfo, &outFile));
    checkResult(inFile->SaveCopyAs(outFile));
    
    checkResult(outFile->Close());
    release(outFile);

    checkResult(inFile->Close());
    release(inFile);
}

void copyAndTouch(const aafCharacter* inFileName, const aafCharacter* outFileName)
{
    char tmp[256];
    wcstombs(tmp, outFileName, 256);
    remove(tmp);
    
    char copyCmd[512];
#if defined (_WIN32)
    strcpy(copyCmd, "copy ");
#else
    strcpy(copyCmd, "cp ");
#endif    
    wcstombs(tmp, inFileName, 256);
    strcat(copyCmd, tmp);
    wcstombs(tmp, outFileName, 256);
    strcat(copyCmd, " ");
    strcat(copyCmd, tmp);
#if defined (_WIN32)
    // replace '/' separator with '\' for benefit of copy command
    char* sep = copyCmd;
    while ((sep = strchr(sep, '/')) != NULL)
    {
        *sep = '\\';
        sep++;
    }
#endif
    int result = system(copyCmd);
    if (result != 0)
    {
        fprintf(stderr, "\nSystem copy command returned '%0x', at line %d in %s\n", result,
            __LINE__, __FILE__);
        throw "System copy command failed";
    }

    
    IAAFFile* outFile = 0;

    aafProductVersion_t v;
    v.major = 0;
    v.minor = 1;
    v.tertiary = 0;
    v.patchLevel = 0;
    v.type = kAAFVersionUnknown;

    const aafUID_t productUID = 
        {0xd3ec8680, 0x1e79, 0x48f9, {0x99, 0x41, 0x0a, 0x10, 0xa1, 0x68, 0x86, 0xaf}};
    aafProductIdentification_t  productInfo;
    productInfo.companyName = companyName;
    productInfo.productName = productName;
    productInfo.productVersion = &v;
    productInfo.productVersionString = 0;
    productInfo.productID = productUID;
    productInfo.platform = 0;

    checkResult(AAFFileOpenExistingModify(outFileName, 0, &productInfo, &outFile));
    checkResult(outFile->Save());

    checkResult(outFile->Close());
    release(outFile);
}


//
// NOTE: this diff breaks very easily !!
//
bool diff(const aafCharacter* fileNameA, const aafCharacter* fileNameB)
{
    char tmp[256];
    wcstombs(tmp, fileNameA, 256);
    FILE* fileA = fopen(tmp, "r");
    if (fileA == 0)
    {
        return true;
    }

    wcstombs(tmp, fileNameB, 256);
    FILE* fileB = fopen(tmp, "r");
    if (fileB == 0)
    {
        fclose(fileA);
        return true;
    }
    
    char* bufferA = 0;
    char* bufferB = 0;
    bool diff = false;
    size_t checkSize;
    
    bufferA = new char[1048576];
    bufferB = new char[1048576];

    
    // read into memory
    size_t numReadA = fread(bufferA, 1, 1048575, fileA);
    bufferA[numReadA] = '\0';
    size_t numReadB = fread(bufferB, 1, 1048575, fileB);
    bufferB[numReadB] = '\0';
    if (numReadA == 1048575 || numReadB == 1048575)
    {
        delete [] bufferA;
        delete [] bufferB;
        throw "Diff buffer too small - test update required";
    }

    checkSize = numReadA;
    
    // blank out the LastModified
    char* lastModA = strstr(bufferA, "<LastModified>");
    char* lastModEndA = strstr(bufferA, "</LastModified>");
    char* lastModB = strstr(bufferB, "<LastModified>");
    char* lastModEndB = strstr(bufferB, "</LastModified>");
    if ((lastModA == 0) != (lastModB == 0) || 
        (lastModEndA == 0) != (lastModEndB == 0))
    {
        diff = true;
    }
    
    // replace last modified timestamp with spaces
    if (lastModA != 0)
    {
        char* timestamp = lastModA + strlen("<LastModified>");
        while (timestamp != lastModEndA)
        {
            *timestamp = ' ';
            timestamp++;
        }
        timestamp = lastModB + strlen("<LastModified>");
        while (timestamp != lastModEndB)
        {
            *timestamp = ' ';
            timestamp++;
        }
    }

    // check after <AAF    
    char* rootA = strstr(bufferA, "<AAF");
    char* rootB = strstr(bufferB, "<AAF");
    if (rootA == 0 || rootB == 0)
    {
        delete [] bufferA;
        delete [] bufferB;
        throw "No root element found";
    }
    
    // don't check after the identification list
    char* idListA = strstr(bufferA, "<IdentificationList>");
    char* idListB = strstr(bufferB, "<IdentificationList>");
    if (idListA == 0 || idListB == 0)
    {
        diff = true;
    }
    
    checkSize = idListA - rootA;
    if ((size_t)(idListB - rootB) < checkSize)
    {
        checkSize = idListB - rootB;
    }
    
    diff = diff || memcmp(rootA, rootB, checkSize) != 0;

    
    delete [] bufferA;
    delete [] bufferB;
    
    fclose(fileA);
    fclose(fileB);
    
    return diff;
}



int main(int argc, char *argv[])
{
    CAAFInitialize aafInit;

    int result[15];
    memset(result, 0, sizeof(result));
    const char* tests[15] = 
    {
        "Character ",
        "Enum      ",
        "ExtEnum   ",
        "FixedArray",
        "Indirect  ",
        "Integer   ",
        "Opaque    ",
        "Record    ",
        "Rename    ",
        "Set       ",
        "Stream    ",
        "String    ",
        "VarArray  ",
        "WeakRef   ",
        "Examples  "
    };
        
    printf("\n");
    printf("**************************\n");
    printf("* XML Stored Format Test *\n");
    printf("**************************\n");
    printf("\n");
    
    result[0] = testCharacter();
    result[1] = testEnum();
    result[2] = testExtEnum();
    result[3] = testFixedArray();
    result[4] = testIndirect();
    result[5] = testInteger();
    result[6] = testOpaque();
    result[7] = testRecord();
    result[8] = testRename();
    result[9] = testSet();
    result[10] = testStream();
    result[11] = testString();
    result[12] = testVarArray();
    result[13] = testWeakRef();
    result[14] = testExampleFiles();

    printf("\nResults summary:\n");
    bool passed = true;
    for (unsigned int i = 0; i < sizeof(tests) / sizeof(char*); i++)
    {
        printf("  %s", tests[i]);
        if (result[i] == 0)
        {
            printf("\t\tpassed\n");
        }
        else if (result[i] == 1)
        {
            printf("\t\tpassed (with known FAILURES)\n");
        }
        else
        {
            printf("\t\tFAILED\n");
            passed = false;
        }
    }
    
    return passed ? 0 : 1;
}


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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>

#include "AAF.h"
#include "AAFResult.h"

#include "CAAFBuiltinDefs.h"
#include "AAFDefUIDs.h"
#include "ModuleTest.h"

using namespace std;

struct CAAFSoundDescriptorTestFixture
{
    aafMobID_t                      sourceMobID;
    aafUID_t                        compressionID;
    aafUInt32                       channelCount;
    aafRational_t                   samplingRate;
    aafBoolean_t                    isLocked;
    aafElectroSpatialFormulation_t  esf;
    aafInt8                         audioRefLevel;
    aafInt8                         dialNorm;
    aafUInt32                       quantizationBitCount;
    aafUInt16                       blockAlign;
};

static const CAAFSoundDescriptorTestFixture  gTestData = 
{
    { { 0x06, 0x0c, 0x2b, 0x34,
        0x02, 0x05, 0x11, 0x01,
        0x01, 0x00, 0x10, 0x00 },
      0x13, 0x00, 0x00, 0x00,
      { 0xf6d43ded, 0xa5aa, 0x4a05,
      {  0xa2, 0x84, 0x37, 0x8f,
        0x94, 0x16, 0xd0, 0x56 } } },

    { 0x77343294, 0x39ea, 0x4751,
      { 0xaf, 0x38, 0xcf, 0x8a,
        0x3c, 0xc4, 0xba, 0x10 } },

    4,
    { 48000, 1 },
    kAAFTrue,
    kAAFElectroSpatialFormulation_StereophonicMode,
    40,
    -25,
    256,
    4096
};



// Convenient error handlers.
inline void checkResult(HRESULT r)
{
    if (FAILED(r))
        throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
    if (!expression)
        throw r;
}



// Required function prototype.
extern "C" HRESULT CAAFSoundDescriptor_test(testMode_t mode);

static HRESULT OpenAAFFile(
    aafWChar*           pFileName,
    aafMediaOpenMode_t  mode,
    IAAFFile**          ppFile,
    IAAFHeader**        ppHeader);

static HRESULT CreateAAFFile(
    aafWChar * pFileName);

static HRESULT ReadAAFFile(
    aafWChar * pFileName);

static HRESULT Test_IAAFSoundDescriptor( IAAFSoundDescriptor*, testMode_t );

static HRESULT Test_IAAFSoundDescriptor_Compression(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode );
static HRESULT Test_IAAFSoundDescriptor_Channels(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode );
static HRESULT Test_IAAFSoundDescriptor_AudioSamplingRate(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode );
static HRESULT Test_IAAFSoundDescriptor_Locked(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode );
static HRESULT Test_IAAFSoundDescriptor_ElectroSpatial(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode );
static HRESULT Test_IAAFSoundDescriptor_AudioRefLevel(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode );
static HRESULT Test_IAAFSoundDescriptor_DialNorm(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode );
static HRESULT Test_IAAFSoundDescriptor_QuantizationBits(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode );

bool operator ==( const aafUID_t uid1, const aafUID_t uid2 );
bool operator !=( const aafUID_t uid1, const aafUID_t uid2 );





//
// The public entry for this module test,
//
HRESULT CAAFSoundDescriptor_test(
    testMode_t mode )
{
    HRESULT  hr = AAFRESULT_NOT_IMPLEMENTED;
    aafWChar* pFileName = L"AAFSoundDescriptorTest.aaf";


    try
    {
        if(mode == kAAFUnitTestReadWrite)
            hr = CreateAAFFile(pFileName);
        else
            hr = AAFRESULT_SUCCESS;
        if(hr == AAFRESULT_SUCCESS)
            hr = ReadAAFFile( pFileName );
    }
    catch (...)
    {
        cerr << "CAAFSoundDescriptor_test...Caught general C++"
             << " exception!" << endl; 
        hr = AAFRESULT_TEST_FAILED;
    }


    return hr;
}


static HRESULT OpenAAFFile(
    aafWChar*           pFileName,
    aafMediaOpenMode_t  mode,
    IAAFFile**          ppFile,
    IAAFHeader**        ppHeader)
{
    aafProductIdentification_t    ProductInfo;
    HRESULT                        hr = AAFRESULT_SUCCESS;

    aafProductVersion_t v;
    v.major = 1;
    v.minor = 0;
    v.tertiary = 0;
    v.patchLevel = 0;
    v.type = kAAFVersionUnknown;
    ProductInfo.companyName = L"AAF Developers Desk";
    ProductInfo.productName = L"AAFSoundDescriptor Test";
    ProductInfo.productVersion = &v;
    ProductInfo.productVersionString = NULL;
    ProductInfo.productID = UnitTestProductID;
    ProductInfo.platform = NULL;

    *ppFile = NULL;

    switch (mode)
    {
        case kAAFMediaOpenReadOnly:
            hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
            break;

        case kAAFMediaOpenAppend:
            hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
            break;

        default:
            hr = AAFRESULT_TEST_FAILED;
            break;
    }

    if (FAILED(hr))
    {
        if (*ppFile)
        {
            (*ppFile)->Release();
            *ppFile = NULL;
        }
        return hr;
    }
  
    hr = (*ppFile)->GetHeader(ppHeader);
    if (FAILED(hr))
    {
        (*ppFile)->Release();
        *ppFile = NULL;
        return hr;
    }
     
    return hr;
}



static HRESULT CreateAAFFile(
    aafWChar * pFileName)
{
    IAAFFile*               pFile = 0;
    IAAFHeader*             pHeader = 0;
    IAAFDictionary*         pDictionary = 0;
    IAAFSourceMob*          pSourceMob = 0;
    IAAFMob*                pMob = 0;
    IAAFEssenceDescriptor*  pEssenceDesc = 0;
    IAAFSoundDescriptor*    pSoundDesc = 0;

    HRESULT hr = S_OK;

    try
    {
        // Remove the previous test file if any.
        RemoveTestFile(pFileName);


        // Create the test file
        checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));


        // Get the AAF Dictionary
        checkResult(pHeader->GetDictionary(&pDictionary));
        CAAFBuiltinDefs  defs (pDictionary);


        // Create a mob, set its ID
        checkResult(defs.cdSourceMob()->CreateInstance( IID_IAAFSourceMob,
                                                        (IUnknown **)&pSourceMob) );
        checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));

        checkResult(pMob->SetMobID(gTestData.sourceMobID));
        checkResult(pMob->SetName(L"SoundDescriptorTest"));


        // Create a SoundDescriptor
        checkResult(defs.cdSoundDescriptor()->CreateInstance(
            IID_IAAFSoundDescriptor,
            (IUnknown **)&pSoundDesc));


        // Test SoundDescriptor.
        checkResult(Test_IAAFSoundDescriptor(pSoundDesc,
                                             kAAFUnitTestReadWrite));


        // Attach the descriptor to the source mob and
        // then attach the source mob to the file.
        checkResult(pSoundDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **) &pEssenceDesc));
        checkResult(pSourceMob->SetEssenceDescriptor(pEssenceDesc));

        checkResult(pHeader->AddMob(pMob));


        // Save the file
        checkResult(pFile->Save());
    }
    catch (HRESULT& rResult)
    {
        hr = rResult;
    }


    if (pSoundDesc) 
    {
        pSoundDesc->Release();
    }
    if (pEssenceDesc) 
    {
        pEssenceDesc->Release();
    }
    if (pMob) 
    {
        pMob->Release();
    }
    if (pSourceMob) 
    {
        pSourceMob->Release();
    }
    if (pDictionary) 
    {
        pDictionary->Release();
    }
    if (pHeader) 
    {
        pHeader->Release();
    }
    if (pFile) 
    {
        pFile->Close();
        pFile->Release();
    }

    return hr;
}



static HRESULT ReadAAFFile(
    aafWChar * pFileName )
{
    HRESULT                 hr = S_OK;
    IAAFFile*               pFile = 0;
    IAAFHeader*             pHeader = 0;
    IAAFMob*                pMob = 0;
    IAAFSourceMob*          pSourceMob = 0;
    IAAFEssenceDescriptor*  pEssenceDesc = 0;
    IAAFSoundDescriptor*    pSoundDesc = 0;

    try
    {
        // Open the test file
        checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
        checkResult(pFile->GetHeader(&pHeader));

        // Get the source mob
        checkResult(pHeader->LookupMob(gTestData.sourceMobID, &pMob));
        checkResult(pMob->QueryInterface(IID_IAAFSourceMob,
                                         (void **)&pSourceMob));

        // Get the sound descriptor
        checkResult(pSourceMob->GetEssenceDescriptor(&pEssenceDesc));
        checkResult(pEssenceDesc->QueryInterface(IID_IAAFSoundDescriptor,
                                                 (void **)&pSoundDesc));


        checkResult(Test_IAAFSoundDescriptor(pSoundDesc,
                                             kAAFUnitTestReadOnly));
    }
    catch (HRESULT& rResult)
    {
        hr = rResult;
    }


    if (pSoundDesc) 
    {
        pSoundDesc->Release();
    }
    if (pEssenceDesc) 
    {
        pEssenceDesc->Release();
    }
    if (pSourceMob) 
    {
        pSourceMob->Release();
    }
    if (pMob) 
    {
        pMob->Release();
    }
    if (pHeader) 
    {
        pHeader->Release();
    }
    if (pFile) 
    {
        pFile->Close();
        pFile->Release();
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        checkResult( Test_IAAFSoundDescriptor_Compression( pSoundDesc, mode ) );
        checkResult( Test_IAAFSoundDescriptor_Channels( pSoundDesc, mode ) );
        checkResult( Test_IAAFSoundDescriptor_AudioSamplingRate( pSoundDesc, mode ) );
        checkResult( Test_IAAFSoundDescriptor_Locked( pSoundDesc, mode ) );
        checkResult( Test_IAAFSoundDescriptor_ElectroSpatial( pSoundDesc, mode ) );
        checkResult( Test_IAAFSoundDescriptor_AudioRefLevel( pSoundDesc, mode ) );
        checkResult( Test_IAAFSoundDescriptor_DialNorm( pSoundDesc, mode ) );
        checkResult( Test_IAAFSoundDescriptor_QuantizationBits( pSoundDesc, mode ) );

        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_Compression(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUID_t bogusCompressionID =
            { 0xcc263b77, 0x875d, 0x4f23,
              { 0xac, 0x81, 0x87, 0xbd, 0xf4, 0x42, 0xca, 0x78 } };


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetCompression() should fail if the property not present.
            aafUID_t  compressionID = bogusCompressionID;
            hr = pSoundDesc->GetCompression( &compressionID );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( compressionID == bogusCompressionID,
                             AAFRESULT_TEST_FAILED );


            // GetCompression() should fail if function arguments are invalid.
            compressionID = bogusCompressionID;
            hr = pSoundDesc->GetCompression( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( compressionID == bogusCompressionID,
                             AAFRESULT_TEST_FAILED );


            // SetCompression() should always succeed
            hr = pSoundDesc->SetCompression( gTestData.compressionID );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetCompression() should succeed.
            aafUID_t  compressionID = bogusCompressionID;
            hr = pSoundDesc->GetCompression( &compressionID );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( compressionID == gTestData.compressionID,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_Channels(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        const aafUInt32 bogusChannelCount = 87;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetChannelCount() should succeed because Channels
            // is a required property.
            aafUInt32  channelCount = bogusChannelCount;
            hr = pSoundDesc->GetChannelCount( &channelCount );
            checkExpression( hr == AAFRESULT_SUCCESS,
                             AAFRESULT_TEST_FAILED );
            checkExpression( channelCount != bogusChannelCount,
                             AAFRESULT_TEST_FAILED );


            // GetChannelCount() should fail if function arguments are invalid.
            channelCount = bogusChannelCount;
            hr = pSoundDesc->GetChannelCount( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( channelCount == bogusChannelCount,
                             AAFRESULT_TEST_FAILED );


            // SetChannelCount() should always succeed
            hr = pSoundDesc->SetChannelCount( gTestData.channelCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetChannelCount() should succeed.
            aafUInt32  channelCount = bogusChannelCount;
            hr = pSoundDesc->GetChannelCount( &channelCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( channelCount == gTestData.channelCount,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_AudioSamplingRate(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        const aafRational_t bogusAudioSamplingRate = { 365, 15235 };


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetAudioSamplingRate() should succeed because AudioSamplingRate
            // is a required property.
            aafRational_t  audioSamplingRate = bogusAudioSamplingRate;
            hr = pSoundDesc->GetAudioSamplingRate( &audioSamplingRate );
            checkExpression( hr == AAFRESULT_SUCCESS,
                             AAFRESULT_TEST_FAILED );
            checkExpression( audioSamplingRate != bogusAudioSamplingRate,
                             AAFRESULT_TEST_FAILED );


            // GetAudioSamplingRate() should fail if function
            // arguments are invalid.
            audioSamplingRate = bogusAudioSamplingRate;
            hr = pSoundDesc->GetAudioSamplingRate( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( audioSamplingRate == bogusAudioSamplingRate,
                             AAFRESULT_TEST_FAILED );


            // SetAudioSamplingRate() should always succeed
            hr = pSoundDesc->SetAudioSamplingRate( gTestData.samplingRate );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetAudioSamplingRate() should succeed.
            aafRational_t  audioSamplingRate = bogusAudioSamplingRate;
            hr = pSoundDesc->GetAudioSamplingRate( &audioSamplingRate );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( audioSamplingRate == gTestData.samplingRate,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_Locked(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafBoolean_t bogusLockedFlag = kAAFFalse;


        if( mode == kAAFUnitTestReadWrite )
        {
            // IsLocked() should fail if the property not present.
            aafBoolean_t  locked = bogusLockedFlag;
            hr = pSoundDesc->IsLocked( &locked );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( locked == bogusLockedFlag,
                             AAFRESULT_TEST_FAILED );


            // IsLocked() should fail if function arguments are invalid.
            locked = bogusLockedFlag;
            hr = pSoundDesc->IsLocked( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( locked == bogusLockedFlag,
                             AAFRESULT_TEST_FAILED );


            // SetIsLocked() should always succeed
            hr = pSoundDesc->SetIsLocked( gTestData.isLocked );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // IsLocked() should succeed.
            aafBoolean_t  locked = bogusLockedFlag;
            hr = pSoundDesc->IsLocked( &locked );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( locked == gTestData.isLocked,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_ElectroSpatial(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafElectroSpatialFormulation_t bogusEsf =
            kAAFElectroSpatialFormulation_StereoRightChannelDoubleSamplingFrequencyMode;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetElectroSpatialFormulation() should fail
            // if the property not present.
            aafElectroSpatialFormulation_t  esf = bogusEsf;
            hr = pSoundDesc->GetElectroSpatialFormulation( &esf );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( esf == bogusEsf,
                             AAFRESULT_TEST_FAILED );


            // GetElectroSpatialFormulation() should fail if
            // function arguments are invalid.
            esf = bogusEsf;
            hr = pSoundDesc->GetElectroSpatialFormulation( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( esf == bogusEsf,
                             AAFRESULT_TEST_FAILED );


            // SetElectroSpatialFormulation() should always succeed
            hr = pSoundDesc->SetElectroSpatialFormulation( gTestData.esf );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetElectroSpatialFormulation() should succeed.
            aafElectroSpatialFormulation_t  esf = bogusEsf;
            hr = pSoundDesc->GetElectroSpatialFormulation( &esf );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( esf == gTestData.esf,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_AudioRefLevel(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafInt8  bogusAudioRefLevel = -61;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetAudioRefLevel() should fail if the property not present.
            aafInt8  audioRefLevel = bogusAudioRefLevel;
            hr = pSoundDesc->GetAudioRefLevel( &audioRefLevel );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( audioRefLevel == bogusAudioRefLevel,
                             AAFRESULT_TEST_FAILED );


            // GetAudioRefLevel() should fail if function arguments are invalid.
            audioRefLevel = bogusAudioRefLevel;
            hr = pSoundDesc->GetAudioRefLevel( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( audioRefLevel == bogusAudioRefLevel,
                             AAFRESULT_TEST_FAILED );


            // SetAudioRefLevel() should always succeed
            hr = pSoundDesc->SetAudioRefLevel( gTestData.audioRefLevel );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetAudioRefLevel() should succeed.
            aafInt8  audioRefLevel = bogusAudioRefLevel;
            hr = pSoundDesc->GetAudioRefLevel( &audioRefLevel );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( audioRefLevel == gTestData.audioRefLevel,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_DialNorm(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafInt8  bogusDialNorm = 72;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetDialNorm() should fail if the property not present.
            aafInt8  dialNorm = bogusDialNorm;
            hr = pSoundDesc->GetDialNorm( &dialNorm );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( dialNorm == bogusDialNorm,
                             AAFRESULT_TEST_FAILED );


            // GetDialNorm() should fail if function arguments are invalid.
            dialNorm = bogusDialNorm;
            hr = pSoundDesc->GetDialNorm( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( dialNorm == bogusDialNorm,
                             AAFRESULT_TEST_FAILED );


            // SetDialNorm() should always succeed
            hr = pSoundDesc->SetDialNorm( gTestData.dialNorm );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetDialNorm() should succeed.
            aafInt8  dialNorm = bogusDialNorm;
            hr = pSoundDesc->GetDialNorm( &dialNorm );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( dialNorm == gTestData.dialNorm,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_QuantizationBits(
    IAAFSoundDescriptor* pSoundDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        const aafUInt32 bogusBitCount = 55700;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetQuantizationBits() should succeed because QuantizationBits
            // is a required property.
            aafUInt32  bitCount = bogusBitCount;
            hr = pSoundDesc->GetQuantizationBits( &bitCount );
            checkExpression( hr == AAFRESULT_SUCCESS,
                             AAFRESULT_TEST_FAILED );
            checkExpression( bitCount != bogusBitCount,
                             AAFRESULT_TEST_FAILED );


            // GetQuantizationBits() should fail if
            // function arguments are invalid.
            bitCount = bogusBitCount;
            hr = pSoundDesc->GetQuantizationBits( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( bitCount == bogusBitCount,
                             AAFRESULT_TEST_FAILED );


            // SetQuantizationBits() should always succeed
            hr = pSoundDesc->SetQuantizationBits(
                gTestData.quantizationBitCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetQuantizationBits() should succeed.
            aafUInt32  bitCount = bogusBitCount;
            hr = pSoundDesc->GetQuantizationBits( &bitCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( bitCount == gTestData.quantizationBitCount,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}




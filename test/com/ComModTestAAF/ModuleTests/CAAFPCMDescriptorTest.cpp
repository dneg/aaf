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
//=---------------------------------------------------------------------=

#include <iostream>

#include "AAF.h"
#include "AAFResult.h"

#include "CAAFBuiltinDefs.h"
#include "AAFDefUIDs.h"
#include "ModuleTest.h"

using namespace std;


const aafUInt32  peakEnvelopeDataSize = 16;
struct CAAFPCMDescriptorTestFixture
{
    aafMobID_t              sourceMobID;
    aafUInt16               blockAlign;
    aafUInt8                sequenceOffset;
    aafUInt32               averageBPS;
    aafUID_t                channelAssignment;
    aafUInt32               peakEnvelopeVersion;
    aafUInt32               peakEnvelopeFormat;
    aafUInt32               pointsPerPeakValue;
    aafUInt32               peakEnvelopeBlockSize;
    aafUInt32               peakChannelCount;
    aafUInt32               peakFrameCount;
    aafUInt32               peakOfPeaksPosition;
    aafTimeStamp_t          peakEnvelopeTimestamp;
    unsigned char           peakEnvelopeData[ peakEnvelopeDataSize ];
};

static const CAAFPCMDescriptorTestFixture  gTestData = 
{
    { { 0x06, 0x0c, 0x2b, 0x34,
        0x02, 0x05, 0x11, 0x01,
        0x01, 0x00, 0x10, 0x00 },
      0x13, 0x00, 0x00, 0x00,
      { 0xf6d43ded, 0xa5aa, 0x4a05,
        0xa2, 0x84, 0x37, 0x8f,
        0x94, 0x16, 0xd0, 0x56 } }, // sourceMobID

    8976,   // blockAlign;
    51,     // sequenceOffset;
    55372,  // averageBPS;
    { 0xa0262ea8, 0x9800, 0x47d2, { 0xad, 0x93, 0x98, 0x27, 0xfe, 0x82, 0xe3, 0x6d } }, // channelAssignment
    17234,  // peakEnvelopeVersion;
    12,     // peakEnvelopeFormat;
    17,     // pointsPerPeakValue;
    29,     // peakEnvelopeBlockSize;
    35,     // peakChannelCount;
    41,     // peakFrameCount;
    54,     // peakOfPeaksPosition;
    { {1979, 12, 31}, {11, 30, 0, 0} }, // peakEnvelopeTimestamp

    { '\x001', '\x002', '\x003', '\x004',
      '\x005', '\x006', '\x007', '\x008',
      '\x009', '\x00a', '\x00b', '\x00c',
      '\x00d', '\x00e', '\x00f', '\x010' }  // peakEnvelopeData
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
extern "C" HRESULT CAAFPCMDescriptor_test(
    testMode_t mode);

static HRESULT OpenAAFFile(
    aafWChar*           pFileName,
    aafMediaOpenMode_t  mode,
    IAAFFile**          ppFile,
    IAAFHeader**        ppHeader);

static HRESULT CreateAAFFile(
    aafWChar * pFileName);

static HRESULT ReadAAFFile(
    aafWChar * pFileName);

static HRESULT Test_IAAFPCMDescriptor_Uninitialized(
    IAAFPCMDescriptor* );
static HRESULT Test_IAAFPCMDescriptor(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_BlockAlign(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_SequenceOffset(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_AverageBPS(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_ChannelAssignment(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_AreAllPeakEnvelopePropertiesPresent(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeVersion(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeFormat(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PointsPerPeakValue(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeBlockSize(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PeakChannelCount(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PeakFrameCount(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PeakOfPeaksPosition(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeTimestamp(
    IAAFPCMDescriptor*,
    testMode_t );
static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeData(
    IAAFPCMDescriptor*,
    testMode_t );



//
// The public entry for this module test,
//
HRESULT CAAFPCMDescriptor_test(
    testMode_t mode )
{
    HRESULT  hr = AAFRESULT_NOT_IMPLEMENTED;
    const size_t fileNameBufLen = 128;
    aafWChar* pFileName = L"AAFPCMDescriptorTest.aaf";


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


// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
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
    IAAFPCMDescriptor*      pPCMDesc = 0;

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
        checkResult(pMob->SetName(L"PCMDescriptorTest"));


        // Create a PCMDescriptor,
        // add the descriptor to the source mob.
        checkResult(defs.cdPCMDescriptor()->CreateInstance(
            IID_IAAFPCMDescriptor,
            (IUnknown **)&pPCMDesc));


        // Before initialization test if the SoundDescriptor
        // methods return valid error values.
        checkResult(Test_IAAFPCMDescriptor_Uninitialized (pPCMDesc));



        // Initialize PCMDescriptor object.
        checkResult(pPCMDesc->Initialize());


        // Attach the descriptor to the source mob and
        // then attach the source mob to the file.
        checkResult(pPCMDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **) &pEssenceDesc));
        checkResult(pSourceMob->SetEssenceDescriptor(pEssenceDesc));

        checkResult(pHeader->AddMob(pMob));


        // Test initialized SoundDescriptor.
        checkResult(Test_IAAFPCMDescriptor(pPCMDesc,
                                           kAAFUnitTestReadWrite));


        // Save the file
        checkResult(pFile->Save());
    }
    catch (HRESULT& rResult)
    {
        hr = rResult;
    }

    if (pPCMDesc) 
    {
        pPCMDesc->Release();
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
    IAAFPCMDescriptor*      pPCMDesc = 0;

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
        checkResult(pEssenceDesc->QueryInterface(IID_IAAFPCMDescriptor,
                                                 (void **)&pPCMDesc));


        checkResult(Test_IAAFPCMDescriptor(pPCMDesc,
                                           kAAFUnitTestReadOnly));
    }
    catch (HRESULT& rResult)
    {
        hr = rResult;
    }


    if (pPCMDesc) 
    {
        pPCMDesc->Release();
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



static HRESULT Test_IAAFPCMDescriptor_Uninitialized(
    IAAFPCMDescriptor* pPCMDesc )
{
    HRESULT  hr = S_OK;


    try
    {
        aafUInt16  blockAlign = 0;
        hr = pPCMDesc->SetBlockAlign( blockAlign );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetBlockAlign( &blockAlign );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUInt8  sequenceOffset = 0;
        hr = pPCMDesc->SetSequenceOffset( sequenceOffset );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetSequenceOffset( &sequenceOffset );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUInt32  averageBPS;
        hr = pPCMDesc->SetAverageBPS( gTestData.averageBPS );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetAverageBPS( &averageBPS );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUID_t  channelAssignment;
        hr = pPCMDesc->SetChannelAssignment( channelAssignment );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetChannelAssignment( &channelAssignment );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUInt32  peakEnvelopeVersion = 0;
        hr = pPCMDesc->SetPeakEnvelopeVersion( peakEnvelopeVersion );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPeakEnvelopeVersion( &peakEnvelopeVersion );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUInt32  peakEnvelopeFormat;
        hr = pPCMDesc->SetPeakEnvelopeFormat( gTestData.peakEnvelopeFormat );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPeakEnvelopeFormat( &peakEnvelopeFormat );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUInt32  pointsPerPeakValue;
        hr = pPCMDesc->SetPointsPerPeakValue( gTestData.pointsPerPeakValue );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPointsPerPeakValue( &pointsPerPeakValue );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUInt32  peakEnvelopeBlockSize;
        hr = pPCMDesc->SetPeakEnvelopeBlockSize( gTestData.peakEnvelopeBlockSize );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPeakEnvelopeBlockSize( &peakEnvelopeBlockSize );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUInt32  peakChannelCount;
        hr = pPCMDesc->SetPeakChannelCount( gTestData.peakChannelCount );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPeakChannelCount( &peakChannelCount );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafUInt32  peakFrameCount;
        hr = pPCMDesc->SetPeakFrameCount( gTestData.peakFrameCount );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPeakFrameCount( &peakFrameCount );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafPosition_t  peakOfPeaksPosition;
        hr = pPCMDesc->SetPeakOfPeaksPosition( gTestData.peakOfPeaksPosition );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPeakOfPeaksPosition( &peakOfPeaksPosition );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafTimeStamp_t  peakEnvelopeTimestamp;
        hr = pPCMDesc->SetPeakEnvelopeTimestamp( gTestData.peakEnvelopeTimestamp );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPeakEnvelopeTimestamp( &peakEnvelopeTimestamp );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafPosition_t position = 0;
        hr = pPCMDesc->SetPeakEnvelopeDataPosition( position );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->GetPeakEnvelopeDataPosition( &position );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        aafLength_t  size;
        hr = pPCMDesc->GetPeakEnvelopeDataSize( &size );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


        unsigned char  peakEnvelopeData[ peakEnvelopeDataSize ];
        aafUInt32  bytesTransferred;
        hr = pPCMDesc->WritePeakEnvelopeData( peakEnvelopeDataSize,
                                              (aafDataBuffer_t)(peakEnvelopeData),
                                              &bytesTransferred );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );
        hr = pPCMDesc->ReadPeakEnvelopeData( peakEnvelopeDataSize,
                                             peakEnvelopeData,
                                             &bytesTransferred );
        checkExpression( hr == AAFRESULT_NOT_INITIALIZED, AAFRESULT_TEST_FAILED );


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



static HRESULT Test_IAAFPCMDescriptor(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        checkResult( Test_IAAFPCMDescriptor_BlockAlign( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_SequenceOffset( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_AverageBPS( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_ChannelAssignment( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_AreAllPeakEnvelopePropertiesPresent( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PeakEnvelopeVersion( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PeakEnvelopeFormat( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PointsPerPeakValue( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PeakEnvelopeBlockSize( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PeakChannelCount( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PeakFrameCount( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PeakOfPeaksPosition( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PeakEnvelopeTimestamp( pPCMDesc, mode ) );
        checkResult( Test_IAAFPCMDescriptor_PeakEnvelopeData( pPCMDesc, mode ) );

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



static HRESULT Test_IAAFPCMDescriptor_BlockAlign(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        const aafUInt16 bogusBlockAlign = 39;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetBlockAlign() should succeed because BlockAlign
            // is a required property.
            aafUInt16  blockAlign = bogusBlockAlign;
            hr = pPCMDesc->GetBlockAlign( &blockAlign );
            checkExpression( hr == AAFRESULT_SUCCESS,
                             AAFRESULT_TEST_FAILED );
            checkExpression( blockAlign != bogusBlockAlign,
                             AAFRESULT_TEST_FAILED );


            // GetBlockAlign() should fail if function arguments are invalid.
            blockAlign = bogusBlockAlign;
            hr = pPCMDesc->GetBlockAlign( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( blockAlign == bogusBlockAlign,
                             AAFRESULT_TEST_FAILED );


            // SetBlockAlign() should always succeed
            hr = pPCMDesc->SetBlockAlign( gTestData.blockAlign );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetBlockAlign() should succeed.
            aafUInt16  blockAlign = bogusBlockAlign;
            hr = pPCMDesc->GetBlockAlign( &blockAlign );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( blockAlign == gTestData.blockAlign,
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



static HRESULT Test_IAAFPCMDescriptor_SequenceOffset(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt8  bogusSequenceOffset = 67;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetSequenceOffset() should fail if the property not present.
            aafUInt8  sequenceOffset = bogusSequenceOffset;
            hr = pPCMDesc->GetSequenceOffset( &sequenceOffset );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( sequenceOffset == bogusSequenceOffset,
                             AAFRESULT_TEST_FAILED );


            // GetSequenceOffset() should fail if function arguments are invalid.
            sequenceOffset = bogusSequenceOffset;
            hr = pPCMDesc->GetSequenceOffset( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( sequenceOffset == bogusSequenceOffset,
                             AAFRESULT_TEST_FAILED );


            // SetSequenceOffset() should always succeed
            hr = pPCMDesc->SetSequenceOffset( gTestData.sequenceOffset );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetSequenceOffset() should succeed.
            aafUInt8  sequenceOffset = bogusSequenceOffset;
            hr = pPCMDesc->GetSequenceOffset( &sequenceOffset );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( sequenceOffset == gTestData.sequenceOffset,
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



static HRESULT Test_IAAFPCMDescriptor_AverageBPS(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusAverageBPS = 12082;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetAverageBPS() should succeed because AverageBPS
            // is a required property.
            aafUInt32  averageBPS = bogusAverageBPS;
            hr = pPCMDesc->GetAverageBPS( &averageBPS );
            checkExpression( hr == AAFRESULT_SUCCESS,
                             AAFRESULT_TEST_FAILED );
            checkExpression( averageBPS != bogusAverageBPS,
                             AAFRESULT_TEST_FAILED );


            // GetAverageBPS() should fail if function arguments are invalid.
            averageBPS = bogusAverageBPS;
            hr = pPCMDesc->GetAverageBPS( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( averageBPS == bogusAverageBPS,
                             AAFRESULT_TEST_FAILED );


            // SetAverageBPS() should always succeed
            hr = pPCMDesc->SetAverageBPS( gTestData.averageBPS );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetAverageBPS() should succeed.
            aafUInt32  averageBPS = bogusAverageBPS;
            hr = pPCMDesc->GetAverageBPS( &averageBPS );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( averageBPS == gTestData.averageBPS,
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



static HRESULT Test_IAAFPCMDescriptor_ChannelAssignment(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUID_t bogusChannelAssignmentID =
            { 0x4eb11210, 0x59c3, 0x4b7d,
              { 0x80, 0x50, 0x8a, 0x12, 0x99, 0x2c, 0x96, 0xf1 } };


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetChannelAssignment() should fail if the property not present.
            aafUID_t  channelAssignmentID = bogusChannelAssignmentID;
            hr = pPCMDesc->GetChannelAssignment( &channelAssignmentID );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( channelAssignmentID == bogusChannelAssignmentID,
                             AAFRESULT_TEST_FAILED );


            // GetChannelAssignment() should fail if function arguments are invalid.
            channelAssignmentID = bogusChannelAssignmentID;
            hr = pPCMDesc->GetChannelAssignment( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( channelAssignmentID == bogusChannelAssignmentID,
                             AAFRESULT_TEST_FAILED );


            // SetChannelAssignment() should always succeed
            hr = pPCMDesc->SetChannelAssignment( gTestData.channelAssignment );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetChannelAssignment() should succeed.
            aafUID_t  channelAssignmentID = bogusChannelAssignmentID;
            hr = pPCMDesc->GetChannelAssignment( &channelAssignmentID );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( channelAssignmentID == gTestData.channelAssignment,
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



static HRESULT Test_IAAFPCMDescriptor_AreAllPeakEnvelopePropertiesPresent(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t /* mode */)
{
    HRESULT  hr = S_OK;


    try
    {
        // AreAllPeakEnvelopePropertiesPresent() should fail if function
        // arguments are invalid.
        hr = pPCMDesc->AreAllPeakEnvelopePropertiesPresent( 0 );
        checkExpression( hr == AAFRESULT_NULL_PARAM,
                         AAFRESULT_TEST_FAILED );


        // See if all the needed properties are present
        aafUInt32  dummyUInt32 = 0;
        aafPosition_t  dummyPosition = 0;
        aafTimeStamp_t  dummyTimeStamp = { {0, 0, 0}, {0, 0, 0, 0} };
        aafBoolean_t  allPeakEnvelopePropsPresent = kAAFTrue;

        if( pPCMDesc->GetPeakEnvelopeVersion( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakEnvelopeFormat( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPointsPerPeakValue( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakEnvelopeBlockSize( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakChannelCount( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakFrameCount( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakOfPeaksPosition( &dummyPosition ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakEnvelopeTimestamp( &dummyTimeStamp ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakEnvelopeDataPosition( &dummyPosition ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }


        aafBoolean_t  arePresent = ~allPeakEnvelopePropsPresent;
        hr = pPCMDesc->AreAllPeakEnvelopePropertiesPresent( &arePresent );
        checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        checkExpression( allPeakEnvelopePropsPresent == arePresent,
                         AAFRESULT_TEST_FAILED );


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



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeVersion(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakEnvelopeVersion = 283;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakEnvelopeVersion() should fail if the property
            // not present.
            aafUInt32  peakEnvelopeVersion = bogusPeakEnvelopeVersion;
            hr = pPCMDesc->GetPeakEnvelopeVersion( &peakEnvelopeVersion );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeVersion == bogusPeakEnvelopeVersion,
                             AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeVersion() should fail if function
            // arguments are invalid.
            peakEnvelopeVersion = bogusPeakEnvelopeVersion;
            hr = pPCMDesc->GetPeakEnvelopeVersion( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeVersion == bogusPeakEnvelopeVersion,
                             AAFRESULT_TEST_FAILED );


            // SetPeakEnvelopeVersion() should always succeed
            hr = pPCMDesc->SetPeakEnvelopeVersion(
                gTestData.peakEnvelopeVersion );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakEnvelopeVersion() should succeed.
            aafUInt32  peakEnvelopeVersion = bogusPeakEnvelopeVersion;
            hr = pPCMDesc->GetPeakEnvelopeVersion( &peakEnvelopeVersion );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeVersion == gTestData.peakEnvelopeVersion,
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



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeFormat(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakEnvelopeFormat = 134;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakEnvelopeFormat() should fail if the property not present.
            aafUInt32  peakEnvelopeFormat = bogusPeakEnvelopeFormat;
            hr = pPCMDesc->GetPeakEnvelopeFormat( &peakEnvelopeFormat );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeFormat == bogusPeakEnvelopeFormat,
                             AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeFormat() should fail if function
            // arguments are invalid.
            peakEnvelopeFormat = bogusPeakEnvelopeFormat;
            hr = pPCMDesc->GetPeakEnvelopeFormat( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeFormat == bogusPeakEnvelopeFormat,
                             AAFRESULT_TEST_FAILED );


            // SetPeakEnvelopeFormat() should always succeed
            hr = pPCMDesc->SetPeakEnvelopeFormat( gTestData.peakEnvelopeFormat );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakEnvelopeFormat() should succeed.
            aafUInt32  peakEnvelopeFormat = bogusPeakEnvelopeFormat;
            hr = pPCMDesc->GetPeakEnvelopeFormat( &peakEnvelopeFormat );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeFormat == gTestData.peakEnvelopeFormat,
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



static HRESULT Test_IAAFPCMDescriptor_PointsPerPeakValue(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPointsPerPeakValue = 139;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPointsPerPeakValue() should fail if the property not present.
            aafUInt32  pointsPerPeakValue = bogusPointsPerPeakValue;
            hr = pPCMDesc->GetPointsPerPeakValue( &pointsPerPeakValue );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( pointsPerPeakValue == bogusPointsPerPeakValue,
                             AAFRESULT_TEST_FAILED );


            // GetPointsPerPeakValue() should fail if function
            // arguments are invalid.
            pointsPerPeakValue = bogusPointsPerPeakValue;
            hr = pPCMDesc->GetPointsPerPeakValue( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( pointsPerPeakValue == bogusPointsPerPeakValue,
                             AAFRESULT_TEST_FAILED );


            // SetPointsPerPeakValue() should always succeed
            hr = pPCMDesc->SetPointsPerPeakValue( gTestData.pointsPerPeakValue );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPointsPerPeakValue() should succeed.
            aafUInt32  pointsPerPeakValue = bogusPointsPerPeakValue;
            hr = pPCMDesc->GetPointsPerPeakValue( &pointsPerPeakValue );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( pointsPerPeakValue == gTestData.pointsPerPeakValue,
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



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeBlockSize(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakEnvelopeBlockSize = 142;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakEnvelopeBlockSize() should fail if the property not present.
            aafUInt32  peakEnvelopeBlockSize = bogusPeakEnvelopeBlockSize;
            hr = pPCMDesc->GetPeakEnvelopeBlockSize( &peakEnvelopeBlockSize );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeBlockSize == bogusPeakEnvelopeBlockSize,
                             AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeBlockSize() should fail if function
            // arguments are invalid.
            peakEnvelopeBlockSize = bogusPeakEnvelopeBlockSize;
            hr = pPCMDesc->GetPeakEnvelopeBlockSize( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeBlockSize == bogusPeakEnvelopeBlockSize,
                             AAFRESULT_TEST_FAILED );


            // SetPeakEnvelopeBlockSize() should always succeed
            hr = pPCMDesc->SetPeakEnvelopeBlockSize( gTestData.peakEnvelopeBlockSize );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakEnvelopeBlockSize() should succeed.
            aafUInt32  peakEnvelopeBlockSize = bogusPeakEnvelopeBlockSize;
            hr = pPCMDesc->GetPeakEnvelopeBlockSize( &peakEnvelopeBlockSize );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeBlockSize == gTestData.peakEnvelopeBlockSize,
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



static HRESULT Test_IAAFPCMDescriptor_PeakChannelCount(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakChannelCount = 11198;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakChannelCount() should fail if the property not present.
            aafUInt32  peakChannelCount = bogusPeakChannelCount;
            hr = pPCMDesc->GetPeakChannelCount( &peakChannelCount );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakChannelCount == bogusPeakChannelCount,
                             AAFRESULT_TEST_FAILED );


            // GetPeakChannelCount() should fail if function
            // arguments are invalid.
            peakChannelCount = bogusPeakChannelCount;
            hr = pPCMDesc->GetPeakChannelCount( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakChannelCount == bogusPeakChannelCount,
                             AAFRESULT_TEST_FAILED );


            // SetPeakChannelCount() should always succeed
            hr = pPCMDesc->SetPeakChannelCount( gTestData.peakChannelCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakChannelCount() should succeed.
            aafUInt32  peakChannelCount = bogusPeakChannelCount;
            hr = pPCMDesc->GetPeakChannelCount( &peakChannelCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakChannelCount == gTestData.peakChannelCount,
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



static HRESULT Test_IAAFPCMDescriptor_PeakFrameCount(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakFrameCount = 1200;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakFrameCount() should fail if the property not present.
            aafUInt32  peakFrameCount = bogusPeakFrameCount;
            hr = pPCMDesc->GetPeakFrameCount( &peakFrameCount );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakFrameCount == bogusPeakFrameCount,
                             AAFRESULT_TEST_FAILED );


            // GetPeakFrameCount() should fail if function
            // arguments are invalid.
            peakFrameCount = bogusPeakFrameCount;
            hr = pPCMDesc->GetPeakFrameCount( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakFrameCount == bogusPeakFrameCount,
                             AAFRESULT_TEST_FAILED );


            // SetPeakFrameCount() should always succeed
            hr = pPCMDesc->SetPeakFrameCount( gTestData.peakFrameCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakFrameCount() should succeed.
            aafUInt32  peakFrameCount = bogusPeakFrameCount;
            hr = pPCMDesc->GetPeakFrameCount( &peakFrameCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakFrameCount == gTestData.peakFrameCount,
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



static HRESULT Test_IAAFPCMDescriptor_PeakOfPeaksPosition(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafPosition_t  bogusPeakOfPeaksPosition = -49100;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakOfPeaksPosition() should fail if the property not present.
            aafPosition_t  peakOfPeaksPosition = bogusPeakOfPeaksPosition;
            hr = pPCMDesc->GetPeakOfPeaksPosition( &peakOfPeaksPosition );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakOfPeaksPosition == bogusPeakOfPeaksPosition,
                             AAFRESULT_TEST_FAILED );


            // GetPeakOfPeaksPosition() should fail if function
            // arguments are invalid.
            peakOfPeaksPosition = bogusPeakOfPeaksPosition;
            hr = pPCMDesc->GetPeakOfPeaksPosition( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakOfPeaksPosition == bogusPeakOfPeaksPosition,
                             AAFRESULT_TEST_FAILED );


            // SetPeakOfPeaksPosition() should always succeed
            hr = pPCMDesc->SetPeakOfPeaksPosition( gTestData.peakOfPeaksPosition );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakOfPeaksPosition() should succeed.
            aafPosition_t  peakOfPeaksPosition = bogusPeakOfPeaksPosition;
            hr = pPCMDesc->GetPeakOfPeaksPosition( &peakOfPeaksPosition );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakOfPeaksPosition == gTestData.peakOfPeaksPosition,
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



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeTimestamp(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafTimeStamp_t  bogusPeakEnvelopeTimestamp =
                                        { {1941, 7, 22}, {4, 0, 0, 0} };


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakEnvelopeTimestamp() should fail if the property not present.
            aafTimeStamp_t  peakEnvelopeTimestamp = bogusPeakEnvelopeTimestamp;
            hr = pPCMDesc->GetPeakEnvelopeTimestamp( &peakEnvelopeTimestamp );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeTimestamp == bogusPeakEnvelopeTimestamp,
                             AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeTimestamp() should fail if function
            // arguments are invalid.
            peakEnvelopeTimestamp = bogusPeakEnvelopeTimestamp;
            hr = pPCMDesc->GetPeakEnvelopeTimestamp( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeTimestamp == bogusPeakEnvelopeTimestamp,
                             AAFRESULT_TEST_FAILED );


            // SetPeakEnvelopeTimestamp() should always succeed
            hr = pPCMDesc->SetPeakEnvelopeTimestamp( gTestData.peakEnvelopeTimestamp );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakEnvelopeTimestamp() should succeed.
            aafTimeStamp_t  peakEnvelopeTimestamp = bogusPeakEnvelopeTimestamp;
            hr = pPCMDesc->GetPeakEnvelopeTimestamp( &peakEnvelopeTimestamp );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeTimestamp == gTestData.peakEnvelopeTimestamp,
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



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeData(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        unsigned char  bogusPeakEnvelopeData[ peakEnvelopeDataSize ];
        memset( bogusPeakEnvelopeData, 0xED, peakEnvelopeDataSize );


        if( mode == kAAFUnitTestReadWrite )
        {
            // ReadPeakEnvelopeData() should fail if the property not present.
            unsigned char  buffer[ peakEnvelopeDataSize ];
            memcpy( buffer, bogusPeakEnvelopeData, peakEnvelopeDataSize );
            aafUInt32  bytesRead = 0;
            hr = pPCMDesc->ReadPeakEnvelopeData( peakEnvelopeDataSize,
                                                 buffer,
                                                 &bytesRead );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( memcmp( buffer, bogusPeakEnvelopeData,
                                     peakEnvelopeDataSize ) == 0,
                             AAFRESULT_TEST_FAILED );
            checkExpression( bytesRead == 0, AAFRESULT_TEST_FAILED );


            // ReadPeakEnvelopeData() should fail if function
            // arguments are invalid.
            hr = pPCMDesc->ReadPeakEnvelopeData( peakEnvelopeDataSize,
                                                 0,
                                                 &bytesRead );
            checkExpression( hr==AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED );
            hr = pPCMDesc->ReadPeakEnvelopeData( peakEnvelopeDataSize,
                                                 buffer,
                                                 0 );
            checkExpression( hr==AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED );
            checkExpression( memcmp( buffer, bogusPeakEnvelopeData,
                                     peakEnvelopeDataSize ) == 0,
                             AAFRESULT_TEST_FAILED );
            checkExpression( bytesRead == 0, AAFRESULT_TEST_FAILED );


            // WritePeakEnvelopeData() should fail if the function
            // arguments are invalid.
            aafUInt32  bytesWritten = 0;
            hr = pPCMDesc->WritePeakEnvelopeData( peakEnvelopeDataSize,
                                                  0,
                                                  &bytesWritten );
            checkExpression( hr==AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED );
            checkExpression( bytesWritten == 0, AAFRESULT_TEST_FAILED );


            // WritePeakEnvelopeData() should always succeed
            hr = pPCMDesc->WritePeakEnvelopeData(
                    peakEnvelopeDataSize,
                    (aafDataBuffer_t)(gTestData.peakEnvelopeData),
                    &bytesWritten );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( bytesWritten == peakEnvelopeDataSize, AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeDataPosition() should fail if the function
            // arguments are invalid.
            hr = pPCMDesc->GetPeakEnvelopeDataPosition( 0 );
            checkExpression( hr==AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeDataPosition() should always succeed
            aafPosition_t  position = 0;
            hr = pPCMDesc->GetPeakEnvelopeDataPosition( &position );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( position == peakEnvelopeDataSize, AAFRESULT_TEST_FAILED );


            // After writing to the stream the current position is
            // past the end of the last byte, thefore Read() should
            // fail.
            bytesRead = 0;
            hr = pPCMDesc->ReadPeakEnvelopeData( peakEnvelopeDataSize,
                                                 buffer,
                                                 &bytesRead );
            checkExpression( hr == AAFRESULT_END_OF_DATA, AAFRESULT_TEST_FAILED );
            checkExpression( bytesRead == 0, AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeDataSize() should fail if the function
            // arguments are invalid.
            hr = pPCMDesc->GetPeakEnvelopeDataSize( 0 );
            checkExpression( hr==AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeDataSize() should always succeed
            aafLength_t  size = 0;
            hr = pPCMDesc->GetPeakEnvelopeDataSize( &size );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( size == peakEnvelopeDataSize, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // Check the stream size
            aafLength_t  size = 0;
            hr = pPCMDesc->GetPeakEnvelopeDataSize( &size );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( size == peakEnvelopeDataSize, AAFRESULT_TEST_FAILED );


            // Make sure the current position is at the beginning of the stream
            hr = pPCMDesc->SetPeakEnvelopeDataPosition( 0 );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );


            // Read less data than there's in the stream
            unsigned char  buffer[ peakEnvelopeDataSize * 2 ];
            memcpy( buffer, bogusPeakEnvelopeData, peakEnvelopeDataSize );
            aafUInt32  bytesRead = 0;
            aafUInt32  bytesToRead = peakEnvelopeDataSize / 2;
            hr = pPCMDesc->ReadPeakEnvelopeData( bytesToRead,
                                                 buffer,
                                                 &bytesRead );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( bytesRead == bytesToRead,
                             AAFRESULT_TEST_FAILED );
            checkExpression( memcmp( buffer, gTestData.peakEnvelopeData,
                                     bytesToRead ) == 0,
                             AAFRESULT_TEST_FAILED );


            // Make sure the current position is at the beginning of the stream
            hr = pPCMDesc->SetPeakEnvelopeDataPosition( 0 );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );


            // Read more data than there's in the stream
            memcpy( buffer, bogusPeakEnvelopeData, peakEnvelopeDataSize );
            bytesRead = 0;
            bytesToRead = peakEnvelopeDataSize * 2;
            hr = pPCMDesc->ReadPeakEnvelopeData( bytesToRead,
                                                 buffer,
                                                 &bytesRead );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( bytesRead == peakEnvelopeDataSize,
                             AAFRESULT_TEST_FAILED );
            checkExpression( memcmp( buffer, gTestData.peakEnvelopeData,
                                     peakEnvelopeDataSize ) == 0,
                             AAFRESULT_TEST_FAILED );


            // Make sure the current position is at the beginning of the stream
            hr = pPCMDesc->SetPeakEnvelopeDataPosition( 0 );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );


            // Read whole stream
            memcpy( buffer, bogusPeakEnvelopeData, peakEnvelopeDataSize );
            bytesRead = 0;
            bytesToRead = peakEnvelopeDataSize;
            hr = pPCMDesc->ReadPeakEnvelopeData( bytesToRead,
                                                 buffer,
                                                 &bytesRead );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( bytesRead == bytesToRead,
                             AAFRESULT_TEST_FAILED );
            checkExpression( memcmp( buffer, gTestData.peakEnvelopeData,
                                     bytesToRead ) == 0,
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




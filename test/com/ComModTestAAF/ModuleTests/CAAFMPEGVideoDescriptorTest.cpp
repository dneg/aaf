//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
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


#define ksingleSequence				kAAFTrue
#define kconstantBPictureCount		kAAFTrue
#define kcodedContentScanning		kAAFContentScanning_Progressive
#define klowDelay					kAAFTrue
#define kclosedGOP					kAAFTrue
#define kidenticalGOP				kAAFTrue
#define kmaxGOP						256
#define kmaxBPictureCount			128
#define kbitRate					1024
#define kprofileAndLevel			8


static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x09273e8e, 0x0406, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

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
extern "C" HRESULT CAAFMPEGVideoDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID);

static HRESULT ReadAAFFile(
    aafWChar * pFileName);

static HRESULT Test_IAAFMPEGVideoDescriptor( IAAFMPEGVideoDescriptor*, testMode_t );

static HRESULT Test_IAAFMPEGVideoDescriptor_SingleSequence(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_ConstantBPictureCount(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_CodedContentScanning(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_LowDelay(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_ClosedGOP(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_IdenticalGOP(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_MaxGOP(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_MaxBPictureCount(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_BitRate(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );
static HRESULT Test_IAAFMPEGVideoDescriptor_ProfileAndLevel(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode );

bool operator ==( const aafUID_t uid1, const aafUID_t uid2 );
bool operator !=( const aafUID_t uid1, const aafUID_t uid2 );





//
// The public entry for this module test,
//
HRESULT CAAFMPEGVideoDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID )
{
    HRESULT  hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar pFileName[ fileNameBufLen ] = L"";
	GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );


    try
    {
        if(mode == kAAFUnitTestReadWrite)
            hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
        else
            hr = AAFRESULT_SUCCESS;
        if(hr == AAFRESULT_SUCCESS)
            hr = ReadAAFFile( pFileName );
    }
    catch (...)
    {
        cerr << "CAAFMPEGVideoDescriptor_test...Caught general C++"
             << " exception!" << endl; 
        hr = AAFRESULT_TEST_FAILED;
    }


    return hr;
}


static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
    IAAFFile*               pFile = 0;
    IAAFHeader*             pHeader = 0;
    IAAFDictionary*         pDictionary = 0;
    IAAFSourceMob*          pSourceMob = 0;
    IAAFMob*                pMob = 0;
    IAAFEssenceDescriptor*  pEssDesc = 0;
    IAAFMPEGVideoDescriptor*    pMPEGVideoDesc = 0;

    HRESULT hr = S_OK;

    try
    {
    	    
     	// Remove the previous test file if any.
    	RemoveTestFile(pFileName);


	  	// Create the AAF file
    	checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));

    	// We can't really do anthing in AAF without the header.
    	checkResult(pFile->GetHeader(&pHeader));

    	// Get the AAF Dictionary so that we can create valid AAF objects.
    	checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
 		
	  	// Create a source mob
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
								   (IUnknown **)&pSourceMob));
		checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));

		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"MPEGVideoDescriptorTest"));
		checkResult(defs.cdMPEGVideoDescriptor()->
					CreateInstance(IID_IAAFMPEGVideoDescriptor, 
								   (IUnknown **)&pMPEGVideoDesc));		

		Test_IAAFMPEGVideoDescriptor(pMPEGVideoDesc, kAAFUnitTestReadWrite );
		

    	checkResult(pMPEGVideoDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
		checkResult(pSourceMob->SetEssenceDescriptor(pEssDesc));

		// Add the MOB to the file
		checkResult(pHeader->AddMob(pMob));
	}
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }


    if (pMPEGVideoDesc) 
    {
        pMPEGVideoDesc->Release();
    }
    if (pEssDesc) 
    {
        pEssDesc->Release();
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
    	pFile->Save();
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
    IAAFMPEGVideoDescriptor*    pMPEGVideo = 0;

    try
    {

        // Open the test file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));

		// Get the AAF file header.
		checkResult(pFile->GetHeader(&pHeader));
        
        // Get the source mob
        checkResult(pHeader->LookupMob(TEST_MobID, &pMob));
        checkResult(pMob->QueryInterface(IID_IAAFSourceMob,
                                         (void **)&pSourceMob));

        // Get the sound descriptor
        checkResult(pSourceMob->GetEssenceDescriptor(&pEssenceDesc));
        checkResult(pEssenceDesc->QueryInterface(IID_IAAFMPEGVideoDescriptor,
                                                 (void **)&pMPEGVideo));


        checkResult(Test_IAAFMPEGVideoDescriptor(pMPEGVideo,
                                             kAAFUnitTestReadOnly));
    }
    catch (HRESULT& rResult)
    {
        hr = rResult;
    }


    if (pMPEGVideo) 
    {
        pMPEGVideo->Release();
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



static HRESULT Test_IAAFMPEGVideoDescriptor(
    IAAFMPEGVideoDescriptor* pMPEGVideoDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
    	
        checkResult( Test_IAAFMPEGVideoDescriptor_SingleSequence( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_CodedContentScanning( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_ConstantBPictureCount( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_LowDelay( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_ClosedGOP( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_IdenticalGOP( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_MaxGOP( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_MaxBPictureCount( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_BitRate( pMPEGVideoDesc, mode ) );
        checkResult( Test_IAAFMPEGVideoDescriptor_ProfileAndLevel( pMPEGVideoDesc, mode ) );

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


static HRESULT Test_IAAFMPEGVideoDescriptor_SingleSequence(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafBoolean_t bogusSingleSequence = kAAFFalse;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetSingleSequence() should fail if the property not present.
            aafBoolean_t  singleSequence = bogusSingleSequence;
            hr = pMPEGVideo->GetSingleSequence( &singleSequence );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( singleSequence == bogusSingleSequence,
                             AAFRESULT_TEST_FAILED );


            // GetSingleSequence() should fail if function arguments are invalid.
            singleSequence = bogusSingleSequence;
            hr = pMPEGVideo->GetSingleSequence( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( singleSequence == bogusSingleSequence,
                             AAFRESULT_TEST_FAILED );


            // SetSingleSequence() should always succeed
            hr = pMPEGVideo->SetSingleSequence( ksingleSequence );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetSingleSequence() should succeed.
            aafBoolean_t  singleSequence = bogusSingleSequence;
            hr = pMPEGVideo->GetSingleSequence( &singleSequence );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( singleSequence == ksingleSequence,
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


static HRESULT Test_IAAFMPEGVideoDescriptor_ConstantBPictureCount(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafBoolean_t bogusConstantBPictureCount = kAAFFalse;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetConstantBPictureCount() should fail if the property not present.
            aafBoolean_t  constantBPictureCount = bogusConstantBPictureCount;
            hr = pMPEGVideo->GetConstantBPictureCount( &constantBPictureCount );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( constantBPictureCount == bogusConstantBPictureCount,
                             AAFRESULT_TEST_FAILED );


            // GetConstantBPictureCount() should fail if function arguments are invalid.
            constantBPictureCount = bogusConstantBPictureCount;
            hr = pMPEGVideo->GetConstantBPictureCount( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( constantBPictureCount == bogusConstantBPictureCount,
                             AAFRESULT_TEST_FAILED );


            // SetConstantBPictureCount() should always succeed
            hr = pMPEGVideo->SetConstantBPictureCount( kconstantBPictureCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetConstantBPictureCount() should succeed.
            aafBoolean_t  constantBPictureCount = bogusConstantBPictureCount;
            hr = pMPEGVideo->GetConstantBPictureCount( &constantBPictureCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( constantBPictureCount == kconstantBPictureCount,
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



static HRESULT Test_IAAFMPEGVideoDescriptor_CodedContentScanning(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafContentScanningType_t bogusCodedContentScanning = kAAFFalse;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetCodedContentScanning() should fail if the property not present.
            aafContentScanningType_t  codedContentScanning = bogusCodedContentScanning;
            hr = pMPEGVideo->GetCodedContentScanning( &codedContentScanning );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( codedContentScanning == bogusCodedContentScanning,
                             AAFRESULT_TEST_FAILED );


            // GetCodedContentScanning() should fail if function arguments are invalid.
            codedContentScanning = bogusCodedContentScanning;
            hr = pMPEGVideo->GetCodedContentScanning( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( codedContentScanning == bogusCodedContentScanning,
                             AAFRESULT_TEST_FAILED );


            // SetCodedContentScanning() should always succeed
            hr = pMPEGVideo->SetCodedContentScanning( kcodedContentScanning );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetCodedContentScanning() should succeed.
            aafContentScanningType_t  codedContentScanning = bogusCodedContentScanning;
            hr = pMPEGVideo->GetCodedContentScanning( &codedContentScanning );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( codedContentScanning == kcodedContentScanning,
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


static HRESULT Test_IAAFMPEGVideoDescriptor_LowDelay(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafBoolean_t bogusLowDelay = kAAFFalse;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetLowDelay() should fail if the property not present.
            aafBoolean_t  lowDelay = bogusLowDelay;
            hr = pMPEGVideo->GetLowDelay( &lowDelay );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( lowDelay == bogusLowDelay,
                             AAFRESULT_TEST_FAILED );


            // GetLowDelay() should fail if function arguments are invalid.
            lowDelay = bogusLowDelay;
            hr = pMPEGVideo->GetLowDelay( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( lowDelay == bogusLowDelay,
                             AAFRESULT_TEST_FAILED );


            // SetLowDelay() should always succeed
            hr = pMPEGVideo->SetLowDelay( klowDelay );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetLowDelay() should succeed.
            aafBoolean_t  lowDelay = bogusLowDelay;
            hr = pMPEGVideo->GetLowDelay( &lowDelay );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( lowDelay == klowDelay,
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

static HRESULT Test_IAAFMPEGVideoDescriptor_ClosedGOP(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafBoolean_t bogusClosedGOP = kAAFFalse;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetClosedGOP() should fail if the property not present.
            aafBoolean_t  closedGOP = bogusClosedGOP;
            hr = pMPEGVideo->GetClosedGOP( &closedGOP );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( closedGOP == bogusClosedGOP,
                             AAFRESULT_TEST_FAILED );


            // GetClosedGOP() should fail if function arguments are invalid.
            closedGOP = bogusClosedGOP;
            hr = pMPEGVideo->GetClosedGOP( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( closedGOP == bogusClosedGOP,
                             AAFRESULT_TEST_FAILED );


            // SetClosedGOP() should always succeed
            hr = pMPEGVideo->SetClosedGOP( kclosedGOP );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetClosedGOP() should succeed.
            aafBoolean_t  closedGOP = bogusClosedGOP;
            hr = pMPEGVideo->GetClosedGOP( &closedGOP );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( closedGOP == kclosedGOP,
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

static HRESULT Test_IAAFMPEGVideoDescriptor_IdenticalGOP(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafBoolean_t bogusIdenticalGOP = kAAFFalse;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetIdenticalGOP() should fail if the property not present.
            aafBoolean_t  identicalGOP = bogusIdenticalGOP;
            hr = pMPEGVideo->GetIdenticalGOP( &identicalGOP );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( identicalGOP == bogusIdenticalGOP,
                             AAFRESULT_TEST_FAILED );


            // GetIdenticalGOP() should fail if function arguments are invalid.
            identicalGOP = bogusIdenticalGOP;
            hr = pMPEGVideo->GetIdenticalGOP( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( identicalGOP == bogusIdenticalGOP,
                             AAFRESULT_TEST_FAILED );


            // SetIdenticalGOP() should always succeed
            hr = pMPEGVideo->SetIdenticalGOP( kidenticalGOP );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetIdenticalGOP() should succeed.
            aafBoolean_t  identicalGOP = bogusIdenticalGOP;
            hr = pMPEGVideo->GetIdenticalGOP( &identicalGOP );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( identicalGOP == kidenticalGOP,
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


static HRESULT Test_IAAFMPEGVideoDescriptor_MaxGOP(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt16 bogusMaxGOP = 516;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetMaxGOP() should fail if the property not present.
            aafUInt16 maxGOP = bogusMaxGOP;
            
            hr = pMPEGVideo->GetMaxGOP( &maxGOP );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( maxGOP == bogusMaxGOP,
                             AAFRESULT_TEST_FAILED );


            // GetMaxGOP() should fail if function arguments are invalid.
            maxGOP = bogusMaxGOP;
            hr = pMPEGVideo->GetMaxGOP( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( maxGOP == bogusMaxGOP,
                             AAFRESULT_TEST_FAILED );


            // SetMaxGOP() should always succeed
            hr = pMPEGVideo->SetMaxGOP( kmaxGOP );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetMaxGOP() should succeed.
            aafUInt16  maxGOP = bogusMaxGOP;
            hr = pMPEGVideo->GetMaxGOP( &maxGOP );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( maxGOP == kmaxGOP,
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

static HRESULT Test_IAAFMPEGVideoDescriptor_MaxBPictureCount(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt16 bogusMaxBPictureCount = 516;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetMaxBPictureCount() should fail if the property not present.
            aafUInt16 maxBPictureCount = bogusMaxBPictureCount;
            
            hr = pMPEGVideo->GetMaxBPictureCount( &maxBPictureCount );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( maxBPictureCount == bogusMaxBPictureCount,
                             AAFRESULT_TEST_FAILED );


            // GetMaxBPictureCount() should fail if function arguments are invalid.
            maxBPictureCount = bogusMaxBPictureCount;
            hr = pMPEGVideo->GetMaxBPictureCount( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( maxBPictureCount == bogusMaxBPictureCount,
                             AAFRESULT_TEST_FAILED );


            // SetMaxBPictureCount() should always succeed
            hr = pMPEGVideo->SetMaxBPictureCount( kmaxBPictureCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetMaxBPictureCount() should succeed.
            aafUInt16  maxBPictureCount = bogusMaxBPictureCount;
            hr = pMPEGVideo->GetMaxBPictureCount( &maxBPictureCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( maxBPictureCount == kmaxBPictureCount,
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


static HRESULT Test_IAAFMPEGVideoDescriptor_BitRate(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32 bogusBitRate = 21000;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetBitRate() should fail if the property not present.
            aafUInt32 bitRate = bogusBitRate;
            
            hr = pMPEGVideo->GetBitRate( &bitRate );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( bitRate == bogusBitRate,
                             AAFRESULT_TEST_FAILED );


            // GetBitRate() should fail if function arguments are invalid.
            bitRate = bogusBitRate;
            hr = pMPEGVideo->GetBitRate( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( bitRate == bogusBitRate,
                             AAFRESULT_TEST_FAILED );


            // SetBitRate() should always succeed
            hr = pMPEGVideo->SetBitRate( kbitRate );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetBitRate() should succeed.
            aafUInt32  bitRate = bogusBitRate;
            hr = pMPEGVideo->GetBitRate( &bitRate );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( bitRate == kbitRate,
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


static HRESULT Test_IAAFMPEGVideoDescriptor_ProfileAndLevel(
    IAAFMPEGVideoDescriptor* pMPEGVideo,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt8 bogusProfileAndLevel = 7;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetProfileAndLevel() should fail if the property not present.
            aafUInt8 profileAndLevel = bogusProfileAndLevel;
            
            hr = pMPEGVideo->GetProfileAndLevel( &profileAndLevel );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( profileAndLevel == bogusProfileAndLevel,
                             AAFRESULT_TEST_FAILED );


            // GetProfileAndLevel() should fail if function arguments are invalid.
            profileAndLevel = bogusProfileAndLevel;
            hr = pMPEGVideo->GetProfileAndLevel( NULL );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( profileAndLevel == bogusProfileAndLevel,
                             AAFRESULT_TEST_FAILED );


            // SetProfileAndLevel() should always succeed
            hr = pMPEGVideo->SetProfileAndLevel( kprofileAndLevel );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly)
        {
            // GetProfileAndLevel() should succeed.
            aafUInt8  profileAndLevel = bogusProfileAndLevel;
            hr = pMPEGVideo->GetProfileAndLevel( &profileAndLevel );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( profileAndLevel == kprofileAndLevel,
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



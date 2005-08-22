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

#include <iostream>
#include <assert.h>
using namespace std;

#include "AAF.h"
#include "AAFResult.h"

#include "ModuleTest.h"
#include "AAFWideString.h"


//
// Test results.
// 
// These values can be used to accumulate
// results of number of tests, like this:
//  TESTRESULT s = TR_SUCCEEDED;
//  s |= test1();
//  s |= test2();
//  s |= test3();
//
typedef unsigned short  TESTRESULT;

const TESTRESULT TR_SUCCEEDED   = 0x0000;
const TESTRESULT TR_FAILED      = 0x0001;


//
// The following are "magic" values to test
// if variables get changed by function calls.
//
const aafUInt32 MAGIC_UINT32    = 0xD7D7D7D7;
const aafUInt32 MAGIC_UINT16    = 0xD5D5;
const aafUInt32 MAGIC_UCHAR8    = 0x05;
void* const MAGIC_POINTER       = reinterpret_cast<void*>( 0x07 );
const aafUID_t  MAGIC_UID    = { MAGIC_UINT32,
                                 MAGIC_UINT16,
                                 MAGIC_UINT16,
                                 { MAGIC_UCHAR8,
                                   MAGIC_UCHAR8,
                                   MAGIC_UCHAR8,
                                   MAGIC_UCHAR8,
                                   MAGIC_UCHAR8,
                                   MAGIC_UCHAR8,
                                   MAGIC_UCHAR8,
                                   MAGIC_UCHAR8 } };


// Required function prototype.
extern "C" HRESULT CAAFFileEncoding_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

// IAAFFileEncoding test subroutines
static AAFRESULT  Test_IAAFFileEncoding_All(
    IAAFFileEncoding* p_encoding );
static TESTRESULT Test_IAAFFileEncoding_GetNameBufLen(
    IAAFFileEncoding* p_encoding );
static TESTRESULT Test_IAAFFileEncoding_GetName(
    IAAFFileEncoding* p_encoding );
static TESTRESULT Test_IAAFFileEncoding_GetDescriptionBufLen(
    IAAFFileEncoding* p_encoding );
static TESTRESULT Test_IAAFFileEncoding_GetDescription(
    IAAFFileEncoding* p_encoding );
static TESTRESULT Test_IAAFFileEncoding_GetFileKind(
    IAAFFileEncoding* p_encoding );

// Misc. test utilities.
TESTRESULT CheckError( const char* p_msg, bool succeeded );
void ClearBuffer( aafUInt8* p_buffer, aafUInt32 buffer_size );
bool IsBufferCleared( aafUInt8* p_buffer, aafUInt32 buffer_size );

// Release a COM object and zero out the pointer.
#define ReleaseCOMObject( p_object )    if( p_object )\
                                        {\
                                            p_object->Release();\
                                            p_object = 0;\
                                        }



//
// The public entry for this module test,
//
HRESULT CAAFFileEncoding_test(
    testMode_t,
    aafUID_t,
    testRawStorageType_t,
    aafProductIdentification_t)
{
    AAFRESULT               ar = AAFRESULT_SUCCESS;
    IEnumAAFFileEncodings*  p_enum_encodings = 0;
    IAAFFileEncoding*       p_encoding = 0;


    try
    {
        //
        // Attempt to get an enumeration of
        // available file encodings.
        //
        ar = AAFGetFileEncodings( &p_enum_encodings );
        if( CheckError( "AAFGetFileEncodings() succeeds",
                         ar == AAFRESULT_SUCCESS ) != TR_SUCCEEDED )
        {
            return ar;
        }


        //
        // Test the available file ancodings objects one by one.
        //
        int found_encodings_count = 0;
        while( p_enum_encodings->NextOne( &p_encoding ) == AAFRESULT_SUCCESS )
        {
            // Test all the methods on the current
            // file encoding object.
            AAFRESULT   test_result =
                            Test_IAAFFileEncoding_All( p_encoding );

            if( test_result != AAFRESULT_SUCCESS )
            {
                ar = test_result;
            }


            // Bump up the encodings count.
            found_encodings_count++;


            ReleaseCOMObject( p_encoding );
        }


        //
        // For this test to succeed at least one IAAFFileEncoding
        // object must be present.
        //
        if( ar == AAFRESULT_SUCCESS )
        {
            if( CheckError( "At least one FileEncoding object is "
                            "supported by this library",
                             found_encodings_count > 0 ) != TR_SUCCEEDED )
            {
                ar = AAFRESULT_TEST_FAILED;
            }
        }


        ReleaseCOMObject( p_enum_encodings );
    }
    catch( ... )
    {
        cerr << "CAAFFileEncoding_test...Caught general C++"
             << " exception!" << endl; 
        ar = AAFRESULT_TEST_FAILED;
    }


    //
    // This is cleanup code in case unexpected exception is thrown.
    //
    ReleaseCOMObject( p_encoding );
    ReleaseCOMObject( p_enum_encodings );




    return ar;
}



static AAFRESULT Test_IAAFFileEncoding_All(IAAFFileEncoding* p_encoding )
{
    assert( p_encoding != 0 );


    TESTRESULT      s = TR_SUCCEEDED;

    // The order of calls is important because some test routines
    // may use methods which are assumed to be working.
    s |= Test_IAAFFileEncoding_GetNameBufLen( p_encoding );
    s |= Test_IAAFFileEncoding_GetName( p_encoding );
    s |= Test_IAAFFileEncoding_GetDescriptionBufLen( p_encoding );
    s |= Test_IAAFFileEncoding_GetDescription( p_encoding );
    s |= Test_IAAFFileEncoding_GetFileKind( p_encoding );


    return (s == TR_SUCCEEDED ? AAFRESULT_SUCCESS : AAFRESULT_TEST_FAILED);
}



static TESTRESULT Test_IAAFFileEncoding_GetNameBufLen(
    IAAFFileEncoding*       p_encoding )
{
    assert( p_encoding != 0 );

    TESTRESULT  s = TR_SUCCEEDED;       // This test status.
    AAFRESULT       ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    aafUInt32       name_buf_size = MAGIC_UINT32; // Required size of the
                                                  // name buffer.


    // GetNameBufLen should fail with NULL pointer to
    // a buffer size variable passed in.
    ar = p_encoding->GetNameBufLen( 0 );
    s |= CheckError( "GetNameBufLen() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );


    // GetNameBufLen should succeed with valid pointer to a buffer
    // size variable  passed in.
    assert( name_buf_size == MAGIC_UINT32 );
    ar = p_encoding->GetNameBufLen( &name_buf_size );
    s |= CheckError( "GetNameBufLen() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );

    if( ar != AAFRESULT_SUCCESS )
    {
        // In case GetNameBufLen fails the name_buf_size variable value
        // should stay unchanged.
        s |= CheckError(
                "Output value does not change if GetNameBufLen() fails",
                name_buf_size == MAGIC_UINT32 );
    }
    else
    {
        assert( name_buf_size != MAGIC_UINT32 );
    }


    return s;
}



static TESTRESULT Test_IAAFFileEncoding_GetName(
    IAAFFileEncoding*       p_encoding )
{
    assert( p_encoding != 0 );

    TESTRESULT  s = TR_SUCCEEDED;       // This test status.
    AAFRESULT       ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    aafUInt32       name_buf_size = MAGIC_UINT32;

    const aafUInt32 large_buffer_size = 4096;
    aafCharacter    large_buffer[ large_buffer_size ];
    ClearBuffer( reinterpret_cast<aafUInt8*>(large_buffer),
                 sizeof(aafCharacter)*large_buffer_size );


    // GetNameBufLen is assumed to be working correctly.
    ar = p_encoding->GetNameBufLen( &name_buf_size );
    assert( ar == AAFRESULT_SUCCESS );


    // Make sure large_buffer_size is large enough. If the assertion fails
    // large_buffer_size value needs to be incremented.
    assert( name_buf_size <= (large_buffer_size * sizeof(aafCharacter)) );


    // GetName should fail with NULL pointer to a name buffer passed in.
    ar = p_encoding->GetName( 0, name_buf_size );
    s |= CheckError( "GetName() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );


    // GetName should fail when smaller than required buffer
    // size is passed in. The output value should not change.
    ar = p_encoding->GetName( large_buffer, name_buf_size-1 );
    s |= CheckError( "GetName() with invalid arguments fails",
                     ar == AAFRESULT_SMALLBUF );
    if( ar != AAFRESULT_SUCCESS )
    {
        s |= CheckError( "Output value does not change if GetName() fails",
                        IsBufferCleared(
                            reinterpret_cast<aafUInt8*>(large_buffer),
                            sizeof(aafCharacter)*large_buffer_size ) == true );
    }


    // GetName should succeed with valid pointer to a name buffer
    // and a valid name buffer size value passed in.
    ar = p_encoding->GetName( large_buffer, name_buf_size );
    s |= CheckError( "GetName() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );
    if( ar != AAFRESULT_SUCCESS )
    {
        // In case GetName fails the name buffer contents
        // should stay unchanged.
        s |= CheckError( "Output value does not change if GetName() fails",
                         IsBufferCleared(
                            reinterpret_cast<aafUInt8*>(large_buffer),
                            sizeof(aafCharacter)*large_buffer_size ) == true );
    }
    else
    {
        assert( IsBufferCleared(
                            reinterpret_cast<aafUInt8*>(large_buffer),
                            name_buf_size ) == false );

        s |= CheckError(
                "Name string returned by GetName() has correct size",
                (wcslen( large_buffer ) * sizeof(aafCharacter)) ==
                (name_buf_size - sizeof(aafCharacter)) );
    }


    return s;
}



static TESTRESULT Test_IAAFFileEncoding_GetDescriptionBufLen(
    IAAFFileEncoding*       p_encoding )
{
    assert( p_encoding != 0 );

    TESTRESULT  s = TR_SUCCEEDED;       // This test status.
    AAFRESULT       ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    aafUInt32       desc_buf_size = MAGIC_UINT32; // Required size of the
                                                  // description buffer.


    // GetDescriptionBufLen should fail with NULL pointer to
    // a buffer size variable passed in.
    ar = p_encoding->GetDescriptionBufLen( 0 );
    s |= CheckError( "GetDescriptionBufLen() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );


    // GetDescriptionBufLen should succeed with valid pointer to a buffer
    // size variable  passed in.
    assert( desc_buf_size == MAGIC_UINT32 );
    ar = p_encoding->GetDescriptionBufLen( &desc_buf_size );
    s |= CheckError( "GetDescriptionBufLen() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );

    if( ar != AAFRESULT_SUCCESS )
    {
        // In case GetDescriptionBufLen fails the desc_buf_size variable value
        // should stay unchanged.
        s |= CheckError(
                "Output value does not change if GetDescriptionBufLen() fails",
                desc_buf_size == MAGIC_UINT32 );
    }
    else
    {
        assert( desc_buf_size != MAGIC_UINT32 );
    }


    return s;
}



static TESTRESULT Test_IAAFFileEncoding_GetDescription(
    IAAFFileEncoding*       p_encoding )
{
    assert( p_encoding != 0 );

    TESTRESULT  s = TR_SUCCEEDED;       // This test status.
    AAFRESULT       ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    aafUInt32       desc_buf_size = MAGIC_UINT32;

    const aafUInt32 large_buffer_size = 4096;
    aafCharacter    large_buffer[ large_buffer_size ];
    ClearBuffer( reinterpret_cast<aafUInt8*>(large_buffer),
                 sizeof(aafCharacter)*large_buffer_size );


    // GetDescriptionBufLen is assumed to be working correctly.
    ar = p_encoding->GetDescriptionBufLen( &desc_buf_size );
    assert( ar == AAFRESULT_SUCCESS );


    // Make sure large_buffer_size is large enough. If the assertion fails
    // large_buffer_size value needs to be incremented.
    assert( desc_buf_size <= (large_buffer_size * sizeof(aafCharacter)) );


    // GetDescription should fail with NULL pointer to a
    // description buffer passed in.
    ar = p_encoding->GetDescription( 0, desc_buf_size );
    s |= CheckError( "GetDescription() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );


    // GetDescription should fail when smaller than required buffer
    // size is passed in. The output value should not change.
    ar = p_encoding->GetDescription( large_buffer, desc_buf_size-1 );
    s |= CheckError( "GetDescription() with invalid arguments fails",
                     ar == AAFRESULT_SMALLBUF );
    if( ar != AAFRESULT_SUCCESS )
    {
        s |= CheckError(
            "Output value does not change if GetDescription() fails",
            IsBufferCleared(
                reinterpret_cast<aafUInt8*>(large_buffer),
                sizeof(aafCharacter)*large_buffer_size ) == true );
    }


    // GetDescription should succeed with valid pointer to a description
    // buffer and a valid description buffer size value passed in.
    ar = p_encoding->GetDescription( large_buffer, desc_buf_size );
    s |= CheckError( "GetDescription() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );
    if( ar != AAFRESULT_SUCCESS )
    {
        // In case GetDescription fails the description buffer contents
        // should stay unchanged.
        s |= CheckError(
            "Output value does not change if GetDescription() fails",
                IsBufferCleared(
                    reinterpret_cast<aafUInt8*>(large_buffer),
                    sizeof(aafCharacter)*large_buffer_size ) == true );
    }
    else
    {
        assert( IsBufferCleared(
                            reinterpret_cast<aafUInt8*>(large_buffer),
                            desc_buf_size ) == false );

        s |= CheckError(
            "Description string returned by GetDescription() has correct size",
            (wcslen( large_buffer ) * sizeof(aafCharacter)) ==
             (desc_buf_size - sizeof(aafCharacter)) );
    }


    return s;
}



static TESTRESULT Test_IAAFFileEncoding_GetFileKind(
    IAAFFileEncoding*       p_encoding )
{
    assert( p_encoding != 0 );

    TESTRESULT  s = TR_SUCCEEDED;       // This test status.
    AAFRESULT       ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    aafUID_t        file_kind = MAGIC_UID;


    // GetFileKind should fail with NULL pointer to
    // a file kind variable passed in.
    ar = p_encoding->GetFileKind( 0 );
    s |= CheckError( "GetFileKind() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );


    // GetFileKind should succeed with valid pointer to
    // sia file kind variable passed in.
    assert( file_kind == MAGIC_UID );
    ar = p_encoding->GetFileKind( &file_kind );
    s |= CheckError( "GetFileKind() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );

    // In case GetFileKind fails the file_kind variable value
    // should stay unchanged.
    if( ar != AAFRESULT_SUCCESS )
    {
        s |= CheckError(
                "Output value does not change if GetFileKind() fails",
                file_kind == MAGIC_UID );
    }
    else
    {
        assert( file_kind != MAGIC_UID );
    }


    return s;
}



TESTRESULT CheckError( const char* p_msg, bool succeeded )
{
    TESTRESULT  tr = TR_SUCCEEDED;

    if( ! succeeded )
    {
        static bool first_failure = true;

        if( first_failure )
        {
            cerr << endl;
            first_failure = false;
        }

        cout << "    Test failed: " << p_msg << endl;
        tr = TR_FAILED;
    }


    return tr;
}



void ClearBuffer( aafUInt8* p_buffer, aafUInt32 buffer_size )
{
    assert( p_buffer != 0 );
    assert( buffer_size > 0 );


    aafUInt8*       p = p_buffer;
    aafUInt32       i = 0;


    for( i=0; i<buffer_size; i++ )
    {
        *p++ = MAGIC_UCHAR8;
    }
}



bool IsBufferCleared( aafUInt8* p_buffer, aafUInt32 buffer_size )
{
    assert( p_buffer != 0 );
    assert( buffer_size > 0 );


    bool            is_cleared = true;
    aafUInt8*       p = p_buffer;
    aafUInt32       i = 0;


    for( i=0; i<buffer_size; i++ )
    {
        if( *p++ != MAGIC_UCHAR8 )
        {
            is_cleared = false;
            break;
        }
    }


    return is_cleared;
}




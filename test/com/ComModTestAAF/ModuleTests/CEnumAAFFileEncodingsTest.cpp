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
extern "C" HRESULT CEnumAAFFileEncodings_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

// IEnumAAFFileEncodings test subroutines
static AAFRESULT  Test_IEnumAAFFileEncodings_All();
static TESTRESULT Test_IEnumAAFFileEncodings_NextOne();
static TESTRESULT Test_IEnumAAFFileEncodings_Next();
static TESTRESULT Test_IEnumAAFFileEncodings_Skip();
static TESTRESULT Test_IEnumAAFFileEncodings_Reset();
static TESTRESULT Test_IEnumAAFFileEncodings_Clone();

// IEnumAAFFileEncodings::Next() test subroutines
static TESTRESULT Test_IEnumAAFFileEncodings_Next_WithInvalidArguments(
    IEnumAAFFileEncodings*, const int );
static TESTRESULT Test_IEnumAAFFileEncodings_Next_WithValidArguments(
    IEnumAAFFileEncodings*, const int, const int );

// Returns the number of elements in the enumeration.
// Moves the iterator to the end of enumeration.
// Uses NextOne() method.
static int CountRemainingElements( IEnumAAFFileEncodings* p_enum );

// Misc. test utilities.
TESTRESULT CheckError( const char* p_msg, bool succeeded );
bool operator ==( const aafUID_t uid1, const aafUID_t uid2 );
bool operator !=( const aafUID_t uid1, const aafUID_t uid2 );
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
HRESULT CEnumAAFFileEncodings_test(
    testMode_t,
    aafUID_t,
    testRawStorageType_t,
    aafProductIdentification_t)
{
    HRESULT                 ar = AAFRESULT_SUCCESS;


    try
    {
        ar = Test_IEnumAAFFileEncodings_All();
    }
    catch( ... )
    {
        cerr << "CEnumAAFFileEncodings_test...Caught general C++"
             << " exception!" << endl; 
        ar = AAFRESULT_TEST_FAILED;
    }


    return ar;
}



static AAFRESULT Test_IEnumAAFFileEncodings_All()
{
    TESTRESULT      s = TR_SUCCEEDED;
    
    // The order of calls is important because some test routines
    // may use methods which are assumed to be working.
    s |= Test_IEnumAAFFileEncodings_NextOne();
    s |= Test_IEnumAAFFileEncodings_Reset();
    s |= Test_IEnumAAFFileEncodings_Next();
    s |= Test_IEnumAAFFileEncodings_Skip();
    s |= Test_IEnumAAFFileEncodings_Clone();


    return (s == TR_SUCCEEDED ? AAFRESULT_SUCCESS : AAFRESULT_TEST_FAILED);
}



static TESTRESULT Test_IEnumAAFFileEncodings_NextOne()
{
    TESTRESULT          s = TR_SUCCEEDED;       // This test status.
    AAFRESULT           ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    IEnumAAFFileEncodings*  p_enum_encodings = 0;


    //
    // Attempt to get an enumeration of
    // available file encodings.
    //
    ar = AAFGetFileEncodings( &p_enum_encodings );
    if( CheckError( "AAFGetFileEncodings() succeeds",
                     ar == AAFRESULT_SUCCESS ) != TR_SUCCEEDED )
    {
        return TR_FAILED;
    }


    // NextOne should fail with NULL pointer as an address of returned
    // element passed in.
    ar = p_enum_encodings->NextOne( 0 );
    s |= CheckError( "NextOne() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );


    int  found_encodings_count = 0; // Number of fetched encodings
    ar = AAFRESULT_SUCCESS;
    while( ar == AAFRESULT_SUCCESS )
    {
        IAAFFileEncoding*   p_encoding =
            reinterpret_cast<IAAFFileEncoding*>( MAGIC_POINTER );


        ar = p_enum_encodings->NextOne( &p_encoding );

        if( ar == AAFRESULT_SUCCESS )
        {
            assert( p_encoding !=
                    reinterpret_cast<IAAFFileEncoding*>( MAGIC_POINTER ) );

            // Delete retrieved encoding
            ReleaseCOMObject( p_encoding );

            found_encodings_count++;
        }
        else
        {
            // AAFRESULT_NO_MORE_OBJECTS is the only error vaule
            // returned by NextOne call with valid arguments.
            s |= CheckError( "NextOne() with valid arguments succeeds",
                             ar == AAFRESULT_NO_MORE_OBJECTS );


            // In case NextOne fails the value of output pointer
            // should stay unchanged.
            s |= CheckError(
                    "Output value does not change if NextOne() fails",
                    p_encoding ==
                    reinterpret_cast<IAAFFileEncoding*>( MAGIC_POINTER ) );
        }
    }


    // Iterating past the last object should fail
    // returning AAFRESULT_NO_MORE_OBJECTS.
    if( ar == AAFRESULT_NO_MORE_OBJECTS )
    {
        IAAFFileEncoding*   p_encoding = 0;
        ar = p_enum_encodings->NextOne( &p_encoding );
        s |= CheckError( "NextOne() past the last element fails "
                         "with correct error code",
                         ar == AAFRESULT_NO_MORE_OBJECTS );
    }
    else
    {
        // Couldn't run the test...
        s |= CheckError( "Was able to test call to NextOne() past the "
                         "end of enumeration",
                         false );
    }


    ReleaseCOMObject( p_enum_encodings );


    return s;
}



static TESTRESULT Test_IEnumAAFFileEncodings_Next()
{
    TESTRESULT          s = TR_SUCCEEDED;       // This test status.
    AAFRESULT           ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    IEnumAAFFileEncodings*  p_enum_encodings = 0;


    //
    // Attempt to get an enumeration of
    // available file encodings.
    //
    ar = AAFGetFileEncodings( &p_enum_encodings );
    if( CheckError( "AAFGetFileEncodings() succeeds",
                     ar == AAFRESULT_SUCCESS ) != TR_SUCCEEDED )
    {
        return TR_FAILED;
    }


    //
    // Count enumeration elements.
    //
    p_enum_encodings->Reset();
    const int   encodings_count = CountRemainingElements( p_enum_encodings );
    p_enum_encodings->Reset();
    assert( encodings_count > 0 );



    //
    // Test Next() with invalid arguments passed in.
    //
    p_enum_encodings->Reset();
    s |= Test_IEnumAAFFileEncodings_Next_WithInvalidArguments(
            p_enum_encodings, encodings_count );



    //
    // Test Next() with number of requested
    // elements less than there are present.
    //
    if( encodings_count == 1 )
    {
        s |= CheckError( "Is able to test Next() with less than present "
                         "elements requested",
                         false );
    }
    else
    {
        p_enum_encodings->Reset();
        s |= Test_IEnumAAFFileEncodings_Next_WithValidArguments(
                p_enum_encodings, encodings_count, encodings_count/2 );
    }



    //
    // Test Next() with number of requested
    // elements equal to what's present.
    //
    p_enum_encodings->Reset();
    s |= Test_IEnumAAFFileEncodings_Next_WithValidArguments(
            p_enum_encodings, encodings_count, encodings_count );



    //
    // Test Next() with number of requested
    // elements greater than there are present.
    //
    p_enum_encodings->Reset();
    s |= Test_IEnumAAFFileEncodings_Next_WithValidArguments(
            p_enum_encodings, encodings_count, encodings_count*2 );

    ReleaseCOMObject( p_enum_encodings );



    return s;
}



static TESTRESULT Test_IEnumAAFFileEncodings_Next_WithInvalidArguments(
    IEnumAAFFileEncodings*  p_enum_encodings,
    const int               encodings_count )
{
    assert( p_enum_encodings != 0 );
    assert( encodings_count > 0 );



    TESTRESULT          s = TR_SUCCEEDED;       // This test status.
    AAFRESULT           ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.


    aafUInt32           fetched_encodings_count = MAGIC_UINT32;
    IAAFFileEncoding**  p_fetched_encodings =
                            new IAAFFileEncoding*[ encodings_count ];
    ClearBuffer( reinterpret_cast<aafUInt8*>(p_fetched_encodings),
                 sizeof(IAAFFileEncoding*) * encodings_count );



    //
    // Next() should fail with NULL passed in as the either pointer
    // argument's value or when the requested number of objects is
    // zero.
    //

    // Why encodings_count+1? The third argument is allowed to
    // be NULL if the requested number of objects is 1. Using
    // encodings_count+1 ensures that the value is greater then 1.
    ar = p_enum_encodings->Next( encodings_count+1,
                                 p_fetched_encodings,
                                 0 );
    s |= CheckError( "Next() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );

    ar = p_enum_encodings->Next( encodings_count,
                                 0,
                                 &fetched_encodings_count );
    s |= CheckError( "Next() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );

    ar = p_enum_encodings->Next( 0,
                                 p_fetched_encodings,
                                 &fetched_encodings_count );
    s |= CheckError( "Next() with invalid arguments fails",
                     ar == AAFRESULT_INVALID_PARAM );

    // All of the arguments are invalid, but the error should be
    // returned for the first argument.
    ar = p_enum_encodings->Next( 0, 0, 0 );
    s |= CheckError( "Next() with invalid arguments fails",
                     ar == AAFRESULT_INVALID_PARAM );


    //
    // If Next() fails iterator should stay at the same position.
    //
    int  count = CountRemainingElements( p_enum_encodings );
    s |= CheckError( "Number of remaining enumeration elements "
                     "does not change if Next() fails",
                     count == encodings_count );
    // Need to reset the iterator position because
    // CountRemainingElements changes it.
    s |= CheckError( "The buffer that holds fetched objects "
                     "does not change if Next() fails",
                     count == encodings_count );

    //
    // If Next() fails output values should not change.
    //
    s |= CheckError(
        "The buffer that holds fetched objects "
        "does not change if Next() fails",
        IsBufferCleared( reinterpret_cast<aafUInt8*>(p_fetched_encodings),
                         sizeof(IAAFFileEncoding*) * encodings_count ) );
    s |= CheckError(
        "The number of fetched objects  does not change if Next() fails",
        fetched_encodings_count == MAGIC_UINT32 );



    // If Next() works correctly p_fetched_encodings should not
    // contain any valid pointers to COM objects. If Next()
    // doesn't work correctly some memory may be leaked here.
    delete[] p_fetched_encodings;



    return s;
}



static TESTRESULT Test_IEnumAAFFileEncodings_Next_WithValidArguments(
    IEnumAAFFileEncodings*  p_enum_encodings,
    const int               encodings_count,
    const int               encodings_to_fetch )
{
    assert( p_enum_encodings != 0 );
    assert( encodings_count >= 2 );
    assert( encodings_to_fetch > 0 );



    TESTRESULT          s = TR_SUCCEEDED;       // This test status.
    AAFRESULT           ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.


    aafUInt32           fetched_encodings_count = MAGIC_UINT32;
    IAAFFileEncoding**  p_fetched_encodings = new IAAFFileEncoding*[ encodings_to_fetch ];
    ClearBuffer( reinterpret_cast<aafUInt8*>(p_fetched_encodings),
                 sizeof(IAAFFileEncoding*) * encodings_to_fetch );



    ar = p_enum_encodings->Next( encodings_to_fetch,
                                 p_fetched_encodings,
                                 &fetched_encodings_count );
    s |= CheckError( "Next() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );

    if( ar == AAFRESULT_SUCCESS )
    {
        s |= CheckError(
            "Next() with valid arguments returns valid ouput",
            fetched_encodings_count != MAGIC_UINT32 );

        s |= CheckError(
            "Next() with valid arguments returns valid ouput",
            IsBufferCleared(
                reinterpret_cast<aafUInt8*>(p_fetched_encodings),
                sizeof(IAAFFileEncoding*) * encodings_count ) == false );

        int  count = CountRemainingElements( p_enum_encodings );
        if( encodings_to_fetch < encodings_count )
        {
            s |= CheckError( "The correct number of elements is left "
                             "after Next()",
                             count == (encodings_count - encodings_to_fetch));
        }
        else
        {
            s |= CheckError( "The correct number of elements is left "
                             "after Next()", count == 0 );
        }


        // Relese returned COM objects first making sure they're valid.
        for( aafUInt32 i=0; i<fetched_encodings_count; i++ )
        {
            // Check if p_fetched_encodings[i] is a valid COM object.
            // If it is release it.
            IAAFFileEncoding*  p_tmp = 0;

            try
            {
                if( p_fetched_encodings[i]->QueryInterface(
                        IID_IAAFFileEncoding, (void **)&p_tmp ) ==
                    AAFRESULT_SUCCESS )
                {
                    ReleaseCOMObject( p_fetched_encodings[i] );
                }
                else
                {
                    s |= CheckError( "Objects returned by Next() are "
                                     "valid COM objects",
                                     false );
                }
            }
            catch(...)
            {
                s |= CheckError( "Objects returned by Next() are "
                                 "valid COM objects",
                                 false );
            }

            ReleaseCOMObject( p_tmp );
        }


        // If reached the end of the enum, try to retreive more
        // objects, should return AAFRESULT_NO_MORE_OBJECTS.
        if( encodings_to_fetch >= encodings_count )
        {
            ar = p_enum_encodings->Next( encodings_to_fetch,
                                       p_fetched_encodings,
                                       &fetched_encodings_count );
            s |= CheckError( "Next() with valid arguments succeeds",
                             ar == AAFRESULT_NO_MORE_OBJECTS );

            // One more time...
            ar = p_enum_encodings->Next( encodings_to_fetch,
                                       p_fetched_encodings,
                                       &fetched_encodings_count );
            s |= CheckError( "Next() with valid arguments succeeds",
                             ar == AAFRESULT_NO_MORE_OBJECTS );
        }
    }


    // If Next() works correctly p_fetched_encodings should not
    // contain any valid pointers to COM objects. If Next()
    // doesn't work correctly some memory may be leaked here.
    delete[] p_fetched_encodings;


    return s;
}



static TESTRESULT Test_IEnumAAFFileEncodings_Reset()
{
    TESTRESULT          s = TR_SUCCEEDED;       // This test status.
    AAFRESULT           ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    IEnumAAFFileEncodings*  p_enum_encodings = 0;
    IAAFFileEncoding*       p_encoding = 0;


    //
    // Attempt to get an enumeration of
    // available file encodings.
    //
    ar = AAFGetFileEncodings( &p_enum_encodings );
    if( CheckError( "AAFGetFileEncodings() succeeds",
                     ar == AAFRESULT_SUCCESS ) != TR_SUCCEEDED )
    {
        return TR_FAILED;
    }


    // Count enumeration elements.
    // CountRemainingElements() will also move
    // the iterator to the end of the enumeration.
    const int   encodings_count = CountRemainingElements( p_enum_encodings );
    assert( encodings_count > 0 );


    // Make sure no elements left.
    assert( p_enum_encodings->NextOne( &p_encoding ) ==
            AAFRESULT_NO_MORE_OBJECTS );


    //
    // Now Reset.
    //
    ar = p_enum_encodings->Reset();
    s |= CheckError( "Reset() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );


    //
    // After Reset() the number of elements available for retrieval
    // should be equal to encodings_count
    //
    int  count = CountRemainingElements( p_enum_encodings );
    s |= CheckError( "Reset() successfully moved iterator to the end "
                     "of enumeration",
                     count == encodings_count );



    //
    // Try double-Reset.
    //
    ar = p_enum_encodings->Reset();
    s |= CheckError( "Reset() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );

    ar = p_enum_encodings->Reset();
    s |= CheckError( "Double Reset() succeeds",
                     ar == AAFRESULT_SUCCESS );


    ReleaseCOMObject( p_enum_encodings );


    return s;
}



static TESTRESULT Test_IEnumAAFFileEncodings_Skip()
{
    TESTRESULT          s = TR_SUCCEEDED;       // This test status.
    AAFRESULT           ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.
    IEnumAAFFileEncodings*  p_enum_encodings = 0;


    //
    // Attempt to get an enumeration of
    // available file encodings.
    //
    ar = AAFGetFileEncodings( &p_enum_encodings );
    if( CheckError( "AAFGetFileEncodings() succeeds",
                     ar == AAFRESULT_SUCCESS ) != TR_SUCCEEDED )
    {
        return TR_FAILED;
    }


    //
    // Count enumeration elements.
    //
    p_enum_encodings->Reset();
    const int   encodings_count = CountRemainingElements( p_enum_encodings );
    p_enum_encodings->Reset();
    assert( encodings_count > 0 );



    //
    // Skip should fail when the requested
    // number of elements to skip is zero.
    //
    p_enum_encodings->Reset();
    ar = p_enum_encodings->Skip( 0 );
    s |= CheckError( "Skip() with invalid arguments fails",
                     ar == AAFRESULT_INVALID_PARAM );

    // If Skip fails iterator should stay at the same position.
    int  count = CountRemainingElements( p_enum_encodings );
    s |= CheckError( "Number of remaining enumeration elements "
                     "does not change if Skip() fails",
                     count == encodings_count );



    //
    // Attempt to Skip less elements then there are present.
    //
    if( encodings_count == 1 )
    {
        s |= CheckError( "Is able to test Skip()-ing to the middle "
                         "of enumeration",
                         false );
    }
    else
    {
        p_enum_encodings->Reset();

        assert( encodings_count >= 2 );

        const int   encodings_to_skip = encodings_count / 2;

        ar = p_enum_encodings->Skip( encodings_to_skip );
        s |= CheckError( "Skip() with valid arguments succeeds",
                         ar == AAFRESULT_SUCCESS );

        count = CountRemainingElements( p_enum_encodings );
        s |= CheckError( "The correct number of elements is left "
                         " after Skip()",
                         count == (encodings_count - encodings_to_skip) );
    }



    //
    // Attempt to Skip all elements that are present.
    //
    p_enum_encodings->Reset();

    ar = p_enum_encodings->Skip( encodings_count );
    s |= CheckError( "Skip() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );

    count = CountRemainingElements( p_enum_encodings );
    s |= CheckError( "The correct number of elements is left after Skip()",
                     count == 0 );



    //
    // Attempt to Skip more elements than there are present.
    //
    p_enum_encodings->Reset();

    ar = p_enum_encodings->Skip( encodings_count*2 );
    s |= CheckError( "Skip() with valid arguments succeeds",
                     ar == AAFRESULT_NO_MORE_OBJECTS );

    count = CountRemainingElements( p_enum_encodings );
    s |= CheckError( "The correct number of elements is left "
                     "after Skip()",
                     count == 0 );



    //
    // Double Skip()!
    // Skip past the end of enumeration than
    // attempt to Skip again.
    //
    p_enum_encodings->Reset();

    ar = p_enum_encodings->Skip( encodings_count*2 );
    s |= CheckError( "Skip() with valid arguments succeeds",
                     ar == AAFRESULT_NO_MORE_OBJECTS );

    ar = p_enum_encodings->Skip( 1 );
    s |= CheckError( "Double Skip() fails with correct value",
                     ar == AAFRESULT_NO_MORE_OBJECTS );

    count = CountRemainingElements( p_enum_encodings );
    s |= CheckError( "The correct number of elements is left "
                     "after double Skip()",
                     count == 0 );



    ReleaseCOMObject( p_enum_encodings );


    return s;
}



static TESTRESULT Test_IEnumAAFFileEncodings_Clone()
{
    TESTRESULT          s = TR_SUCCEEDED;       // This test status.
    AAFRESULT           ar = AAFRESULT_SUCCESS; // Ret vals of AAF DM calls.


    //
    // Attempt to get an enumeration of
    // available file encodings.
    //
    IEnumAAFFileEncodings*  p_src_enum = 0;
    ar = AAFGetFileEncodings( &p_src_enum );
    if( CheckError( "AAFGetFileEncodings() succeeds",
                     ar == AAFRESULT_SUCCESS ) != TR_SUCCEEDED )
    {
        return TR_FAILED;
    }


    // Clone should fail with NULL pointer as an address of returned
    // enumeration passed in.
    ar = p_src_enum->Clone( 0 );
    s |= CheckError( "Clone() with invalid arguments fails",
                     ar == AAFRESULT_NULL_PARAM );


    IEnumAAFFileEncodings*  p_dst_enum = 0;
    ar = p_src_enum->Clone( &p_dst_enum );
    s |= CheckError( "Clone() with valid arguments succeeds",
                     ar == AAFRESULT_SUCCESS );


    //
    // Compare the original and cloned enumerations.
    //
    if( ar == AAFRESULT_SUCCESS )
    {
        IAAFFileEncoding*   p_src_encoding = 0;
        IAAFFileEncoding*   p_dst_encoding = 0;
        while( p_src_enum->NextOne( &p_src_encoding ) == AAFRESULT_SUCCESS )
        {
            ar = p_dst_enum->NextOne( &p_dst_encoding );
            s |= CheckError( "Cloned enumeration contains the same number "
                             "of elements as the original enumeration",
                             ar == AAFRESULT_SUCCESS );

            //
            // Compare enumerations elements.
            //
            if( ar == AAFRESULT_SUCCESS )
            {
                aafUID_t    src_file_kind;
                ar = p_src_encoding->GetFileKind( &src_file_kind );
                assert( ar == AAFRESULT_SUCCESS );

                aafUID_t    dst_file_kind;
                ar = p_dst_encoding->GetFileKind( &dst_file_kind );
                assert( ar == AAFRESULT_SUCCESS );


                s |= CheckError( "Cloned enumeration contains elements "
                                 "identical to those in the original "
                                 "enumeration",
                                 src_file_kind == dst_file_kind );

                ReleaseCOMObject( p_dst_encoding );
            }


            ReleaseCOMObject( p_src_encoding );
        }


        //
        // Should be at the end of the cloned enum.
        //
        ar = p_dst_enum->NextOne( &p_dst_encoding );
        s |= CheckError( "Cloned enumeration contains the same number "
                         "of elements as the original enumeration",
                         ar != AAFRESULT_SUCCESS );


        ReleaseCOMObject( p_dst_enum );
    }

    ReleaseCOMObject( p_src_enum );


    return s;
}



// Returns the number of elements left in the enumeration.
// Moves the iterator to the end of enumeration.
// Uses NextOne() method.
static int CountRemainingElements( IEnumAAFFileEncodings* p_enum )
{
    assert( p_enum != 0 );


    IAAFFileEncoding* p_encoding = 0;
    int  count = 0;
    while( p_enum->NextOne( &p_encoding ) == AAFRESULT_SUCCESS )
    {
        count++;
        ReleaseCOMObject( p_encoding );
    }


    assert( count >= 0 );

    return count;
}



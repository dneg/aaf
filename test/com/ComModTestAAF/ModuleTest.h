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

#ifndef MODULE_TEST_COMMON_H
#define MODULE_TEST_COMMON_H

#include "AAF.h"


typedef enum unitTestMode
{
	kAAFUnitTestReadWrite, kAAFUnitTestReadOnly
} testMode_t;


//
// Enum to specify type of raw storage
// to be used for a module test.
//
enum testRawStorageType_t
{
    kAAFNamedFile,
    kAAFDiskRawStorage,
    kAAFCachedDiskRawStorage,
    kAAFMappedFileRawStorage,
    kAAFMemoryRawStorage
};


#ifdef _cplusplus
extern "C" {
#endif

//
// Feature support
//

// Return the current version of the AAF toolkit shared library and
// the version of the toolkit used to create the file.
// NOTE: The parameter fileAAFVersion may be NULL.
HRESULT GetAAFVersions(IAAFHeader * pHeader, 
                       aafProductVersion_t * currentAAFVersion, 
                       aafProductVersion_t * fileAAFVersion);


// Support for Sets (strong reference sets) added in DR4
bool StrongReferenceSetsSupported(aafProductVersion_constref toolkitVersion);

// Support for weak references was added after DR4
bool WeakReferencesSupported(aafProductVersion_constref toolkitVersion);

// Support for weak reference sets was added after DR4
bool WeakReferenceSetsSupported(aafProductVersion_constref toolkitVersion);

// Support for extending AAFObject was added after DR4
bool ExtendingAAFObjectSupported(aafProductVersion_constref toolkitVersion);

// Return true is greater than DR3
bool DR4TestSupported(aafProductVersion_constref toolkitVersion);

// Return true is greater than DR4 (a.k.a. RC1)
bool RC1TestSupported(aafProductVersion_constref toolkitVersion);

//
// AAF files support.
//

// Create an AAF file to be used by module test.
AAFRESULT CreateTestFile( const aafCharacter* p_file_name,
                          aafUID_constref file_kind,
                          const testRawStorageType_t raw_storage_type,
                          IAAFFile** pp_new_file );

// Create an AAF file to be used by module test.
AAFRESULT CreateTestFile( const aafCharacter* p_file_name,
                          aafUID_constref file_kind,
                          const testRawStorageType_t raw_storage_type,
                          aafProductIdentification_constref product_identification,
                          IAAFFile** pp_new_file );

// Create an AAF file to be used by module test.
AAFRESULT CreateTestFile( const aafCharacter* p_file_name,
                          aafUID_constref file_kind,
                          const testRawStorageType_t raw_storage_type,
                          aafUInt32 file_mode_flags,
                          aafProductIdentification_constref product_identification,
                          IAAFFile** pp_new_file );

// Delete a file used by module test.
AAFRESULT RemoveTestFile( const aafCharacter* p_file_name );

// Return product identification to be used by module test.
aafProductIdentification_t MakeProductID();

// Given the name searches for the first file encoding
// with the same name and returns it's ID.
AAFRESULT FindFileEncodingByName( const aafCharacter* p_encoding_name,
                                  aafUID_t* p_encoding_id );

// Genereates a file name based on the passed in
// base name and file encoding specified by file_kind.
// The resulting file name is stored in the buffer
// addressed by the passed in p_generated_name_buffer
// pointer. The number of characters that can fit in
// the buffer (including NULL-terminator) is specified
// by generated_name_buffer_length.
bool GenerateTestFileName( const aafCharacter* p_base_name,
                           const aafUID_t& file_kind,
                           const size_t generated_name_buffer_size,
                           aafCharacter* p_generated_name_buffer );


//
// This value indicates that default file encoding
// should be used to create new AAF file.
//
// The following functions can handle this value:
//  CreateTestFile()
//  GenerateTestFileName()
//
// {94CF281B-179D-4612-B079-0B10CE2B33BC}
//
const aafUID_t testFileKindDefault =
{ 0x94cf281b, 0x179d, 0x4612, { 0xb0, 0x79, 0xb, 0x10, 0xce, 0x2b, 0x33, 0xbc } };


//
// EffectiveTestFileEncoding
//
// The function returns file encoding to be used by AAF Toolkit's file
// creation functions. The passed in encoding ID is either one of the
// valid encoding IDs listed in AAFFileKinds.h or the testFileKindDefault
// ID which means that default file encoding will be used.
//
aafUID_t EffectiveTestFileEncoding( const aafUID_t& encoding );


//
// KLV parsing
//

//#define TEST_CHECK_STREAM_SIZE_ON_RESTORE

// If defined, TEST_NEW_STREAM_WRITING turns on
// the code to accomodate the new KLV stream writing
// mechanism.
//#define TEST_NEW_STREAM_WRITING


// KLV key
struct testKLVKey_t
{
    aafUInt8    octet0;
    aafUInt8    octet1;
    aafUInt8    octet2;
    aafUInt8    octet3;
    aafUInt8    octet4;
    aafUInt8    octet5;
    aafUInt8    octet6;
    aafUInt8    octet7;
    aafUInt8    octet8;
    aafUInt8    octet9;
    aafUInt8    octet10;
    aafUInt8    octet11;
    aafUInt8    octet12;
    aafUInt8    octet13;
    aafUInt8    octet14;
    aafUInt8    octet15;
};

// The KLV keys used in the module tests
const testKLVKey_t testMetadataKey =
    {0x06, 0x0e, 0x2b, 0x34, 0x01, 0x02, 0x01, 0x01,
     0x0e, 0x04, 0x03, 0x01, 0xff, 0xff, 0xff, 0xff};


AAFRESULT WriteKLVKey(
    IAAFPropertyValue* p_property_value,
    const testKLVKey_t& key );
AAFRESULT WriteKLVLength(
    IAAFPropertyValue* p_property_value,
    aafUInt64 length );
AAFRESULT WriteKLVLength(
    IAAFPropertyValue* p_property_value,
    aafInt64 length_position,
    aafUInt64 length );
AAFRESULT ReserveKLVLength(
    IAAFPropertyValue* p_property_value,
    aafInt64* p_length_position );
AAFRESULT FixupKLVLength(
    IAAFPropertyValue* p_property_value,
    aafInt64 length_position );

bool IsKLVStream( IAAFPropertyValue* p_property_value );
AAFRESULT SkipKL( IAAFPropertyValue* p_property_value );



//
// Given an IAAFObject and a property ID
// return the property type and value.
//
AAFRESULT GetPropertyValue(
    IUnknown* pUnknown,
    const aafUID_t& propertyId,
    IAAFPropertyValue** ppPropValue );
AAFRESULT GetPropertyType(
    IUnknown* pUnknown,
    const aafUID_t& propertyId,
    IAAFTypeDef** ppPropTypeDef );



#ifdef _cplusplus
}
#endif


//
// Comparison operators
//
bool operator ==( const aafUID_t uid1, const aafUID_t uid2 );
bool operator !=( const aafUID_t uid1, const aafUID_t uid2 );

bool operator ==( const aafRational_t& a, const aafRational_t& b );
bool operator !=( const aafRational_t& a, const aafRational_t& b );

bool operator ==( const aafTimeStamp_t& a, const aafTimeStamp_t& b );
bool operator !=( const aafTimeStamp_t& a, const aafTimeStamp_t& b );


#endif  // MODULE_TEST_COMMON_H

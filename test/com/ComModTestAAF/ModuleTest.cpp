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
//
// File: ModuleTest.cpp
//
// Implementation for types and utilities shared by two or more module
// tests.
//

#include "AAFWideString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "AAF.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFFileKinds.h"

#include "ModuleTest.h"


#define ReleaseCOMObject( p )  if( p ) {p->Release(); p = 0;}


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



// Create an AAF file to be used by module test.
AAFRESULT CreateTestFile(
    const aafCharacter*                 p_file_name,
    aafUID_constref                     file_kind,
    const testRawStorageType_t          raw_storage_type,
    aafUInt32                           file_mode_flags,
    aafProductIdentification_constref   product_identification,
    IAAFFile**                          pp_new_file )
{
  assert( p_file_name != 0 );
  assert( pp_new_file != 0 );


  //
  // Determine the file encoding to use.
  //
  aafUID_t use_file_kind = EffectiveTestFileEncoding( file_kind );


  //
  // Create a raw storage object if necessary.
  //
  AAFRESULT  hr = AAFRESULT_SUCCESS;
  IAAFRawStorage* p_raw_storage = 0;
  if( raw_storage_type == kAAFNamedFile )
  {
    p_raw_storage = 0;
    hr = AAFRESULT_SUCCESS;
  }
  else if( raw_storage_type == kAAFDiskRawStorage )
  {
    // Attempt to create an IAAFRawStorage object
    // of the specified type.
    hr = AAFCreateRawStorageDisk( p_file_name,
                                  kAAFFileExistence_new,
                                  kAAFFileAccess_modify,
                                  &p_raw_storage );
  }
  else if( raw_storage_type == kAAFCachedDiskRawStorage )
  {
    hr = AAFRESULT_NOT_IMPLEMENTED;
  }
  else if( raw_storage_type == kAAFMappedFileRawStorage )
  {
    hr = AAFRESULT_NOT_IMPLEMENTED;
  }
  else if( raw_storage_type == kAAFMemoryRawStorage )
  {
    hr = AAFRESULT_NOT_IMPLEMENTED;
  }
  else
  {
    // The raw storage type is not supported by the toolkit
    hr = AAFRESULT_INVALID_PARAM;
  }


  //
  // Create a file object.
  //
  if( hr == AAFRESULT_SUCCESS )
  {
    //
    // Create a file object.
    //
    IAAFFile*  p_file = 0;
    if( p_raw_storage != 0 )
    {
      // Use raw storage API to create a file.
      hr = AAFCreateAAFFileOnRawStorage( p_raw_storage,
                                         kAAFFileExistence_new,
                                         kAAFFileAccess_modify,
                                         &use_file_kind,
                                         file_mode_flags,
                                         &product_identification,
                                         &p_file );
      p_raw_storage->Release();
      p_raw_storage = 0;

      // Open the file.
      if( hr == AAFRESULT_SUCCESS )
      {
        hr = p_file->Open();

        if( hr != AAFRESULT_SUCCESS )
        {
          p_file->Release();
          p_file = 0;
        }
      }
    }
    else
    {
      // Use 'traditional' API to create a file.
      hr = AAFFileOpenNewModifyEx(
        p_file_name,
        &use_file_kind,
        file_mode_flags,
        const_cast<aafProductIdentification_t*>( &product_identification ),
        &p_file );
    }


    if( hr == AAFRESULT_SUCCESS )
    {
      assert( p_file != 0 );
      *pp_new_file = p_file;
    }
  }


  return hr;
}



// Create an AAF file to be used by module test.
AAFRESULT CreateTestFile(
    const aafCharacter*                 p_file_name,
    aafUID_constref                     file_kind,
    const testRawStorageType_t          raw_storage_type,
    aafProductIdentification_constref   product_identification,
    IAAFFile**                          pp_new_file )
{
  assert( p_file_name != 0 );
  assert( pp_new_file != 0 );


  return CreateTestFile( p_file_name,
                         file_kind,
                         raw_storage_type,
                         0,
                         product_identification,
                         pp_new_file );
}



// Create an AAF file to be used by module test.
AAFRESULT CreateTestFile(
    const aafCharacter*         p_file_name,
    aafUID_constref             file_kind,
    const testRawStorageType_t  raw_storage_type,
    IAAFFile**                  pp_new_file )
{
  assert( p_file_name != 0 );
  assert( pp_new_file != 0 );


  aafProductIdentification_t    product_identification = MakeProductID();

  return CreateTestFile( p_file_name,
                         file_kind,
                         raw_storage_type,
                         product_identification,
                         pp_new_file );
}



// Delete a file used by module test.
AAFRESULT RemoveTestFile( const aafCharacter* p_file_name )
{
  assert( p_file_name != 0 );

  AAFRESULT     hr = AAFRESULT_SUCCESS;
  const size_t  file_name_length = wcslen( p_file_name );
  char*         p_char_buf = new char[ file_name_length+1 ];


  if( wcstombs( p_char_buf, p_file_name, file_name_length+1 ) == (size_t)-1 )
  {
    hr = AAFRESULT_INTERNAL_ERROR;
  }
  else
  {
    if( remove( p_char_buf ) == -1 )
    {
      hr = AAFRESULT_INTERNAL_ERROR;
    }
    else
    {
      hr = AAFRESULT_SUCCESS;
    }
  }

  delete[] p_char_buf;


  return hr;
}



#if defined( OS_MACOS )
#define TEST_PLATFORM_NAME		L"MacOS"
#elif defined( OS_WINDOWS )
#define TEST_PLATFORM_NAME		L"Win32"
#elif defined( OS_UNIX )
#define TEST_PLATFORM_NAME		L"Unix"
#else
#define TEST_PLATFORM_NAME		L"Unknown"
#endif



aafProductIdentification_t MakeProductID()
{
    static aafProductVersion_t  product_version;

    product_version.major = 1;
    product_version.minor = 0;
    product_version.tertiary = 0;
    product_version.patchLevel = 0;
    product_version.type = kAAFVersionUnknown;


    static aafCharacter    company_name[] = L"AAF Developers Desk";
    static aafCharacter    product_name[] = L"ComModTestAAF";
    static aafCharacter    product_version_string[] = L"1.0.0.0";
    static aafCharacter    platform[] = TEST_PLATFORM_NAME;
    aafProductIdentification_t  product_identification;
 
    product_identification.companyName = company_name;
    product_identification.productName = product_name;
    product_identification.productVersion = &product_version;
    product_identification.productVersionString = product_version_string;
    product_identification.productID = UnitTestProductID;
    product_identification.platform = platform;


    return product_identification;
}



// Given the name searches for the first file encoding
// with the same name and returns it's ID.
AAFRESULT FindFileEncodingByName(
    const aafCharacter* p_encoding_name,
    aafUID_t*           p_encoding_id )
{
    const aafUInt32     encoding_name_length = wcslen( p_encoding_name );
    const aafUInt32     name_buf_size = (encoding_name_length + 1) *
                                        sizeof(aafCharacter);
    aafCharacter*       p_name_buf = new aafCharacter[encoding_name_length+1 ];
    AAFRESULT           ar = AAFRESULT_SUCCESS;


    try
    {
        // Get an enumeration of available file encodings.
        IEnumAAFFileEncodings*  p_enum_encodings = 0;
        ar = AAFGetFileEncodings( &p_enum_encodings );

        if( ar == AAFRESULT_DLL_SYMBOL_NOT_FOUND )
        {
            // Handle the case of using an old version of the AAF DLL
            // which doesn't support enumeration over supported file
            // encodings (AAFGetFileEncodings, IAAFFileEncoding, and
            // IEnumAAFFileEncodings APIs).
            // Old versions of the toolkit always provide
            // support for MS Structured Storage encoding.
            if( wcscmp( p_encoding_name, L"AAF" ) == 0 )
            {
                *p_encoding_id = aafFileKindAafSSBinary;
                ar = AAFRESULT_SUCCESS;
            }
            else
            {
                ar = AAFRESULT_NO_MORE_OBJECTS;
            }
        }
        else if( ar == AAFRESULT_SUCCESS )
        {
            // Iterate over available file encodings searching
            // for the one with the specified name.
            bool  found = false;
            IAAFFileEncoding*  p_encoding = 0;

            while( found == false  &&
                   p_enum_encodings->NextOne( &p_encoding ) ==
                   AAFRESULT_SUCCESS )
            {
                // Get the name of the next encoding and
                // compare it to the requested name.
                aafUInt32  next_name_buf_size = 0;
                p_encoding->GetNameBufLen( &next_name_buf_size );

                if( next_name_buf_size == name_buf_size )
                {
                    *p_name_buf = L'\0';

                    ar = p_encoding->GetName( p_name_buf, name_buf_size );
                    if( ar == AAFRESULT_SUCCESS )
                    {
                        if( wcscmp( p_name_buf, p_encoding_name ) == 0 )
                        {
                            // Names match - get the ID of found
                            // file encoding
                            p_encoding->GetFileKind( p_encoding_id );
                            found = true;
                        }
                    }
                }

                p_encoding->Release();
                p_encoding = 0;
            }


            // If the requested encoding is not found and
            // no error occured during search set the return
            // value to indicate success.
            if( found == false )
            {
                if( ar == AAFRESULT_SUCCESS )
                {
                    ar = AAFRESULT_NO_MORE_OBJECTS;
                }
            }
            else
            {
	            }
        }
        if (p_enum_encodings != 0)
        {
            p_enum_encodings->Release();
        }
        p_enum_encodings = 0;
    }
    catch(...)
    {
        ar = AAFRESULT_INTERNAL_ERROR;
    }


    delete[] p_name_buf;
    p_name_buf = 0;


    return ar;
}



// Given the ID searches for the first
// file encoding with the same ID.
AAFRESULT FindFileEncoding(
    const aafUID_t&     encoding_id,
    IAAFFileEncoding**  pp_encoding )
{
    AAFRESULT           ar = AAFRESULT_SUCCESS;
    IAAFFileEncoding*   p_found_encoding = 0;


    try
    {
        // Get an enumeration of available file encodings.
        IEnumAAFFileEncodings*  p_enum_encodings = 0;
        ar = AAFGetFileEncodings( &p_enum_encodings );

        if( ar == AAFRESULT_DLL_SYMBOL_NOT_FOUND )
        {
            // Handle the case of using an old version of the AAF DLL
            // which doesn't support enumeration over supported file
            // encodings (AAFGetFileEncodings, IAAFFileEncoding, and
            // IEnumAAFFileEncodings APIs).
        }
        else if( ar == AAFRESULT_SUCCESS )
        {
            // Iterate over available file encodings searching
            // for the one with the specified ID.
            bool  found = false;
            IAAFFileEncoding*  p_next_encoding = 0;

            while( found == false  &&
                   p_enum_encodings->NextOne( &p_next_encoding ) ==
                   AAFRESULT_SUCCESS )
            {
                // Get the ID of the next encoding and
                // compare it to the requested ID.
                aafUID_t  next_encoding_id;
                p_next_encoding->GetFileKind( &next_encoding_id );

                if( memcmp( reinterpret_cast<void*>(
                                const_cast<aafUID_t*>(&encoding_id)),
                            reinterpret_cast<void*>(&next_encoding_id),
                            sizeof(aafUID_t) ) == 0 )
                {
                    found = true;
                    p_found_encoding = p_next_encoding;
                    p_found_encoding->AddRef();
                }

                p_next_encoding->Release();
                p_next_encoding = 0;
            }


            // If the requested encoding is not found and
            // no error occured during search set the return
            // value to indicate success.
            if( found == false )
            {
                if( ar == AAFRESULT_SUCCESS )
                {
                    ar = AAFRESULT_NO_MORE_OBJECTS;
                }
            }
            else
            {
                assert( ar == AAFRESULT_SUCCESS );
            }
        }
        if (p_enum_encodings != 0)
        {
            p_enum_encodings->Release();
        }
        p_enum_encodings = 0;
    }
    catch(...)
    {
        ar = AAFRESULT_INTERNAL_ERROR;
    }


    if( ar == AAFRESULT_SUCCESS )
    {
        assert( p_found_encoding != 0 );
        *pp_encoding = p_found_encoding;
    }


    return ar;
}



AAFRESULT FindFileEncodingName(
    const aafUID_t&     encoding_id,
    aafCharacter*       p_encoding_name_buf,
    aafUInt32           encoding_name_buf_size )
{
    AAFRESULT           ar = AAFRESULT_SUCCESS;
    IAAFFileEncoding*   p_encoding = 0;


    ar = FindFileEncoding( encoding_id, &p_encoding );

    if( ar == AAFRESULT_SUCCESS )
    {
        ar = p_encoding->GetName( p_encoding_name_buf,
                                  encoding_name_buf_size );

        p_encoding->Release();
        p_encoding = 0;
    }


    return ar;
}



// Genereates a file name based on the passed in
// base name and file encoding specified by file_kind.
// The resulting file name is stored in the buffer
// addressed by the passed in p_generated_name_buffer
// pointer. The number of characters that can fit in
// the buffer (including NULL-terminator) is specified
// by generated_name_buffer_length.
bool GenerateTestFileName(
    const aafCharacter* p_base_name,
    const aafUID_t& file_kind,
    const size_t generated_name_buffer_length,
    aafCharacter* p_generated_name_buffer )
{
    assert( p_base_name != 0 );
    assert( wcslen( p_base_name ) != 0 );
    assert( generated_name_buffer_length > 1 );
    assert( p_generated_name_buffer != 0 );


    //
    // 1. Base name
    //
    const size_t  base_name_length = wcslen( p_base_name );



    //
    // 3. Encoding name (skip if using the default encoding)
    //
    const size_t  max_encoding_name_length = 63;
    const size_t  encoding_name_buffer_size =
                    (max_encoding_name_length+1) * sizeof(aafCharacter);
    aafCharacter  encoding_name_buffer[ max_encoding_name_length+1 ] = L"";

    // If the file encoding is specified (not default encoding)
    // add encoding name to the generated file name. Otherwise
    // use default encoding and do not change the generated name.
    if( memcmp(&file_kind, &testFileKindDefault, sizeof(aafUID_t)) != 0 )
    {
        FindFileEncodingName( file_kind,
                              encoding_name_buffer,
                              encoding_name_buffer_size);
    }

    // If FindFileEncodingName() fails the
    // encoding_name_buffer does not change
    // still containing an empty string.
    size_t  encoding_name_length = wcslen( encoding_name_buffer );



    //
    // 2. Separator between file name and encoding name
    //
    aafCharacter  name_encoding_separator[2] = { L'\0', L'\0' };
    if( encoding_name_length != 0 )
    {
        name_encoding_separator[0] = L'.';
    }

    const size_t        name_encoding_separator_length =
                            wcslen( name_encoding_separator );



    //
    //  4. Extension
    //
    const aafCharacter  extension[] = L".aaf";
    const size_t extension_length = wcslen( extension );



    //
    // Putting it all together
    //
    const size_t  file_name_length = base_name_length +
                                     name_encoding_separator_length +
                                     encoding_name_length +
                                     extension_length;

    bool succeeded = true;

    if( file_name_length < generated_name_buffer_length )
    {
        wcscpy( p_generated_name_buffer, p_base_name );
        wcscat( p_generated_name_buffer, name_encoding_separator );
        wcscat( p_generated_name_buffer, encoding_name_buffer );
        wcscat( p_generated_name_buffer, extension );

        succeeded = true;
    }
    else
    {
        succeeded = false;
    }


    return succeeded;
}



//
// EffectiveTestFileEncoding
//
// The function returns file encoding to be used by AAF Toolkit's file
// creation functions. The passed in encoding ID is either one of the
// valid encoding IDs listed in AAFFileKinds.h or the testFileKindDefault
// ID which means that default file encoding will be used.
//
aafUID_t EffectiveTestFileEncoding( const aafUID_t& encoding )
{
    // Default encoding
    aafUID_t  effective_encoding = aafFileKindAafSSBinary;


    // If passed in encoding ID does not ask for default
    // encoding to be used (testFileKindDefault) use
    // the passed in ID.
    if( memcmp(&encoding, &testFileKindDefault, sizeof(aafUID_t)) != 0 )
    {
        effective_encoding = encoding;
    }


    return effective_encoding;
}



AAFRESULT GetPropertyValue(
    IUnknown* pUnknown,
    const aafUID_t& propertyId,
    IAAFPropertyValue** ppPropValue )
{
    AAFRESULT hr = AAFRESULT_SUCCESS;


    IAAFObject*  pObject = 0;
    hr = pUnknown->QueryInterface( IID_IAAFObject, (void **)&pObject );
    if( hr == AAFRESULT_SUCCESS )
    {
        IAAFClassDef*  pClassDef = 0;
        hr = pObject->GetDefinition( &pClassDef );
        if( hr == AAFRESULT_SUCCESS )
        {
            IAAFPropertyDef* pPropDef = 0;
            hr = pClassDef->LookupPropertyDef( propertyId, &pPropDef );
            if( hr == AAFRESULT_SUCCESS )
            {
                hr = pObject->GetPropertyValue( pPropDef, ppPropValue );

                pPropDef->Release();
                pPropDef = 0;
            }

            pClassDef->Release();
            pClassDef = 0;
        }

        pObject->Release();
        pObject = 0;
    }


    return hr;
}



AAFRESULT GetPropertyType(
    IUnknown* pUnknown,
    const aafUID_t& propertyId,
    IAAFTypeDef** ppPropTypeDef )
{
    AAFRESULT hr = AAFRESULT_SUCCESS;


    IAAFObject*  pObject = 0;
    hr = pUnknown->QueryInterface( IID_IAAFObject, (void **)&pObject );
    if( hr == AAFRESULT_SUCCESS )
    {
        IAAFClassDef*  pClassDef = 0;
        hr = pObject->GetDefinition( &pClassDef );
        if( hr == AAFRESULT_SUCCESS )
        {
            IAAFPropertyDef* pPropDef = 0;
            hr = pClassDef->LookupPropertyDef( propertyId, &pPropDef );
            if( hr == AAFRESULT_SUCCESS )
            {
                hr = pPropDef->GetTypeDef( ppPropTypeDef );

                pPropDef->Release();
                pPropDef = 0;
            }

            pClassDef->Release();
            pClassDef = 0;
        }

        pObject->Release();
        pObject = 0;
    }


    return hr;
}



bool operator ==( const aafUID_t uid1, const aafUID_t uid2 )
{
    bool    are_equal = true;
    int     i = 0;

    for( i=0; i<8; i++ )
    {
        if( uid1.Data4[i] != uid2.Data4[i] )
        {
            are_equal = false;
            break;
        }
    }

    if( are_equal == true )
    {
        if( uid1.Data1 != uid2.Data1  ||
            uid1.Data2 != uid2.Data2  ||
            uid1.Data3 != uid2.Data3 )
        {
            are_equal = false;
        }
    }


    return are_equal;
}



bool operator !=( const aafUID_t uid1, const aafUID_t uid2 )
{
    return (! operator==( uid1, uid2 ) );
}



bool operator ==( const aafRational_t& a, const aafRational_t& b )
{
    bool  are_equal = true;

    if( a.numerator != b.numerator  ||  a.denominator != b.denominator )
    {
        are_equal = false;
    }


    return are_equal;
}



bool operator !=( const aafRational_t& a, const aafRational_t& b )
{
    return (! operator==( a, b ) );
}



bool operator ==( const aafTimeStamp_t& a, const aafTimeStamp_t& b )
{
    bool  are_equal = true;

    if( a.date.year != b.date.year  ||
        a.date.month != b.date.month  ||
        a.date.day != b.date.day  ||
        a.time.hour != b.time.hour  ||
        a.time.minute != b.time.minute  ||
        a.time.second != b.time.second  ||
        a.time.fraction != b.time.fraction )
    {
        are_equal = false;
    }


    return are_equal;
}



bool operator !=( const aafTimeStamp_t& a, const aafTimeStamp_t& b )
{
    return (! operator==( a, b ) );
}



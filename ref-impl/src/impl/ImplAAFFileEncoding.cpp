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


#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"

#include "OMStoredObjectFactory.h"
#include "OMUtilities.h"

#include "ImplAAFFileEncoding.h"



ImplAAFFileEncoding::ImplAAFFileEncoding() :
    ImplAAFRoot(),

    _p_encoding( 0 )
{
}



ImplAAFFileEncoding::~ImplAAFFileEncoding()
{
}



AAFRESULT STDMETHODCALLTYPE ImplAAFFileEncoding::GetName(
    aafCharacter*   pNameBuf,
    aafUInt32       nameBufSize ) const
{
    //
    // Is object initialized?
    //
    if( ! isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }

    //
    // Check input arguments
    //
    if( pNameBuf == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }

    aafUInt32   required_name_buffer_size = 0;
    GetNameBufLen( &required_name_buffer_size );
    if( nameBufSize < required_name_buffer_size )
    {
        return AAFRESULT_SMALLBUF;
    }


    copyWideString( pNameBuf, _p_encoding->name() );


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFFileEncoding::GetNameBufLen(
    aafUInt32*      pNameBufSize ) const
{
    //
    // Is object initialized?
    //
    if( ! isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }

    //
    // Check input arguments
    //
    if( pNameBufSize == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }


    size_t  name_buffer_size =
        (lengthOfWideString(_p_encoding->name()) + 1) * sizeof(wchar_t);


    //
    // Set return value.
    //
    *pNameBufSize = static_cast<aafUInt32>( name_buffer_size );


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFFileEncoding::GetDescription(
    aafCharacter*   pDescriptionBuf,
    aafUInt32       descriptionBufSize ) const
{
    //
    // Is object initialized?
    //
    if( ! isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }

    //
    // Check input arguments
    //
    if( pDescriptionBuf == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }

    aafUInt32   required_desc_buffer_size = 0;
    GetDescriptionBufLen( &required_desc_buffer_size );
    if( descriptionBufSize < required_desc_buffer_size )
    {
        return AAFRESULT_SMALLBUF;
    }


    copyWideString( pDescriptionBuf, _p_encoding->description() );


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFFileEncoding::GetDescriptionBufLen(
    aafUInt32*      pDescriptionBufSize ) const
{
    //
    // Is object initialized?
    //
    if( ! isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }

    //
    // Check input arguments
    //
    if( pDescriptionBufSize == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }


    size_t  desc_buffer_size =
        (lengthOfWideString(_p_encoding->description()) + 1) * sizeof(wchar_t);


    //
    // Set return value.
    //
    *pDescriptionBufSize = static_cast<aafUInt32>( desc_buffer_size );


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFFileEncoding::GetFileKind(
    aafUID_t*       pFileKind ) const
{
    //
    // Is object initialized?
    //
    if( ! isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }

    //
    // Check input arguments
    //
    if( pFileKind == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }


    OMStoredObjectEncoding  om_encoding = _p_encoding->encoding();
    *pFileKind = *(reinterpret_cast<aafUID_t*>( &om_encoding ));
    


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFFileEncoding::Initialize(
    OMStoredObjectFactory*  pEncoding )
{
    //
    // Is object already initialized?
    //
    if( isInitialized() )
    {
        return AAFRESULT_ALREADY_INITIALIZED;
    }

    //
    // Check input arguments
    //
    if( pEncoding == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }


    _p_encoding = pEncoding;

    setInitialized();


    return AAFRESULT_SUCCESS;
}



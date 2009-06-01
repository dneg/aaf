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


#include "AAFTypes.h"
#include "AAFResult.h"

#include "OMSetIterator.h"

#include "ImplAAFObjectCreation.h"
#include "ImplAAFFileEncoding.h"
#include "ImplEnumAAFFileEncodings.h"



extern "C" const aafClassID_t CLSID_EnumAAFFileEncodings;
extern "C" const aafClassID_t CLSID_AAFFileEncoding;



ImplEnumAAFFileEncodings::ImplEnumAAFFileEncodings() :
    ImplAAFRoot(),

    _factory_set_iterator( *OMFile::factories() )
{
    setInitialized();
}



ImplEnumAAFFileEncodings::~ImplEnumAAFFileEncodings()
{
    delete &_factory_set_iterator;
}



AAFRESULT STDMETHODCALLTYPE ImplEnumAAFFileEncodings::NextOne(
    ImplAAFFileEncoding**   ppFileEncoding )
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
    if( ppFileEncoding == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }


    //
    // Attempt to retrieve the next element.
    //
    AAFRESULT               ar = AAFRESULT_SUCCESS;
    ImplAAFFileEncoding*    p_encoding = 0;

    if( _factory_set_iterator.after() )
    {
        // The iterator is already past the end of the set.
        ar = AAFRESULT_NO_MORE_OBJECTS;
    }
    else
    {
        if( ++_factory_set_iterator )
        {
            // Create an object
            p_encoding = dynamic_cast<ImplAAFFileEncoding*>(
                CreateImpl( CLSID_AAFFileEncoding ) );

            if( p_encoding != 0 )
            {
                ar = p_encoding->Initialize( _factory_set_iterator.value() );
            }
            else
            {
                ar = E_FAIL;
            }
        }
        else
        {
            ar = AAFRESULT_NO_MORE_OBJECTS;
        }
    }


    //
    // Set return value, if succeeded.
    //
    if( ar == AAFRESULT_SUCCESS )
    {
        *ppFileEncoding = p_encoding;
    }


    return ar;
}



AAFRESULT STDMETHODCALLTYPE ImplEnumAAFFileEncodings::Next(
    aafUInt32               count,
    ImplAAFFileEncoding**   pFileEncodings,
    aafUInt32*              pNumFetched )
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
    if( count == 0 )
    {
        return AAFRESULT_INVALID_PARAM;
    }

    if( pFileEncodings == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }

    if( count > 1 )
    {
        if( pNumFetched == 0 )
        {
            return AAFRESULT_NULL_PARAM;
        }
    }


    //
    // Attempt to retrieve requested number of elements,
    // one by one.
    //
    AAFRESULT               ar = AAFRESULT_SUCCESS;
    aafUInt32               fetched_count = 0;
    ImplAAFFileEncoding**   pp_next_encoding = pFileEncodings;

    while( fetched_count < count  &&
           ar == AAFRESULT_SUCCESS )
    {
        ar = NextOne( pp_next_encoding );
        if( ar == AAFRESULT_SUCCESS )
        {
            fetched_count++;
            pp_next_encoding++;
        }
    }


    //
    // AAFRESULT_NO_MORE_OBJECTS is only returned when there are
    // no enumeration elements left at all. If the number of
    // elements available is less than requested but not zero
    // AAFRESULT_SUCCESS is returned with the *pNumFetched value
    // indicating the number of elements actually retrieved.
    //
    if( ar == AAFRESULT_NO_MORE_OBJECTS  &&  fetched_count > 0 )
    {
        ar = AAFRESULT_SUCCESS;
    }


    //
    // Set return value
    //
    if( ar == AAFRESULT_SUCCESS  ||  ar == AAFRESULT_NO_MORE_OBJECTS )
    {
        if( pNumFetched != 0 )
        {
            *pNumFetched = fetched_count;
        }
    }


    return ar;
}



AAFRESULT STDMETHODCALLTYPE ImplEnumAAFFileEncodings::Skip(
    aafUInt32           count )
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
    if( count == 0 )
    {
        return AAFRESULT_INVALID_PARAM;
    }


    //
    // Advance the iterator by no more than
    // number_to_skip.
    //
    AAFRESULT               ar = AAFRESULT_SUCCESS;


    if( _factory_set_iterator.after() )
    {
        // The iterator is already past the end of the set.
        ar = AAFRESULT_NO_MORE_OBJECTS;
    }
    else
    {
        aafUInt32               skipped_count = 0;

        while( skipped_count < count  &&
               ar == AAFRESULT_SUCCESS )
        {
            if( ++_factory_set_iterator )
            {
                skipped_count++;
            }
            else
            {
                ar = AAFRESULT_NO_MORE_OBJECTS;
            }
        }
    }


    return ar;
}



AAFRESULT STDMETHODCALLTYPE ImplEnumAAFFileEncodings::Reset()
{
    //
    // Is object initialized?
    //
    if( ! isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }

    _factory_set_iterator.reset( OMBefore );


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplEnumAAFFileEncodings::Clone(
    ImplEnumAAFFileEncodings**  ppEnum )
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
    if( ppEnum == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }


    //
    // Attempt to create new instance of this object.
    //
    AAFRESULT                   ar = AAFRESULT_SUCCESS;
    ImplEnumAAFFileEncodings*   p_new_enum =
        dynamic_cast<ImplEnumAAFFileEncodings*>(
            CreateImpl( CLSID_EnumAAFFileEncodings ) );

    if( p_new_enum != 0 )
    {
        ar = AAFRESULT_SUCCESS;
    }
    else
    {
        ar = E_FAIL;
    }


    //
    // Set return value, if succeeded.
    //
    if( ar == AAFRESULT_SUCCESS )
    {
        *ppEnum = p_new_enum;
    }


    return ar;
}



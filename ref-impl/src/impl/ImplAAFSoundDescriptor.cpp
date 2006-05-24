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


#include "OMAssertions.h"

#include "ImplAAFSoundDescriptor.h"
#include "AAFPropertyIDs.h"



ImplAAFSoundDescriptor::ImplAAFSoundDescriptor() :

    _compression(       PID_SoundDescriptor_Compression,        L"Compression"),
    _channels(          PID_SoundDescriptor_Channels,           L"Channels"),
    _audioSamplingRate( PID_SoundDescriptor_AudioSamplingRate,  L"AudioSamplingRate"),
    _locked(            PID_SoundDescriptor_Locked,             L"Locked"),
    _electroSpatial(    PID_SoundDescriptor_ElectroSpatial,     L"ElectroSpatial"),
    _audioRefLevel(     PID_SoundDescriptor_AudioRefLevel,      L"AudioRefLevel"),
    _dialNorm(          PID_SoundDescriptor_DialNorm,           L"DialNorm"),
    _quantizationBits(  PID_SoundDescriptor_QuantizationBits,   L"QuantizationBits")
{
    _persistentProperties.put( _compression.address() );
    _persistentProperties.put( _channels.address() );
    _persistentProperties.put( _audioSamplingRate.address() );
    _persistentProperties.put( _locked.address() );
    _persistentProperties.put( _electroSpatial.address() );
    _persistentProperties.put( _audioRefLevel.address() );
    _persistentProperties.put( _dialNorm.address() );
    _persistentProperties.put( _quantizationBits.address() );


    // Initialize the required properties with bogus values.
    // Initialize() may be called to complete initialization
    // of the object.
    _channels = 0;

    const aafRational_t  null_rational = { 0, 0 };
    _audioSamplingRate = null_rational;

    _quantizationBits = 0;

}



ImplAAFSoundDescriptor::~ImplAAFSoundDescriptor()
{
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::Initialize()
{
	// ImplAAFSoundDescriptor::Initialize() cannot be called directly by a client
	// application because an Initialize() method was omitted from
	// IAAFSoundDescriptor.
	// As a result, this implementation does not require the object to be
	// initialized. However, to support derived classes, which may have
	// public Initialize() methods that wish in turn to initialize this
	// class, this Initialize() method is provided.
	// It doesn't do anything beyond that done in the constructor.

    ASSERTU( !isInitialized() );


    // Initialize required properties
    _channels = 0;
    const aafRational_t  null_rational = { 0, 0 };
    _audioSamplingRate = null_rational;
    _quantizationBits = 0;



    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::SetCompression(
    const aafUID_t& compression )
{
    _compression = compression;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::GetCompression(
    aafUID_t* pCompression )
{
    if( pCompression == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
	if( !_compression.isPresent() )
    {
		return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pCompression = _compression;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::SetChannelCount(
    aafUInt32 channelCount )
{
    _channels = channelCount;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::GetChannelCount(
    aafUInt32* pChannelCount )
{
    if( pChannelCount == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }


    *pChannelCount = _channels;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::SetAudioSamplingRate(
    aafRational_t rate )
{
    _audioSamplingRate = rate;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::GetAudioSamplingRate(
    aafRational_t* pRate )
{
    if( pRate == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }

    *pRate = _audioSamplingRate;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::SetIsLocked(
    aafBoolean_t locked )
{
    _locked = locked;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::IsLocked(
    aafBoolean_t* pLocked )
{
    if( pLocked == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
	if( !_locked.isPresent() )
    {
		return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pLocked = _locked;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::SetElectroSpatialFormulation(
    aafElectroSpatialFormulation_t electroSpatialFormulation )
{
    _electroSpatial = electroSpatialFormulation;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::GetElectroSpatialFormulation(
    aafElectroSpatialFormulation_t*  pElectroSpatialFormulation )
{
    if( pElectroSpatialFormulation == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
	if( !_electroSpatial.isPresent() )
    {
		return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pElectroSpatialFormulation = _electroSpatial;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::SetAudioRefLevel(
    aafInt8 level )
{
    _audioRefLevel = level;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::GetAudioRefLevel(
    aafInt8* pLevel )
{
    if( pLevel == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
	if( !_audioRefLevel.isPresent() )
    {
		return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pLevel = _audioRefLevel;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::SetDialNorm(
    aafInt8 dialNorm )
{
    _dialNorm = dialNorm;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::GetDialNorm(
    aafInt8* pDialNorm )
{
    if( pDialNorm == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
	if( !_dialNorm.isPresent() )
    {
		return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pDialNorm = _dialNorm;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::SetQuantizationBits(
    aafUInt32 bitsCount )
{
    _quantizationBits = bitsCount;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFSoundDescriptor::GetQuantizationBits(
    aafUInt32*  pBitsCount )
{
    if( pBitsCount == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }


    *pBitsCount = _quantizationBits;


    return AAFRESULT_SUCCESS;
}

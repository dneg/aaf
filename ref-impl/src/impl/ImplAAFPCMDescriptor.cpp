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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "ImplAAFPCMDescriptor.h"
#include "AAFPropertyIDs.h"



ImplAAFPCMDescriptor::ImplAAFPCMDescriptor() :

    _blockAlign( PID_PCMDescriptor_BlockAlign, L"BlockAlign" ),
    _sequenceOffset( PID_PCMDescriptor_SequenceOffset, L"SequenceOffset" ),
    _averageBPS( PID_PCMDescriptor_AverageBPS, L"AverageBPS" ),
    _channelAssignment( PID_PCMDescriptor_ChannelAssignment, L"ChannelAssignment" ),
    _peakEnvelopeVersion( PID_PCMDescriptor_PeakEnvelopeVersion, L"PeakEnvelopeVersion" ),
    _peakEnvelopeFormat( PID_PCMDescriptor_PeakEnvelopeFormat, L"PeakEnvelopeFormat" ),
    _pointsPerPeakValue( PID_PCMDescriptor_PointsPerPeakValue, L"PointsPerPeakValue" ),
    _peakEnvelopeBlockSize( PID_PCMDescriptor_PeakEnvelopeBlockSize, L"PeakEnvelopeBlockSize" ),
    _peakChannels( PID_PCMDescriptor_PeakChannels, L"PeakChannels" ),
    _peakFrames( PID_PCMDescriptor_PeakFrames, L"PeakFrames" ),
    _peakOfPeaksPosition( PID_PCMDescriptor_PeakOfPeaksPosition, L"PeakOfPeaksPosition" ),
    _peakEnvelopeTimestamp( PID_PCMDescriptor_PeakEnvelopeTimestamp, L"PeakEnvelopeTimestamp" ),
    _peakEnvelopeData( PID_PCMDescriptor_PeakEnvelopeData, L"PeakEnvelopeData" )
{
    _persistentProperties.put( _blockAlign.address() );
    _persistentProperties.put( _sequenceOffset.address() );
    _persistentProperties.put( _averageBPS.address() );
    _persistentProperties.put( _channelAssignment.address() );
    _persistentProperties.put( _peakEnvelopeVersion.address() );
    _persistentProperties.put( _peakEnvelopeFormat.address() );
    _persistentProperties.put( _pointsPerPeakValue.address() );
    _persistentProperties.put( _peakEnvelopeBlockSize.address() );
    _persistentProperties.put( _peakChannels.address() );
    _persistentProperties.put( _peakFrames.address() );
    _persistentProperties.put( _peakOfPeaksPosition.address() );
    _persistentProperties.put( _peakEnvelopeTimestamp.address() );
    _persistentProperties.put( _peakEnvelopeData.address() );


    // Set the required properties to bogus values.
    // Initialize() needs to be called to compelete
    // initialization of the object.
    _blockAlign = 0;
}



ImplAAFPCMDescriptor::~ImplAAFPCMDescriptor ()
{
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::Initialize()
{
    if( isInitialized() )
    {
        return AAFRESULT_ALREADY_INITIALIZED;
    }


    // Call parent class' Initialize.
    AAFRESULT  ar = ImplAAFSoundDescriptor::Initialize();

    // Initialize this class required properties
    // and set the object initialized.
    if( ar == AAFRESULT_SUCCESS )
    {
        _blockAlign = 0;
        setInitialized();
    }


    return ar;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetBlockAlign(
    aafUInt16 blockAlign )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _blockAlign = blockAlign;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetBlockAlign(
    aafUInt16* pBlockAlign )
{
    if( pBlockAlign == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    *pBlockAlign = _blockAlign;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetSequenceOffset(
    aafUInt8 offset )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _sequenceOffset = offset;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetSequenceOffset(
    aafUInt8* pOffset )
{
    if( pOffset == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_sequenceOffset.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pOffset = _sequenceOffset;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetAverageBPS(
    aafUInt32 bps )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _averageBPS = bps;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetAverageBPS(
    aafUInt32* pBps )
{
    if( pBps == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    *pBps = _averageBPS;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetChannelAssignment(
    const aafUID_t& channelAssignment )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _channelAssignment = channelAssignment;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetChannelAssignment(
    aafUID_t *  pChannelAssignment )
{
    if( pChannelAssignment == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_channelAssignment.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pChannelAssignment = _channelAssignment;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPCMDescriptor::AreAllPeakEnvelopePropertiesPresent(
        aafBoolean_t*  pArePresent )
{
    if( pArePresent == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    if( _peakEnvelopeVersion.isPresent() &&
        _peakEnvelopeFormat.isPresent() &&
        _pointsPerPeakValue.isPresent() &&
        _peakEnvelopeBlockSize.isPresent() &&
        _peakChannels.isPresent() &&
        _peakFrames.isPresent() &&
        _peakOfPeaksPosition.isPresent() &&
        _peakEnvelopeTimestamp.isPresent() &&
        _peakEnvelopeData.isPresent() )
    {
        *pArePresent = kAAFTrue;
    }
    else
    {
        *pArePresent = kAAFFalse;
    }


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPeakEnvelopeVersion(
    aafUInt32 version )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _peakEnvelopeVersion = version;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakEnvelopeVersion(
    aafUInt32 *pVersion )
{
    if( pVersion == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakEnvelopeVersion.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pVersion = _peakEnvelopeVersion;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPeakEnvelopeFormat(
    aafUInt32 format )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _peakEnvelopeFormat = format;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakEnvelopeFormat(
    aafUInt32 *pFormat )
{
    if( pFormat == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakEnvelopeFormat.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pFormat = _peakEnvelopeFormat;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPointsPerPeakValue(
    aafUInt32 pointCount )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _pointsPerPeakValue = pointCount;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPointsPerPeakValue(
    aafUInt32 *pPointCount )
{
    if( pPointCount == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_pointsPerPeakValue.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pPointCount = _pointsPerPeakValue;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPeakEnvelopeBlockSize(
    aafUInt32 blockSize )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _peakEnvelopeBlockSize = blockSize;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakEnvelopeBlockSize(
    aafUInt32 *pBlockSize )
{
    if( pBlockSize == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakEnvelopeBlockSize.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pBlockSize = _peakEnvelopeBlockSize;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPeakChannelCount(
    aafUInt32 channelCount )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _peakChannels = channelCount;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakChannelCount(
    aafUInt32 *pChannelCount )
{
    if( pChannelCount == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakChannels.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pChannelCount = _peakChannels;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPeakFrameCount(
    aafUInt32 frameCount )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _peakFrames = frameCount;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakFrameCount(
    aafUInt32 *pFrameCount )
{
    if( pFrameCount == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakFrames.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pFrameCount = _peakFrames;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPeakOfPeaksPosition(
    aafPosition_t position )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _peakOfPeaksPosition = position;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakOfPeaksPosition(
    aafPosition_t *pPosition )
{
    if( pPosition == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakOfPeaksPosition.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pPosition = _peakOfPeaksPosition;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPeakEnvelopeTimestamp(
    aafTimeStamp_constref timeStamp )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }


    _peakEnvelopeTimestamp = timeStamp;


    return AAFRESULT_SUCCESS;
}


        
AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakEnvelopeTimestamp(
    aafTimeStamp_t *pTimeStamp )
{
    if( pTimeStamp == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakEnvelopeTimestamp.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pTimeStamp = _peakEnvelopeTimestamp;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::SetPeakEnvelopeDataPosition(
    aafPosition_t position )
{
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omPosition = position;
    _peakEnvelopeData.setPosition( omPosition );


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakEnvelopeDataPosition(
    aafPosition_t*  pPosition )
{
    if( pPosition == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakEnvelopeData.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omPosition = _peakEnvelopeData.position();
    *pPosition = omPosition;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::GetPeakEnvelopeDataSize(
    aafLength_t* pSize )
{
    if( pSize == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakEnvelopeData.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    OMUInt64  omSize = _peakEnvelopeData.size();
    *pSize = omSize;


    return AAFRESULT_SUCCESS;
}




AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::WritePeakEnvelopeData(
    aafUInt32 bytes,
    aafDataBuffer_t buffer,
    aafUInt32* pBytesWritten)
{
    if( bytes == 0 )
    {
        return AAFRESULT_INVALID_PARAM;
    }
    if( buffer == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( pBytesWritten == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    AAFRESULT  ar = AAFRESULT_SUCCESS;

    _peakEnvelopeData.write( buffer, bytes, *pBytesWritten );
    if( *pBytesWritten == 0 )
    {
        ar = AAFRESULT_CONTAINERWRITE;
    }


    return ar;
}



AAFRESULT STDMETHODCALLTYPE ImplAAFPCMDescriptor::ReadPeakEnvelopeData(
    aafUInt32 bytes,
    aafDataBuffer_t buffer,
    aafUInt32* pBytesRead)
{
    if( bytes == 0 )
    {
        return AAFRESULT_INVALID_PARAM;
    }
    if( buffer == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( pBytesRead == 0 )
    {
        return AAFRESULT_NULL_PARAM;
    }
    if( !isInitialized() )
    {
        return AAFRESULT_NOT_INITIALIZED;
    }
    if( !_peakEnvelopeData.isPresent() )
    {
        return AAFRESULT_PROP_NOT_PRESENT;
    }
    if( !persistent() )
    {
        return AAFRESULT_OBJECT_NOT_PERSISTENT;
    }


    AAFRESULT  ar = AAFRESULT_SUCCESS;

    _peakEnvelopeData.read( buffer, bytes, *pBytesRead );
    if( *pBytesRead == 0 )
    {
        ar = AAFRESULT_END_OF_DATA;
    }


    return ar;
}



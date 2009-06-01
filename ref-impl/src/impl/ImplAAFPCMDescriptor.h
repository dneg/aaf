#ifndef __ImplAAFPCMDescriptor_h__
#define __ImplAAFPCMDescriptor_h__

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


#include "OMDataStreamProperty.h"

#include "ImplAAFSoundDescriptor.h"



class OMDataStreamPropertyFilter;


class ImplAAFPCMDescriptor : public ImplAAFSoundDescriptor
{
public:
  //****************
  // Constructor
  //
  ImplAAFPCMDescriptor ();

protected:
  //****************
  // Desctructor
  //
  virtual ~ImplAAFPCMDescriptor ();

public:
  //********************************
  // AAF SDK exported methods start
  //********************************

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();

  //****************
  // SetBlockAlign()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBlockAlign
        // @parm [in] The number of bytes used to store one sample of all channels
        (aafUInt16 blockAlign);

  //****************
  // GetBlockAlign()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBlockAlign
        // @parm [out] The number of bytes used to store one sample of all channels
        (aafUInt16 *  pBlockAlign);

  //****************
  // SetSequenceOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSequenceOffset
        // @parm [in] Zero-based ordinal frame number of the beginning of
        // the essence data within a five-frame sequence
        (aafUInt8 offset);

  //****************
  // GetSequenceOffset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSequenceOffset
        // @parm [out] Zero-based ordinal frame number of the beginning of
        // the essence data within a five-frame sequence
        (aafUInt8 *  pOffset);

  //****************
  // SetAverageBPS()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAverageBPS
        // @parm [in] Average bytes per second of the essence stream
        (aafUInt32 bps);

  //****************
  // GetAverageBPS()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAverageBPS
        // @parm [out] Average bytes per second of the essence stream
        (aafUInt32 *  pBps);

  //****************
  // SetChannelAssignment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetChannelAssignment
        // @parm [in] The channel assignment to use
        (const aafUID_t& channelAssignment);

  //****************
  // GetChannelAssignment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetChannelAssignment
        // @parm [out] The channel assignment in use
        (aafUID_t *  pChannelAssignment);

  //****************
  // IsRequiredPeakEnvelopeInfoPresent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AreAllPeakEnvelopePropertiesPresent
        // @parm [out] The flag indicating presence of the optional
        // properties that form peak envelope.,
        (aafBoolean_t *  pArePresent);

  //****************
  // SetPeakEnvelopeVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPeakEnvelopeVersion
        // @parm [in] Version of the peak envelope data
        (aafUInt32 version);

  //****************
  // GetPeakEnvelopeVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakEnvelopeVersion
        // @parm [out] Version of the peak envelope data
        (aafUInt32 *pVersion);

  //****************
  // SetPeakEnvelopeFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPeakEnvelopeFormat
        // @parm [in] Format of the peak point
        (aafUInt32 format);

  //****************
  // GetPeakEnvelopeFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakEnvelopeFormat
        // @parm [out] Format of the peak point
        (aafUInt32 *pFormat);

  //****************
  // SetPointsPerPeakValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPointsPerPeakValue
        // @parm [in] The number of peak points per peak value
        (aafUInt32 pointCount);

  //****************
  // GetPointsPerPeakValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPointsPerPeakValue
        // @parm [out] The number of peak points per peak value
        (aafUInt32 *pPointCount);

  //****************
  // SetPeakEnvelopeBlockSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPeakEnvelopeBlockSize
        // @parm [in] The number of audio samples used to generate each peak frame
        (aafUInt32 blockSize);

  //****************
  // GetPeakEnvelopeBlockSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakEnvelopeBlockSize
        // @parm [out] The number of audio samples used to generate each peak frame
        (aafUInt32 *pBlockSize);

  //****************
  // SetPeakChannelCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPeakChannelCount
        // @parm [in] The number of peak channels
        (aafUInt32 channelCount);

  //****************
  // GetPeakChannelCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakChannelCount
        // @parm [out] The number of peak channels
        (aafUInt32 *pChannelCount);

  //****************
  // SetPeakFrameCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPeakFrameCount
        // @parm [in] The number of peak frames
        (aafUInt32 frameCount);

  //****************
  // GetPeakFrameCount()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakFrameCount
        // @parm [out] The number of peak frames
        (aafUInt32 *pFrameCount);

  //****************
  // SetPeakOfPeaksPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPeakOfPeaksPosition
        // @parm [in] The offset to the first audio sample whose absolute
        //            value is the maximum value of the entire audio file
        (aafPosition_t position);

  //****************
  // GetPeakOfPeaksPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakOfPeaksPosition
        // @parm [out] The offset to the first audio sample whose absolute
        //             value is the maximum value of the entire audio file
        (aafPosition_t *pPosition);

  //****************
  // SetPeakEnvelopeTimestamp()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPeakEnvelopeTimestamp
        // @parm [in] The time stamp of the creation of the peak data
        (aafTimeStamp_constref timeStamp);

  //****************
  // GetPeakEnvelopeTimestamp()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakEnvelopeTimestamp
        // @parm [out] The time stamp of the creation of the peak data
        (aafTimeStamp_t *pTimeStamp);

  //****************
  // SetPeakEnvelopeDataPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPeakEnvelopeDataPosition
        // @parm [in] Offset from the beginning of peak envelope data
        (aafPosition_t position);

  //****************
  // GetPeakEnvelopeDataPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakEnvelopeDataPosition
        // @parm [out] Offset from the beginning of peak envelope data
        (aafPosition_t *  pPosition);

  //****************
  // GetPeakEnvelopeDataSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPeakEnvelopeDataSize
        // @parm [out] The size of peak envelope data
        (aafLength_t *  pSize);

  //****************
  // WritePeakEnvelopeData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WritePeakEnvelopeData
        // @parm [in] Write this many bytes
        (aafUInt32 bytes,

		 // @parm [out, size_is(buflen), length_is(*pBytesWritten)] Data to write
        aafDataBuffer_t buffer,

		 // @parm [out,ref] Number of bytes actually written
        aafUInt32* pBytesWritten);

  //****************
  // ReadPeakEnvelopeData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadPeakEnvelopeData
        // @parm [in] Read this many bytes
        (aafUInt32 bytes,

		 // @parm [out, size_is(buflen), length_is(*pBytesRead)] Buffer to read the data to
        aafDataBuffer_t buffer,

		 // @parm [out,ref] Number of bytes actually read
        aafUInt32* pBytesRead);

  //********************************
  // AAF SDK exported methods end
  //********************************

protected:

    OMFixedSizeProperty<aafUInt16>              _blockAlign;
    OMFixedSizeProperty<aafUInt8>               _sequenceOffset;
    OMFixedSizeProperty<aafUInt32>              _averageBPS;
    OMFixedSizeProperty<aafUID_t>               _channelAssignment;
    OMFixedSizeProperty<aafUInt32>              _peakEnvelopeVersion;
    OMFixedSizeProperty<aafUInt32>              _peakEnvelopeFormat;
    OMFixedSizeProperty<aafUInt32>              _pointsPerPeakValue;
    OMFixedSizeProperty<aafUInt32>              _peakEnvelopeBlockSize;
    OMFixedSizeProperty<aafUInt32>              _peakChannels;
    OMFixedSizeProperty<aafUInt32>              _peakFrames;
    OMFixedSizeProperty<aafPosition_t>          _peakOfPeaksPosition;
    OMFixedSizeProperty<aafTimeStamp_t>         _peakEnvelopeTimestamp;
    OMDataStreamProperty                        _peakEnvelopeData;
    OMDataStreamPropertyFilter*                 _peakEnvelopeDataFilter;
};

#endif // ! __ImplAAFPCMDescriptor_h__

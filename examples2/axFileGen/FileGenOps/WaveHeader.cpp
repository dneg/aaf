//=---------------------------------------------------------------------=
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
//=---------------------------------------------------------------------=


#include "WaveHeader.h"

#include <string.h>

#include <assert.h>

WaveHeader::WaveHeader()
{}

WaveHeader::WaveHeader( std::pair<int, std::auto_ptr<aafUInt8> > buf )
{
	// no can do yet
	if ( buf.first != HEADER_SIZE ) {
		throw L"Wave header length too long (or not supported)";
	}

	_headerSize = buf.first;
	memcpy( &_header, buf.second.get(), _headerSize );
	
	_bitsPerSample = GetInt16( BITS_PER_SAMPLE );

	_sampleRate = GetInt32( SAMPLE_RATE_IDX );

	_numChannels = GetInt16( NUM_CHANNELS_IDX );

	_bytesPerSample = GetInt32( BYTE_RATE_IDX ) / _numChannels / _sampleRate;

	_numSamples = GetInt32( SUB_CHUNK2_ID_SIZE_IDX ) / _numChannels / _bytesPerSample;
}

WaveHeader::WaveHeader( int sampleRate,
					    int numSamples,
						int bytesPerSample,
						int bitsPerSample,
						int numChannels )
: _sampleRate( sampleRate ),
  _numSamples( numSamples ),
  _bytesPerSample( bytesPerSample ),
  _bitsPerSample( bitsPerSample ),
  _numChannels( numChannels ),
  _headerSize( HEADER_SIZE )
{
	memset( &_header, 0, sizeof(_header) );

	// Chunk id is four characters: "RIFF"
	SetStr4( CHUNK_ID_IDX, "RIFF" );
	
	// Chunk size is a 32 bit integer that stores size of the entire chunk of wave
	// data, i.e., the header plus samples.
	SetInt32(CHUNK_SIZE_IDX, CHUNK1_SIZE +
				   CHUNK_PREFIX_SIZE +
				   GetAudioDataSizeInBytes() );

    // Format contains the letters "WAVE"
	SetStr4( FORMAT_IDX, "WAVE" );

	// SubchunkID1 contains the letters "fmt ";
	SetStr4( SUB_CHUNK1_ID_IDX, "fmt " );

	// Subchunk size is a 32 bit integer that stores the number of
	// bytes following this number for this sub chunk.  (16 for PCM)
	SetInt16( SUB_CHUNK1_SIZE_IDX, 16 );

	// AudioFormat is PCM - indicated by the value 1
	SetInt16( AUDIO_FORMAT_IDX, 1 );

	// Set num changes as a 16 bit value;
	SetInt16( NUM_CHANNELS_IDX, _numChannels );

	// Sample rate
	SetInt32( SAMPLE_RATE_IDX, _sampleRate );

	// Byte rate
	SetInt32( BYTE_RATE_IDX, _sampleRate * _bytesPerSample * _numChannels );

	// Block alignment i.e. size of a frame of audio data.
	SetInt16( BLOCK_ALIGN_IDX, _numChannels * _bytesPerSample );

	// Bits per sample
	SetInt16( BITS_PER_SAMPLE, _bitsPerSample );

	// SUB_CHUNK2_ID_IDX - The letters "data".
	SetStr4( SUB_CHUNK2_ID_IDX, "data" );

	// SUB_CHUNK2_ID_SIZE_ID - Size, in bytes, of the audio data.
	SetInt32( SUB_CHUNK2_ID_SIZE_IDX, GetAudioDataSizeInBytes() );
}


WaveHeader::~WaveHeader()
{}


WaveHeader& WaveHeader::operator=( const WaveHeader& rhs )
{
	if ( this == &rhs ) {
		return *this;
	}

	_sampleRate = rhs._sampleRate;
    _numSamples = rhs._numSamples;
	_bytesPerSample = rhs._bytesPerSample;
	_bitsPerSample = rhs._bitsPerSample;
	_numChannels = rhs._numChannels;
	_headerSize = rhs._headerSize;

	memcpy( &_header, &rhs._header, sizeof(_header) );

	return *this;
}

int WaveHeader::GetHeaderSizeInBytes()
{
	return HEADER_SIZE;
}
	
aafUInt32 WaveHeader::GetAudioDataSizeInBytes()
{
	return _numChannels * _bytesPerSample * _numSamples;
}

void WaveHeader::ChangeNumSamples( int numSamples )
{
	// Yes... it's a cheap trick.
	
	_numSamples = numSamples;

	WaveHeader other( _sampleRate,
					  _numSamples,
					  _bytesPerSample,
				 	  _bitsPerSample,
					  _numChannels );

	memcpy( &_header, &other._header, sizeof(_header) );
}


int WaveHeader::GetSampleRate()
{
	return _sampleRate;
}

int WaveHeader::GetNumSamples()
{
	return _numSamples;
}

int WaveHeader::GetBytesPerSample()
{
	return _bytesPerSample;
}

int WaveHeader::GetBitsPerSample()
{
	return _bitsPerSample;
}

int WaveHeader::GetNumChannels()
{
	return _numChannels;
}


void WaveHeader::SetInt32( int idx, aafUInt32 val )
{
	assert( reinterpret_cast<aafUInt64>(&_header[idx]) % 4 == 0 );
	*reinterpret_cast<aafUInt32*>(&_header[idx]) = val;
}

void WaveHeader::SetInt16( int idx, aafUInt16 val )
{
	assert( reinterpret_cast<aafUInt64>(&_header[idx]) % 2 == 0 );
	*reinterpret_cast<aafUInt16*>(&_header[idx]) = val;
}

void WaveHeader::SetStr4( int idx, const char str[4] )
{
	int i;
	for (i = 0; i < 4; i++ ) {
		_header[ idx + i ] = str[i];
	}
}

aafUInt32 WaveHeader::GetInt32( int idx )
{
	aafUInt32 val;
	assert( reinterpret_cast<aafUInt64>(&_header[idx]) % 4 == 0 );
	val = *reinterpret_cast<aafUInt32*>(&_header[idx]);
	return val;
}

aafUInt16 WaveHeader::GetInt16( int idx )
{
	aafUInt16 val;
	assert( reinterpret_cast<aafUInt64>(&_header[idx]) % 2 == 0 );
	val = *reinterpret_cast<aafUInt16*>(&_header[idx]);
	return val;
}

std::pair<int, std::auto_ptr<aafUInt8> > WaveHeader::GetHeader()
{		
	aafUInt8* buffer = new aafUInt8[ sizeof(_header) ];
	
	memcpy( buffer, &_header, sizeof(_header) );

	return 	std::pair<int, std::auto_ptr<aafUInt8> >(
		sizeof(_header), std::auto_ptr<aafUInt8>( buffer ) );
}

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

#include "SampleSource.h"
#include "WaveHeader.h"

#include <axFileGen.h>

#include <AxEssence.h>

#include <math.h>
#include <assert.h>

namespace {

// The tone generator can generate a single cycle and repeat it, or 
// generate every sample.  The single cycle approach is efficient, but
// suffers as the tone frequency approaches the nyquist frequence because
// we can only generate tones that are integer multiples of the
// sample_freq/tone_freq.  The error introduced by this grows as the tone_freq
// increases.  Since, in general we assume nothing about the sample frequency
// or the tone frequency values, we choose the more accurate approach and
// generate every sample at the expense of a bit of cpu time.

class ToneGenerator {
public:
	ToneGenerator( int toneFequency,
				   int sampleFrequency,
		           int level,
				   int bitsPerSample );

	~ToneGenerator();

	void Reset();

	aafUInt32 NextSample();

private:

	int _toneFrequency;
	int _sampleFrequency;
	int _level;
	int _bitsPerSample;
	int _bytesPerSample;

	unsigned long _time;

	double _normalizedFrequency;

	double _normalizedLevel;

	// Where the heck is M_PI on windows?
	double _pi;
};


ToneGenerator::ToneGenerator( int toneFrequency,
							  int sampleFrequency,
							  int level,
							  int bitsPerSample )
: _toneFrequency( toneFrequency ),
  _sampleFrequency( sampleFrequency ),
  _level( level ),
  _bitsPerSample( bitsPerSample ),
  _time(0),
  _pi(2.0*asin(1.0))
{
	if ( _bitsPerSample > 32 ) {
		throw AxFGEx( L"Tone generator limited to 32 bits per sample." );
	}

	if ( _toneFrequency > _sampleFrequency/2 ) {
		throw AxFGEx( L"Nyquist frequency exceeded." );
	}
	
	// Get 2*pi in there now as well...
	_normalizedFrequency = (2.0 * _pi * _toneFrequency) / _sampleFrequency;

	// Sine is from -1.0 to 1.0.
	// (1+sin(wt))/2 gives 0.0 to 1.0, centered on 0.5
	// Max value is 2^bits_per_sample
	// Reduce this by level/100 percent.
	// Thus, value of sample i is:  level/100 * (2^bits_per_sample-1) * (1+sin(w*i))/2
	//
	// Combine constants:  _normalizedLevel * (1 + sin(_normalizedFrequency * i) )

	_normalizedLevel = (level/100.0) * (pow(2.0, _bitsPerSample)-1) / 2.0;  
}

ToneGenerator::~ToneGenerator()
{}

void ToneGenerator::Reset() 
{
	_time = 0;
}

inline aafUInt32 ToneGenerator::NextSample()
{
	double val;

	val = _normalizedLevel * (1 + ::sin(_normalizedFrequency * _time++));

	return static_cast<aafUInt32>(val);
}

//=---------------------------------------------------------------------=

class ToneSource : public AxFGOp, public SampleSource { 

public:

  ToneSource ( const AxString& ToneSource )
    : AxFGOp( ToneSource ),
	  _count( 0 )
	{}

  virtual ~ToneSource();

  virtual void Execute( const std::vector<AxString>& args );

  // SampleSource methods
  virtual void Reset();
  virtual std::auto_ptr< SampleSrcBuffer > GetNext();

private:
	int _toneFrequency;
	int _toneLevel;
	int _count;
	WaveHeader _waveHeader;

	// auto_ptr so that we don't have to construct until
	// Execute time.
	std::auto_ptr<ToneGenerator> _toneGen;
};


AXFG_OP_FACTORY_DECLARATION(
  ToneSource,           
  L"ToneSource",
  L"Generate a tone.",
  L"SourceName WaveDescName frequency_hz level",
  L"Support only wave audio descriptors.  Level is an integer from 0 to 100 representing percent of max.",
  5,
  5 ) 

ToneSource::~ToneSource()
{}

void ToneSource::Execute( const std::vector<AxString>& argv )
{
	AxString sourceName = argv[1];
	AxString descName   = argv[2];
	AxString frequency  = argv[3];
	AxString level      = argv[4];

	IAAFWAVEDescriptorSP spDesc;
	GetInstance( descName ).GetCOM( spDesc );
	AxWAVEDescriptor axDesc( spDesc );

	int freqVal  = AxStringUtil::strtol( frequency );
	int levelVal = AxStringUtil::strtol( level );

	if ( ! (0 <= levelVal  ||  levelVal <= 100) ) {
		throw AxFGOpUsageEx( *this, L"level out of range" );
	}

	AxBuffer<aafUInt8> waveHeaderBuffer = axDesc.GetSummary();

	WaveHeader waveHeader( waveHeaderBuffer );

	_waveHeader = waveHeader;
	
	// Only mono support for the moment.
	if ( 1  != _waveHeader.GetNumChannels() ) {
		throw AxFGEx( L"Only mono support exists." );
	}

	_toneGen = std::auto_ptr<ToneGenerator>(
					new ToneGenerator( freqVal,
					 				   _waveHeader.GetSampleRate(),
									   levelVal,
									   _waveHeader.GetBitsPerSample() ) );

	RegisterInstance( sourceName );
}

void ToneSource::Reset()
{
	_toneGen->Reset();
	_count = 0;
}

std::auto_ptr< SampleSrcBuffer > ToneSource::GetNext()
{
	if ( 1 == _count ) {
		return std::auto_ptr< SampleSrcBuffer >( new SimpleSampleSrcBuffer() );
	}

	auto_ptr<aafUInt8> buf( new aafUInt8[ _waveHeader.GetAudioDataSizeInBytes() ] );

	// yikes... there's gotta be a better way...

	if ( 1 == _waveHeader.GetBytesPerSample() ) {
		aafUInt8* buf8 = buf.get();
		int i;
		for( i = 0; i < _waveHeader.GetNumSamples(); i++ ) {
			buf8[i] = _toneGen->NextSample();
		}
	}
	else if ( 2 == _waveHeader.GetBytesPerSample() ) {
		aafUInt16* buf16 = reinterpret_cast<aafUInt16*>(buf.get());

		// just in case... better check
		assert( reinterpret_cast<aafUInt64>(buf16) % 2== 0 );
	
		int i;
		for( i = 0; i < _waveHeader.GetNumSamples(); i++ ) {
			buf16[i] = _toneGen->NextSample();
		}
	}
	else if ( 4 == _waveHeader.GetBytesPerSample() ) {
		aafUInt32* buf32 = reinterpret_cast<aafUInt32*>(buf.get());

		// just in case... better check
		assert( reinterpret_cast<aafUInt64>(buf32) % 4== 0 );
	
		int i;
		for( i = 0; i < _waveHeader.GetNumSamples(); i++ ) {
			buf32[i] = _toneGen->NextSample();
		}

	}
	else {
		throw AxFGEx( L"unsupported byte per sample size" );
	}

	_count++;

	return std::auto_ptr< SampleSrcBuffer >(
		new SimpleSampleSrcBuffer( _waveHeader.GetNumSamples(),
		  						   _waveHeader.GetAudioDataSizeInBytes(),
								   buf ) );
}

} // end of namespace

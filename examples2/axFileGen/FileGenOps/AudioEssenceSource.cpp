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

namespace {

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
	int _count;
	WaveHeader _waveHeader;
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

	std::pair<int, std::auto_ptr<aafUInt8> > waveHeaderBuffer = axDesc.GetSummary();

	WaveHeader waveHeader( waveHeaderBuffer );

	_waveHeader = waveHeader;

	RegisterInstance( sourceName );
}

void ToneSource::Reset()
{
	_count = 0;
}

std::auto_ptr< SampleSrcBuffer > ToneSource::GetNext()
{
	auto_ptr<aafUInt8> buf( new aafUInt8[ _waveHeader.GetAudioDataSizeInBytes() ] );

	if ( 1 == _count ) {
		return std::auto_ptr< SampleSrcBuffer >( new SimpleSampleSrcBuffer() );
	}

	// Fill in the tone here!

	_count++;

	return std::auto_ptr< SampleSrcBuffer >(
		new SimpleSampleSrcBuffer( _waveHeader.GetNumSamples(),
		  						   _waveHeader.GetAudioDataSizeInBytes(),
								   buf ) );
}

} // end of namespace



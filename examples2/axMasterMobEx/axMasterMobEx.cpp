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
//
//=---------------------------------------------------------------------

#include <AxUtil.h>
#include <AxFile.h>
#include <AxTypes.h>
#include <AxInit.h>
#include <AxIterator.h>
#include <AxHeader.h>
#include <AxContentStorage.h>
#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxEssence.h>
#include <AxComponent.h>
#include <AxDictionary.h>
#include <AxEx.h>

#include <AAFStoredObjectIDs.h>
#include <AAFDataDefs.h>
#include <AAFCodecDefs.h>
#include <AAFContainerDefs.h>
#include <AAFEssenceFormats.h>
#include <AAFResult.h>

#include <iostream>
#include <sstream>

#include <assert.h>
#include <stdlib.h>

#include <map>
#include <new>




// TODO - Verify data in the read test.
//      - Implement a "-extend" option that opens a file for modification and extends the essence.


void _throwUsage()
{
	wostringstream msg;
	msg << L"Usage: " << L" -file aaf_filename" << endl;
	msg << L"       " << L" -read | -write" << endl;
	msg << L"       " << L" -source audio | video" << endl;
	msg << L"       " << L" -numSamples | -numKSamples | -numMSamples  number" << endl;
	msg << L"       " << L" [-codec jpeg | cdci | wav | aifc]" << endl;
	msg << L"       " << L" [-netloc filename_prefix filename_suffix]" << endl;
	msg << endl;
	msg << L"Examples: axMasterMobEx -file mmex.aaf -write -source video -netloc ess .jpg  -numSamples   200 -codec jpeg" << endl;
	msg << L"          axMasterMobEx -file mmex.aaf -write -source video -netloc ess .cdci -numSamples   200 -codec cdci" << endl;
	msg << L"          axMasterMobEx -file mmex.aaf -write -source audio -netloc ess .wav  -numMSamples 2000 -codec wav" << endl;
	msg << L"          axMasterMobEx -file mmex.aaf -write -source audio -netloc ess .aifc -numMSamples 2000 -codec aifc" << endl;
	msg << L"          axMasterMobEx -file mmex.aaf -read -source video" << endl;
	msg << L"          axMasterMobEx -file mmex.aaf -read -source audio" << endl;
	msg << endl;
	msg << L"Notes:  - The default video codec is \"jpeg\" if the codec is not specified." << endl;
	msg << L"        - The default audio codec is \"wav\" if the codec is not specified." << endl;
	msg << L"        - Mismatched source and codec essence types will yield undefined results." << endl;
	msg << L"        - The default container is ContainerAAF.  -netloc specifies ContainerFile." << endl;
	msg << L"        - The -netloc option arguments are used to create a filename for the external essence." << endl;
	msg << L"        - If EssenceAccess::WriteEssence() returns AAFRESULT_EOF, the essence is extended and" << endl;
	msg << L"        writing continues.  This is the means by which essence in excess of 2GB is supported." << endl;
	msg << L"        - The \"-read\" option will sequentially read all essence data from the first slot of the" << endl;
	msg << L"        first master mob in the file. The \"-source\" option must be used to specify the essence type." << endl;
	throw AxEx( msg.str().c_str() );
}

#if 0
// This is useful for debug of the command line parser.
#define throwUsage()  { cout << "Line " << __LINE__ << endl; _throwUsage(); }
#else
#define throwUsage() _throwUsage();   

#endif

//============================================================================

class EssenceLocator {
public:
	EssenceLocator()
	{}

	virtual ~EssenceLocator()
	{}

	virtual IAAFLocatorSP GetNextLocator( AxDictionary& )
	{
		IAAFLocatorSP null;
		return null;
	}

	virtual aafUID_t GetNextContainer()
	{
		return ContainerAAF;
	}
};

class NetworkEssenceLocator : public EssenceLocator {
public:
	NetworkEssenceLocator( AxString& prefix, AxString& suffix )
	: EssenceLocator(),
	  _prefix( prefix ),
	  _suffix( suffix ),
	  _counter(0)
	{}
	
	~NetworkEssenceLocator()
	{}

	IAAFLocatorSP GetNextLocator( AxDictionary& axDictionary )
	{
		wostringstream pathStream;

		pathStream << _prefix << _counter++ << _suffix;
		AxNetworkLocator axNetworkLocator(
			AxCreateInstance<IAAFNetworkLocator>( axDictionary ) );


		axNetworkLocator.Initialize();
		AxString path( pathStream.str().c_str() );

		wcout << L"Path is: " << path << endl;
		
		axNetworkLocator.SetPath( path );

		return axNetworkLocator;
	}

	aafUID_t GetNextContainer() 
	{
		return ContainerFile;
	}

private:
	AxString _prefix;
	AxString _suffix;
	int _counter;
};

//============================================================================

class SampleSource {

public:

	SampleSource( aafUID_t dataDef, aafRational_t rate,
				  aafCompressEnable_t compression, int sampleByteSize,
				  int numSamplesPerChunk )
		: _dataDef( dataDef ),
		  _rate( rate ),
		  _compression( compression ),
		  _sampleByteSize( sampleByteSize ),
		  _numSamplesPerChunk( numSamplesPerChunk )
	{}

	virtual ~SampleSource()
	{}
	
	aafUID_t GetDataDef()
	{ return _dataDef; }

	aafRational_t GetRate()
	{ return _rate; }

	aafCompressEnable_t GetCompression()
	{ return _compression; }
	
	int GetSampleByteSize()
	{ return _sampleByteSize; }
	
	int GetNumSamplesPerChunk()
	{ return _numSamplesPerChunk; }

	virtual void SetupFormatSpecifiers( AxEssenceAccess& ) = 0;

	virtual bool AtEnd() = 0;

	virtual pair<int, AxAutoPtr<aafUInt8> > GetSamples() = 0;

private:

	aafUID_t _dataDef;
	aafRational_t _rate;
	aafCompressEnable_t _compression;
	int _sampleByteSize;
	int _numSamplesPerChunk;
};

//============================================================================

class VideoSampleSource : public SampleSource {
public:
	VideoSampleSource(	int width, int height, int bytesPerPixel, aafRational_t rate,
						int numFramesToWrite,
					    aafCompressEnable_t compression )
		: SampleSource( DDEF_Picture,
						rate,
						compression,
						bytesPerPixel * width * height,
						1 ),
		  _numFramesToWrite( numFramesToWrite ),
		  _width( width ),
		  _height( height ),
		  _bytesPerPixel( bytesPerPixel ),
		  _rate( _rate )
	{}

	virtual ~VideoSampleSource()
	{}

	virtual void SetupFormatSpecifiers( AxEssenceAccess& axEssenceAccess )
	{
		AxEssenceFormat axEssenceFormat( axEssenceAccess.GetEmptyFileFormat() );

		// Add essence format specifiers.
		aafRect_t storedRect = {0,0,_width,_height};
		axEssenceFormat.AddFormatSpecifier(
			kAAFStoredRect, sizeof(storedRect), (aafDataBuffer_t)&storedRect );

		aafColorSpace_t colorSpace = kAAFColorSpaceYUV;   // FIXME - Is that 8 bit YUV??
		axEssenceFormat.AddFormatSpecifier(
			kAAFPixelFormat, sizeof(colorSpace), (aafDataBuffer_t)&colorSpace );
	
		aafUInt32 horizontalSubsample = 2;
		axEssenceFormat.AddFormatSpecifier(
			kAAFCDCIHorizSubsampling, sizeof(horizontalSubsample), (aafDataBuffer_t)&horizontalSubsample);


		axEssenceAccess.PutFileFormat( axEssenceFormat );
	}

	virtual bool AtEnd()
	{
		return _numFramesToWrite > 0 ? false : true;
	}

	virtual pair<int, AxAutoPtr<aafUInt8> > GetSamples()
	{
		const int numSamples = GetNumSamplesPerChunk();  // frames
		const int numBytes = GetSampleByteSize();

		AxAutoPtr<aafUInt8> pixels( new aafUInt8 [numBytes] );

		// Fill with random data.
		int i;
		aafInt32* p = reinterpret_cast<aafInt32*>( pixels.get() );
		for( i = 0; i < numBytes/sizeof(aafInt32); i++ ) {
			p[i] = rand();
		}

		_numFramesToWrite -= numSamples;
		pair<int, AxAutoPtr<aafUInt8> > ret(numSamples, pixels);
		return ret;
	}
	
private:

	int _width;
	int _height;
	int _bytesPerPixel;
	int _rate;

	int _numFramesToWrite;
};						

//============================================================================

class AudioSampleSource : public SampleSource {
public:
	AudioSampleSource(	int bitsPerSample, int bytesPerSample, aafRational_t rate,
						aafInt64 numSamplesToWrite,
					    aafCompressEnable_t compression )
		: SampleSource( DDEF_Sound,
						rate,
						compression,
						bytesPerSample,
						10*1024*1024/bytesPerSample ),
		  _numSamplesToWrite( numSamplesToWrite ),
		  _bitsPerSample( bitsPerSample ),
		  _bytesPerSample( bytesPerSample )
	{}

	virtual ~AudioSampleSource()
	{}

	virtual void SetupFormatSpecifiers( AxEssenceAccess& axEssenceAccess )
	{
		AxEssenceFormat axEssenceFormat( axEssenceAccess.GetEmptyFileFormat() );

		aafUInt32 sampleSize = _bitsPerSample;
		axEssenceFormat.AddFormatSpecifier( kAAFAudioSampleBits, sizeof( sampleSize ),
						    reinterpret_cast<aafUInt8*>(&sampleSize) );


		axEssenceAccess.PutFileFormat( axEssenceFormat );
	}

	virtual bool AtEnd()
	{
		return _numSamplesToWrite > 0 ? false : true;
	}

	virtual pair<int, AxAutoPtr<aafUInt8> > GetSamples()
	{
		// FIXME - Hardcoded: write 10 MByte at a time.
		
		int numSamples = GetNumSamplesPerChunk();

		if ( numSamples > _numSamplesToWrite ) {
			numSamples = _numSamplesToWrite;
		}
		
		const int numBytes = numSamples * _bytesPerSample;

		AxAutoPtr<aafUInt8> samples( new aafUInt8 [numBytes] );

		// Fill with random data.
		int i;
		aafInt32* s = reinterpret_cast<aafInt32*>( samples.get() );
		for( i = 0; i < numBytes/sizeof(aafInt32); i++ ) {
			s[i] = rand();
		}

		_numSamplesToWrite -= numSamples;
		pair<int, AxAutoPtr<aafUInt8> > ret(numSamples, samples );
		return ret;
	}
	
private:

	int _bitsPerSample;
	int _bytesPerSample;
	aafInt64 _numSamplesToWrite;
};						

//============================================================================

class CmdLineParser {

public:

	typedef enum {READ, WRITE, EXTEND} Op_t;

	CmdLineParser( AxCmdLineArgs& args );

	~CmdLineParser()
	{}

	aafUID_t GetCodec()
	{ return _codecId; }

	aafUID_t GetContainerId()
	{ return _containerId; }

	AxString GetAAFFileName()
	{ return _aafFile; };

	Op_t GetOp()
	{ return _op; }

	const char* GetCountSymbol()
	{ return _symbol; }

	int GetCountMultiplier()
	{ return _multiplier; }

	auto_ptr<EssenceLocator> CreateEssenceLocator()
	{
		if ( _containerId == ContainerFile ) {
			auto_ptr<EssenceLocator> networkLocatorPtr(
				new NetworkEssenceLocator( _netlocPrefix, _netlocSuffix ) );
			return networkLocatorPtr;
		}
		else {
			auto_ptr<EssenceLocator> locatorPtr(
				new EssenceLocator() );
			return locatorPtr;
		}
	}

	auto_ptr<SampleSource> CreateSampleSource()
	{
		// FIXME - Compression should be an option.  It is disabled here because
		// it takes too long to compress images when running a test designed to
		// write very large amounts of essense (i.e. many GB's)
		if ( _source == "video" ) {
			auto_ptr< SampleSource > sampleSource(
				new VideoSampleSource( _videoWidth, _videoHeight, _videoBytesPerPixel, _rate,
									   _numSamples, kAAFCompressionDisable ) );
			return sampleSource;
		}
		else if ( _source == "audio" ) {
			auto_ptr< SampleSource > audioSource(
				new AudioSampleSource( _audioBitsPerSample,
									   (_audioBitsPerSample+7)/8,
									   _rate,
									   _numSamples,
									    kAAFCompressionDisable ) );
		
			return audioSource;
		}
		else {
			throw "Unexpected _souce value.";
		}
	
	}

private:

	aafInt64 _numSamples;

	AxString _aafFile;
	AxString _netlocPrefix;
	AxString _netlocSuffix;

	aafUID_t _codecId;
	aafUID_t _containerId;

	int _videoWidth;
	int _videoHeight;
	int _videoBytesPerPixel;

	int _audioBitsPerSample;

	aafRational_t _rate;

	Op_t	_op;
	
	string _source;

	int _multiplier;
	char* _symbol;
};

CmdLineParser::CmdLineParser( AxCmdLineArgs& args )
{
	// Video parameters hardcode for the moment.
	_videoWidth         = 4096;
	_videoHeight        = 4096;
	_videoBytesPerPixel =    2;


	// Audio parameters hardcoded for the moment
	_audioBitsPerSample = 24;
	
	// AAF file name - required
	{
		pair<bool, int> fileOpArg = args.get( "-file" );
		if ( !fileOpArg.first ){
			throwUsage();
		}

		pair<bool, const char*> fileNameArg = args.get( fileOpArg.second + 1 );
		if ( !fileNameArg.first ) {
			throwUsage();
		}
		_aafFile = AxString( AxStringUtil::mbtowc(fileNameArg.second) );

		wcout << L"AAF File: " << _aafFile << endl;
	}

	// Sample Source - required 
	{
		pair<bool, int> sourceOpArg = args.get( "-source" );
		if ( !sourceOpArg.first ){
			throwUsage();
		}
		pair<bool, const char*> sourceArg = args.get( sourceOpArg.second + 1 );
		if ( !sourceArg.first ) {
			throwUsage();
		}
		_source = string( sourceArg.second );
		if ( _source == "video" ) {
			_codecId = kAAFCodecJPEG;

			// Default video rate.
			_rate.numerator     =   24;
			_rate.denominator   =    1;

		}
		else if ( _source == "audio" ) {
			_codecId = kAAFCodecWAVE;

			// Default audio rate.
			_rate.numerator     =   48000;
			_rate.denominator   =		1;
		}
		else {
			throwUsage();
		}

		cout << "Sample source: " << _source<< endl;
	}

	// Operation option (read/write/extend) - required
	{
		pair<bool,int> opRead;
		pair<bool,int> opWrite;
		pair<bool,int> opExtend;

		opRead = args.get( "-read" );
		opWrite = args.get( "-write" );
		//opExtend = args.get( "-extend" );

		if ( opRead.first ) {
			_op = READ;
			return;
		}
		else if ( opWrite.first ) {
			_op = WRITE;
		}
		else if ( opExtend.first ) {
			_op = EXTEND;
		}
		else {
			throwUsage();
		}
	}

	// Rate - default is determined above.
	{
		// There is no option yet.
		cout << "Rate: " << _rate.numerator << "/" << _rate.denominator << endl;
	}

	// Codec - optional, overrides default set above.
	{
		std::map<string,aafUID_t> map;
		std::map<string,aafUID_t>::const_iterator iter;

		map[ string("jpeg") ] = kAAFCodecJPEG;
		map[ string("wav") ]  = kAAFCodecWAVE;
		map[ string("aifc") ] = kAAFCODEC_AIFC;
		map[ string("cdci") ] = kAAFCodecCDCI;

		pair<bool, int> codecOpArg = args.get( "-codec" );
		if ( codecOpArg.first ) {
			pair<bool, const char*> codecArg = args.get( codecOpArg.second + 1 );
			if ( !codecArg.first ) {
				throwUsage();
			}

			iter = map.find( string(codecArg.second) );
			if ( iter == map.end() ) {
				throwUsage();
			}
	
			_codecId = iter->second;
		}

		for( iter = map.begin(); iter != map.end(); ++iter ) {
			if ( _codecId == iter->second ) {
				cout << "Codec: " << iter->first << endl;
				break;
			}
		}
				
	}

	// Locator - default is AAF Container 
	{
		pair<bool, int> netlocOpArg = args.get( "-netloc" );
		if ( netlocOpArg.first ) {
		
			pair<bool, const char*> netlocPrefix = args.get( netlocOpArg.second + 1 );
			pair<bool, const char*> netlocSuffix = args.get( netlocOpArg.second + 2 );
			if ( !(netlocPrefix.first && netlocSuffix.first) ) {
				throwUsage();
			}
			_netlocPrefix = AxString( AxStringUtil::mbtowc(netlocPrefix.second) );
			_netlocSuffix = AxString( AxStringUtil::mbtowc(netlocSuffix.second) );
			_containerId = ContainerFile;
			cout << "Container: ContainerFile, network locator = ";
			wcout << _netlocPrefix << L"#" << _netlocSuffix << endl;
		}
		else {
			_containerId == ContainerAAF;
			cout << "Container: ContainerAAF" << endl;
		}
	}


	// number of samples - required 
	{
		_symbol = "";
		_multiplier = 1;
		pair<bool, int> numSamplesOpArg = args.get( "-numSamples" );
		if ( !numSamplesOpArg.first ) {

			_symbol = "K";
			_multiplier = 1000;
			numSamplesOpArg = args.get( "-numKSamples" );
			if ( !numSamplesOpArg.first ) {
				
				_symbol = "M";
				_multiplier = 1000*1000;
				numSamplesOpArg = args.get( "-numMSamples" );
				if ( !numSamplesOpArg.first ) {
					throwUsage();
				}
			}
		}

		pair<bool, const char*> numSamples = args.get( numSamplesOpArg.second + 1 );
		if ( !numSamples.first ) {
			throwUsage();
		}
		_numSamples = ::atol( numSamples.second ) * _multiplier;
		if ( _numSamples <= 0 ) {
			throwUsage();
		}

		cout << "Number of samples: " << atoi( numSamples.second ) << " " << _symbol << "samples." << endl;
	}

}

//============================================================================

void create_mastermob_and_write_essence( AxHeader axHeader,
										 CmdLineParser& cmdLineParser )
{
	auto_ptr<SampleSource> sampleSource = cmdLineParser.CreateSampleSource();
	auto_ptr<EssenceLocator> essenceLocator = cmdLineParser.CreateEssenceLocator();

	AxDictionary axDictionary( axHeader.GetDictionary() );

	AxMasterMobEx axMasterMobEx(
		AxCreateInstance<IAAFMasterMobEx>( axDictionary ) );
	axMasterMobEx.Initialize();

	axHeader.AddMob( axMasterMobEx );

	axMasterMobEx.SetName( L"MasterMob with a Sequence of SourceClips" );
		
	aafRational_t sampleRate = sampleSource->GetRate();
	aafRational_t editRate = sampleRate;

	auto_ptr<AxEssenceAccess> axEssenceAccessPtr(
		new  AxEssenceAccess(
			axMasterMobEx.CreateEssence(
				1,
				axDictionary.LookupDataDef( sampleSource->GetDataDef() ),
				cmdLineParser.GetCodec(),
				editRate,
				sampleRate,
				sampleSource->GetCompression(),
				essenceLocator->GetNextLocator( axDictionary ),
				essenceLocator->GetNextContainer() ) ) );

	sampleSource->SetupFormatSpecifiers( *axEssenceAccessPtr );

	aafInt64 numSamplesWrittenTotal = 0;

	while ( !sampleSource->AtEnd() ) {
	
		pair<int, AxAutoPtr<aafUInt8> > samples = sampleSource->GetSamples();

		int numSamplesToWriteThisTime = samples.first;
		int numSamplesWrittenThisTime = 0;
	
		while( numSamplesWrittenThisTime < numSamplesToWriteThisTime ) {

			AxEssenceAccess::WriteResult writeResult = 
			  axEssenceAccessPtr->WriteSamples( numSamplesToWriteThisTime,
							  	  			    numSamplesToWriteThisTime * sampleSource->GetSampleByteSize(),
											    reinterpret_cast<aafDataBuffer_t>(samples.second.get()) );

			numSamplesWrittenThisTime += writeResult.samplesWritten;
			numSamplesWrittenTotal += writeResult.samplesWritten;

			cout << (unsigned int)(numSamplesWrittenTotal/cmdLineParser.GetCountMultiplier());
			cout << " " <<   cmdLineParser.GetCountSymbol() << "samples written" << endl;

			if ( writeResult.hr == AAFRESULT_EOF ) {

				cout << "Extending essence in order to continue writing beyond sample ";
				// FIXME - Must implement 64 bit integer output.
				cout << (unsigned int)(numSamplesWrittenTotal) << "." << endl;

				axEssenceAccessPtr->CompleteWrite();

				auto_ptr<AxEssenceAccess> axExtendedEssenceAccessPtr(
					new  AxEssenceAccess(
						axMasterMobEx.ExtendEssence(
						1,
						axDictionary.LookupDataDef( sampleSource->GetDataDef() ),
						cmdLineParser.GetCodec(),
						editRate,
						sampleRate,
						sampleSource->GetCompression(),
						essenceLocator->GetNextLocator( axDictionary ),
						essenceLocator->GetNextContainer() ) ) );

				sampleSource->SetupFormatSpecifiers( *axExtendedEssenceAccessPtr );
		
				axEssenceAccessPtr = axExtendedEssenceAccessPtr;
			}

		} // end while( numSamplesWritten < numSamplesToWrite )

	} // end while( !samplesSource.AtEnd() )

	axEssenceAccessPtr->CompleteWrite();
}

//============================================================================

void open_mastermob_and_read_essence( AxHeader& axHeader,
									  CmdLineParser& cmdLineParser )
{
	auto_ptr<SampleSource> sampleSource = cmdLineParser.CreateSampleSource();

	AxContentStorage axContentStorage( axHeader.GetContentStorage() );

	AxDictionary axDictionary( axHeader.GetDictionary() );

	aafSearchCrit_t	criteria;
	criteria.searchTag = kAAFByMobKind;
	criteria.tags.mobKind = kAAFMasterMob;
	AxMobIter axMobIter( axContentStorage.GetMobs( &criteria ) );
	IAAFMobSP nextMob;
	bool notAtEnd;

	// Expect at least one master mob, and read the first mob found.
	
	notAtEnd = axMobIter.NextOne( nextMob );
	assert( notAtEnd );
	
	AxMasterMob axMasterMob( AxQueryInterface<IAAFMob,IAAFMasterMob>( nextMob ) );

	wcout << L"MasterMob name: " << axMasterMob.GetName() << endl;
	wcout << L"Number of Slots: " << axMasterMob.CountSlots() << endl;

	AxDataDef axDataDef( axDictionary.LookupDataDef( sampleSource->GetDataDef() ) );

	AxEssenceAccess axEssenceAccess(
		axMasterMob.OpenEssence(1, 0, kAAFMediaOpenReadOnly, kAAFCompressionDisable ) );

	wcout << L"Codec Name: " << axEssenceAccess.GetCodecName() << endl;	

	aafLength_t maxSize = axEssenceAccess.GetLargestSampleSize( axDataDef );

	unsigned int divisor;
	char* symbol;
	if ( maxSize > 1024*1024 ) {
		symbol = "M";
		divisor = 1024/1024;
	}
	else if ( maxSize > 1024 ) {
		symbol = "K";
		divisor = 1024;
	}
	else {
		symbol = "";
		divisor = 1;
	}

	cout << "Largest Sample Size: " << (unsigned int)(maxSize/divisor);
	cout << " " << symbol << "bytes" << endl;

	aafLength_t numSamples = axEssenceAccess.CountSamples( axDataDef );

	if ( numSamples > 1024*1024 ) {
		symbol = "M";
		divisor = 1000000;
	}
	else if ( numSamples > 1024 ) {
		symbol = "K";
		divisor = 1000;
	}
	else {
		symbol = "";
		divisor = 1;
	}

	cout << "Number of samples: " << (unsigned int) numSamples/divisor;
	cout << " " << symbol << "samples" << endl;

	sampleSource->SetupFormatSpecifiers( axEssenceAccess );	

	int numSamplesPerChunk = sampleSource->GetNumSamplesPerChunk();
	int buflen = maxSize * numSamplesPerChunk;
	auto_ptr<char> buffer( new char [ buflen ] );

	int numSamplesStillToRead = numSamples;
	
	while( numSamplesStillToRead > 0 ) {

		AxEssenceAccess::ReadResult readResult;

		int numSamplesToRead;
		
		if ( numSamplesPerChunk > numSamplesStillToRead ) {
			numSamplesToRead = numSamplesStillToRead;
		}
		else {
			numSamplesToRead = numSamplesPerChunk;
		}
		
		
		readResult = axEssenceAccess.ReadSamples( numSamplesToRead, buflen, (aafDataBuffer_t)buffer.get() );

		numSamplesStillToRead -= readResult.samplesRead;

		cout << "Read " << (unsigned int)(numSamples - numSamplesStillToRead)/divisor;
		cout << " of " << (unsigned int)(numSamples/divisor) << " " << symbol << "samples" << endl;

	}

}

//============================================================================

int real_main( int argc, const char** argv )
{
	try {

		AxCmdLineArgs args( argc, argv );
		CmdLineParser cmdLineParser( args );

		// One time init stuff.  Including loading the com api library.
		AxInit initObj;
	
		// Open the file.
		AxFile axFile;
		

		if ( cmdLineParser.GetOp() == CmdLineParser::WRITE ) {

			axFile.OpenNewModify( cmdLineParser.GetAAFFileName() );
			
			AxHeader axHeader( axFile.getHeader() );
			create_mastermob_and_write_essence( axHeader, cmdLineParser );

			axFile.Save();
		}
		else if ( cmdLineParser.GetOp() == CmdLineParser::READ ) {

			axFile.OpenExistingRead( cmdLineParser.GetAAFFileName() );

			AxHeader axHeader( axFile.getHeader() );
			open_mastermob_and_read_essence( axHeader, cmdLineParser );
		}
		else {
			assert(0);
		}

		axFile.Close();
		wcout << endl << L"axMasterMobEx finished" << endl;
	}
	catch ( const AxEx& ex ) {
		wcout << ex.what() << endl;
	}

	return 0;
}

int main( int argc , char* argv[] )
{
#if 0
	char *fake_argv[] ={ argv[0],
						"-file",
						"c:/cygwin/tmp/mmex.aaf",
					    "-source", "audio", "-netloc", "c:/cygwin/tmp/ess", ".wav",
						"-numMSamples", "1700", "-write" };

	return real_main( sizeof(fake_argv)/sizeof(char*),
			          (const char**)fake_argv );
#elif 0
	char *fake_argv[] = { argv[0],
						  "-file", "c:/cygwin/tmp/mmex.aaf",
						  "-source", "video",
						  "-read" };
#else
	return real_main( argc, (const char**)argv );
#endif
}




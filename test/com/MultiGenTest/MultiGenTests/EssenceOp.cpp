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

#include <MultiGenTest.h>

#include "MultiGenCommon.h"

#include <AAFEssenceFormats.h>
#include <AAFStoredObjectIDs.h>
#include <AAFDataDefs.h>
#include <AAFContainerDefs.h>

#include <assert.h>

#include <memory>

namespace {

//====================================================================

template <class Type>
inline void add_format_specifier( IAAFEssenceFormat* format,
				  const aafUID_t& id,
				  Type& value )
{
  CHECK_HRESULT(
    format->AddFormatSpecifier(
      id,
      sizeof(Type),
      reinterpret_cast<aafDataBuffer_t>(&value) ) );
}

//====================================================================

void write_video_samples( IAAFSmartPointer<IAAFEssenceAccess> access,
			  int numSamples )
{
  IAAFSmartPointer<IAAFEssenceFormat> format;
  CHECK_HRESULT( access->GetEmptyFileFormat( &format ) );
    
  aafRect_t rect = {0,0,MULTI_GEN_VIDEO_WIDTH,MULTI_GEN_VIDEO_HEIGHT};
  add_format_specifier(  format, kAAFStoredRect, rect );
  
  aafUInt32 horizontalSubsampling = 2;
  add_format_specifier( format, kAAFCDCIHorizSubsampling, horizontalSubsampling );
  
  aafColorSpace_t colorSpace = kAAFColorSpaceYUV;
  add_format_specifier( format, kAAFPixelFormat, colorSpace );
  
  CHECK_HRESULT( access->PutFileFormat( format ) );

  int numSamplesStillToWrite = numSamples;
  
  while (numSamplesStillToWrite > 0) {
    
    const int numBytesPerSample = 2 * rect.xSize * rect.ySize;
    auto_ptr<aafUInt8> pixels( new aafUInt8 [numBytesPerSample] );
    
    // TODO - Write some unique data into the buffer that can be
    // verified in a later test.  Probably will have to disable
    // compression to do that easily.
    
    aafUInt32 samplesWritten;
    aafUInt32 bytesWritten;
    CHECK_HRESULT( access->WriteSamples( 1,
					 numBytesPerSample,
					 reinterpret_cast<aafDataBuffer_t>(pixels.get()),
					 &samplesWritten,
					 &bytesWritten ) );
    assert( bytesWritten == numBytesPerSample );
    assert( samplesWritten == 1 );
    numSamplesStillToWrite -= samplesWritten;
  }
  
  CHECK_HRESULT( access->CompleteWrite() );
}

//====================================================================

void write_audio_samples( IAAFSmartPointer<IAAFEssenceAccess> access,
			  int numSamples ) 
{
  IAAFSmartPointer<IAAFEssenceFormat> format;
  CHECK_HRESULT( access->GetEmptyFileFormat( &format ) );
    
  aafUInt32 sampleSize = MULTI_GEN_AUDIO_SAMPLE_BPP;
  add_format_specifier( format, kAAFAudioSampleBits, sampleSize );
  
  CHECK_HRESULT( access->PutFileFormat( format ) );
  
  int numSamplesStillToWrite = numSamples;

  while (numSamplesStillToWrite > 0) {

    int numSamplesThisTime = 1000;

    if ( numSamplesThisTime > numSamplesStillToWrite ) {
      numSamplesThisTime = numSamplesStillToWrite;
    }

    const int numBytesPerSample = (MULTI_GEN_AUDIO_SAMPLE_BPP+7)/8;
    const int numBytesThisTime = numSamplesThisTime * numBytesPerSample;
    auto_ptr<aafUInt8> pixels( new aafUInt8 [numBytesThisTime] );
    
    // TODO - Write some unique data into the buffer that can be
    // verified in a later test.  Probably will have to disable
    // compression to do that easily.
    
    aafUInt32 samplesWritten;
    aafUInt32 bytesWritten;

    CHECK_HRESULT( access->WriteSamples( numSamplesThisTime,
					 numBytesThisTime,
					 reinterpret_cast<aafDataBuffer_t>(pixels.get()),
					 &samplesWritten,
					 &bytesWritten ) );
    assert( bytesWritten == numBytesThisTime );
    assert( samplesWritten == numSamplesThisTime );
    numSamplesStillToWrite -= samplesWritten;
  }
  
  CHECK_HRESULT( access->CompleteWrite() );
}

//====================================================================

STANDARD_TEST_DECLARATION(EssenceCreate);
StandardFactory<EssenceCreate> EssenceCreateFactory(
  "EssenceCreate",
  "Create essence data structure for a named master mob, and write some samples.",
  "master_mob_name slot_id {sound|picture} {cdci|jpeg|wave|aifc} {compressed|uncompressed} num_samples [file_path]",
  "Optinally include \"file_path\" to store essence in an external file.",
  7,
  8
  );

void EssenceCreate::RunTest( CmdState& state, int argc, char** argv )
{
  const char* name = argv[1];
  aafSlotID_t slotId = ToULong( argv[2] );
  string type( argv[3] );
  string codec( argv[4] );
  string compress( argv[5] );
  unsigned long numSamples = ToULong( argv[6] );

  char *file_path = 0;
  if ( argc == 8 ) {
    file_path = argv[7];
  }

  aafUID_t codecId;
  aafUID_t dataDefId;
  aafRational_t sampleRate;
  aafRational_t editRate;
  aafCompressEnable_t compressEnable;
  
  editRate.numerator = MULTI_GEN_EDIT_RATE_NUM;
  editRate.denominator = MULTI_GEN_EDIT_RATE_DEN;

  if ( compress == "compressed" ) {
    compressEnable = kAAFCompressionEnable;
  }
  if ( compress == "uncompressed" ) {
    compressEnable = kAAFCompressionDisable;
  }
  else {
    throw UsageEx( "compress argument must be one of {compressed|uncompressed}" );
  }
  
  if ( codec == "cdci" ) {
    codecId = kAAFCodecCDCI;
  }
  else if ( codec == "jpeg" ) {
    codecId = kAAFCodecJPEG;
  }
  else if ( codec == "wave" ) {
    codecId = kAAFCodecWAVE;
  }
  else if ( codec == "aifc" ) {
    codecId = kAAFCODEC_AIFC;
  }
  else {
    throw UsageEx( "codec not one of {cdci|jpeg|wave|cdci}" );
  }

  if ( type == "picture" ) {
    dataDefId = DDEF_Picture;
    sampleRate.numerator = MULTI_GEN_VIDEO_RATE_NUM;
    sampleRate.denominator = MULTI_GEN_VIDEO_RATE_DEN;
  }
  else if ( type == "sound" ) {
    dataDefId = DDEF_Sound;
    sampleRate.numerator = MULTI_GEN_AUDIO_RATE_NUM;
    sampleRate.denominator = MULTI_GEN_AUDIO_RATE_DEN;
  }
  else {
    throw UsageEx( "data type not one of {sound|picture}" );
  }

  IAAFSmartPointer<IAAFFile> file = state.GetFile();
  IAAFSmartPointer<IAAFHeader> header;
  IAAFSmartPointer<IAAFDictionary> dictionary;
  CHECK_HRESULT( file->GetHeader( &header ) );
  CHECK_HRESULT( header->GetDictionary( &dictionary ) );

  IAAFSmartPointer<IAAFDataDef> dataDef;
  CHECK_HRESULT( dictionary->LookupDataDef( dataDefId, &dataDef ) );

  aafUID_t containerId;
  IAAFSmartPointer<IAAFLocator> locator;

  if ( file_path ) {
    containerId = ContainerFile;

    IAAFSmartPointer<IAAFNetworkLocator> netLocator;
    CHECK_HRESULT( dictionary->CreateInstance( AUID_AAFNetworkLocator,
					       IID_IAAFNetworkLocator,
					       ToIUnknown(&netLocator) ) );
    CHECK_HRESULT( netLocator->Initialize() );
    CHECK_HRESULT( netLocator->QueryInterface( IID_IAAFLocator,
					       ToVoid( &locator ) ) );
    auto_ptr<wchar_t> wpath( ToWideString( file_path ) );
    CHECK_HRESULT( locator->SetPath( wpath.get() ) );
  }
  else {
    containerId = ContainerAAF;
  }

  IAAFSmartPointer<IAAFMasterMob> mob;
  get_mob_throw_if_not_found( state, name, mob );

  IAAFSmartPointer<IAAFEssenceAccess> access;

  CHECK_HRESULT( mob->CreateEssence( slotId, dataDef, codecId,
				     editRate, sampleRate, compressEnable,
				     locator,
				     containerId,
				     &access ) );

  if ( type == "picture" ) {
    write_video_samples( access, numSamples );
  }
  else {
    assert( type == "sound" );
    write_audio_samples( access, numSamples );
  }
}

//====================================================================

STANDARD_TEST_DECLARATION(EssenceAppend);
StandardFactory<EssenceAppend> EssenceAppendFactory(
  "EssenceAppend",
  "Append essence data to existing essence.",
  "master_mob_name slot_id {compressed|uncompressed} num_samples",
  "Data type is determined automatically.",
  5,
  5 
  );

void EssenceAppend::RunTest( CmdState& state, int argc, char** argv)
{
  const char* name = argv[1];
  aafSlotID_t slotId = ToULong( argv[2] );
  string compress( argv[3] );
  unsigned long numSamples = ToULong( argv[4] );

  aafCompressEnable_t compressEnable;
 
  if ( compress == "compressed" ) {
    compressEnable = kAAFCompressionEnable;
  }
  else if ( compress == "uncompressed" ) {
    compressEnable = kAAFCompressionDisable;
  }
  else {
    throw UsageEx( "compress argument must be one of {compress|nocompress}" );
  }

  IAAFSmartPointer<IAAFMasterMob> masterMob;
  get_mob_throw_if_not_found( state, name, masterMob );

  IAAFSmartPointer<IAAFEssenceAccess> access;
  CHECK_HRESULT( masterMob->OpenEssence( slotId,
					 0, // No search criteria
					 kAAFMediaOpenAppend,
					 compressEnable,
					 &access ) );
  
  IAAFSmartPointer<IAAFMob> mob;
  CHECK_HRESULT( masterMob->QueryInterface( IID_IAAFMob, ToVoid( &mob ) ) );
  IAAFSmartPointer<IAAFMobSlot> slot;
  CHECK_HRESULT( mob->LookupSlot( slotId, &slot ) );

  IAAFSmartPointer<IAAFDataDef> dataDef;
  CHECK_HRESULT( slot->GetDataDef( &dataDef ) );

  aafBoolean_t isPicture;
  CHECK_HRESULT( dataDef->IsPictureKind( &isPicture ) );

  aafBoolean_t isSound;
  CHECK_HRESULT( dataDef->IsSoundKind( &isSound ) );

  if ( isPicture ) {
    write_video_samples( access, numSamples );
  }
  else if ( isSound ) {
    write_audio_samples( access, numSamples );
  }
  else {
    throw TestFailedEx( "DataDef is not picture or sound." );
  }

}

//====================================================================

STANDARD_TEST_DECLARATION(CountSamples);
StandardFactory<CountSamples> CountSamplesFactory(
  "CountSamples",
  "Open essence read only and verify the sample count.",
  "master_mob_name slot_id {compressed|uncompressed| expected_num_samples",
  "Another test is required to verify the actual essence data.",
  5,
  5
  );

void CountSamples::RunTest( CmdState& state, int argc, char** argv)
{
  const char* name = argv[1];
  aafSlotID_t slotId = ToULong( argv[2] );
  string compress( argv[3] );
  unsigned long expectedCount = ToULong( argv[4] );

  aafCompressEnable_t compressEnable;
 
  if ( compress == "compressed" ) {
    compressEnable = kAAFCompressionEnable;
  }
  else if ( compress == "uncompressed" ) {
    compressEnable = kAAFCompressionDisable;
  }
  else {
    throw UsageEx( "compress argument must be one of {compress|nocompress}" );
  }

  IAAFSmartPointer<IAAFMasterMob> masterMob;
  get_mob_throw_if_not_found( state, name, masterMob );

  IAAFSmartPointer<IAAFEssenceAccess> access;
  CHECK_HRESULT( masterMob->OpenEssence( slotId,
					 0, // No search criteria
					 kAAFMediaOpenReadOnly,
					 compressEnable,
					 &access ) );
 
  IAAFSmartPointer<IAAFMob> mob;
  CHECK_HRESULT( masterMob->QueryInterface( IID_IAAFMob, ToVoid( &mob ) ) );
  IAAFSmartPointer<IAAFMobSlot> slot;
  CHECK_HRESULT( mob->LookupSlot( slotId, &slot ) );

  IAAFSmartPointer<IAAFDataDef> dataDef;
  CHECK_HRESULT( slot->GetDataDef( &dataDef ) );

  aafLength_t count;
  CHECK_HRESULT( access->CountSamples( dataDef, &count ) );

  if ( expectedCount != count ) {
    stringstream anError;
    anError << "sample counts do not match ("
	    << static_cast<int>(count) << " != " << static_cast<int>(expectedCount) << ")";
    throw TestFailedEx( anError.str() );
  }
}

//====================================================================

} // end of namespace

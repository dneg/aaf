#ifndef __AxEssence_h__
#define __AxEssence_h__

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
//=---------------------------------------------------------------------=


#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

#include <utility>
#include <vector>

//=---------------------------------------------------------------------=

class AxEssenceMultiAccess {
public:
	AxEssenceMultiAccess( IAAFEssenceMultiAccessSP spIaafEssenceMultiAccess );
	~AxEssenceMultiAccess();

private:
	AxEssenceMultiAccess();
	AxEssenceMultiAccess( const AxEssenceMultiAccess& );
	AxEssenceMultiAccess& operator=( const AxEssenceMultiAccess& );
	
	
	IAAFEssenceMultiAccessSP _spIaafEssenceMultiAccess;
};

//=---------------------------------------------------------------------=

class AxEssenceAccess : public AxEssenceMultiAccess {
public:

	struct WriteResult {
		HRESULT hr;
		aafUInt32 samplesWritten;
		aafUInt32 bytesWritten;
	};

	struct ReadResult {
		aafUInt32 samplesRead;
		aafUInt32 bytesRead;
	};

	AxEssenceAccess( IAAFEssenceAccessSP spIaafEssenceAccess );
	~AxEssenceAccess();

	IAAFEssenceFormatSP GetEmptyFileFormat();

	IAAFEssenceFormatSP GetFileFormat( IAAFEssenceFormatSP spTemplate );

	IAAFEssenceFormatSP GetFileFormatParameterList();

	void PutFileFormat( IAAFEssenceFormatSP spIaafEssenceFormat );

	aafLength_t CountSamples( IAAFDataDefSP spDataDef );
		
	WriteResult WriteSamples( aafUInt32 nSamples,
					  		  aafUInt32 bufLength,
							  aafDataBuffer_t buffer );

	void CompleteWrite();

	AxString GetCodecName();

	aafLength_t GetLargestSampleSize( IAAFDataDefSP spIaafDataDef );

	ReadResult ReadSamples( aafUInt32 nSamples,
					  	    aafUInt32 buflen,
						    aafDataBuffer_t buffer );

private:
	AxEssenceAccess();
	AxEssenceAccess( const AxEssenceAccess& );
	AxEssenceAccess& operator=( const AxEssenceAccess& );
	
	IAAFEssenceAccessSP _spIaafEssenceAccess;
};


//=---------------------------------------------------------------------=

class AxLocator : public AxObject {
public:
	AxLocator( IAAFLocatorSP spIaafLocator );
	~AxLocator();

	void SetPath( const AxString& path );
	AxString GetPath();

	inline operator IAAFLocatorSP ()
	{ return _spIaafLocator; }

private:
	AxLocator();
	AxLocator( const AxLocator& );
	AxLocator& operator=( const AxLocator& );
	
	IAAFLocatorSP _spIaafLocator;
};


//=---------------------------------------------------------------------=

class AxNetworkLocator : public AxLocator {
public:
	AxNetworkLocator( IAAFNetworkLocatorSP spIaafNetworkLocator );
	~AxNetworkLocator();

	void Initialize();

	inline operator IAAFNetworkLocatorSP ()
	{ return _spIaafNetworkLocator; }

private:
	AxNetworkLocator();
	AxNetworkLocator( const AxNetworkLocator& );
	AxNetworkLocator& operator=( const AxNetworkLocator& );
	
	IAAFNetworkLocatorSP _spIaafNetworkLocator;
};

//=---------------------------------------------------------------------=

class AxEssenceDescriptor : public AxObject {
public:
	AxEssenceDescriptor( IAAFEssenceDescriptorSP spIaafEssenceDescriptor );
	~AxEssenceDescriptor();

	aafUInt32 CountLocators();
	void AppendLocator( IAAFLocatorSP spLocator );
	IEnumAAFLocatorsSP GetLocators();

	inline operator IAAFEssenceDescriptorSP ()
	{ return _spIaafEssenceDescriptor; }
private:
	AxEssenceDescriptor();
	AxEssenceDescriptor( const AxEssenceDescriptor& );
	AxEssenceDescriptor& operator=( const AxEssenceDescriptor& );
	
	IAAFEssenceDescriptorSP _spIaafEssenceDescriptor;
};

//=---------------------------------------------------------------------=

class AxFileDescriptor : public AxEssenceDescriptor {
public:
	AxFileDescriptor( IAAFFileDescriptorSP spIaafFileDescriptor );
	~AxFileDescriptor();

	IAAFCodecDefSP GetCodecDef();
	IAAFContainerDefSP GetContainerFormat();
	aafLength_t GetLength();
	aafRational_t GetSampleRate();

	void SetCodecDef(IAAFCodecDefSP codec);
	void SetContainerFormat(IAAFContainerDefSP container);
	void SetLength(aafLength_t length);
	void SetSampleRate(aafRational_constref rate);

private:
	AxFileDescriptor();
	AxFileDescriptor( const AxFileDescriptor& );
	AxFileDescriptor& operator=( const AxFileDescriptor& );
	
	IAAFFileDescriptorSP _spIaafFileDescriptor;
};

//=---------------------------------------------------------------------=

class AxWAVEDescriptor : public AxFileDescriptor {
public:

	AxWAVEDescriptor( IAAFWAVEDescriptorSP spIaafWAVEDescriptor );
	~AxWAVEDescriptor();

	void SetSummary( aafUInt32 size, aafDataValue_t  pSummary );

	AxBuffer<aafUInt8> GetSummary();

private:
	AxWAVEDescriptor();
	AxWAVEDescriptor( const AxWAVEDescriptor& );
	AxWAVEDescriptor& operator=( const AxWAVEDescriptor& );
	
	IAAFWAVEDescriptorSP _spIaafWAVEDescriptor;
};

//=---------------------------------------------------------------------=

class AxDigitalImageDescriptor : public AxFileDescriptor {
public:
	AxDigitalImageDescriptor( IAAFDigitalImageDescriptorSP spIaafDigitalImageDescriptor );
	~AxDigitalImageDescriptor();

	void SetStoredView( aafUInt32 StoredHeight, aafUInt32 StoredWidth);

	void SetSampledView( aafUInt32 SampledHeight, aafUInt32 SampledWidth,
			             aafInt32 SampledXOffset, aafInt32 SampledYOffset );
	
	void SetDisplayView( aafUInt32 DisplayedHeight, aafUInt32 DisplayedWidth,
				 	     aafInt32 DisplayedXOffset, aafInt32 DisplayedYOffset );

	void SetFrameLayout( aafFrameLayout_t FrameLayout );

	void SetVideoLineMap( aafUInt32  numberElements, const aafInt32*  pVideoLineMap );

	void SetCompression( const aafUID_t& compression );

	void SetAlphaTransparency( const aafAlphaTransparency_t& alphaTransparency );

	void SetGamma( const aafUID_t& gamma );

	void SetImageAspectRatio( const aafRational_t & aspect);

	void SetImageAlignmentFactor( aafUInt32 ImageAlignmentFactor);



	void GetStoredView( aafUInt32& StoredHeight, aafUInt32& StoredWidth);

	void GetSampledView( aafUInt32& SampledHeight, aafUInt32& SampledWidth,
			             aafInt32& SampledXOffset, aafInt32& SampledYOffset );
	
	void GetDisplayView( aafUInt32& DisplayedHeight, aafUInt32& DisplayedWidth,
			             aafInt32& DisplayedXOffset, aafInt32& DisplayedYOffset );

	aafFrameLayout_t GetFrameLayout();

	// FIXME - surely we can do better than this
	void GetVideoLineMap( aafUInt32  numberElements, aafInt32*  pVideoLineMap );

	aafUID_t GetCompression();

	aafAlphaTransparency_t GetAlphaTransparency();
	
	aafUID_t GetGamma();

	aafRational_t GetImageAspectRatio();

	aafUInt32 GetImageAlignmentFactor();

private:
	AxDigitalImageDescriptor();
	AxDigitalImageDescriptor( const AxDigitalImageDescriptor& );
	AxDigitalImageDescriptor& operator=( const AxDigitalImageDescriptor& );
	
	IAAFDigitalImageDescriptorSP _spIaafDigitalImageDescriptor;
};

//=---------------------------------------------------------------------=

class AxCDCIDescriptor : public AxDigitalImageDescriptor {
public:
	AxCDCIDescriptor( IAAFCDCIDescriptorSP spIaafCDCIDescriptor );
	~AxCDCIDescriptor();

	void SetPaddingBits( aafInt16 PaddingBits );
	void SetComponentWidth( aafInt32 ComponentWidth );
	void SetHorizontalSubsampling( aafUInt32  HorizontalSubsampling );
	void SetVerticalSubsampling( aafUInt32 VerticalSubsampling );
	void SetColorSiting( const aafColorSiting_t& ColorSiting );
	void SetBlackReferenceLevel( aafUInt32 BlackReferenceLevel );
	void SetWhiteReferenceLevel( aafUInt32 WhiteReferenceLevel );
	void SetColorRange( aafUInt32 ColorRange );

	aafInt16  GetPaddingBits();
	aafInt32  GetComponentWidth();
	aafUInt32 GetHorizontalSubsampling();
	aafUInt32 GetVerticalSubsampling();
	aafColorSiting_t GetColorSiting();
	aafUInt32 GetBlackReferenceLevel();
	aafUInt32 GetWhiteReferenceLevel();
	aafUInt32 GetColorRange();

private:
	AxCDCIDescriptor();
	AxCDCIDescriptor( const AxCDCIDescriptor& );
	AxCDCIDescriptor& operator=( const AxCDCIDescriptor& );
	
	IAAFCDCIDescriptorSP _spIaafCDCIDescriptor;
};

//=---------------------------------------------------------------------=

class AxRGBADescriptor : public AxDigitalImageDescriptor {
public:
    AxRGBADescriptor( IAAFRGBADescriptor2SP spIaafRGBADescriptor );
    AxRGBADescriptor( IAAFRGBADescriptorSP spIaafRGBADescriptor );
    ~AxRGBADescriptor();

    void SetPixelLayout( std::vector<aafRGBAComponent_t> pixelLayoutArray );
    void SetPalette( std::vector<aafUInt8> palette );
    void SetPaletteLayout( std::vector<aafRGBAComponent_t> paletteLayoutArray );
    void SetComponentMaxRef( aafUInt32 componentMaxRef );
    void SetComponentMinRef( aafUInt32 componentMinRef );
    void SetAlphaMaxRef( aafUInt32 alphaMaxRef );
    void SetAlphaMinRef( aafUInt32 alphaMinRef );
    void SetScanningDirection( aafScanningDirection_t scanningDirection );

    std::vector<aafRGBAComponent_t> GetPixelLayout();
    std::vector<aafUInt8> GetPalette();
    std::vector<aafRGBAComponent_t> GetPaletteLayout();
    aafUInt32 GetComponentMaxRef();
    aafUInt32 GetComponentMinRef();
    aafUInt32 GetAlphaMaxRef();
    aafUInt32 GetAlphaMinRef();
    aafScanningDirectionType_t GetScanningDirection();

    inline operator IAAFRGBADescriptorSP ()
    { return AxQueryInterface<IAAFRGBADescriptor2,IAAFRGBADescriptor>( _spIaafRGBADescriptor ); }

    inline operator IAAFRGBADescriptor2SP ()
    { return _spIaafRGBADescriptor; }

private:
    AxRGBADescriptor();
    AxRGBADescriptor( const AxRGBADescriptor& );
    AxRGBADescriptor& operator=( const AxRGBADescriptor& );
    
    IAAFRGBADescriptor2SP _spIaafRGBADescriptor;
};

//=---------------------------------------------------------------------=

class AxTapeDescriptor : public AxEssenceDescriptor {
public:
	AxTapeDescriptor( IAAFTapeDescriptorSP spIaafTapeDescriptor );
	~AxTapeDescriptor();

	void Initialize();

	void SetTapeManufacturer( const AxString& manufacturer );
	AxString GetTapeManufacturer();

	void SetTapeModel( const AxString& model );
	AxString GetTapeModel();

	void SetTapeFormFactor( aafTapeCaseType_t formFactor );
	aafTapeCaseType_t GetTapeFormFactor();

	void SetSignalType( aafVideoSignalType_t signalType );
	aafVideoSignalType_t GetSignalType();

	void SetTapeFormat( aafTapeFormatType_t tapeFormat );
	aafTapeFormatType_t GetTapeFormat();

	void SetTapeLength( aafUInt32 length );
	aafUInt32 GetTapeLength();

	inline operator IAAFTapeDescriptorSP ()
	{ return _spIaafTapeDescriptor; }

private:
	AxTapeDescriptor();
	AxTapeDescriptor( const AxTapeDescriptor& );
	AxTapeDescriptor& operator=( const AxTapeDescriptor& );

	IAAFTapeDescriptorSP _spIaafTapeDescriptor;
};

//=---------------------------------------------------------------------=

class AxFilmDescriptor : public AxEssenceDescriptor {
public:
    AxFilmDescriptor( IAAFFilmDescriptorSP spIaafFilmDescriptor );
    ~AxFilmDescriptor();

    void SetFilmManufacturer( const AxString& manufacturer );
    AxString GetFilmManufacturer();
    
    void SetFilmModel( const AxString& model );
    AxString GetFilmModel();
    
    void SetFilmFormat( aafFilmType_t filmFormat );
    aafFilmType_t GetFilmFormat();
    
    void SetFrameRate( aafUInt32 rate );
    aafUInt32 GetFrameRate();
    
    void SetPerfPerFrame( aafUInt8 perfPerFrame );
    aafUInt8 GetPerfPerFrame();
    
    void SetFilmAspectRatio( aafRational_t aspectRatio );
    aafRational_t GetFilmAspectRatio();

    inline operator IAAFFilmDescriptorSP ()
    { return _spIaafFilmDescriptor; }

private:
    AxFilmDescriptor();
    AxFilmDescriptor( const AxFilmDescriptor& );
    AxFilmDescriptor& operator=( const AxFilmDescriptor& );

    IAAFFilmDescriptorSP _spIaafFilmDescriptor;
};

//=---------------------------------------------------------------------=

class AxPhysicalDescriptor : public AxEssenceDescriptor {
public:
    AxPhysicalDescriptor( IAAFPhysicalDescriptorSP spIaafPhysicalDescriptor );
    ~AxPhysicalDescriptor();

    inline operator IAAFPhysicalDescriptorSP ()
    { return _spIaafPhysicalDescriptor; }

private:
    AxPhysicalDescriptor();
    AxPhysicalDescriptor( const AxPhysicalDescriptor& );
    AxPhysicalDescriptor& operator=( const AxPhysicalDescriptor& );

    IAAFPhysicalDescriptorSP _spIaafPhysicalDescriptor;
};

//=---------------------------------------------------------------------=

class AxImportDescriptor : public AxPhysicalDescriptor {
public:
    AxImportDescriptor( IAAFImportDescriptorSP spIaafImportDescriptor );
    ~AxImportDescriptor();
    
    void Initialize();

    inline operator IAAFImportDescriptorSP ()
    { return _spIaafImportDescriptor; }

private:
    AxImportDescriptor();
    AxImportDescriptor( const AxImportDescriptor& );
    AxImportDescriptor& operator=( const AxImportDescriptor& );

    IAAFImportDescriptorSP _spIaafImportDescriptor;
};

//=---------------------------------------------------------------------=

class AxRecordingDescriptor : public AxPhysicalDescriptor {
public:
    AxRecordingDescriptor( IAAFRecordingDescriptorSP spIaafRecordingDescriptor );
    ~AxRecordingDescriptor();
    
    void Initialize();

    inline operator IAAFRecordingDescriptorSP ()
    { return _spIaafRecordingDescriptor; }

private:
    AxRecordingDescriptor();
    AxRecordingDescriptor( const AxRecordingDescriptor& );
    AxRecordingDescriptor& operator=( const AxRecordingDescriptor& );

    IAAFRecordingDescriptorSP _spIaafRecordingDescriptor;
};

//=---------------------------------------------------------------------=

// Intentionally not derived from AxBaseObj.  I think it is just a
// utility interface, not directly tied to any particular object.

class AxEssenceFormat {
public:
	AxEssenceFormat( IAAFEssenceFormatSP spIaafEssenceFormat );
	~AxEssenceFormat();

	void AddFormatSpecifier( const aafUID_t& essenceFormatCode,
							 aafInt32 valueSize,
							 aafDataBuffer_t pValue );

	
	void GetFormatSpecifier( const aafUID_t& essenceFormatCode,
							 aafInt32 valueSize,
							 aafDataBuffer_t pValue,
							 aafInt32& bytesRead );


	template <typename FormatType>
	void AddFormatSpecifier( const aafUID_t& essenceFormatCode,
							 const FormatType& value )
	{
		AddFormatSpecifier( essenceFormatCode, sizeof(FormatType),
							reinterpret_cast<aafDataBuffer_t>(
								const_cast<FormatType*>(&value)) );
	}

	template <typename FormatType>
	void GetFormatSpecifier( const aafUID_t& essenceFormatCode,
							 FormatType& valueRet )
	{
		aafInt32 bytesRead = 0;

		GetFormatSpecifier( essenceFormatCode, sizeof(FormatType),
							reinterpret_cast<aafDataBuffer_t>(&valueRet),
							bytesRead );

		if ( sizeof(FormatType) != bytesRead ) {
			throw AxEx( L"unexpected size returned by AxEssenceFormat::GetFormatSpecifier()" );
		}
	}

	inline operator IAAFEssenceFormatSP ()
	{ return _spIaafEssenceFormat; }

private:
	AxEssenceFormat();
	AxEssenceFormat( const AxEssenceFormat& );
	AxEssenceFormat& operator=( const AxEssenceFormat& );
	
	IAAFEssenceFormatSP _spIaafEssenceFormat;
};

//=---------------------------------------------------------------------=

class AxAuxiliaryDescriptor : public AxPhysicalDescriptor {
public:
    AxAuxiliaryDescriptor( IAAFAuxiliaryDescriptorSP spIaafAuxiliaryDescriptor );
    ~AxAuxiliaryDescriptor();
    
    void Initialize();

    AxString GetMimeType ();
    AxString GetCharSet ();
    
    void SetMimeType ( const AxString& mimeType );
    void SetCharSet ( const AxString& charSet );

    inline operator IAAFAuxiliaryDescriptorSP ()
    { return _spIaafAuxiliaryDescriptor; }

private:
    AxAuxiliaryDescriptor();
    AxAuxiliaryDescriptor( const AxAuxiliaryDescriptor& );
    AxAuxiliaryDescriptor& operator=( const AxAuxiliaryDescriptor& );

    IAAFAuxiliaryDescriptorSP _spIaafAuxiliaryDescriptor;
};

//=---------------------------------------------------------------------=

class AxSoundDescriptor : public AxFileDescriptor {
public:
    AxSoundDescriptor( IAAFSoundDescriptorSP spIaafSoundDescriptor );
    ~AxSoundDescriptor();
    
    aafUID_t GetCompression();
    aafUInt32 GetChannelCount();
    aafRational_t GetAudioSamplingRate();
    aafBoolean_t IsLocked();
    aafElectroSpatialFormulation_t GetElectroSpatialFormulation();
    aafInt8 GetAudioRefLevel();
    aafInt8 GetDialNorm();
    aafUInt32 GetQuantizationBits();
                    
    void SetCompression( aafUID_constref compression );
    void SetChannelCount( aafUInt32 channelCount );
    void SetAudioSamplingRate( aafRational_t rate );
    void SetIsLocked( aafBoolean_t locked );
    void SetElectroSpatialFormulation( aafElectroSpatialFormulation_t formulation );
    void SetAudioRefLevel( aafInt8 level );
    void SetDialNorm( aafInt8 dialNorm );
    void SetQuantizationBits( aafUInt32 bitsCount );

    inline operator IAAFSoundDescriptorSP ()
    { return _spIaafSoundDescriptor; }

private:
    AxSoundDescriptor();
    AxSoundDescriptor( const AxSoundDescriptor& );
    AxSoundDescriptor& operator=( const AxSoundDescriptor& );

    IAAFSoundDescriptorSP _spIaafSoundDescriptor;
};

#endif

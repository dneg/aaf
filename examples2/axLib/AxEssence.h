#ifndef __AxEssence_h__
#define __AxEssence_h__

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


#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxObject.h"

#include <utility>

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

	void PutFileFormat( IAAFEssenceFormatSP spIaafEssenceFormat );

	aafLength_t CountSamples( IAAFDataDefSP spDataDef );
		
	// pair<samplesWritten, bytesWritten>
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

private:
	AxNetworkLocator();
	AxNetworkLocator( const AxNetworkLocator& );
	AxNetworkLocator& operator=( const AxNetworkLocator& );
	
	IAAFNetworkLocatorSP _spIaafNetworkLocator;
};

//=---------------------------------------------------------------------=

class AxDefObject : public AxObject {
public:
	AxDefObject( IAAFDefObjectSP spIaafDefObject );
	~AxDefObject();

	aafUID_t GetAUID();

	AxString GetName();

	AxString GetDescription();

	inline operator IAAFDefObjectSP ()
	{ return _spIaafDefObject; }

private:
	AxDefObject();
	AxDefObject ( const AxDefObject& );
	AxDefObject & operator=( const AxDefObject & );
	
	IAAFDefObjectSP _spIaafDefObject;
};

//=---------------------------------------------------------------------=

class AxCodecDef : public AxDefObject {
public:
	AxCodecDef( IAAFCodecDefSP spIaafCodecDef );
	~AxCodecDef();

	aafBoolean_t IsEssenceKindSupported( IAAFDataDefSP spIaafDataDef );

	aafUInt32 CountEssenceKinds();

	IEnumAAFDataDefsSP GetEssenceKinds();

	aafBoolean_t AreThereFlavours();

	IEnumAAFCodecFlavoursSP EnumCodecFlavours();


private:
	AxCodecDef();
	AxCodecDef( const AxCodecDef& );
	AxCodecDef& operator=( const AxCodecDef& );

    IAAFCodecDefSP _spIaafCodecDef;
};

//=---------------------------------------------------------------------=

class AxDataDef: public AxDefObject {
public:
	AxDataDef( IAAFDataDefSP spIaafDataDef );
	~AxDataDef();

	bool IsSoundKind();
	bool IsPictureKind();
		
	inline operator IAAFDataDefSP ()
	{ return _spIaafDataDef; }

private:
	AxDataDef();
	AxDataDef( const AxDataDef& );
	AxDataDef& operator=( const AxDataDef& );
	
	IAAFDataDefSP _spIaafDataDef;
};

//=---------------------------------------------------------------------=

class AxOperationDef: public AxDefObject {
public:
	AxOperationDef( IAAFOperationDefSP spIaafOperationDef );
	~AxOperationDef();

	void Initialize( const aafUID_t&,
				const AxString& name,
				const AxString& desc );
	
	void SetDataDef( IAAFDataDefSP );
	void SetIsTimeWarp( aafBoolean_t );
	void SetCategory( const aafUID_t& category_auid );
	void SetNumberInputs( aafInt32 );
	void SetBypass( aafUInt32 );
	
	inline operator IAAFOperationDefSP ()
	{ return _spIaafOperationDef; }

private:
	AxOperationDef();
	AxOperationDef( const AxOperationDef& );
	AxOperationDef& operator=( const AxOperationDef& );
	
	IAAFOperationDefSP _spIaafOperationDef;
};

//=---------------------------------------------------------------------=

class AxEssenceDescriptor : public AxObject {
public:
	AxEssenceDescriptor( IAAFEssenceDescriptorSP spIaafEssenceDescriptor );
	~AxEssenceDescriptor();
	aafUInt32 CountLocators();

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
					aafUInt32 SampledXOffset, aafUInt32 SampledYOffset );
	
	void SetDisplayView( aafUInt32 DisplayedHeight, aafUInt32 DisplayedWidth,
				 	     aafUInt32 DisplayedXOffset, aafUInt32 DisplayedYOffset );

	void SetFrameLayout( aafFrameLayout_t FrameLayout );

	void SetVideoLineMap( aafUInt32  numberElements, const aafInt32*  pVideoLineMap );

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

	void SetComponentWidth( aafInt32 ComponentWidth );
	void SetHorizontalSubsampling( aafUInt32  HorizontalSubsampling );
	void SetVerticalSubsampling( aafUInt32 VerticalSubsampling );
	void SetColorRange( aafUInt32 ColorRange );

private:
	AxCDCIDescriptor();
	AxCDCIDescriptor( const AxCDCIDescriptor& );
	AxCDCIDescriptor& operator=( const AxCDCIDescriptor& );
	
	IAAFCDCIDescriptorSP _spIaafCDCIDescriptor;
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
							 aafDataBuffer_t value );

	template <typename FormatType>
	void AddFormatSpecifier( const aafUID_t& essenceFormatCode,
							 const FormatType& value )
	{
		AddFormatSpecifier( essenceFormatCode, sizeof(FormatType),
							reinterpret_cast<aafDataBuffer_t>(
								const_cast<FormatType*>(&value)) );
	}

	inline operator IAAFEssenceFormatSP ()
	{ return _spIaafEssenceFormat; }

private:
	AxEssenceFormat();
	AxEssenceFormat( const AxEssenceFormat& );
	AxEssenceFormat& operator=( const AxEssenceFormat& );
	
	IAAFEssenceFormatSP _spIaafEssenceFormat;
};

#endif

#ifndef __AxImplNullEssenceCodec_h__
#define __AxImplNullEssenceCodec_h__

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

#include "AxPluginFormatSpecifiers.h"

#include "AxPluginUtil.h"

#include <AxTypes.h>
#include <AxSmartPointer.h>

#include <AAFTypes.h>
#include <AAFPlugin.h>
#include <AAFEssenceFormats.h>

#include <vector>
#include <string>
#include <wchar.h>

// This implementation class is intended to be wrapped by
// CAxNullEssenceCodec.cpp.  CaXNullEssenceCode will catch
// exception, and report HRESULT's.  The implementation of this
// class throws AxExHResult rather than return an error code.

class AxImplNullEssenceCodec
{
public:

	AxImplNullEssenceCodec();
	~AxImplNullEssenceCodec();

	const aafUID_t& GetDescriptorAUID();
	const aafUID_t& GetDataDefID();
	const aafUID_t& GetCategoryClassAUID();
	const AxString& GetName();
	const AxString& GetDesc();

	//
	// IAAFEssenceCodec methods
	//

	void SetEssenceAccess(
		IAAFEssenceAccess * pEssenceAccess );

	void CountFlavours(
		aafUInt32*  pCount );

	void GetIndexedFlavourID(
		aafUInt32  index,
		aafUID_t *  pVariant );

	void CountDataDefinitions(
		aafUInt32*  pCount );

	void GetIndexedDataDefinition(
		aafUInt32  index,
		aafUID_t *  pDataDefID );

	void GetMaxCodecDisplayNameLength(
		aafUInt32*  pBufSize );

	void GetCodecDisplayName(
		aafUID_constref  flavour,
		aafCharacter *  pName,
		aafUInt32  bufSize );
	
	void CountChannels(
		IAAFSourceMob * fileMob,
		aafUID_constref  essenceKind,
		IAAFEssenceStream * stream,
		aafUInt16 *  pNumChannels );

	void GetSelectInfo(
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafSelectInfo_t *  pSelectInfo );

	void ValidateEssence(
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafCheckVerbose_t  verbose,
		aafCheckWarnings_t  outputWarnings,
		aafUInt32  bufSize,
		aafCharacter *  pErrorText,
		aafUInt32*  pBytesRead);

	void Create(
		IAAFSourceMob * fileMob,
		aafUID_constref  flavour,
		aafUID_constref  essenceKind,
		aafRational_constref  sampleRate,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable);

	void Open(
		IAAFSourceMob * fileMob,
		aafMediaOpenMode_t  openMode,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable);
	
	void CountSamples(
		aafUID_constref  essenceKind,
		aafLength_t *  pNumSamples);

	void WriteSamples(
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesWritten,
		aafUInt32 *  bytesWritten);

	void ReadSamples(
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesRead,
		aafUInt32 *  bytesRead);

	void Seek(
		aafPosition_t  sampleFrame);

	void CompleteWrite(
		IAAFSourceMob * pFileMob);

	void CreateDescriptorFromStream(
		IAAFEssenceStream * pStream,
		IAAFSourceMob * pSourceMob);

	void GetCurrentEssenceStream(
		IAAFEssenceStream ** ppStream);

	void PutEssenceFormat(
		IAAFEssenceFormat * pFormat);
	
	void GetEssenceFormat(
		IAAFEssenceFormat * pFormatTemplate,
		IAAFEssenceFormat ** ppNewFormat);

	void GetDefaultEssenceFormat(
		IAAFEssenceFormat ** ppNewFormat);

	void GetEssenceDescriptorID(
		aafUID_t *  pDescriptorID);

	void GetEssenceDataID(
		aafUID_t *  pEssenceDataID);
	
	void GetIndexedSampleSize(
		aafUID_constref  essenceDefID,
		aafPosition_t  sampleOffset,
		aafLength_t *  pLength);

	void GetLargestSampleSize(
		aafUID_constref  essenceDefID,
		aafLength_t *  pLength);

private:

	// Prohibited
	AxImplNullEssenceCodec( const AxImplNullEssenceCodec& );
	AxImplNullEssenceCodec& operator=( const AxImplNullEssenceCodec& );

	void InitFormatSpecifiers();
	void FiniFormatSpecifiers();

	// Fetch a cdci descriptor from the source mob.
	IAAFCDCIDescriptorSP GetEssenceDescriptor( IAAFSourceMobSP spMob );
	void UpdateEssenceDescriptor( IAAFSourceMobSP spMob );

	// Return size of one frame in bytes.
	// All frames assumed the same size.
	int GetFrameSize();

	// The descriptor object associated with this essence.
	// SetEssenceDescriptor sets this pointer.
	//IAAFCDCIDescriptorSP _spCDCIDescriptor;
	
	// Sample count
	int _numSamples;

	// Format specifiers - the cdci specifiers are used in this example.
	// These are initialized by InitFormatSpecifiers().  Each specifier
	// is assigned a UID.  Some are considered read-only. This is enforced
	// by the codec implementation, not the specifier
	// (i.e AxPluginFormatSpecifier).  Although the specifer will store a
	// read only attribute in order to make it easier for the codec to
	// enforce read only sematics.
	AxPluginFormatSpecifier< aafUID_t >		          _compression;
	AxPluginFormatSpecifier< aafColorSpace_t >        _colorSpace;
	AxPluginFormatSpecifier< aafFrameLayout_t >       _frameLayout;
	AxPluginFormatSpecifier< aafFieldDom_t >          _fieldDominance;
	AxPluginFormatSpecifier< aafRect_t>               _storedRect;
	AxPluginFormatSpecifier< aafRect_t>               _sampledRect;
	AxPluginFormatSpecifier< aafRect_t>               _displayRect;
	AxPluginFormatSpecifier< aafRational_t >          _aspectRatio;
	AxPluginFormatSpecifier< aafAlphaTransparency_t > _alphaTransparency;
	AxPluginFormatSpecifier< aafUID_t >               _gamma;
	AxPluginFormatSpecifier< aafUInt32  >             _imageAlignmentFactor;
	AxPluginFormatSpecifier< aafVideoLineMap_t >      _videoLineMap;
	AxPluginFormatSpecifier< aafInt32 >               _componentWidth;
	AxPluginFormatSpecifier< aafUInt32 >              _horizSubsampling;
	// AxPluginFormatSpecifier< aafUInt32 >              _vertSubsampling;
	AxPluginFormatSpecifier< aafColorSiting_t >       _colorSiting;
	AxPluginFormatSpecifier< aafUInt32 >              _blackLevel;	
	AxPluginFormatSpecifier< aafUInt32 >              _whiteLevel;
	AxPluginFormatSpecifier< aafUInt32 >              _colorRange;
	AxPluginFormatSpecifier< aafUInt16 >              _padBits;    
	AxPluginFormatSpecifier< aafUInt16 >              _padBytesPerRow;
	AxPluginFormatSpecifier< aafUInt32 >              _fieldStartOffset;
	AxPluginFormatSpecifier< aafUInt32 >              _fieldEndOffset;	
	AxPluginFormatSpecifier< aafRational_t >          _sampleRate;    

	// Read only format specifiers.
	AxPluginFormatSpecifier< aafInt32 >               _numChannels;
	AxPluginFormatSpecifier< aafInt16 >               _pixelSize;
	AxPluginFormatSpecifier< aafUInt32 >              _maxSampleBytes;
	AxPluginFormatSpecifier< aafBoolean_t >			  _willTransferLines;
	AxPluginFormatSpecifier< aafBoolean_t >           _isCompressed;

	// This map stores pointers to the specifiers declared above.  It is initialized
	// in the constructor and used to implement PutEssenceFormat(), GetEssenceFormat(),
	// GetDefaultEssenceFormat().  It is possible to implement these methods generically
	// (for the most part) if the specifiers are stored in such a map.
	AxPluginSpecifierMap _specifierMap;
	int _maxValBufSize;

	// CDCICodec also has a "legacy" property.  Setting it 
	// causes a number of other of the codecs private data members
	// to be set.   In CAAFCDCICodec.cpp it does't have a type, or
	// stored a value.  Here, aafBoolean is used.  If the property is
	// set the value is changed to true.  In CAAFCDCICodec.cpp there
	// is no way to unset the sepecifier.
	// This specifier is somewhat weird.  It is not placed in the _specifierMap
	// and is not subject to any sort of automated processing.  It handled in 
	// an ad-hoc manner.
	AxPluginFormatSpecifier< aafBoolean_t >           _legacyFlag;
	

	// This is the id of the essence descriptor, i.e. AUID_AAFCDCIDescriptor.
	// An instance of the object identified by this id is created by the SKD
	// and attached to the SourceMob that represents the essence.
	// This is returned by GetPluginDescriptorID().  The same descriptor ID
	// is used to initalize one (or more) of the IAAFDefObject's created an returned by
	// IAAFCodecDef::GetIndexedDefinitionObject().
	const aafUID_t _descriptorAUID;

	const aafUID_t _dataDefID;
	const AxString _codecName;
	const AxString _codecDesc;
	const aafUID_t _categoryClassAUID;

	aafUID_t _essenceDataAUID;
	aafUID_t _essenceDataDefID;

	// Flavours are indexed, so store in a vector.
	// Should these be part of the definition parameters?  Do the they map to
	// each other?
	std::vector< aafUID_t > _flavours;

	// Map flavour id's to display names.  This maps uids to strings.
	// The map must contain the same set of uids as the _flavours vectr.
	AxPluginNameMap _flavourNames;

	// Some _displayNames related types to make life a bit easier.
	typedef AxCharTraits::char_type DisplayNameCharType;
	typedef std::map< aafUID_t, AxString >::iterator DisplayNameIterType;


	// The pointer should *not* be referenced counted.  This pointer is set by
	// SetEssenceAccess() and is the the container (owner) for this IAAFEssenceCodec.
	// Referencing counting this interface would cause a circular reference count.
	IAAFEssenceAccess* _pAccess;


	// The essence stream is reference counted.
	IAAFEssenceStreamSP _spStream;
};


#endif

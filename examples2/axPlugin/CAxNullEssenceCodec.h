#ifndef __CAxUnknownCodec_h__
#define __CAxUnknownCodec_h__

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


#include "CAxUnknown.h"

#include <AAFPlugin.h>

// CAxNullEssenceCodec - sample implementation of an AAF Plugin.
// This example implements IAAFPlugin (as must all AAF plugins),
// and IAAFEssenceCodec.
// The IUknown implementation is provided by CAxUnknown.

// The implementation of this of CAxNullEssenceCodec is complete to
// the extent necessary to exectute the axPlugInfo program.

class CAxNullEssenceCodec
: public IAAFEssenceCodec,
  public IAAFPlugin,
  public CAxUnknown
{
public:

	CAXUNKNOWN_DECLARE_IUNKNOWN_METHODS
	
	// Override CAxUnknown::NondelegatingQueryInterface() in order to added
	// support for the interfaces supported by this class.
	STDMETHOD( NondelegatingQueryInterface(const IID& iid, void** ppv) );

	//
	// IAAFPlugin Interface
	//

	STDMETHOD(CountDefinitions)(
		aafUInt32 *pDefCount );

	STDMETHOD(GetIndexedDefinitionID)(
		aafUInt32 index, aafUID_t *result );

	STDMETHOD(GetPluginDescriptorID)(
		aafUID_t *result );

    STDMETHOD(GetIndexedDefinitionObject)( 
		aafUInt32 index,
        IAAFDictionary *dict,
        IAAFDefObject **def );
 
	STDMETHOD(CreateDescriptor)(
		IAAFDictionary *dict,
		IAAFPluginDef **desc );
		
	//
	// IAAFEssenceCodec interface methods
	//

	STDMETHOD(SetEssenceAccess)(
		IAAFEssenceAccess * pEssenceAccess );

	STDMETHOD(CountFlavours)(
		aafUInt32*  pCount );

	STDMETHOD(GetIndexedFlavourID)(
		aafUInt32  index,
		aafUID_t *  pVariant );
 
	STDMETHOD(CountDataDefinitions)(
		aafUInt32*  pCount );

	STDMETHOD(GetIndexedDataDefinition) (
		aafUInt32  index,
		aafUID_t *  pDataDefID );

	STDMETHOD(GetMaxCodecDisplayNameLength) (
		aafUInt32*  pBufSize );

	STDMETHOD(GetCodecDisplayName) (
		aafUID_constref  flavour,
		aafCharacter *  pName,
		aafUInt32  bufSize );
	
	STDMETHOD(CountChannels) (
		IAAFSourceMob * fileMob,
		aafUID_constref  essenceKind,
		IAAFEssenceStream * stream,
		aafUInt16 *  pNumChannels );

	STDMETHOD(GetSelectInfo) (
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafSelectInfo_t *  pSelectInfo );

	STDMETHOD(ValidateEssence) (
		IAAFSourceMob * fileMob,
		IAAFEssenceStream * stream,
		aafCheckVerbose_t  verbose,
		aafCheckWarnings_t  outputWarnings,
		aafUInt32  bufSize,
		aafCharacter *  pErrorText,
		aafUInt32*  pBytesRead);

	STDMETHOD(Create) (
		IAAFSourceMob * fileMob,
		aafUID_constref  flavour,
		aafUID_constref  essenceKind,
		aafRational_constref  sampleRate,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable);

	STDMETHOD(Open) (
		IAAFSourceMob * fileMob,
		aafMediaOpenMode_t  openMode,
		IAAFEssenceStream * stream,
		aafCompressEnable_t  compEnable);
	
	STDMETHOD(CountSamples) (
		aafUID_constref  essenceKind,
		aafLength_t *  pNumSamples);

	STDMETHOD(WriteSamples) (
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesWritten,
		aafUInt32 *  bytesWritten);

	STDMETHOD(ReadSamples) (
		aafUInt32  nSamples,
		aafUInt32  buflen,
		aafDataBuffer_t  buffer,
		aafUInt32 *  samplesRead,
		aafUInt32 *  bytesRead);

	STDMETHOD(Seek) (
		aafPosition_t  sampleFrame);


	STDMETHOD(CompleteWrite) (
		IAAFSourceMob * pFileMob);

	STDMETHOD(CreateDescriptorFromStream) (
		IAAFEssenceStream * pStream,
		IAAFSourceMob * pSourceMob);

	STDMETHOD(GetCurrentEssenceStream) (
		IAAFEssenceStream ** ppStream);

	STDMETHOD(PutEssenceFormat) (
		IAAFEssenceFormat * pFormat);
	
	STDMETHOD(GetEssenceFormat) (
		IAAFEssenceFormat * pFormatTemplate,
		IAAFEssenceFormat ** ppNewFormat);

	STDMETHOD(GetDefaultEssenceFormat) (
		IAAFEssenceFormat ** ppNewFormat);

	STDMETHOD(GetEssenceDescriptorID) (
		aafUID_t *  pDescriptorID);

	STDMETHOD(GetEssenceDataID) (
		aafUID_t *  pEssenceDataID);
	
	STDMETHOD(GetIndexedSampleSize) (
		aafUID_constref  essenceDefID,
		aafPosition_t  sampleOffset,
		aafLength_t *  pLength);

	STDMETHOD(GetLargestSampleSize) (
		aafUID_constref  essenceDefID,
		aafLength_t *  pLength);

	CAxNullEssenceCodec( IUnknown * pUnkOuter );

	~CAxNullEssenceCodec();

private:

	aafUInt32 _numSupportedDefinitions;
	aafUID_t  _codecDefAUID;
	aafUInt32  _numFlavours;

	const wchar_t* _codecName;
	const wchar_t* _codecDesc;
};


#endif

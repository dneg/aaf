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

#include <axFileGen.h>

#include <AxEssence.h>

#include <AAFEssenceFormats.h>

namespace {

AXFG_OP(
  JPEGFormatSpecifiersSet,           
  L"JPEGFormatSpecifiersSet",
  L"Set minimal format specifiers for the standard AAF JPEG codec.",
  L"CDCIEssenceDescriptorName  EssenceAccessName",
  L"kAAFStoredRect, kAAFCDCIHorizSubsampling, and kAAFColorSpaceYUV are set.",
  3,
  3 ) 

JPEGFormatSpecifiersSet::~JPEGFormatSpecifiersSet()
{}

void JPEGFormatSpecifiersSet::Execute( const std::vector<AxString>& argv )
{
	AxString descName   = argv[1];
	AxString accessName = argv[2];

	IAAFCDCIDescriptorSP spDesc;
	GetInstance( descName ).GetCOM( spDesc );
	AxCDCIDescriptor axDesc( spDesc );

	IAAFEssenceAccessSP spAccess;
	GetInstance( accessName ).GetCOM( spAccess );
	AxEssenceAccess axAccess( spAccess );

	// And... we'll need one these.
	AxEssenceFormat axEssenceFormat( axAccess.GetEmptyFileFormat() );
	
	aafRect_t rect = { 0,0,0,0 };
	aafUInt32 w;
	aafUInt32 h;

	axDesc.GetStoredView( w, h );
	rect.xSize = w;
	rect.ySize = h;

	axEssenceFormat.AddFormatSpecifier( kAAFStoredRect, rect );

	axEssenceFormat.AddFormatSpecifier( kAAFCDCIHorizSubsampling,
										axDesc.GetHorizontalSubsampling() );

	// So.. how does one figure this out from the essence descriptor?
	// As far as I can tell you can't... hmmm... never noticed that before.
	// Subsampling of 2 would be a good bet... but still not stricly an
	// accurate assumption.
	aafColorSpace_t colorSpace = kAAFColorSpaceYUV;
	axEssenceFormat.AddFormatSpecifier( kAAFPixelFormat, colorSpace );

	axAccess.PutFileFormat( axEssenceFormat );
}

//=---------------------------------------------------------------------=


} // end of namespace



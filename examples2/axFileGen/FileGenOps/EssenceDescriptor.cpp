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

#include <AxMob.h>
#include <AxEssence.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  GetEssenceDescriptor,           
  L"GetEssenceDescriptor",
  L"Get and essence descriptor (any type) from a source mob.",
  L"DescriptorName SourceMobName",
  L"",
  3,
  3 ) 

GetEssenceDescriptor::~GetEssenceDescriptor()
{}

void GetEssenceDescriptor::Execute( const std::vector<AxString>& argv )
{
	AxString descName = argv[1];
	AxString mobName  = argv[2];

	IAAFSourceMobSP spMob;
	GetInstance( mobName ).GetCOM( spMob );
	AxSourceMob axMob( spMob );

	SetCOM( axMob.GetEssenceDescriptor() );
	RegisterInstance( descName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CDCIDescSetMin,           
  L"CDCIDescSetMin",
  L"Set minimal CDCI essence descriptor parameters",
  L"DescriptorName image_width image_height horz_sub_sampling vert_sub_sampling",
  L"",
  6,
  6 ) 

CDCIDescSetMin::~CDCIDescSetMin()
{}

void CDCIDescSetMin::Execute( const std::vector<AxString>& argv )
{
	AxString descName = argv[1];
	AxString width    = argv[2];
	AxString height   = argv[3];
	AxString sampHorz = argv[4];
	AxString sampVert = argv[5];

	IAAFCDCIDescriptorSP spCDCIDesc;
	GetInstance( descName ).GetCOM( spCDCIDesc );
	AxCDCIDescriptor axCDCIDesc( spCDCIDesc );


	aafRect_t rect;
	rect.xOffset = 0;
	rect.yOffset = 0;
	rect.xSize   = AxStringUtil::strtol( width );
	rect.ySize   = AxStringUtil::strtol( height );

	axCDCIDesc.SetStoredView (                             rect.xSize, rect.ySize );
	axCDCIDesc.SetSampledView( rect.xOffset, rect.yOffset, rect.xSize, rect.ySize );
	axCDCIDesc.SetDisplayView( rect.xOffset, rect.yOffset, rect.xSize, rect.ySize );

	axCDCIDesc.SetHorizontalSubsampling( AxStringUtil::strtol( sampHorz ) );
	axCDCIDesc.SetVerticalSubsampling( AxStringUtil::strtol( sampVert ) );
}

//=---------------------------------------------------------------------=



} // end of namespace



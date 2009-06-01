//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

#include <axFileGen.h>

#include "ParamMaps.h"

#include <AxDictionary.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  DataDef,           
  L"DataDef",
  L"Create a DataDef that can be reference by other operations.",
  L"file_ref name {Picture | Sound | Timecode | PictureWithMatte | Edgecode | Unknown }",
  L"",
  4,
  4 ) 

DataDef::~DataDef()
{}

void DataDef::Execute( const std::vector<AxString>& argv )
{
	AxString fileOpName = argv[1];
	AxString defOpName  = argv[2];
	AxString type       = argv[3];

	AxDictionary axDictionary( DictionaryFromFileOp( fileOpName ) );

	IAAFDataDefSP spDataDef = 
		axDictionary.LookupDataDef( DDefParams::GetInstance().Find( *this, type ) );

	SetCOM( spDataDef );
	RegisterInstance( defOpName );
}

} // end of namespace



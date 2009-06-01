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

#include <AxDictionary.h>
#include <AxComponent.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  OperationGroup,           
  L"OperationGroup",
  L"Create and operation group.",
  L"FileName OpGroupName DataDefName OperationDefName length",
  L"",
  6,
  6 ) 

OperationGroup::~OperationGroup()
{}

void OperationGroup::Execute( const std::vector<AxString>& argv )
{
	AxString fileName    = argv[1];
	AxString opGroupName = argv[2];
	AxString dataDefName = argv[3];
	AxString opDefName   = argv[4];
	AxString length      = argv[5];
	
	IAAFOperationGroupSP spOpGroup;
	AxCreateInstance( DictionaryFromFileOp( fileName ), spOpGroup );
	AxOperationGroup axOpGroup( spOpGroup );

	IAAFDataDefSP spDataDef;
	GetInstance( dataDefName ).GetCOM( spDataDef );

	IAAFOperationDefSP spOpDef;
	GetInstance( opDefName ).GetCOM( spOpDef );

	axOpGroup.Initialize( spDataDef,
		  		          AxStringUtil::strtol( length ),
						  spOpDef );

	SetCOM( spOpGroup );
	RegisterInstance( opGroupName );
}

} // end of namespace



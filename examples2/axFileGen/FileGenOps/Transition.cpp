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
  Transition,           
  L"Transition",
  L"Create a transition.",
  L"FileName TransitionName OpGroupName DataDefName position length",
  L"",
  7,
  7 ) 

Transition::~Transition()
{}

void Transition::Execute( const std::vector<AxString>& argv )
{
	AxString fileName    = argv[1];
	AxString transName   = argv[2];
	AxString opGroupName = argv[3];
	AxString dataDefName = argv[4];
	AxString position    = argv[5];
	AxString length      = argv[6];


	IAAFTransitionSP spTrans;
	AxCreateInstance( DictionaryFromFileOp( fileName ), spTrans );
	AxTransition axTrans( spTrans );

	IAAFOperationGroupSP spOpGroup;
	GetInstance( opGroupName ).GetCOM( spOpGroup );
	
	IAAFDataDefSP spDataDef;
	GetInstance( dataDefName ).GetCOM( spDataDef );

	axTrans.Initialize( spDataDef,
			  		    AxStringUtil::strtol( length ),
						AxStringUtil::strtol( position ),
						spOpGroup );


	SetCOM( spTrans );
	RegisterInstance( transName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  SetTransitionCutPoint,           
  L"SetTransitionCutPoint",
  L"Set a transitions cut point (fall back edit point).",
  L"TransitionName position",
  L"",
  3,
  3 ) 

SetTransitionCutPoint::~SetTransitionCutPoint()
{}

void SetTransitionCutPoint::Execute( const std::vector<AxString>& argv )
{
	AxString transName   = argv[1];
	AxString position    = argv[2];

	IAAFTransitionSP spTrans;
	GetInstance( transName ).GetCOM( spTrans );
	AxTransition axTrans( spTrans );

	axTrans.SetCutPoint( AxStringUtil::strtol( position ) );
}


} // end of namespace



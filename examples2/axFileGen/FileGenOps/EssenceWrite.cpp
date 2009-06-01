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

#include "SampleSource.h"

#include <axFileGen.h>

#include <AxEssence.h>

#include <iostream>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  WriteEssence,         
  L"WriteEssence",
  L"Write arbitrary essence of any type.",
  L"EssenceSourceName EssenceAccessName",
  L"",
  3,
  3 ) 

WriteEssence::~WriteEssence()
{}

void WriteEssence::Execute( const std::vector<AxString>& argv )
{
	AxString essenceSrcName    = argv[1];
	AxString essenceAccessName = argv[2];

	const char *busy[4] = { "-", "\\", "|", "/" };

	IAAFEssenceAccessSP spAccess;
	GetInstance( essenceAccessName ).GetCOM( spAccess );
	AxEssenceAccess axAccess( spAccess );

	SampleSource& src = GetSampleSource( *this, essenceSrcName );

	// FIXME - the busy indicator should not be implemented here...
	// should be in axFileGen.cpp

	std::auto_ptr<SampleSrcBuffer> buf;
	src.Reset();
	int count;
	std::cout << " ";
	for ( buf = src.GetNext(), count = 0;
		  buf->GetBufferPtr();
		  buf = src.GetNext(), count++ ) {

			std::cout << busy[count%4];

			axAccess.WriteSamples( buf->GetNumSamples(),
								   buf->GetNumBytes(),
								   buf->GetBufferPtr() );
		  
			std::cout << "\b";
	}

	axAccess.CompleteWrite();
}

//=---------------------------------------------------------------------=

}

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

#include <iostream>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  InstanceDump,           
  L"InstanceDump",
  L"Dump the instance registry.",
  L"",
  L"",
  1,
  1 ) 

InstanceDump::~InstanceDump()
{}

void InstanceDump::Execute( const std::vector<AxString>& argv )
{
	using namespace std;
	
	std::pair<AxString, const AxFGOp*> op;

	AxFGOpRegistry::GetInstance().ResetIter();

	wcout << endl << endl;
	
	for ( op = AxFGOpRegistry::GetInstance().Next();
		  op.second;
		  op = AxFGOpRegistry::GetInstance().Next() ) {
		wcout << L"      Name:\t"  << op.first << endl;
	    wcout << L"Created By:\t" << op.second->GetOpName() << endl;
	    wcout << L"Ref Count:\t" << op.second->GetRef() << endl;
		wcout << endl;
	}

	wcout << endl;

}

} // end of namespace



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

#include <AxDictionary.h>
#include <AxComponent.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  Sequence,           
  L"Sequence",
  L"Create a sequence object.",
  L"FileName DataDefName SequenceName",
  L"",
  4,
  4 ) 

Sequence::~Sequence()
{}

void Sequence::Execute( const std::vector<AxString>& argv )
{
	AxString fileName    = argv[1];
	AxString dataDefName = argv[2];
	AxString seqName     = argv[3];

	AxDictionary axDict( DictionaryFromFileOp( fileName ) );

	IAAFSequenceSP spSeq;
	AxCreateInstance( axDict, spSeq );
	AxSequence axSeq( spSeq );

	IAAFDataDefSP spDataDef;
	GetInstance( dataDefName).GetCOM( spDataDef );
	
	axSeq.Initialize( spDataDef );

	SetCOM( spSeq );
	RegisterInstance( seqName );
}


//=---------------------------------------------------------------------=

AXFG_OP(
  AppendComponent,           
  L"AppendComponent",
  L"Append a component to a sequence.",
  L"SequenceName ComponentName",
  L"",
  3,
  3 ) 

AppendComponent::~AppendComponent()
{}

void AppendComponent::Execute( const std::vector<AxString>& argv )
{
	AxString seqName  = argv[1];
	AxString compName = argv[2];

	IAAFSequenceSP spSeq;
	GetInstance( seqName ).GetCOM( spSeq );
	AxSequence axSeq( spSeq );

	IAAFComponentSP spComp;
	GetInstance( compName ).GetCOM( spComp );

	axSeq.AppendComponent( spComp );
}

//=---------------------------------------------------------------------=

} // end of namespace



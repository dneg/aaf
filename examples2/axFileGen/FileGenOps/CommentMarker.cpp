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
  CommentMarker,           
  L"CommentMarker",
  L"Create a comment marker.",
  L"GF CommentName DataDefName comment_string",
  L"",
  5,
  5 ) 

CommentMarker::~CommentMarker()
{}

void CommentMarker::Execute( const std::vector<AxString>& argv )
{
	AxString fileName          = argv[1];
	AxString commentMarkerName = argv[2];
	AxString dataDefName       = argv[3];
	AxString comment           = argv[4];

	IAAFCommentMarkerSP spCommentMarker;
	AxCreateInstance(  DictionaryFromFileOp( fileName ), spCommentMarker );
	AxCommentMarker axCommentMarker( spCommentMarker );

	IAAFDataDefSP spDataDef;
	GetInstance( dataDefName ).GetCOM( spDataDef );

	axCommentMarker.SetDataDef( spDataDef );
	axCommentMarker.SetComment( comment );

	SetCOM( spCommentMarker );
	RegisterInstance( commentMarkerName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CommentMarkerSetRegion,           
  L"CommentMarkerSetRegion",
  L"Set the position and length of a comment marker",
  L"CommentMarkerName position length",
  L"",
  4,
  4 ) 

CommentMarkerSetRegion::~CommentMarkerSetRegion()
{}

void CommentMarkerSetRegion::Execute( const std::vector<AxString>& argv )
{
	AxString commentMarkerName = argv[1];
	AxString position          = argv[2];
	AxString length            = argv[3];

	IAAFCommentMarkerSP spCommentMarker;
	GetInstance( commentMarkerName ).GetCOM( spCommentMarker );
	AxCommentMarker axCommentMarker( spCommentMarker );

	axCommentMarker.SetPosition( AxStringUtil::strtol( position ) );
	axCommentMarker.SetLength( AxStringUtil::strtol( length ) );
}


} // end of namespace



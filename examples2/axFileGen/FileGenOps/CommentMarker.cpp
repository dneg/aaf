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
  L"GF CommentName SourceRefName comment_string",
  L"The comment marker will have the same data def as the soruce reference.",
  5,
  5 ) 

CommentMarker::~CommentMarker()
{}

void CommentMarker::Execute( const std::vector<AxString>& argv )
{
	AxString fileName          = argv[1];
	AxString commentMarkerName = argv[2];
	AxString srcRefName        = argv[3];
	AxString comment           = argv[4];

	IAAFCommentMarkerSP spCommentMarker;
	AxCreateInstance(  DictionaryFromFileOp( fileName ), spCommentMarker );
	AxCommentMarker axCommentMarker( spCommentMarker );

	IAAFSourceReferenceSP spSrcRef;
	GetInstance( srcRefName ).GetCOM( spSrcRef );
	AxSourceReference axSrcRef( spSrcRef );

	axCommentMarker.SetDataDef( axSrcRef.GetDataDef() );

	axCommentMarker.SetAnnotation( spSrcRef );
	axCommentMarker.SetComment( comment );

	SetCOM( spCommentMarker );
	RegisterInstance( commentMarkerName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  CommentMarkerSetPos,           
  L"CommentMarkerSetPos",
  L"Set the position of a comment marker",
  L"CommentMarkerName position",
  L"",
  3,
  3 ) 

CommentMarkerSetPos::~CommentMarkerSetPos()
{}

void CommentMarkerSetPos::Execute( const std::vector<AxString>& argv )
{
	AxString commentMarkerName = argv[1];
	AxString position          = argv[2];

	IAAFCommentMarkerSP spCommentMarker;
	GetInstance( commentMarkerName ).GetCOM( spCommentMarker );
	AxCommentMarker axCommentMarker( spCommentMarker );

	axCommentMarker.SetPosition( AxStringUtil::strtol( position ) );
}


} // end of namespace



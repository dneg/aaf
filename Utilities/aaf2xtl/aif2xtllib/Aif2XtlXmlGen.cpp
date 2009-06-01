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
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Siemens SBS Media
// All rights reserved.
//
//=---------------------------------------------------------------------=
 
#include <Aif2XtlXmlGen.h>

#include <AifSanteFeLoad.h>
#include <AifParseTree.h>
#include <Aif2XtlParseTreeNodeFactory.h>
#include <Aif2XtlInit.h>

#include <Aif2XtlParseTreeVisitor.h>
#include <Aif2XtlSourceClipVisitor.h>
#include <Aif2XtlMobSelectVisitor.h>
#include <Aif2XtlGroupVisitor.h>
#include <Aif2XtlTransitionVisitor.h>
#include <Aif2XtlSeqEditVisitor.h>
#include <Aif2XtlSeqOffsetVisitor.h>
#include <Aif2XtlXmlGenVisitor.h>
#include <Aif2XtlObjectRemoveVisitor.h>
#include <Aif2XtlEssencePreprocessVisitor.h>

#include <AAFResult.h>

#include <AxUtil.h>

#include <iostream>

HRESULT __stdcall Aif2XtlXmlGen::CreateInstance( const wchar_t* pInFileName,
												 const char* pAafComLib,
						                         Aif2XtlXmlGen** ppInstance )
{
  try {
    if ( !pInFileName || !ppInstance ) {
      return AAFRESULT_NULL_PARAM;
    }

    *ppInstance = new Aif2XtlXmlGen( pInFileName, pAafComLib );
    if ( !*ppInstance ) {
      return AAFRESULT_NOMEMORY;
    }

  }
  catch(...)
  {
    return AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return AAFRESULT_SUCCESS;
}

HRESULT __stdcall Aif2XtlXmlGen::CreateInstance( const char* pInFileName,
												 const char* pAafComLib,
										         Aif2XtlXmlGen** ppInstance )
{
  try {
    AxString wideFileName = AxStringUtil::mbtowc( pInFileName );
    return CreateInstance( wideFileName.c_str(), pAafComLib, ppInstance );
  }
  catch ( ... ) {
  }
  
  return AAFRESULT_UNEXPECTED_EXCEPTION;
}

// Fixme - the default extract directory should not be hardcode to
// "c:".

Aif2XtlXmlGen::Aif2XtlXmlGen( const wchar_t* pInFileName,
 							  const char* pAafComLib )
  : _inFileName( pInFileName ),
    _extDir( "C:/tmp/aaf2xtl_essence" ),
    _cutsOnly( false ),
    _extEssence( false ),
    _stripEssence( false ),
	_dllName( pAafComLib )
{}

Aif2XtlXmlGen::~Aif2XtlXmlGen()
{
  // FIXME - The need to do this sort of defeats the purpose of
  // auto_ptrs.  Re-examine.

  // Take care with the destruction order.  Make sure the loader is
  // last because it will unload the AAF library.

  {
    std::auto_ptr<AifParseTree> null;
    _apParseTree = null;
  }

  {
    std::auto_ptr<AifSanteFeLoader> null;
    _apLoader = null;
  }
}

void Aif2XtlXmlGen::SetCutsOnly( bool cutsOnly )
{
  _cutsOnly = cutsOnly;
}

HRESULT Aif2XtlXmlGen::Execute( std::ostream& outStream, std::wostream& logStream )
{
  using namespace std;

  try {

	// FIXME - Look up the com library in the registry, or relative
	// to ARGV0
    _apLoader = std::auto_ptr<AifSanteFeLoader>(
		new AifSanteFeLoader( _inFileName.c_str(), _dllName.c_str(), true ) );

    Aif2XtlInit();

    Aif2XtlParseTreeNodeFactory nodeFactory;

    _apParseTree = _apLoader->ParseFile( nodeFactory );

    // Resolve source clip's and mob chains.
    Aif2XtlSourceClipVisitor srcClipVisitor( nodeFactory, logStream );
    _apParseTree->Traverse( &srcClipVisitor );
		
    // Select source mobs in mob chain.
    Aif2XtlSelectFirstFileMob selectFunc;
    Aif2XtlMobSelectVisitor mobSelectVisitor( selectFunc, logStream );
    _apParseTree->Traverse( &mobSelectVisitor );

    // Mark objects that must be removed (e.g. TaggedValue objects)
    // then do a second traveral to remove them.
    Aif2XtlObjectMarkForRemovalVisitor markForRemovalVisitor( logStream );
    _apParseTree->Traverse( &markForRemovalVisitor );
    Aif2XtlObjectRemoveMarkedVisitor removedMarkedObjectsVisitor( logStream );
    _apParseTree->Traverse( &removedMarkedObjectsVisitor );
		
    // Insert group nodes
    Aif2XtlGroupVisitor groupVisitor( logStream );
    _apParseTree->Traverse( &groupVisitor );

    // Transition visitor will identify what transitions
    // are supported and add a decoration to the transition
    // nodes to record the information.
    Aif2XtlTransitionVisitor transitionVisitor( _cutsOnly, logStream );
    _apParseTree->Traverse( &transitionVisitor );

    // Sequence edit visitor will traverse the children of a 
    // sequence node (AAF Segment and Transition objects) and 
    // compute the edit points.
    Aif2XtlSeqEditVisitor sequenceEditVisitor( logStream );
    _apParseTree->Traverse( &sequenceEditVisitor );
		
    // Sequence offset visitor determines sub composition offsets,
	// and applies that offset to nodes with sequence edit info
	// decorations.
    Aif2XtlSeqOffsetVisitor sequenceOffsetVisitor( logStream );
    _apParseTree->Traverse( &sequenceOffsetVisitor );

	// Essence Process traversal.
    // This generates a reference to the essence data suitable for use
    // in the xtl file.  The options to extract, and/or strip the
    // essence are legacy and should dissappear when everything is
    // working well.
    Aif2XtlEssenceProcessVisitor essPreprocessVisitor( _apLoader->GetHeader(),
						       _inFileName,
						       AxStringUtil::mbtowc( _extDir.c_str() ),
						       _extEssence,
						       _stripEssence,
						       nodeFactory,
							   logStream );
    _apParseTree->Traverse( &essPreprocessVisitor );

    // Generate final Xtl output.
    Aif2XtlXmlGenVisitor xmlGenVisitor( outStream, logStream );
    _apParseTree->Traverse( &xmlGenVisitor );
  }
  catch ( const AxExHResult& ex ) {
    logStream << ex.what() << endl;
    return ex.getHResult();
  }
  catch ( const AxEx& ex ) {
    logStream << ex.what() << endl;
    return AAFRESULT_UNEXPECTED_EXCEPTION;
  }
  catch ( const exception& ex ) {
    logStream << ex.what() << endl;
    return AAFRESULT_UNEXPECTED_EXCEPTION;
  }
  catch ( ... ) {
    logStream << L"unknown exception" << endl;
    return AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return AAFRESULT_SUCCESS;
}

void Aif2XtlXmlGen::Dump( std::wostream& outStream )
{
    Aif2XtlDumpVisitor dumpVisitor( outStream, outStream );
    _apParseTree->Traverse( &dumpVisitor );
}


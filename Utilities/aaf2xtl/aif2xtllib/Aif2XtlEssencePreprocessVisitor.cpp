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

#include <Aif2XtlEssencePreprocessVisitor.h>

#include <Aif2XtlParseTreeNodeDecoration.h>
#include <Aif2XtlParseTreeNodeFactory.h>

#include <AifParseTreeBuilder.h>

#include <AxMob.h>
#include <AxEssence.h>
#include <AxDefObject.h>
#include <AxMetaDef.h>
#include <AxUtil.h>
#include <AxHeader.h>
#include <AxMobSlot.h>
#include <AxDictionary.h>
#include <AxSmartPointer.h>
#include <AxComponent.h>
#include <AxDictionary.h>

#include <AAFContainerDefs.h>
#include <AAFDataDefs.h>
#include <AAFEssenceFormats.h>

#include <iostream>

#include <stdio.h>
#include <assert.h>

// Windows headers to get _wmkdir
#include <direct.h>
#include <wchar.h>


namespace {


//=================

// FIXME = This copied from SourceClipVisitory.cpp.  Factor out.
// FIXME - It is also poorly named.  It doesn't fetch the chained
// source clip, simply the first source clip it encounters.

template <class NodeType, class AAFType>
class FetchChild : public Aif2XtlParseTreeVisitor {
public:
	FetchChild( std::wostream& logStream )
		: Aif2XtlParseTreeVisitor( logStream ),
		  _found( false ),
		  _pFoundNode(0)
	{}

	virtual ~FetchChild()
	{}

	virtual void PreOrderVisit( NodeType& node )
	{
		if ( _found ) {
			return;
		}
		else {
			_found = true;
			_pFoundNode = &node;
		}
	}

	bool GetFound()
	{ return _found; }

	NodeType* GetFoundNode()
	{ return _pFoundNode; }

	IAAFSmartPointer<AAFType> GetFoundSrcClip()
	{ return _pFoundNode->GetAif2XtlAAFObject(); }

private:

	bool _found;
	NodeType* _pFoundNode;
};

bool GetChainedSourceClip( AifParseTreeNode& subTreeRoot,
						  std::wostream& logStream,
						  Aif2XtlSourceClipNode** ppClipNodeRef )
{
	FetchChild<Aif2XtlSourceClipNode, IAAFSourceClip> fetchSrcClipVisitor( logStream );

	subTreeRoot.Visit( &fetchSrcClipVisitor );

	bool found = fetchSrcClipVisitor.GetFound();

	if ( found ) {
		*ppClipNodeRef = fetchSrcClipVisitor.GetFoundNode();
	}

	return found;
}

//=================


template <class Type>
bool isA(  Aif2XtlSourceMob& node, IAAFSmartPointer<Type> sp )
{
	AxSourceMob axSourceMob( node.GetAif2XtlAAFObject() );

	AxEssenceDescriptor axEssDesc( axSourceMob.GetEssenceDescriptor() );

	if ( AxIsA( axEssDesc, sp ) ) {
		return true;
	}
	else {
		return false;
	}
}

bool isAudioNode( Aif2XtlSourceMob& node )
{
	IAAFWAVEDescriptorSP spWav;
	IAAFAIFCDescriptorSP spAif;

	if ( isA(node, spWav) || isA(node, spAif) ) {
		return true;
	}
	else {
		return false;
	}
}

bool isVideoNode( Aif2XtlSourceMob& node )
{
	IAAFCDCIDescriptorSP spCDCI;
	if ( isA(node, spCDCI) ) {
		return true;
	}
	else {
		return false;
	}
}

bool isEssenceInternal( Aif2XtlSourceMob& node )
{
	AxSourceMob axSourceMob( node.GetAif2XtlAAFObject() );

	IAAFFileDescriptorSP spFileDesc;
	AxQueryInterface( axSourceMob.GetEssenceDescriptor(), spFileDesc );

	
	IAAFContainerDefSP spContDef;

	// Fixme - Create a wrapper for this.
	CHECK_HRESULT( spFileDesc->GetContainerFormat( &spContDef ) );
	
	IAAFDefObjectSP spDefObject;
	AxQueryInterface( spContDef, spDefObject );
	AxDefObject axDefObject( spDefObject );
	
	if ( ContainerAAF == axDefObject.GetAUID() ) {
		return true;
	}

	assert( ContainerFile == axDefObject.GetAUID() );

	return false;
}


//========================================================================

class EssenceProcessor {
public:
	EssenceProcessor( Aif2XtlSourceMob& node,
			  IAAFHeaderSP spHeader,
			  const AxString& aafFilename,
			  const AxString& directory,
			  bool writeFile,
			  Aif2XtlParseTreeNodeFactory& nodeFactory,
			  std::wostream& logStream );
	~EssenceProcessor();

	void ProcessEssence();

	void AddLocatorAndRemoveEssence();

private:

	void AddLocatorAndRemoveEssence( IAAFSourceMobSP spSrcMob );

	void WriteEssenceToFile( IAAFEssenceDataSP spEssenceData, const AxString& fileName );

	AxString _directory;
        AxString _aafFilename;
	AxString _fileRefName;
	Aif2XtlSourceMob& _node;
	IAAFHeaderSP _spHeader;
	bool _writeFile;
	Aif2XtlParseTreeNodeFactory& _nodeFactory;
	std::wostream& _logStream;
};

EssenceProcessor::EssenceProcessor( Aif2XtlSourceMob& node,
				    IAAFHeaderSP spHeader,
				    const AxString& aafFilename,
				    const AxString& directory,
				    bool writeFile,
				    Aif2XtlParseTreeNodeFactory& nodeFactory,
					std::wostream& logStream )
	: _directory(directory),
	  _node( node ),
	  _spHeader( spHeader ),
	  _writeFile( writeFile ),
	  _aafFilename( aafFilename ),
	  _fileRefName( L"" ),
	  _nodeFactory( nodeFactory ),
	  _logStream( logStream )
{}

EssenceProcessor::~EssenceProcessor()
{}

void EssenceProcessor::ProcessEssence()
{
  // If the essence is embedded, then generated a source mobid
  // reference that is understood by the Direct Show AAF Essence
  // Access Source Filter.
  //
  // Else if the essence is external than simply pull out the
  // filename.

  AxString fileRef( L"" );
  AxSourceMob axSourceMob( _node.GetAif2XtlAAFObject() );

  if ( isEssenceInternal( _node ) ) {
    fileRef = _aafFilename + L"/" + AxStringUtil::mobid2Str( axSourceMob.GetMobID() ) + L".aafmobid";
  }
  else {

    // Fetch the file name from the first locator that is a child of
    // this source mob.

    FetchChild<Aif2XtlNetworkLocator, IAAFNetworkLocator> fetchLocator( _logStream );

    _node.Visit( &fetchLocator );

    bool found = fetchLocator.GetFound();

    if ( found ) {
      Aif2XtlNetworkLocator* pLocNode = fetchLocator.GetFoundNode();
      AxNetworkLocator axNetLoc( fetchLocator.GetFoundNode()->GetAif2XtlAAFObject() );
      fileRef = axNetLoc.GetPath();

      // FIXME - Must strip "file:" and translate characters if they are encoded as hex.
    }
  }  

  std::auto_ptr<Aif2XtlEssenceInfo> pDecoration(
      new Aif2XtlEssenceInfo( fileRef ) );
  
  _node.PushDecoration( pDecoration );
}

#if 0
// This is legacy stuff from IBC 2003.
// It is redundant now that a source filter exists that can
// read essence data embedded in a file.
// It may be useful, so let it linger for a while.
void EssenceProcessor::ExtractEssence()
{
	// The EssenceData object has the same id as the source mob.
	// Get the souce mob id, lookup the essence data, and write 
	// out to an external file.

	AxSourceMob axSourceMob( _node.GetAif2XtlAAFObject() );

	aafMobID_t mobid = axSourceMob.GetMobID();

	AxHeader axHeader( _spHeader );

	IAAFEssenceDataSP spEssenceData = axHeader.LookupEssenceData( mobid );

	AxString suffix = L"unknown";
	AxString wmCompatSuffix = L"unknown";
	IAAFWAVEDescriptorSP spWav;
	IAAFAIFCDescriptorSP spAif;
	IAAFCDCIDescriptorSP spCdci;

	if ( isA(_node, spWav) ) {
		wmCompatSuffix = suffix = L".wav";
	}
	else if ( isA(_node, spAif) ) {
		wmCompatSuffix = suffix = L".aif";
	}
	else if ( isA(_node, spCdci) ) {
		// FIXME - Here lies vapour.
		suffix = L".dv";
		wmCompatSuffix = L".avi";
	}
	else {
		assert(0);
	}

	AxString fileName = _directory + L"/" +
			 			 AxStringUtil::mobid2Str( axSourceMob.GetMobID() );
	
	if ( _writeFile ) {
		WriteEssenceToFile( spEssenceData, fileName + suffix );
	}

	_fileRefName = fileName + wmCompatSuffix; 
	std::auto_ptr<Aif2XtlExtractedEssenceInfo> pDecoration(
		new Aif2XtlExtractedEssenceInfo( _fileRefName ) );

	_node.PushDecoration( pDecoration );
}
#endif

void EssenceProcessor::WriteEssenceToFile( IAAFEssenceDataSP spEssenceData, const AxString& fileName )
{
	// Windows mkdir call - platform specific.
	_wmkdir( _directory.c_str() );

	// Resort to old C stdio routines here.

	FILE* fp = ::fopen( AxStringUtil::wctomb(fileName).c_str(), "wb" );
	assert(fp);

	int bufSize = 50*1024;
	unsigned char* buffer = new unsigned char[bufSize];

	aafLength_t numBytesToWrite = 0;
	CHECK_HRESULT( spEssenceData->GetSize( &numBytesToWrite ) );

	aafLength_t numBytesWrittenSoFar = 0;

	std::wcout << fileName;

	wchar_t* busy[] = {L"|", L"/",  L"-", L"\\" };
	int busy_count = 1;
	
	std::wcout << L"\t" << busy[0];

	while( numBytesWrittenSoFar < numBytesToWrite ) {

		aafLength_t numBytesThisTime = bufSize;

		assert( numBytesWrittenSoFar <= numBytesToWrite );
		aafLength_t numBytesLeftToWrite = numBytesToWrite - numBytesWrittenSoFar;

		if ( numBytesThisTime > numBytesLeftToWrite ) {
			numBytesThisTime = numBytesLeftToWrite;
		}

		// Read the essence data
		aafUInt32 numBytesRead = 0;
	
		// numBytesWrittenSoFar is an unsigned 64 (aafLength_t) since that is what makes sense
		// in the above calcs.  The SetPosition interface requires a signed 64 (aafPosition_t).
		// Assert that numBytesWrittenSoFar is not greater than the position range of aafPosition_t.
		assert( static_cast<aafPosition_t>(numBytesWrittenSoFar) > 0 );
		CHECK_HRESULT( spEssenceData->SetPosition( static_cast<aafPosition_t>(numBytesWrittenSoFar) ) );

		// Cast of numBytesThisTime is safe because it is limited to bufsize.
		CHECK_HRESULT( spEssenceData->Read( static_cast<aafUInt32>(numBytesThisTime), buffer, &numBytesRead ) );

		// Write the essence data
		size_t numElementsWritten;
		numElementsWritten = ::fwrite( buffer, numBytesRead, 1, fp );
		assert( 1 == numElementsWritten );

		numBytesWrittenSoFar += numBytesRead;

		std::wcout << L"\b" << busy[busy_count++%4];
	
	}

	std::wcout << L"\b " << std::endl;

	::fclose(fp);
	delete [] buffer;
}

#if 0
// This was used during dev... let linger for a bit.  It may be useful.
void EssenceProcessor::InsertFileSourceMob( IAAFSourceClipSP spSrcClip,
											IAAFSourceMobSP spSrcMob )
{
	// This reference must be preserved.
	AxSourceClip axSrcClip( spSrcClip );

	// Clone the mob.
	AxSourceMob axSrcMob( spSrcMob );
	AxString name = axSrcMob.GetName();
	
	if ( isAudioNode(_node) ) {
		name += L" (External audio)";
	}
	else if ( isVideoNode( _node) ) {
		name += L" (External video)";
	}
	else {
		name += L" (External)";
	}

	IAAFMobSP spNewMob = axSrcMob.Copy( name );
	AxMob axNewMob( spNewMob );
	AxHeader axHeader( _spHeader );
	axHeader.AddMob( spNewMob );

	// Build tree routed in the new SourceMob.  Traverse that tree
	// to find the source clip.
	std::auto_ptr<AifParseTreeNode> newSrcMobTree =
		AifBuildParseTree( axNewMob, _nodeFactory );

	FetchChild<Aif2XtlSourceClipNode, IAAFSourceClip> fetchSrcClip;
	newSrcMobTree->Visit( &fetchSrcClip );
	AxSourceClip axNewSrcClip( fetchSrcClip.GetFoundSrcClip() );
		
	// spSrcClip must point to new spNewSrcClip, and spNewSrcClip
	// must point to whatever spSrcClip currently points to.
	aafSourceRef_t srcRef = axSrcClip.GetSourceReference();
 
	aafSourceRef_t newSrcRef = srcRef;
	newSrcRef.sourceID = axNewMob.GetMobID();

	axSrcClip.SetSourceReference( newSrcRef );
	axNewSrcClip.SetSourceReference( srcRef );


	// Now, get the descriptor on the new mob and add a
	// NetworkLocator pointing to the extraced essence,
	// and (re)set the container format on the descriptor.
	IAAFSourceMobSP spNewSrcMob;
	AxQueryInterface( spNewMob, spNewSrcMob );
	AxSourceMob axSourceMob( spNewSrcMob );
	IAAFEssenceDescriptorSP spEssDesc = axSourceMob.GetEssenceDescriptor();
	AxEssenceDescriptor axEssDesc( spEssDesc );

	IAAFNetworkLocatorSP spLoc;
	AxCreateInstance( axHeader.GetDictionary(), spLoc );
	AxNetworkLocator axLoc( spLoc );
	axLoc.SetPath( L"file://" + _fileRefName );

	axEssDesc.AppendLocator( axLoc );

	IAAFFileDescriptorSP spFileDesc;
	AxQueryInterface( spEssDesc, spFileDesc );
	AxFileDescriptor axFileDesc( spFileDesc );
	
	AxDictionary axDict( axHeader.GetDictionary() );
	IAAFContainerDefSP spContDef = axDict.LookupContainerDef( ContainerFile );
	axFileDesc.SetContainerFormat( spContDef );

	// Remove the original source mob and essence data object.
	// This doesn't work.  The !attached() PRECONDITION in OMStorable::~OMStorable
	// fails.  Oddly, according to the stack trace it appears to be the MetaDictionay
	// that is not attached.
	//CHECK_HRESULT( _spHeader->RemoveEssenceData( axHeader.LookupEssenceData( axSrcMob.GetMobID() ) ) );
	//CHECK_HRESULT( _spHeader->RemoveMob( static_cast<IAAFMobSP>(axSrcMob) ) );
}

void EssenceProcessor::InsertFileSourceMob( IAAFSourceMobSP spSrcMob )
{
	// Not implemented.
	assert(0);
}
#endif

void EssenceProcessor::AddLocatorAndRemoveEssence( IAAFSourceMobSP spSrcMob )
{
	AxSourceMob axSrcMob( spSrcMob );
	AxString name = axSrcMob.GetName();
	
	if ( isAudioNode(_node) ) {
		name += L" (External audio)";
	}
	else if ( isVideoNode( _node) ) {
		name += L" (External video)";
	}
	else {
		name += L" (External)";
	}

	IAAFEssenceDescriptorSP spEssDesc = axSrcMob.GetEssenceDescriptor();
	AxEssenceDescriptor axEssDesc( spEssDesc );
	AxHeader axHeader( _spHeader );
	IAAFNetworkLocatorSP spLoc;
	AxCreateInstance( axHeader.GetDictionary(), spLoc );
	AxNetworkLocator axLoc( spLoc );
	axLoc.SetPath( L"file://" + _fileRefName );

	axEssDesc.AppendLocator( axLoc );

	// Essence be gone.
	CHECK_HRESULT( _spHeader->RemoveEssenceData( axHeader.LookupEssenceData( axSrcMob.GetMobID() ) ) );

	// Change the container def to file.
	IAAFFileDescriptorSP spFileDesc;
	AxQueryInterface( spEssDesc, spFileDesc );
	AxFileDescriptor axFileDesc( spFileDesc );
	
	AxDictionary axDict( axHeader.GetDictionary() );
	IAAFContainerDefSP spContDef = axDict.LookupContainerDef( ContainerFile );
	axFileDesc.SetContainerFormat( spContDef );

}

void EssenceProcessor::AddLocatorAndRemoveEssence()
{
	AddLocatorAndRemoveEssence( _node.GetAif2XtlAAFObject() );
}

//========================================================================

//========================================================================

//========================================================================

}; // end of namespace

Aif2XtlEssenceProcessVisitor::Aif2XtlEssenceProcessVisitor( IAAFHeaderSP spHeader,
															const AxString& aafFilename,
														    const AxString& extDirectory,
														    bool writeFile, bool stripEssence,
							                                Aif2XtlParseTreeNodeFactory& nodeFactory,
															std::wostream& logStream )
:	Aif2XtlParseTreeVisitor( logStream ),
    _spHeader( spHeader ),
	_aafFilename( aafFilename ),
	_directory( extDirectory ),
	_writeFile( writeFile ),
	_stripEssence( stripEssence ),
	_nodeFactory( nodeFactory )
{}

Aif2XtlEssenceProcessVisitor::~Aif2XtlEssenceProcessVisitor()
{}

void Aif2XtlEssenceProcessVisitor::PreOrderVisit( Aif2XtlSourceMob& node )
{
  if ( (isAudioNode(node) || isVideoNode(node)) ) {

    EssenceProcessor processor(node, _spHeader, _aafFilename, _directory, _writeFile, _nodeFactory, GetLogStream() );
    processor.ProcessEssence();

#if AAF2XTL_MODIFY_FILE
    if ( _stripEssence && isEssenceInternal() ) {
      extractor.AddLocatorAndRemoveEssence();
    }	
#endif
  }
}

void Aif2XtlEssenceProcessVisitor::PostOrderVisit(  Aif2XtlSourceMob& node )
{}

// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <Aif2XtlSourceClipVisitor.h>

#include <Aif2XtlParseTreeNodeDecoration.h>

#include <AifParseTreeBuilder.h>

#include <AxComponent.h>
#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxSmartPointer.h>

#include <AAFResult.h>
#include <AAFTypes.h>

#include <assert.h>

namespace {


// Accept all objects except slots, unless the slot has the specified
// slot id.

class RejectUnreferencedSlots : public AifParseTreeBuilderAcceptFunc {
public:

	RejectUnreferencedSlots( aafSlotID_t slotId )
		: _slotId( slotId )
	{}

	virtual ~RejectUnreferencedSlots()
	{}

	virtual bool operator()( IAAFObjectSP spObject )
	{
		IUnknownSP spIUnknown;
		AxQueryInterface( spObject, spIUnknown );

		IAAFMobSlotSP spSlot;

		if ( AxIsA( spIUnknown, spSlot ) ) {
		
			AxMobSlot axSlot( spSlot );
			if ( axSlot.GetSlotID() == _slotId ) {
				return true;
			}
			else {
				return false;
			}

		}
		else {
			return true;
		}
	}
		
private:
	aafSlotID_t _slotId;
};

};

namespace {

class FetchChainedSourceClip : public Aif2XtlParseTreeVisitor {
public:
	FetchChainedSourceClip( std::wostream& logStream )
		: Aif2XtlParseTreeVisitor( logStream ),
		  _found( false ),
		  _pFoundSrcClipNode(0)
	{}

	virtual ~FetchChainedSourceClip()
	{}

	virtual void PreOrderVisit( Aif2XtlSourceClipNode& node )
	{
		if ( _found ) {
			return;
		}
		else {
			_found = true;
			_pFoundSrcClipNode = &node;
		}
	}

	bool GetFound()
	{ return _found; }

	Aif2XtlSourceClipNode* GetFoundNode()
	{ return _pFoundSrcClipNode; }

	IAAFSourceClipSP GetFoundSrcClip()
	{ return _pFoundSrcClipNode->GetAif2XtlAAFObject(); }

private:

	bool _found;
	Aif2XtlSourceClipNode* _pFoundSrcClipNode;
};


bool GetChainedSourceClip( AifParseTreeNode& subTreeRoot,
						   std::wostream& logStream,
						   Aif2XtlSourceClipNode** ppClipNodeRef )
{
	FetchChainedSourceClip fetchSrcClipVisitor( logStream );

	subTreeRoot.Visit( &fetchSrcClipVisitor );

	bool found = fetchSrcClipVisitor.GetFound();

	if ( found ) {
		*ppClipNodeRef = fetchSrcClipVisitor.GetFoundNode();
	}

	return found;
}

} // end of namespace

Aif2XtlSourceClipVisitor::Aif2XtlSourceClipVisitor( AifParseTreeNodeFactory& nodeFactory,
												    std::wostream& logStream )
: Aif2XtlParseTreeVisitor( logStream ),
  _nodeFactory( nodeFactory )
{}

Aif2XtlSourceClipVisitor::~Aif2XtlSourceClipVisitor()
{}

void Aif2XtlSourceClipVisitor::PreOrderVisit( Aif2XtlSourceClipNode& node )
{
	// If the source clip references a composition mob then we build the
	// sub tree for that mob.  If the sub tree also contains further
	// sub compositions, these will be processed when

	IAAFSourceClipSP spSourceClip;

	spSourceClip = node.GetAif2XtlAAFObject();

	AxSourceClip axSrcClip( spSourceClip );

	AxMob axReferencedMob( axSrcClip.ResolveRef() );
	
	IAAFCompositionMobSP spCompMob;
	if ( !AxIsA( axReferencedMob, spCompMob ) ) {
		return;
	}

	aafSourceRef_t srcRef = axSrcClip.GetSourceReference();

	// Decorate this node so that we can easily distingquish it from
	// other source clips (i.e. master and chained source clips)
	// later on.
	std::auto_ptr<Aif2XtlUnMasteredSourceClipDecoration> pSrcClipDec
		( new Aif2XtlUnMasteredSourceClipDecoration );
	node.PushDecoration( pSrcClipDec );

	// Build a sub tree that includes only the referenced slot.
	RejectUnreferencedSlots rejectAllSlotsButThisOne( srcRef.sourceSlotID );
	std::auto_ptr<AifParseTreeNode> chainedSubTreeRoot 
			= AifBuildParseTree( axReferencedMob,
							     _nodeFactory,
							     rejectAllSlotsButThisOne );

	node.AddChild( chainedSubTreeRoot );
}

void Aif2XtlSourceClipVisitor::PostOrderVisit( Aif2XtlSourceClipNode& node )
{

	try {

		IAAFSourceClipSP spChainedSourceClip = node.GetAif2XtlAAFObject();
		AxSourceClip axSrcClip( spChainedSourceClip );

		AxMob axReferencedMob( axSrcClip.ResolveRef() );
		IAAFMasterMobSP spMasterMob;
		if ( !AxIsA( axReferencedMob, spMasterMob ) ) {
			return;
		}

		// Decorate this node so that we can easily distingquish it from
		// other source clips (i.e. master and chained source clips)
		// later on.
		std::auto_ptr<Aif2XtlUnMasteredSourceClipDecoration> pSrcClipDec
			( new Aif2XtlUnMasteredSourceClipDecoration );
		node.PushDecoration( pSrcClipDec );
	
		aafSourceRef_t srcRef = axSrcClip.GetSourceReference();

		RejectUnreferencedSlots rejectAllSlotsButThisOne( srcRef.sourceSlotID );

		std::auto_ptr<AifParseTreeNode> chainedSubTreeRoot 
			= AifBuildParseTree( axReferencedMob,
								 _nodeFactory,
								 rejectAllSlotsButThisOne );
			
		Aif2XtlSourceClipNode* pRootSrcClipNode = 0;
		bool notAtEndOfChain = GetChainedSourceClip( *chainedSubTreeRoot, GetLogStream(), &pRootSrcClipNode );

		Aif2XtlSourceClipNode* pChainedSrcClipNode = pRootSrcClipNode;

		// This is a master mob.  It should never be the end of the chain.
		assert(notAtEndOfChain);

		node.AddChild( chainedSubTreeRoot );		

		while ( notAtEndOfChain ) {

			spChainedSourceClip = pChainedSrcClipNode->GetAif2XtlAAFObject();

			AxSourceClip axSrcClip( spChainedSourceClip );

			AxMob axReferencedMob( axSrcClip.ResolveRef() );
	
			aafSourceRef_t srcRef = axSrcClip.GetSourceReference();

			RejectUnreferencedSlots rejectAllSlotsButThisOne( srcRef.sourceSlotID );

			std::auto_ptr<AifParseTreeNode> chainedSubTreeRoot 
				= AifBuildParseTree( axReferencedMob,
						     _nodeFactory,
						     rejectAllSlotsButThisOne );
			
			notAtEndOfChain = GetChainedSourceClip( *chainedSubTreeRoot, GetLogStream(), &pChainedSrcClipNode );

			pRootSrcClipNode->AddChild( chainedSubTreeRoot );
		}
	}
	catch( const AxExHResult& ex ) {
		if ( ex.getHResult() != AAFRESULT_MOB_NOT_FOUND ) {
			throw;
		}
	}

}

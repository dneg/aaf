//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Siemens SBS Media
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <Aif2XtlTransitionVisitor.h>

#include <Aif2XtlParseTreeNodeDecoration.h>

#include <AxComponent.h>
#include <AxDefObject.h>

#include <AAFOperationDefs.h>

#include <map>

#include <assert.h>

namespace {

// This UID came out of the sample xtl files included with the
// DirectX9 SDK, and are part of the Direct Editting Services
// documentation.
aafUID_t DissolveEffectUID =
{ 0x16B280C5, 0xEE70, 0x11D1, { 0x90, 0x66, 0x00, 0xC0, 0x4F, 0xD9, 0x18, 0x9D } };

aafUID_t SMPTEWipeEffectUID =
{ 0xDE75D012, 0x7A65, 0x11D2, { 0x8C, 0xEA, 0x00, 0xA0, 0xC9, 0x44, 0x1E, 0x20 } };

#if 0
//====================================================================================

class EffectHandler
{
public:
	virtual void Execute( Aif2XtlTransition& node, IAAFOperationGroupSP spOpGroup ) = 0;
};


//====================================================================================

// FIXME - If this was really clean, these handlers would be registered from
// the XTL generation code.. rather than leaking this much XTL generate code
// into this file.

class DefaultHandler : public EffectHandler
{
public:
	DefaultHandler()
	{}
	virtual ~DefaultHandler()
	{}

	virtual void Execute( Aif2XtlTransition& node, IAAFOperationGroupSP spOpGroup )
	{
	}
};

//====================================================================================

class DissolveHandler : public EffectHandler
{
public:
	DissolveHandler()
	{}
	virtual ~DissolveHandler()
	{}

	virtual void Execute( Aif2XtlTransition& node, IAAFOperationGroupSP spOpGroup )
	{
	}
};

//====================================================================================

class SMPTEWipeHandler : public EffectHandler
{
public:
	SMPTEWipeHandler()
	{}
	virtual ~SMPTEWipeHandler()
	{}

	virtual void Execute( Aif2XtlTransition& node, IAAFOperationGroupSP spOpGroup )
	{
	}
};

//====================================================================================

class EffectHandlerMap
{
public:
	static EffectHandlerMap& GetInstance()
	{
		EffectHandlerMap* pInstance = 0;
		if ( !pInstance ) {
			pInstance = new EffectHandlerMap;
			assert(pInstance);
		}
		return *pInstance;
	}

	~EffectHandlerMap()
	{
		// Fixme - delete all
	}

	EffectHandler& GetHandler( const aafUID_t& uid )
	{
		return *_handlers[uid];
	}

private:
	EffectHandlerMap()
	{

	}

	std::map<aafUID_t, EffectHandler*> _handlers;
	EffectHandler* _pDefaultHandler;
};


//====================================================================================
#endif

aafUID_t FetchAAFEffectUID( Aif2XtlTransition& node )
{
	AxTransition axTrans( node.GetAif2XtlAAFObject() );
	AxOperationGroup axOpGroup( axTrans.GetOperationGroup() );
	AxOperationDef axOpDef( axOpGroup.GetOperationDef() );
	return axOpDef.GetCategory();
}

} // end of namespace

//====================================================================================

Aif2XtlTransitionVisitor::Aif2XtlTransitionVisitor( bool cutsOnly, std::wostream& logStream )
: Aif2XtlParseTreeVisitor( logStream ),
  _cutsOnly( cutsOnly )
{}

Aif2XtlTransitionVisitor::~Aif2XtlTransitionVisitor()
{}

void Aif2XtlTransitionVisitor::PreOrderVisit( Aif2XtlTransition& node )
{

	AxTransition axTransition( node.GetAif2XtlAAFObject() );	

	// Get the component length, and the transition cut point
	// (i.e.  the point where the edit should occur if the
	// transition is not supported.
	aafPosition_t cutPoint = axTransition.GetCutPoint();
	aafLength_t length = axTransition.GetLength();
	
	bool supported;
	if ( _cutsOnly ) {
		supported = false;
	
	}
	else {
		supported = true;
	}

	aafUID_t aafEffectUID = FetchAAFEffectUID( node );

	aafUID_t xtlEffectUID;

	if ( kAAFEffectVideoDissolve == aafEffectUID     ||
	     kAAFEffectMonoAudioDissolve == aafEffectUID ||
		 kAAFEffectStereoAudioDissolve == aafEffectUID ) {
		xtlEffectUID = DissolveEffectUID;
	}
	else if ( kAAFEffectSMPTEVideoWipe == aafEffectUID ) {
		xtlEffectUID = SMPTEWipeEffectUID;
	}
	else {
		// The effect is not supported.  Use DissolveEffectUID as a default
		// value, but set "supported" to false to cause a cut edit.
		xtlEffectUID = DissolveEffectUID;
		supported = false;
	}

	// Create a decoration with the transition info.  The sequence
	// edit traveral (visitor) will use this information.
	std::auto_ptr<Aif2XtlTransitionInfo> pTransitionInfo(
		new Aif2XtlTransitionInfo( supported, length, cutPoint, xtlEffectUID ) );

	// FIXME - Temporary fix to transition parameter gen issues.
	pTransitionInfo->SetNode( node );
	// end FIXME

	// Decorate the node with the transition info object.
	node.PushDecoration( pTransitionInfo );	

	// No need to traverse further.
	node.SetVisitChildren( false );
}

void Aif2XtlTransitionVisitor::PostOrderVisit( Aif2XtlTransition& node )
{
	node.SetVisitChildren( true );
}


// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <Aif2XtlParseTreeNodeFactory.h>

#include <AxObject.h>
#include <AxMetaDef.h>

//============================================================

Aif2XtlParseTreeNodeFactoryMap* Aif2XtlParseTreeNodeFactoryMap::_instance = 0;

Aif2XtlParseTreeNodeFactoryMap& Aif2XtlParseTreeNodeFactoryMap::GetInstance()
{
	if ( !_instance ) {
		_instance = new Aif2XtlParseTreeNodeFactoryMap;
	}
	
	return *_instance;
}

Aif2XtlParseTreeNodeFactoryMap::Aif2XtlParseTreeNodeFactoryMap()
{}

void Aif2XtlParseTreeNodeFactoryMap::RegisterFactory( const aafUID_t& auid,
						      AifParseTreeNodeFactory* pFactory )
{
	operator[](auid) = pFactory;
}


bool Aif2XtlParseTreeNodeFactoryMap::IsInMap( const aafUID_t& auid )
{
	iterator iter = find( auid );

	if ( iter == end() ) {
		return false;
	}
	else {
		return true;
	}
}

//============================================================

Aif2XtlParseTreeNodeFactory::Aif2XtlParseTreeNodeFactory()
{}

Aif2XtlParseTreeNodeFactory::~Aif2XtlParseTreeNodeFactory()
{}

std::auto_ptr<AifParseTreeNode> Aif2XtlParseTreeNodeFactory::CreateNode( IAAFObjectSP spObject )
{
	AxObject axObject( spObject );

	IAAFClassDefSP spClassDef;
	// FIXME - Must implement wrapper for this.
	CHECK_HRESULT( spObject->GetDefinition( &spClassDef ) );

	AxClassDef axClassDef( spClassDef );

	aafUID_t auid = axClassDef.GetAUID();

	if ( Aif2XtlParseTreeNodeFactoryMap::GetInstance().IsInMap( auid ) ) {
		return Aif2XtlParseTreeNodeFactoryMap::GetInstance()[auid]->CreateNode( spObject );
	}
	else {
		// We don't handle it... pass to base class.
		return AifParseTreeNodeFactory::CreateNode( spObject );
	}
}


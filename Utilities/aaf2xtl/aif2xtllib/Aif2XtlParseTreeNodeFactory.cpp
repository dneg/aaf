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


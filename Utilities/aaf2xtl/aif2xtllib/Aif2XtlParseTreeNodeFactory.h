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

#ifndef _Aif2XtlParseTreeNodeFactory_h_
#define _Aif2XtlParseTreeNodeFactory_h_

#include <AifParseTreeNodeFactory.h>

#include <AAFTypes.h>

#include <map>
#include <memory>

// Singleton object that maps an AAF class UID to a factory implementation. 
class Aif2XtlParseTreeNodeFactoryMap : public std::map< aafUID_t, AifParseTreeNodeFactory* > {
public:

	static Aif2XtlParseTreeNodeFactoryMap& GetInstance();

	void RegisterFactory( const aafUID_t& auid,
			      AifParseTreeNodeFactory* pFactory );

	bool IsInMap( const aafUID_t& auid );
private:
	// Prohibited
	Aif2XtlParseTreeNodeFactoryMap();

	static Aif2XtlParseTreeNodeFactoryMap* _instance;
};


class Aif2XtlParseTreeNodeFactory : public AifParseTreeNodeFactory {
public:


	Aif2XtlParseTreeNodeFactory();
	virtual ~Aif2XtlParseTreeNodeFactory();

	virtual std::auto_ptr<AifParseTreeNode> CreateNode( IAAFObjectSP );

private:
};

#endif

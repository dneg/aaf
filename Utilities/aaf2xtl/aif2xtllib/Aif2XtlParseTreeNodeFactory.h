// Copyright 2003 BBC Technology

// $Id$
// $Name$

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

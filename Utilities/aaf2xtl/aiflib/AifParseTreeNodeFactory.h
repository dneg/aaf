// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _AifParseTreeNodeFactory_h_
#define _AifParseTreeNodeFactory_h_

#include <AifParseTreeNode.h>

#include <AxSmartPointer.h>

#include <memory>

class AifParseTreeNodeFactory {
public:
	AifParseTreeNodeFactory();
	virtual ~AifParseTreeNodeFactory();

	virtual std::auto_ptr<AifParseTreeNode> CreateNode( IAAFObjectSP );
};

#endif

// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <AifParseTreeNodeFactory.h>

AifParseTreeNodeFactory::AifParseTreeNodeFactory()
{}

AifParseTreeNodeFactory::~AifParseTreeNodeFactory()
{}

std::auto_ptr<AifParseTreeNode> AifParseTreeNodeFactory::CreateNode( IAAFObjectSP sp )
{
	std::auto_ptr<AifParseTreeNode> pNode( new AifParseTreeNode(sp) );
	return pNode;
}

// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <AifParseTree.h>

AifParseTree::AifParseTree(  std::auto_ptr<AifParseTreeNode> root )
: _root( root )
{}

AifParseTree::~AifParseTree()
{}

void AifParseTree::Traverse( AifParseTreeVisitor* visitor )
{
	_root->Visit( visitor );
}

// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _AifParseTree_h_
#define _AifParseTree_h_

#include <AifParseTreeNode.h>
#include <memory>

class AifParseTreeVisitorPtrcl;

class AifParseTree {
public:
	AifParseTree( std::auto_ptr<AifParseTreeNode> root );
	~AifParseTree();

	void Traverse( AifParseTreeVisitor* visitor );

private:
	std::auto_ptr<AifParseTreeNode> _root;	
};


#endif

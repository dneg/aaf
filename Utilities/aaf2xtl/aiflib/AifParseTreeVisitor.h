// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _AifParseTreeVisitor_h_
#define _AifParseTreeVisitor_h_

class AifParseTreeNode;

class AifParseTreeVisitor {
public:
	AifParseTreeVisitor();
	virtual ~AifParseTreeVisitor();
	virtual void PreOrderVisit( AifParseTreeNode& node );
	virtual void PostOrderVisit( AifParseTreeNode& node );
};

#endif

// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _Aif2XtlSourceClipVisitor_h_
#define _Aif2XtlSourceClipVisitor_h_

#include <Aif2XtlParseTreeVisitor.h>
#include <Aif2XtlParseTreeNodes.h>

#include <AifParseTreeVisitor.h>

class AifParseTreeNodeFactory;

class Aif2XtlSourceClipVisitor : public Aif2XtlParseTreeVisitor {
public:
	Aif2XtlSourceClipVisitor( AifParseTreeNodeFactory& nodeFactory,
							  std::wostream& logStream );

	virtual ~Aif2XtlSourceClipVisitor();

	virtual void PreOrderVisit( Aif2XtlSourceClipNode& node );

	virtual void PostOrderVisit( Aif2XtlSourceClipNode& node );

private:

	AifParseTreeNodeFactory& _nodeFactory;
};

#endif


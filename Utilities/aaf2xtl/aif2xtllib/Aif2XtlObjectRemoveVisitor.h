// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _Aif2XtlObjectRemove_h_
#define _Aif2XtlObjectRemove_h_

#include <Aif2XtlParseTreeVisitor.h>
#include <Aif2XtlParseTreeNodes.h>

class Aif2XtlObjectMarkForRemovalVisitor : public Aif2XtlParseTreeVisitor {
public:
	Aif2XtlObjectMarkForRemovalVisitor( std::wostream& logStream );
	virtual ~Aif2XtlObjectMarkForRemovalVisitor();

	virtual void PreOrderVisit( Aif2XtlTaggedValue& node );
	virtual void PostOrderVisit( Aif2XtlTaggedValue& node );
};

class Aif2XtlObjectRemoveMarkedVisitor : public Aif2XtlParseTreeVisitor {
public:
	Aif2XtlObjectRemoveMarkedVisitor( std::wostream& logStream );
	virtual ~Aif2XtlObjectRemoveMarkedVisitor();

	virtual void PreOrderVisit( Aif2XtlParseTreeNode& node );
};


#endif

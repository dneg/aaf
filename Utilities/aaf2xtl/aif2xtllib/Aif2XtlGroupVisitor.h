// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _Aif2XtlGroupVisitor_h_
#define _Aif2XtlGroupVisitor_h_

#include <Aif2XtlParseTreeVisitor.h>
#include <Aif2XtlParseTreeNodes.h>

class Aif2XtlGroupVisitor : public Aif2XtlParseTreeVisitor {
public:
	Aif2XtlGroupVisitor( std::wostream& logStream );

	virtual ~Aif2XtlGroupVisitor();

	virtual void PreOrderVisit( Aif2XtlCompMobNode& node );
	virtual void PostOrderVisit( Aif2XtlCompMobNode& node );
};

#endif


// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _Aif2XtlSeqOffsetVisitor_h_
#define _Aif2XtlSeqOffsetVisitor_h_

#include <Aif2XtlParseTreeVisitor.h>
#include <Aif2XtlParseTreeNodes.h>

class Aif2XtlSeqOffsetVisitor : public Aif2XtlParseTreeVisitor {
public:
	Aif2XtlSeqOffsetVisitor( std::wostream& logStream );
	virtual ~Aif2XtlSeqOffsetVisitor();

	virtual void PreOrderVisit( Aif2XtlSourceClipNode& node );
	virtual void PostOrderVisit( Aif2XtlSourceClipNode& node );

private:

	void PreVisit( AifParseTreeNode& node );
	void PostVisit( AifParseTreeNode& node );

	aafLength_t _offset;	
};

#endif

// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _Aif2XtlMobSelectVisitor_h_
#define _Aif2XtlMobSelectVisitor_h_

#include <Aif2XtlParseTreeVisitor.h>
#include <Aif2XtlParseTreeNodes.h>

#include <AifParseTreeVisitor.h>

class Aif2XtlMobSelectFunc {
public:
	virtual bool operator()( Aif2XtlSourceMob& ) = 0;
};

class Aif2XtlSelectFirstFileMob : public Aif2XtlMobSelectFunc {
public:
	Aif2XtlSelectFirstFileMob();
	virtual ~Aif2XtlSelectFirstFileMob();
	virtual bool operator()( Aif2XtlSourceMob& srcMobNode );
};

class Aif2XtlMobSelectVisitor : public Aif2XtlParseTreeVisitor {
public:
	Aif2XtlMobSelectVisitor( Aif2XtlMobSelectFunc& func,
							 std::wostream& logStream );

	virtual ~Aif2XtlMobSelectVisitor();

	virtual void PreOrderVisit( Aif2XtlMasterMob& node );
	virtual void PostOrderVisit( Aif2XtlMasterMob& node );

	virtual void PreOrderVisit( Aif2XtlSourceClipNode& node );
	virtual void PostOrderVisit( Aif2XtlSourceClipNode& node );

private:
	// Prohibited
	Aif2XtlMobSelectVisitor();

	bool _master;
	int  _srcClipCount;
	Aif2XtlMobSelectFunc& _selectFunc;
};

#endif


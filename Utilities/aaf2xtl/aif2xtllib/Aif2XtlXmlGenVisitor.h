// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _Aif2XtlXmlGenVisitor_h_
#define _Aif2XtlXmlGenVisitor_h_

#include <Aif2XtlParseTreeNodes.h>
#include <Aif2XtlParseTreeVisitor.h>

#include <iosfwd>

class Aif2XtlXmlGenVisitor : public Aif2XtlParseTreeVisitor {
public:

	Aif2XtlXmlGenVisitor( std::ostream& os, std::wostream& logStream );
	virtual ~Aif2XtlXmlGenVisitor();

#define _DECLARE_METHODS( TYPE ) \
	virtual void PreOrderVisit( TYPE& node ); \
	virtual void PostOrderVisit( TYPE& node ); \

	_DECLARE_METHODS( Aif2XtlCompMobNode );
	_DECLARE_METHODS( Aif2XtlGroupNode );
	_DECLARE_METHODS( Aif2XtlSequence );
	_DECLARE_METHODS( Aif2XtlSourceMob );
	_DECLARE_METHODS( Aif2XtlSourceClipNode );
	_DECLARE_METHODS( Aif2XtlWaveDescriptor );
	_DECLARE_METHODS( Aif2XtlNetworkLocator );

private:
	// prohibited
	Aif2XtlXmlGenVisitor();
	Aif2XtlXmlGenVisitor( const Aif2XtlXmlGenVisitor& );

	int _level;
	int _compMobNodeLevel;
	std::ostream& _os;
};

#endif

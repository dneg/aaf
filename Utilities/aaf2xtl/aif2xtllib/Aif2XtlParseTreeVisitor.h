// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _Aif2XtlParseTreeVisitor_h_
#define _Aif2XtlParseTreeVisitor_h_

#include <AifParseTreeVisitor.h>

#include <Aif2XtlParseTreeNodes.h>

#include <AxUtil.h>
#include <AxObject.h>

#include <iosfwd>

class Aif2XtlParseTreeVisitor : public AifParseTreeVisitor  {

public:
	Aif2XtlParseTreeVisitor( std::wostream& logStream )
	: _logStream( logStream )
	{}

	virtual ~Aif2XtlParseTreeVisitor()
	{}

#define _DEFINE_METHODS( TYPE ) \
	virtual void PreOrderVisit( TYPE& ) {}; \
	virtual void PostOrderVisit( TYPE& ) {}; \

	_DEFINE_METHODS( Aif2XtlParseTreeNode );
	_DEFINE_METHODS( Aif2XtlCompMobNode );
	_DEFINE_METHODS( Aif2XtlSourceClipNode );
	_DEFINE_METHODS( Aif2XtlTimelineMobSlot );
	_DEFINE_METHODS( Aif2XtlSequence );
	_DEFINE_METHODS( Aif2XtlMasterMob );
	_DEFINE_METHODS( Aif2XtlSourceMob );
	_DEFINE_METHODS( Aif2XtlCDCIDescriptor );
	_DEFINE_METHODS( Aif2XtlWaveDescriptor );
	_DEFINE_METHODS( Aif2XtlNetworkLocator );
	_DEFINE_METHODS( Aif2XtlGroupNode );
	_DEFINE_METHODS( Aif2XtlTransition );
	_DEFINE_METHODS( Aif2XtlTaggedValue );
	_DEFINE_METHODS( Aif2XtlConstantValue );

#undef _DEFINE_METHODS

protected:

	std::wostream& GetLogStream()
	{ return _logStream; }

private:

	// Default ctor is prohibited to avoid use of unititialized _logStream
	Aif2XtlParseTreeVisitor();

	std::wostream& _logStream;
};

class Aif2XtlDumpVisitor : public Aif2XtlParseTreeVisitor {
public:

	Aif2XtlDumpVisitor( std::wostream& os, std::wostream& logStream );
	virtual ~Aif2XtlDumpVisitor();

#define _DEFINE_METHODS( NODE_TYPE ) \
	virtual void Aif2XtlDumpVisitor::PreOrderVisit( NODE_TYPE& node ) \
	{ \
		_level++; \
		if ( node.isAAFObjectPresent() ) { \
			AxObject axObject(node.GetAAFObject()); \
			indent( node.GetExceptionFlag() ); \
			_os << AxStringUtil::mbtowc( #NODE_TYPE ) << L"(" << node.GetNumChildren() << L") (" << axObject.GetClassName() << L")" << std::endl; \
		} \
		else { \
			indent( node.GetExceptionFlag() ); \
			_os << AxStringUtil::mbtowc( #NODE_TYPE ) << L"(" << node.GetNumChildren() << L")" << std::endl; \
		} \
	} \
	virtual void Aif2XtlDumpVisitor::PostOrderVisit( NODE_TYPE& node ) \
	{ \
		_level--; \
	}

	_DEFINE_METHODS( AifParseTreeNode );
	_DEFINE_METHODS( Aif2XtlCompMobNode );
	_DEFINE_METHODS( Aif2XtlSourceClipNode );
	_DEFINE_METHODS( Aif2XtlTimelineMobSlot );
	_DEFINE_METHODS( Aif2XtlSequence );
	_DEFINE_METHODS( Aif2XtlMasterMob );
	_DEFINE_METHODS( Aif2XtlSourceMob );
	_DEFINE_METHODS( Aif2XtlCDCIDescriptor );
	_DEFINE_METHODS( Aif2XtlWaveDescriptor );
	_DEFINE_METHODS( Aif2XtlNetworkLocator );
	_DEFINE_METHODS( Aif2XtlGroupNode );
	_DEFINE_METHODS( Aif2XtlTransition );
	_DEFINE_METHODS( Aif2XtlTaggedValue );
	_DEFINE_METHODS( Aif2XtlConstantValue );
	
#undef _DEFINE_METHODS

private:

	void indent( bool exceptionFlag );

	int _level;
	std::wostream& _os;
};


#endif

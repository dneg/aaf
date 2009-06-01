//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Siemens SBS Media
// All rights reserved.
//
//=---------------------------------------------------------------------=

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

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
// Siemens SBS Media.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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

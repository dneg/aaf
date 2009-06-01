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

#include <Aif2XtlXmlGenVisitor.h>

#include <Aif2XtlParseTreeNodeDecoration.h>
#include <Aif2XtlTimecode.h>

#include <AxEssence.h>
#include <AxComponent.h>
#include <AxParameter.h>
#include <AxDefObject.h>

#include <AAFOperationDefs.h>
#include <AAFTypeDefUIDs.h>
#include <AAFParameterDefs.h>

#include <sstream>

#include <assert.h>
#include <stdio.h>

using namespace std;

namespace {

// This was lifted from AxStringUtil::uid2str and modified to produce
// a string formated in accordance with xtl's expectantions.  It
// should be moved back to AxStringUtil.

std::string uid2XtlStr(const aafUID_t & uid)
{
        const int bufSize = 2 * 16 + 4 + 1; // 16 bytes as hex + 4 hyphens + null termination
        char buf[bufSize];
        int rc;
        using namespace std;

        rc = sprintf( buf,
                                "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x", // UID
                                uid.Data1, uid.Data2, uid.Data3,
                                uid.Data4[0], uid.Data4[1], uid.Data4[2], uid.Data4[3],
                                uid.Data4[4], uid.Data4[5], uid.Data4[6], uid.Data4[7] );
        if (rc != bufSize - 1)
        {
                throw AxEx( L"Error in uid2Str" );
        }
        return std::string( buf );
}


class Level {
public:

	inline Level(int l)
		:_l(l)
	{}


	inline int Get() const
	{ return _l; }
private:
	int _l;
};

std::ostream& operator<<( std::ostream& os, const Level& level )
{
	int l = level.Get();

	while( l-- >= 0 ) {
		os << "  ";
	}

	return os;
}

// Finds decorated parent and returns an unowned pointer to the
// decoration.
template <class Type>
bool SearchForDecoratedParent( Type*& pDecoration,
							   AifParseTreeNode& node )
{
	AifParseTreeNode* pParent = node.GetParent();

	if ( !pParent ) {
		return false;
	}
	
	if ( pParent->IsDecorated( pDecoration ) ) {
		return true;
	}

	return SearchForDecoratedParent( pDecoration, *pParent );
}

// Finds the decorated parent, pops the decoration off the parent,
// returns a pointer to decoration and pointer to the parent.
template <class Type>
bool SearchForDecoratedParent( auto_ptr<Type>& pDecoration,
							   AifParseTreeNode& node,
							   AifParseTreeNode*& pParentRet )
{
	AifParseTreeNode* pParent = node.GetParent();

	if ( !pParent ) {
		return false;
	}
	
	Type* pDecorationUnowned;

	if ( pParent->IsDecorated( pDecorationUnowned ) ) {
		pParent->PopDecoration( pDecoration );
		pParentRet = pParent;
		return true;
	}

	return SearchForDecoratedParent( pDecoration, *pParent, pParentRet );
}

//====================================================================

class CompositeDecoration : public AifParseTreeNodeDecoration {
public:
	CompositeDecoration()
	:	_outputPending(false)
	{}

	~CompositeDecoration()
	{}

	void SetOutputPending()
	{ _outputPending = true; }

	bool GetOutputPending()
	{ return _outputPending; }

private:
	bool _outputPending;
};


class CompositeVisitor : public Aif2XtlParseTreeVisitor {
public:
	CompositeVisitor( std::wostream& logStream )
		: Aif2XtlParseTreeVisitor( logStream )
	{}

	virtual ~CompositeVisitor()
	{}

	virtual void PreOrderVisit( Aif2XtlSourceMob& node )
	{
		CompositeDecoration* pCompDec = 0;
		bool found = SearchForDecoratedParent( pCompDec, node );
		if ( found ) {
			pCompDec->SetOutputPending();
		}
	}
};

//====================================================================

class CDCIVisitor : public Aif2XtlParseTreeVisitor {
public:
	CDCIVisitor( std::wostream& logStream )
	: Aif2XtlParseTreeVisitor( logStream ),
	  _displayWidth(0),
	  _displayHeight(0)
	{
		_displayRate.numerator = 0;
		_displayRate.denominator = 0;
	}

	virtual ~CDCIVisitor()
	{}

	virtual void PreOrderVisit( Aif2XtlCDCIDescriptor& node )
	{
		// FIXME - There is no check to see if these values are already
		// set.  Hence, we implicitly are using the values of the
		// last CDCI descriptor.  That may or may not be an issue, it depends on
		// the data source.
		
		// FIXME - Must examine how to handle differing stored/sampled/displayed
		// view values.  For the moment, assume that media player will show
		// the entire stored view.

		AxCDCIDescriptor axDesc( node.GetAif2XtlAAFObject() );

		axDesc.GetStoredView( _displayHeight, _displayWidth );
		_displayRate = axDesc.GetSampleRate();
	}

	unsigned int GetDisplayWidth()
	{
		return _displayWidth;
	}

	unsigned int GetDisplayHeight()
	{
		// FIXME - obvious concerns here.  The cdci descriptor
		// in the sample files reports a height of 288 -
		// i.e. one PAL field.  But we want the video
		// displayed at frame dimensions.

		if ( 720 == _displayWidth && 576/2 == _displayHeight ) {
			return 2*_displayHeight;
		}
		else if ( 720 == _displayWidth && 486 == _displayWidth ) {
			return 2*_displayHeight;
		}
		else {
 			return _displayHeight;
		}
	}

	aafRational_t GetDisplayRate()
	{ return _displayRate; }

	string GetDisplayRateAsString()
	{
		// generate string of the form xx.xxxx
		// 7 characters plus null.

		double rate = static_cast<double>(_displayRate.numerator)/
					  static_cast<double>(_displayRate.denominator);

		// The code below will fail if the rate has more than
		// three digits to the right of the decimal, or has
		// more than two digits to the left.
		assert( rate < 99.9999 );

		if ( 0 != _displayRate.denominator ) {
			char buffer[8];
			::sprintf( buffer, "%7.4f",
				static_cast<double>(_displayRate.numerator)/
				static_cast<double>(_displayRate.denominator) );
			return string(buffer);
				}
		else {
			return string( "infinity" );
		}
	}

private:
	aafUInt32 _displayWidth;
	aafUInt32 _displayHeight;
	aafRational_t _displayRate;
};

//====================================================================


class ParameterGenVisitor : public Aif2XtlParseTreeVisitor
{
public:
	ParameterGenVisitor( std::ostream& os , int level, std::wostream& logStream )
	: Aif2XtlParseTreeVisitor( logStream ),
	  _os( os ),
	  _level( level )
	{}

	virtual ~ParameterGenVisitor()
	{}

	void PreOrderVisit( Aif2XtlConstantValue& node )
	{
		Level l(_level++);

		AxConstantValue axParamVal( node.GetAif2XtlAAFObject() );
		AxParameterDef axParamDef( axParamVal.GetParameterDefinition() );
		aafUID_t paramID = axParamDef.GetAUID();

		if ( kAAFParameterDefSMPTEWipeNumber == paramID ) {
			aafInt32 wipeNumber;
			axParamVal.GetValue( wipeNumber );
			_os << l << "<param name=\"MaskNum\" value=\"" << wipeNumber <<"\"/>";
			_os << endl;
		}
	}

	void PostOrderVisit( Aif2XtlConstantValue& node )
	{
		Level l(_level--);
	}

private:

	int _level;
	std::ostream& _os;
};

} // end of namespace

//====================================================================

Aif2XtlXmlGenVisitor::Aif2XtlXmlGenVisitor( std::ostream& os, std::wostream& logStream  )
: Aif2XtlParseTreeVisitor( logStream ),
  _os(os),
  _level(-1),
  _compMobNodeLevel(-1)
{}

Aif2XtlXmlGenVisitor::~Aif2XtlXmlGenVisitor()
{}

/////

void Aif2XtlXmlGenVisitor::PreOrderVisit( Aif2XtlCompMobNode& node )
{
	Level l(++_level);

	// FIXME - This is not the slickest, but it works for now.
	// A better approach would be to insert a "timeline" node as the root
	// node in the graph, then generate the timeline output there.  The 
	// "composite" output could then be done here rather than for the
	// sequence node.  In the end it all the same, and this is easier
	// even if less elegant.

	_compMobNodeLevel++;
	if ( 0 == _compMobNodeLevel ) {
		_os << l << "<timeline>" << endl;
	}
	else {
	}
}

void Aif2XtlXmlGenVisitor::PostOrderVisit( Aif2XtlCompMobNode& node )
{
	Level l(_level--);

	if ( 0 == _compMobNodeLevel ) {
		_os << l << "</timeline>" << endl;
	}

	_compMobNodeLevel--;
}

/////

void Aif2XtlXmlGenVisitor::PreOrderVisit( Aif2XtlGroupNode& node )
{
	Level l(++_level);

	string params;

	Aif2XtlGroupInfo* pGroupInfo = 0;
	node.GetDecoration( pGroupInfo );
	if ( Aif2XtlGroupInfo::GROUP_TYPE_AUDIO == pGroupInfo->GetGroupType()) {
		params = "type=\"audio\"";
	}
	else if ( Aif2XtlGroupInfo::GROUP_TYPE_VIDEO == pGroupInfo->GetGroupType()) {

		stringstream ss;

		ss << "type=\"video\" ";

		// Use the CDCIVisitor to determine the width, height, and rate.
		CDCIVisitor cdciVisitor( GetLogStream() );
		node.Visit( &cdciVisitor );

		ss << "width=\"" << cdciVisitor.GetDisplayWidth() << "\" "; 
		ss << "height=\"" << cdciVisitor.GetDisplayHeight() << "\" ";
		ss << "framerate=\"" << cdciVisitor.GetDisplayRateAsString() << "\" ";

		// FIXME - Is 32 always the best choice? Will it depend on the
		// system?
		ss << "bitdepth=\"24\" ";

		params = ss.str();
	}
	else {
		assert(0);
	}

	_os << l << "<group " << params << ">" << endl;
}

void Aif2XtlXmlGenVisitor::PostOrderVisit( Aif2XtlGroupNode& node )
{
	Level l(_level--);

	_os << l << "</group>" << endl;

	_level--;
}

/////

void Aif2XtlXmlGenVisitor::PreOrderVisit( Aif2XtlSequence& node )
{
 	// Decorate this node with a CompositeInfo decoration.
	// Then visit children looking for anything that will generate
	// output.  If no ouput will be generated then don't ouput the
	// <composite> XML element.
	std::auto_ptr<CompositeDecoration> apCompDec( new CompositeDecoration );
	CompositeDecoration* pUnownedCompDec = apCompDec.get();
	node.PushDecoration( apCompDec );

	CompositeVisitor compositeVisitor( GetLogStream() );
	node.Visit( &compositeVisitor );

#if _DEBUG
	// We gave up ownership of the pointer and depend on other code
	// to not destroy it.  We can assert it is valid by getting the
	// pointer back from the node.
	CompositeDecoration* pTestCompDec = 0;
	node.GetDecoration( pTestCompDec );
	assert( pTestCompDec == pUnownedCompDec );
#endif

	if ( !pUnownedCompDec->GetOutputPending() ) {
		node.SetVisitChildren(false);
		return;
	}
	
	Level l(++_level);

	if ( _compMobNodeLevel > 0 ) {
		_os << l << "<composite>" << endl;
	}
}

void Aif2XtlXmlGenVisitor::PostOrderVisit( Aif2XtlSequence& node )
{
  	std::auto_ptr<CompositeDecoration> pCompDec;
	node.PopDecoration( pCompDec );

	if ( !pCompDec->GetOutputPending() ) {
		node.SetVisitChildren(true);
		return;
	}


	// Check if the parent SourceClip (if any) is decorated wit a
	// transition info object.  If so, generate a transition
	// element.

	AifParseTreeNode* pDecoratedParent = 0;
	auto_ptr<Aif2XtlTransitionSequenceEditInfo> pTEditInfo(0);
	bool found = SearchForDecoratedParent( pTEditInfo, node, pDecoratedParent );
	if ( found ) {

		Aif2XtlTransitionInfo* pTransitionInfoUnowned = 0;
		if ( pDecoratedParent->IsDecorated( pTransitionInfoUnowned ) ) {
			auto_ptr<Aif2XtlTransitionInfo> pTransitionInfo(0);
			pDecoratedParent->PopDecoration( pTransitionInfo );

			Aif2XtlTimecode start( pTEditInfo->GetStartPosition(), pTEditInfo->GetEditRate() );
			Aif2XtlTimecode end( pTEditInfo->GetEndPosition(), pTEditInfo->GetEditRate() );

			// FIXME - Need better formattng stategy to get this transitition indented
			// at the same level as the preceeding tracks.
			_level += 3;
			Level l(_level);

			_os << l << "<transition ";
			_os << " start = \"" << start.GetXtlTimecodeString() << "\"";
			_os << " stop = \"" << end.GetXtlTimecodeString() << "\"";
			_os << " clsid=\"{" << uid2XtlStr( pTransitionInfo->GetEffectUID() ) << "}\">";
			_os << endl;

			ParameterGenVisitor paramGen( _os, l.Get(), GetLogStream() );
			pTransitionInfoUnowned->GetNode().Visit( &paramGen );

			_os << l << "</transition>" << endl;

			_level -= 3;
		}
	}

	Level l(_level--);


	if ( _compMobNodeLevel > 0 ) {
		_os << l << "</composite>" << endl;
	}

}

/////

void Aif2XtlXmlGenVisitor::PreOrderVisit( Aif2XtlSourceMob& node )
{
	Level track_l(++_level);
	_os << track_l << "<track>" << endl;

	Aif2XtlEssenceInfo* pDecorationUnowned;
	if ( node.IsDecorated( pDecorationUnowned ) ) {

		auto_ptr<Aif2XtlEssenceInfo> pEssInfo(0);
		node.PopDecoration( pEssInfo );		

		Level clip_l(++_level);
		_os << clip_l << "<clip" << endl;

		Level src_l(++_level);
		_os << src_l << "src=\"" << AxStringUtil::wctomb(pEssInfo->GetFileRef()) << "\" " << endl;
		

		AifParseTreeNode* pDecoratedParent = 0;
		auto_ptr<Aif2XtlSequenceEditInfo> pEditInfo(0);
		bool found = SearchForDecoratedParent( pEditInfo, node, pDecoratedParent );
		assert( found );
		assert( pEditInfo.get() );
		assert( pDecoratedParent );
		

		// Find the unmastered source clip that made the
		// original reference to this and recover the source
		// reference.
		auto_ptr<Aif2XtlUnMasteredSourceClipDecoration> pSrcClipDec(0);
		AifParseTreeNode* pDecoratedSrcClipParent = 0;
		SearchForDecoratedParent( pSrcClipDec, node, pDecoratedSrcClipParent );
		assert( pDecoratedSrcClipParent );
		assert( pSrcClipDec.get() );
		IAAFObjectSP spDecoratedNodeObj = pDecoratedSrcClipParent->GetAAFObject();
		IAAFSourceClipSP spSrcClip;
		AxQueryInterface( spDecoratedNodeObj, spSrcClip );
		AxSourceClip axSourceClip( spSrcClip );
		aafSourceRef_t srcRef = axSourceClip.GetSourceReference();

		// FIXME - Note the use of srcRef.startTime fetched
		// from the source clip that made the original
		// reference to this clip.  The edit info decoration
		// should to store this information.  That should be
		// cleaned up.

		Aif2XtlTimecode start( pEditInfo->GetStartPosition(), pEditInfo->GetEditRate() );
		Aif2XtlTimecode end( pEditInfo->GetEndPosition(), pEditInfo->GetEditRate() );
		Aif2XtlTimecode mstart( srcRef.startTime, pEditInfo->GetEditRate() );
		//Aif2XtlTimecode mend( pEditInfo->GetEndPosition() - pEditInfo->GetStartPosition(), pEditInfo->GetEditRate() );

		_os << src_l;
		_os << " start=\""  << start.GetXtlTimecodeString() << "\"";
		_os << " stop=\""   << end.GetXtlTimecodeString() << "\"";
		_os << " mstart=\"" << mstart.GetXtlTimecodeString() << "\"";
		_os << "/>" << endl;

		// src level decrement
		--_level;

		
		// FIXME - mend is only required if the source material must be temporally
		// resampled.  i.e. if stop-start=5 bug mstop-mstart=10 then the material
		// is output at twice as fast as normal.  This is possible to represent
		// in an AAF file but is not yet supported by this code.  When it is,
		// mstop must be used.
		// _os << " mstop=\""  << mend.GetXtlTimecodeString()   << "\"";

		// Check if there is a transition info object, and a
		// second sequence edit info object decorating this
		// node.  If so, then generate XTL for the transition.
		Aif2XtlTransitionInfo* pTransitionInfoUnowned = 0;
		if ( pDecoratedParent->IsDecorated( pTransitionInfoUnowned ) ) {
			auto_ptr<Aif2XtlTransitionInfo> pTransitionInfo(0);
			pDecoratedParent->PopDecoration( pTransitionInfo );

			auto_ptr<Aif2XtlTransitionSequenceEditInfo> pTEditInfo(0);
			pDecoratedParent->PopDecoration( pTEditInfo );

			Aif2XtlTimecode start( pTEditInfo->GetStartPosition(), pTEditInfo->GetEditRate() );
			Aif2XtlTimecode end( pTEditInfo->GetEndPosition(), pTEditInfo->GetEditRate() );

			_os << clip_l << "<transition ";
			_os << " start = \"" << start.GetXtlTimecodeString() << "\"";
			_os << " stop = \"" << end.GetXtlTimecodeString() << "\"";
			_os << " clsid=\"{" << uid2XtlStr( pTransitionInfo->GetEffectUID() ) << "}\">";
			_os << endl;

			// FIXME - Unusual level here... must clarify level processing.
			ParameterGenVisitor paramGen( _os, clip_l.Get()+3, GetLogStream() );
			pTransitionInfoUnowned->GetNode().Visit( &paramGen );

			_os << clip_l << "</transition>" << endl;
		}


		// clip level decrement
		_level--;
	}

}

void Aif2XtlXmlGenVisitor::PostOrderVisit( Aif2XtlSourceMob& node )
{
 	Level track_l(_level--);
	_os << track_l << "</track>" << endl;
}

/////

void Aif2XtlXmlGenVisitor::PreOrderVisit( Aif2XtlSourceClipNode& node )
{
	Level l(++_level);

}

void Aif2XtlXmlGenVisitor::PostOrderVisit( Aif2XtlSourceClipNode& node )
{
	Level l(--_level);
}

/////

void Aif2XtlXmlGenVisitor::PreOrderVisit( Aif2XtlWaveDescriptor& node )
{
	Level l(++_level);
}

void Aif2XtlXmlGenVisitor::PostOrderVisit( Aif2XtlWaveDescriptor& node )
{
	Level l(_level--);
}

/////

void Aif2XtlXmlGenVisitor::PreOrderVisit( Aif2XtlNetworkLocator& node )
{
	Level l(++_level);

#if 0
	Level l(++_level);

	AxNetworkLocator axLocator( node.GetAif2XtlAAFObject() );

	Aif2XtlSequenceEditInfo* pEditInfo = 0;
	bool found = SearchForDecoratedParent( pEditInfo, node );
	assert( found );
	assert( pEditInfo );

	_os << l << "src = \"" << axLocator.GetPath() << "\"";
	_os << " start = \"" << pEditInfo->GetStartPosition() << "\"";
	_os << " stop = \"" << pEditInfo->GetEndPosition() << "\"";
	_os << " mstart = \"" << pEditInfo->GetSegmentOffset() << "\"";
	_os << " mstop = \"" << pEditInfo->GetEndPosition() - pEditInfo->GetStartPosition() << "\"";
	_os << endl;
#endif
}

void Aif2XtlXmlGenVisitor::PostOrderVisit( Aif2XtlNetworkLocator& node )
{
	Level l(_level--);
}

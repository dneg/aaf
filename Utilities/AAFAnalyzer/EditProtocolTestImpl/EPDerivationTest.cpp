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
//=---------------------------------------------------------------------=

//Edit Protocol Test files
#include "EPDerivationTest.h"
#include "DerivationChainParser.h"

//Base Test files
#include <CompMobDependency.h>

//Test/Result files
#include <TestLevelTestResult.h>
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <Requirement.h>
#include <RequirementRegistry.h>

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//AAF Analyzer Base files
#include <AAFTypedObjNode.h>

//Analyzer Base files
#include <DepthFirstTraversal.h>
#include <TestGraph.h>

//Ax files
#include <AxMob.h>
#include <AxMetaDef.h>
#include <AxEssence.h>
#include <AxComponent.h>
#include <AxUtil.h>

//AAF files
#include <AAFResult.h>
#include <AAFClassDefUIDs.h>
#include <AAFExtEnum.h>

//STL files
#include <vector>
#include <deque>

#include <iostream>
#include <sstream>

// set to 1 to activate debug output
#define DEBUG_OUT 0

namespace {

using namespace aafanalyzer;

// JPT REVIEW - This should be reusable.
AxString Indent( int l )
{
  AxString indent;
  for (; l > 0 ; l-- ) 
  {
    indent += L"  ";
  }
  return indent;
}


const wchar_t* TEST_NAME = L"Mob Derivation Test";
const wchar_t* TEST_DESC = L"Verify the correctness of mob derivation chains.";

//======================================================================

// MobChainVisitor visits all mob types. It asserts the ordering
// conventions specified by the Edit Protocol.
//
// These conventions are implemented using the (state machine based)
// parser implemented by DerivationChainParser.
//
// It does this by building a stack of visited mobs. When the leaf
// object on a particular branch of the source tree is reached
// (normally this is a source clip object) the list of objects is
// parsed to determine if their ordering meets the EP spec.

class MobChainVisitor : public EPTypedVisitor
{
public:

  MobChainVisitor( wostream& log,
                   boost::shared_ptr<TestLevelTestResult> spTestResult )
    : EPTypedVisitor( Visitor::FOLLOW_REFERENCES ),
      _log( log ),
      _spTestResult( spTestResult ),
      _level( 0 ),
      _parseArgs(),
      _spLastPreVisitNode()
  {}

  virtual ~MobChainVisitor()
  {}

  //
  // Generic mob visit methods.
  //

  template <class IAAFType, class AxWrapperType, class EPType>
  bool PreOrderVisitMob( EPTypedObjNode<IAAFType, EPType>& node )
  {
    PreTrace( node, EPType::GetName() );
    AxWrapperType axMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axMob,EPType::GetName() );

    // If the mob has a usage code then we use it for the event id,
    // else, we use the class id (e.g. EPClip is a plain master mob
    // without a usage code).
    pair<bool,aafUID_t> usageCode = axMob.ExistsUsageCode();
    if ( usageCode.first )
    {
      ParseArgs parseArgs( usageCode.second, nodeName, node.GetSharedPointerToNode() );
      _parseArgs.push_back( parseArgs );
    }
    else
    {
      AxClassDef axClassDef( axMob.GetDefinition() );
      ParseArgs parseArgs( axClassDef.GetAUID(), nodeName, node.GetSharedPointerToNode() );
      _parseArgs.push_back( parseArgs );
    }

    _spLastPreVisitNode = node.GetSharedPointerToNode();

    return true;
  }

  template <class IAAFType, class AxWrapperType, class EPType>
  bool PostOrderVisitMob( EPTypedObjNode<IAAFType, EPType>& node )
  {
    ParseStackIfLeafNode( node );

    _parseArgs.pop_back();

    return true;
  }

  template <class EPType>
  bool PreOrderVisitSourceWithDescriptor( EPTypedObjNode<IAAFSourceMob, EPType>& node )
  {
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axSrcMob, EPType::GetName() );

    AxEssenceDescriptor descriptor( axSrcMob.GetEssenceDescriptor() );
    AxClassDef clsDef( descriptor.GetDefinition() );

    ParseArgs parseArgs( GetAcceptedAUID(clsDef), nodeName, node.GetSharedPointerToNode() );

    _parseArgs.push_back( parseArgs );

    _spLastPreVisitNode = node.GetSharedPointerToNode();

    return true;
  }

  template <class EPType>
  bool PostOrderVisitSourceWithDescriptor( EPTypedObjNode<IAAFSourceMob, EPType>& node )
  {
    ParseStackIfLeafNode( node );

    _parseArgs.pop_back();

    return true;
  }

  // 
  // Composition types
  // 

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
  {
    return PreOrderVisitMob<IAAFCompositionMob, AxCompositionMob, EPTopLevelComposition>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
  {
    return PostOrderVisitMob<IAAFCompositionMob, AxCompositionMob, EPTopLevelComposition>( node );
  }

  //========================================
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
  {
    return PreOrderVisitMob<IAAFCompositionMob, AxCompositionMob, EPLowerLevelComposition>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
  {
    return PostOrderVisitMob<IAAFCompositionMob, AxCompositionMob, EPLowerLevelComposition>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
  {
    return PreOrderVisitMob<IAAFCompositionMob, AxCompositionMob, EPSubClipComposition>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
  {
    return PostOrderVisitMob<IAAFCompositionMob, AxCompositionMob, EPSubClipComposition>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
  {
    return PreOrderVisitMob<IAAFCompositionMob, AxCompositionMob, EPAdjustedClipComposition>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
  {
    return PostOrderVisitMob<IAAFCompositionMob, AxCompositionMob, EPAdjustedClipComposition>( node );
  }

  //========================================
  //
  // MasterMob types
  
  virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
  {
    return PreOrderVisitMob<IAAFMasterMob, AxMasterMob, EPTemplateClip>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
  {
    return PostOrderVisitMob<IAAFMasterMob, AxMasterMob, EPTemplateClip>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
  {
    return PreOrderVisitMob<IAAFMasterMob, AxMasterMob, EPClip>( node );    
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
  {
    return PostOrderVisitMob<IAAFMasterMob, AxMasterMob, EPClip>( node );    
  }

  //========================================
  //
  // SourceMob types

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRGBAImageFileSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPRGBAImageFileSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRGBAImageFileSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPRGBAImageFileSource>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPCDCIImageFileSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPCDCIImageFileSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPCDCIImageFileSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPCDCIImageFileSource>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImageFileSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPImageFileSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImageFileSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPImageFileSource>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMonoAudioFileSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPMonoAudioFileSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMonoAudioFileSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPMonoAudioFileSource>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPMultiChannelAudioFileSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPMultiChannelAudioFileSource>( node );
  }

  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPFileSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPFileSource>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPRecordingSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPRecordingSource>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPImportSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPImportSource>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPTapeSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPTapeSource>( node );
  }
  
  //========================================

  virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
  {
    return PreOrderVisitSourceWithDescriptor<EPFilmSource>( node );
  }

  virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
  {
    return PostOrderVisitSourceWithDescriptor<EPFilmSource>( node );
  }

  //========================================
  //
  // Visit a source clip. We must visit this, in addition to the the
  // mobs, because the means by which the mob chain is terminated is
  // significant. In some places it can be terminated by either a zero
  // valued source clip or and out of file reference, in other places
  // only one of those is valid.

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
  {
    // The source clip was already resolved in
    // ResolveRefVisitor.cpp. A kind was assigned at that time to let
    // us know where it is an end of chain, out of file, or in file
    // reference.


    if ( node.GetKind() == AAFTypedObjNode<IAAFSourceClip>::AAFNODE_KIND_OUT_OF_FILE_REF )
    {
      ParseArgs parseArgs( DerivationChainParser::OOF_EVENT_ID, L"Out Of File Reference", node.GetSharedPointerToNode() );
      _parseArgs.push_back( parseArgs );
      _spLastPreVisitNode = node.GetSharedPointerToNode();
    }
    else if ( node.GetKind() == AAFTypedObjNode<IAAFSourceClip>::AAFNODE_KIND_END_OF_CHAIN_REF )
    {
      ParseArgs parseArgs( DerivationChainParser::EOC_EVENT_ID, L"End Of Chain", node.GetSharedPointerToNode() );
      _parseArgs.push_back( parseArgs );
      _spLastPreVisitNode = node.GetSharedPointerToNode();
    }

    // Else, it is a resolved references and we don't care about it,
    // or it is an undefined reference in which case we also don't
    // care (and the derivation chain parser will detect the
    // mal-terminated chain).

    return true;
  }

  virtual bool PostOrderVisit( AAFTypedObjNode<IAAFSourceClip>& node )
  {
    ParseStackIfLeafNode( node );

    // This condition must match the pre order visit method conditions
    // that caused a ParseArgs to be pushed onto the _parseArgs stack.
    if ( node.GetKind() == AAFTypedObjNode<IAAFSourceClip>::AAFNODE_KIND_OUT_OF_FILE_REF  ||
         node.GetKind() == AAFTypedObjNode<IAAFSourceClip>::AAFNODE_KIND_END_OF_CHAIN_REF )
    {
      _parseArgs.pop_back();
    }

    return true;
  }
 
  //========================================

  // 
  // Unspecialized mob types.
  //
  //
  // The following "bare" node types should not be encoutered in a
  // correctly constructed derivation chain. These indicate a mob was
  // used that does not have a usage code, or a decriptor, that could
  // be interpreted to create a more speciallized EP node type.
  //
  // They generate a failure and stop the traversal. Since we don't
  // put any parse args on the stack we don't need to work about a
  // post order visit method.

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node )
  {
    PreTrace(node, L"composition");
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    AxString nodeName = this->GetMobName( axCompMob, L"Composition Mob" ); 
    REQ_EP_258_Failure( L"Composition Mob", axCompMob, node );
    return false;
  }
  
  //========================================

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node )
  {
    PreTrace(node, L"master" );
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    REQ_EP_258_Failure( L"Master Mob", axMastMob, node );
    return false;
  }

  //========================================

  virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node )
  {
    PreTrace(node, L"source" );
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    REQ_EP_258_Failure( L"Source Mob", axSrcMob, node );
    return false;
  }
  
private:

  // prohibited
  MobChainVisitor();
  MobChainVisitor( const MobChainVisitor& );
  MobChainVisitor& operator=( const MobChainVisitor& );

  void ParseStackIfLeafNode( Node& node )
  {
    // This function is called from post order visit methods.  If the
    // last node that was pre order visited is that same as this node
    // then this node is a leaf node.
    if ( node.GetSharedPointerToNode() == _spLastPreVisitNode )
    {
      DerivationChainParser parser( _log, _spTestResult );

      deque<ParseArgs>::const_iterator iter;
      for ( iter  = _parseArgs.begin();
            iter != _parseArgs.end();
	    ++iter )
      {
	if ( !parser.Transition( iter->event, iter->nodeName, *iter->spNode ) )
	{
	  break;
	}
      }

      // If we parsed all tokens then as a final step we need to check
      // the parser was left in a valid end state.
      if ( iter == _parseArgs.end() )
      {
	parser.CheckEndState( *_parseArgs.back().spNode );
      }
    }
  }

  // Return a AUID that is in the state machines Event map.  If the
  // clsDef is not in the map, recursively try the parent class id.
  aafUID_t GetAcceptedAUID( AxClassDef& clsDef ) const
  {
    aafUID_t auid = clsDef.GetAUID();
    
    if ( DerivationChainParser::IsKnownEvent( auid ) )
    {
        return auid;
    }

    AxClassDef parentDef( clsDef.GetParent() );
    return GetAcceptedAUID( parentDef );
  }
  
  void REQ_EP_258_Failure( const wstring& mobTypeName,
			   AxMob& mob,
			   Node& node )
  {
    wstring explain = mobTypeName + L" named \"" + mob.GetName(L"<unnamed>")
                    + L"\" is out of place in the derrivation chain.";    

    boost::shared_ptr<DetailLevelTestResult>
      spFailure = _spTestResult->AddSingleResult( L"REQ_EP_258",
                                                  explain,
                                                  TestResult::FAIL,
						  node );
    spFailure->AddDetail( L"Does not have an Edit Protocol material type." );
  }

  void PreTrace( const AAFObjNode& node, const wstring& what )
  {
    _level++;

#if DEBUG_OUT
    wcout << L"Pre:  " << Indent(_level) << node.GetLID() << L" " << node.GetName();

    IAAFMobSP spMob;
    if ( AxIsA<IAAFMob>( AxObject(node.GetAAFObject()), spMob ) )
    {
      AxMob axMob(spMob);
      //wcout << L", Name = " << axMob.GetName() << L", " << AxStringUtil::mobid2Str(axMob.GetMobID());
      wcout << L", Name = " << axMob.GetName();
    }
    wcout << L", " << what << L" (" << _level << L")" << endl;
#endif
  }

  void PostTrace( const AAFObjNode& node )
  {
#if DEBUG_OUT
    wcout << L"Post: " << Indent(_level) << node.GetLID() << L" " << node.GetName()
          << L" (" << _level << L")" << endl;
#endif
    _level--;
  }

  wostream& _log;
  boost::shared_ptr<TestLevelTestResult> _spTestResult;

  unsigned int _level;
  
  struct ParseArgs
  {
    ParseArgs( DerivationChainParser::EventID e,
	       AxString n,
	       boost::shared_ptr<Node> sp )
      : event(e),
	nodeName(n),
	spNode(sp)
    {}

    DerivationChainParser::EventID event;
    AxString nodeName;
    boost::shared_ptr<Node> spNode;
  };
  
  deque<ParseArgs> _parseArgs;
  boost::shared_ptr<Node> _spLastPreVisitNode;
};

//======================================================================

void AnalyzeMobChain( wostream& log,
                      boost::shared_ptr<const TestGraph> spGraph,
                      CompMobDependency::CompMobNodeSP spRootComposition,
                      boost::shared_ptr<TestLevelTestResult> spTestResult )
{
  AxCompositionMob axCompMob( spRootComposition->GetAAFObjectOfType() );
  AxString mobName = axCompMob.GetName( L"<unnamed>" );

  spTestResult->AddDetail( L"Analyzing root composition mob \"" + mobName + L"\"" );

  DepthFirstTraversal dft( spGraph->GetEdgeMap(),
                           spRootComposition );

  boost::shared_ptr<MobChainVisitor>  spVisitor( new MobChainVisitor(log, spTestResult) );
  dft.TraverseDown( spVisitor );
}

//======================================================================

class DerivationChainAnalyzer
{
public:
  DerivationChainAnalyzer( wostream& log,
			   boost::shared_ptr<const TestGraph> spGraph,
			   boost::shared_ptr<TestLevelTestResult> spTestResult )
    : _log( log ),
      _spGraph( spGraph ),
      _spTestResult( spTestResult )
  {}

  void operator () ( const CompMobDependency::CompMobNodeSP& spRootComposition )
  {
    AnalyzeMobChain( _log, _spGraph, spRootComposition, _spTestResult );
  }

private:

  // prohibited
  DerivationChainAnalyzer();
  DerivationChainAnalyzer& operator=( const DerivationChainAnalyzer& );

  wostream& _log;
  boost::shared_ptr<const TestGraph> _spGraph;
  boost::shared_ptr<TestLevelTestResult> _spTestResult;
};

//======================================================================

class VerifyNotTopLevel
{
public:
  VerifyNotTopLevel( wostream& log,
		     boost::shared_ptr<const TestGraph> spGraph,
		     boost::shared_ptr<TestLevelTestResult> spTestResult )
    : _log( log ),
      _spGraph( spGraph ),
      _spTestResult( spTestResult ),
      _fail(false)
  {}

  void operator () ( const CompMobDependency::CompMobNodeSP& spCompositionNode )
  {
    AxCompositionMob axCompMob( spCompositionNode->GetAAFObjectOfType() );

    pair<bool,aafUID_t> usageCode = axCompMob.ExistsUsageCode();

    if ( usageCode.first && usageCode.second == kAAFUsage_TopLevel )
    {
      AxString mobName = axCompMob.GetName( L"<unnamed>" );
      wstringstream ss;
      ss << "Top level CompositionMob \"" << mobName << L"\" is referenced by one, or more, Mob objects in the file.";
      _spTestResult->AddSingleResult( L"REQ_EP_025.2",
				      ss.str(),
				      TestResult::FAIL,
				      *spCompositionNode );
      _fail = true;
    }
  }

  void AddFinalResult()
  {
    if ( !_fail )
    {
      _spTestResult->AddUnassociatedSingleResult( L"REQ_EP_025.2",
						  L"No top level CompositionMob objects are referenced by other Mob objects in the file.",
						  TestResult::PASS );
    }
  }

private:

  // prohibited
  VerifyNotTopLevel();
  VerifyNotTopLevel& operator=( const VerifyNotTopLevel& );

  wostream& _log;
  boost::shared_ptr<const TestGraph> _spGraph;
  boost::shared_ptr<TestLevelTestResult> _spTestResult;
  bool _fail;
};


//======================================================================

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace std;

EPDerivationTest::EPDerivationTest( wostream& log,
                                    boost::shared_ptr<const TestGraph> spGraph,
                                    boost::shared_ptr<CompMobDependency> spDepTest )
  : Test( log, GetTestInfo() ),
    _spDepTest( spDepTest )
{
    SetTestGraph(spGraph);
}

EPDerivationTest::~EPDerivationTest()
{}

boost::shared_ptr<TestLevelTestResult> EPDerivationTest::Execute()
{
  boost::shared_ptr<TestLevelTestResult> spTestResult = CreateTestResult();

  //
  // Verify non root mobs are not top level.
  //
  CompMobDependency::CompMobNodeVectorSP spNonRootCompMobs = _spDepTest->GetNonRootCompMobNodes();
  VerifyNotTopLevel verifyNotTopLevel = for_each( spNonRootCompMobs->begin(), spNonRootCompMobs->end(),
						  VerifyNotTopLevel(GetOutStream(), GetTestGraph(), spTestResult) );
  verifyNotTopLevel.AddFinalResult();

  //
  // Analyzer derivation chains.
  //

  CompMobDependency::CompMobNodeVectorSP spRootCompMobs = _spDepTest->GetRootCompMobNodes();
  for_each( spRootCompMobs->begin(), spRootCompMobs->end(),
	    DerivationChainAnalyzer( GetOutStream(), GetTestGraph(), spTestResult ) );

  return spTestResult;
}

AxString EPDerivationTest::GetName() const
{
  return TEST_NAME;
}

AxString EPDerivationTest::GetDescription() const
{
  return TEST_DESC;
}

const TestInfo EPDerivationTest::GetTestInfo()
{
    boost::shared_ptr<vector<AxString> > spReqIds(new vector<AxString>);

    // * - Means the the requirement was reviewed and is being used correctly in this test.
    // (JPT - 21 Sept 2007).

    spReqIds->push_back(L"REQ_EP_017");     // * Derivation Chain ends with zero-value source clip.
    spReqIds->push_back(L"REQ_EP_026");     // * Valid transitions from Top-Level.
    spReqIds->push_back(L"REQ_EP_031");     // * Valid transitions from Lower-Level.
    spReqIds->push_back(L"REQ_EP_036");     // * Valid transitions from Sub-Clip.
    spReqIds->push_back(L"REQ_EP_045");     // * Valid transitions from Adjusted Clip.
    spReqIds->push_back(L"REQ_EP_050");     // * Template Clip must end chain.
    spReqIds->push_back(L"REQ_EP_055");     // * Valid transitions from Clip.
    spReqIds->push_back(L"REQ_EP_063");     // * Valid transitions from File Source.
    spReqIds->push_back(L"REQ_EP_072");     // * Recording source must end chain.
    spReqIds->push_back(L"REQ_EP_076");     // * Valid Transitions from Import Source.
    spReqIds->push_back(L"REQ_EP_083");     // * Valid Transitions from Tape Source.
    spReqIds->push_back(L"REQ_EP_258");     // * All Mob objects shall have a have a known Edit Protocol material type.
    spReqIds->push_back(L"REQ_EP_259");     // * Film source shall not reference anything.
    spReqIds->push_back(L"REQ_EP_260");     // * Top-level is first object in derivation chain.
    spReqIds->push_back(L"REQ_EP_261");     // * Unresolved references in derivation chain.
    spReqIds->push_back(L"REQ_EP_262");     // * Valid Derivation Chain Termination

    // REQ_EP_025.2 is unrelated to the derivation chain parsing but
    // this is a logical place to test it.
    spReqIds->push_back(L"REQ_EP_025.2");   // * Top-level composition shall not be referenced by another Mob in the AAF file.

    // TODO: Push an item for valid transitions from Film Source.
    return TestInfo(L"EPDerivationTest", spReqIds);
}

} // end of namespace aafanalyzer

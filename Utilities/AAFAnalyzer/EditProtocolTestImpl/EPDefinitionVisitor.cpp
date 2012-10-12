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
#include "EPDefinitionVisitor.h"
#include "EPDefinitionTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestRegistry.h>

//Analyzer Base files
#include <EdgeMap.h>

//Ax files
#include <AxDefObject.h>
#include <AxComponent.h>
#include <AxHeader.h>

//AAF files
#include <AAFDataDefs.h>

//STL files
#include <sstream>

namespace {
    
using namespace aafanalyzer;

} // end of namespace

//======================================================================

namespace aafanalyzer {
 
EPDefinitionVisitor::EPDefinitionVisitor( wostream& log,
					  boost::shared_ptr<EdgeMap> spEdgeMap,
					  boost::shared_ptr<TestLevelTestResult> spTestResult )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spTestResult( spTestResult )
{}
    
EPDefinitionVisitor::~EPDefinitionVisitor()
{}

bool EPDefinitionVisitor::PreOrderVisit( EPTypedObjNode<IAAFOperationDef, EPEffect>& node )
{
    AxOperationDef axOpDef( node.GetAAFObjectOfType() );
    aafUID_t identification = axOpDef.GetAUID();
    AxDataDef axDataDef( axOpDef.GetDataDef() );
    aafUID_t dataDef = axDataDef.GetAUID();
    
    AxString msg = L"OperationDefinition \"" + 
                   axOpDef.GetName() +
                   L"\" has DataDefinition = DataDef_";
    
    if ( dataDef == kAAFDataDef_Picture || dataDef == kAAFDataDef_Sound )
    {
        _opDataDefCurrent[msg + axDataDef.GetName()] = node.GetSharedPointerToNode();
        
    }
    else if ( dataDef == kAAFDataDef_LegacyPicture || dataDef == kAAFDataDef_LegacySound )
    {
        _opDataDefLegacy[msg + L"Legacy" + axDataDef.GetName()] = node.GetSharedPointerToNode();
    }
    
    boost::shared_ptr<AAFTypedObjNode<IAAFOperationDef> > spGeneric( node.DownCastToAAF<IAAFOperationDef>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPDefinitionVisitor::PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node )
{
    AxOperationDef axOpDef( node.GetAAFObjectOfType() );
    _registeredDefinitions[axOpDef.GetAUID()] = make_pair( axOpDef.GetName(), node.GetSharedPointerToNode() );
    return false;
}

bool EPDefinitionVisitor::PreOrderVisit( AAFTypedObjNode<IAAFHeader>& node )
{
    AxHeader axHeader( node.GetAAFObjectOfType() );
    _fileVersion = axHeader.GetRefImplVersion();
    return true;
}

bool EPDefinitionVisitor::PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node )
{
    AxOperationGroup axOpGroup( node.GetAAFObjectOfType() );
    AxDataDef axOpGroupDDef( axOpGroup.GetDataDef() );

    AxOperationDef axOpDef( axOpGroup.GetOperationDef() );
    _usedDefinitions.insert( axOpDef.GetAUID() );
    AxDataDef axOpDefDDef( axOpDef.GetDataDef() );

    if ( !axOpGroupDDef.IsDataDefOf( axOpDefDDef ) )
    {
        aafUID_t auid = axOpGroupDDef.GetAUID();
        AxString groupLegacy = L"";
        if ( auid == kAAFDataDef_LegacyPicture ||
             auid == kAAFDataDef_LegacySound ||
             auid == kAAFDataDef_LegacyTimecode )
        {
            groupLegacy = L"Legacy ";
        }

        auid = axOpDefDDef.GetAUID();
        AxString defLegacy = L"";
        if ( auid == kAAFDataDef_LegacyPicture ||
             auid == kAAFDataDef_LegacySound ||
             auid == kAAFDataDef_LegacyTimecode )
        {
            defLegacy = L"Legacy ";
        }        
        
        _spTestResult->AddSingleResult( 
            L"REQ_EP_162", 
            L"OperationGroup in " + this->GetMobSlotName( _spEdgeMap, node ) + 
                L" has data definition value \"" + groupLegacy + axOpGroupDDef.GetName() + 
                L"\" and references OperationDefinition \"" + axOpDef.GetName() +
                L"\" with data definition value \"" + defLegacy + axOpDefDDef.GetName() + L"\".", 
            TestResult::FAIL,
	    node );
    }

    //Continue checking even if this node failed.  Need to record AUIDs of
    //data definitions in use.    
    return true;
}

void EPDefinitionVisitor::CheckForUnusedOperationDefinitions()
{
    //Remove all used OperationDefinitions from the registered set.
    set<aafUID_t>::const_iterator sIter;
    for ( sIter = _usedDefinitions.begin(); sIter != _usedDefinitions.end(); sIter++ )
    {
        _registeredDefinitions.erase( *sIter );
    }
    
    //Present a info message for every unused OperationDefinition.
    map<aafUID_t, pair<AxString,boost::shared_ptr<Node> > >::const_iterator mIter;
    for ( mIter = _registeredDefinitions.begin(); mIter != _registeredDefinitions.end(); mIter++ )
    {
      _spTestResult->AddSingleResult(
         L"REQ_EP_162",
	 L"OperationDefinition \"" + mIter->second.first + L"\" is not referenced.",
	 TestResult::INFO,
	 *(mIter->second.second) );
    }
}

bool EPDefinitionVisitor::IsPre11()
{
  if ( _fileVersion.major > 1 || ( _fileVersion.major == 1 && _fileVersion.minor >= 1 ) )
  {
    return false;
  }
  else
  {
    return true;
  }
}

void EPDefinitionVisitor::CheckLegacyData()
{
  // A file version greater than 1.1 should not have legacy data
  // definitions.  Warn if legacy values are found.  Pass if current
  // values are found.  The reverse is true for pre 11 files.

  map<AxString, boost::shared_ptr<Node> >::const_iterator iter;
  for ( iter = _opDataDefCurrent.begin(); iter != _opDataDefCurrent.end(); iter++ )
  {
    TestResult::Result result = TestResult::UNDEFINED;

    if ( IsPre11() )
    {
      result = TestResult::WARN;
    }
    else
    {
      result = TestResult::PASS;
    }

    wstringstream ss;
    ss << iter->first << L" in an AAF Version " << _fileVersion.major
       << L"." << _fileVersion.minor << L" file.";

    _spTestResult->AddSingleResult( L"REQ_EP_163", ss.str().c_str(), result, *iter->second );
  }

  for ( iter = _opDataDefLegacy.begin(); iter != _opDataDefLegacy.end(); iter++ )
  {
    TestResult::Result result = TestResult::UNDEFINED;

    if ( IsPre11() )
    {
      result = TestResult::PASS;
    }
    else
    {
      result = TestResult::WARN;
    }

    wstringstream ss;
    ss << iter->first << L" in an AAF Version " << _fileVersion.major
       << L"." << _fileVersion.minor << L" file.";

    _spTestResult->AddSingleResult( L"REQ_EP_163", ss.str().c_str(), result, *iter->second );
  }

}

} // end of namespace aafanalyzer

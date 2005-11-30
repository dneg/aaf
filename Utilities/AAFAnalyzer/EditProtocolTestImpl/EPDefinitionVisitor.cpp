//=---------------------------------------------------------------------=
//
// $Id$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
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

namespace {
    
using namespace aafanalyzer;

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;
 
EPDefinitionVisitor::EPDefinitionVisitor( wostream& log, shared_ptr<EdgeMap> spEdgeMap )
    : _log(log),
      _spEdgeMap( spEdgeMap ),
      _spResult( new DetailLevelTestResult( L"Edit Protocol Definition Visitor",
                                            L"Visit Data Definitions and Components to make sure all Data Definitions are valid.",
                                            L"", // explain
                                            L"", // DOCREF REQUIRED
                                            TestResult::PASS,
                                            TestRegistry::GetInstance().GetRequirementsForTest( EPDefinitionTest::GetTestInfo().GetName() )
               )                          )
{}
    
EPDefinitionVisitor::~EPDefinitionVisitor()
{}

bool EPDefinitionVisitor::PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node )
{
    AxOperationDef axOpDef( node.GetAAFObjectOfType() );
    _registeredDefinitions[axOpDef.GetAUID()] = axOpDef.GetName();
    return false;
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
        _spResult->AddInformationResult( 
            L"REQ_EP_162", 
            L"OperationGroup in " + this->GetMobSlotName( _spEdgeMap, node ) + 
                L" has data definition value \"" + axOpGroupDDef.GetName() + 
                L"\" and references OperationDefinition \"" + axOpDef.GetName() +
                L"\" with data definition value \"" + axOpDefDDef.GetName() + L"\".", 
            TestResult::FAIL );
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
    
    //Present a warning for every unused OperationDefinition.
    map<aafUID_t, AxString>::const_iterator mIter;
    for ( mIter = _registeredDefinitions.begin(); mIter != _registeredDefinitions.end(); mIter++ )
    {
        _spResult->AddInformationResult(
            L"REQ_EP_162",
            L"OperationDefinition \"" + mIter->second + L"\" is not referenced.",
            TestResult::WARN );
    }
    
}

shared_ptr<DetailLevelTestResult> EPDefinitionVisitor::GetResult()
{
    return _spResult;
}

} // end of namespace aafanalyzer

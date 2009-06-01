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
#include "EPNameVisitor.h"
#include "EPNameTest.h"

//Test/Result files
#include <DetailLevelTestResult.h>
#include <TestLevelTestResult.h>
#include <TestRegistry.h>

//Requirement files
#include <RequirementRegistry.h>
#include <Requirement.h>

//Ax files
#include <AxUtil.h>
#include <AxMobSlot.h>
#include <AxDefObject.h>
#include <AxIterator.h>

//AAF files
#include <AAFResult.h>

//STL files
#include <sstream>

namespace {

} // end of namespace

//======================================================================

namespace aafanalyzer {

using namespace boost;

EPNameVisitor::EPNameVisitor( wostream& log,
                              shared_ptr<EdgeMap> spEdgeMap,
                              shared_ptr<TestLevelTestResult> spTestResult )
    : EPTypedVisitor(),
      _log( log ),
      _spEdgeMap( spEdgeMap ),
      _spTestResult( spTestResult )
{}

EPNameVisitor::~EPNameVisitor()
{}

/*
 * Mob Visitors
 */
bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axCompMob );
    if ( !this->VisitComposition( node, EPTopLevelComposition::GetName(), L"REQ_EP_027", axCompMob ) )
    {
        return false;
    }
    _topLevelNames.insert( axCompMob.GetName() );
    return true;
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axCompMob );
    if ( !this->VisitComposition( node, EPLowerLevelComposition::GetName(), L"REQ_EP_032", axCompMob ) )
    {
        return false;
    }
    _lowerLevelNames.insert( axCompMob.GetName() );
    return true;
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axCompMob );
    return this->VisitComposition( node, EPSubClipComposition::GetName(), L"REQ_EP_038", axCompMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node )
{
    AxCompositionMob axCompMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axCompMob );
    return this->VisitComposition( node, EPAdjustedClipComposition::GetName(), L"REQ_EP_047", axCompMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node )
{
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axMastMob );
    return this->VisitNonComposition( node, EPTemplateClip::GetName(), L"REQ_EP_052", axMastMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node )
{
    AxMasterMob axMastMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axMastMob );
    return this->VisitNonComposition( node, EPClip::GetName(), L"REQ_EP_057", axMastMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axSrcMob );
    return this->VisitNonComposition( node, EPRecordingSource::GetName(), L"REQ_EP_073", axSrcMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axSrcMob );
    return this->VisitNonComposition( node, EPTapeSource::GetName(), L"REQ_EP_081", axSrcMob );
}

bool EPNameVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node )
{
    AxSourceMob axSrcMob( node.GetAAFObjectOfType() );
    CheckForUniqueSlotNamesInMob( node, axSrcMob );
    return this->VisitNonComposition( node, EPFilmSource::GetName(), L"REQ_EP_086", axSrcMob );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node)
{
  AxCompositionMob axMob( node.GetAAFObjectOfType() );
  CheckForUniqueSlotNamesInMob( node, axMob );
  return false;
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node)
{
  AxMasterMob axMob( node.GetAAFObjectOfType() );
  CheckForUniqueSlotNamesInMob( node, axMob );
  return false;
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node)
{
  AxSourceMob axMob( node.GetAAFObjectOfType() );
  CheckForUniqueSlotNamesInMob( node, axMob );
  return false;
}


void EPNameVisitor::CheckForUniqueSlotNamesInMob( Node& node, IAAFMobSP spMob )
{
  set<AxString> slotNameSet;
  AxMob axMob( spMob );
  AxMobSlotIter axSlotIter( axMob.GetSlots() );
  IAAFSmartPointer2<IAAFMobSlot> spNextSlot;
  while( axSlotIter.NextOne( spNextSlot ) )
  {
    AxMobSlot axMobSlot( spNextSlot );
    pair<bool,AxString> slotName = axMobSlot.ExistsName();

    // If it does have a name...
    if ( !slotName.first )
    {
      AxString mobName = axMob.GetName( L"<unnamed>" );
      aafSlotID_t slotId = axMobSlot.GetSlotID();
      wstringstream ss;
      ss << L"MobSlot with ID = " << slotId << L" in " << axMob.GetClassName() << L" named \""
	 << mobName << L"\" does not have a valid name.";
      _spTestResult->AddSingleResult( L"REQ_EP_101",
				      ss.str(),
				      TestResult::FAIL,
				      node );
      continue;
    }
    else
    {
      // Else, it does have name. Make sure that name is unique.

      if ( slotNameSet.find(slotName.second) != slotNameSet.end() )
      {
	// It's already in the set therefore is not unique within the mob.
	AxString mobName = axMob.GetName( L"<unnamed>" );
	aafSlotID_t slotId = axMobSlot.GetSlotID();
	wstringstream ss;
	ss << L"MobSlot with ID = " << slotId << L" named \"" << slotName.second
           << "\" in " << axMob.GetClassName() << L" named \"" << mobName
	   << L"\" is not uniquely named within the mob.";
	_spTestResult->AddSingleResult( L"REQ_EP_101",
					ss.str(),
					TestResult::FAIL,
					node );
	continue;
      }
      else
      {
	slotNameSet.insert(slotName.second);
      }
    }
  } // end while
}

/*
 * Definition Object Visitors
 */
bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFDataDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFParameterDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFPluginDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFContainerDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFInterpolationDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFTaggedValueDefinition>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFCodecDef>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}

bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFKLVDataDefinition>& node )
{
    shared_ptr<AAFTypedObjNode<IAAFDefObject> > spGeneric( node.DownCastToAAF<IAAFDefObject>() );
    return this->PreOrderVisit( *spGeneric );
}
 
bool EPNameVisitor::PreOrderVisit( AAFTypedObjNode<IAAFDefObject>& node )
{
    
    AxDefObject axDefObj( node.GetAAFObjectOfType() );
    
    try
    {
        axDefObj.GetName();
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {       
            AxString explain( L"DefinitionObject does not have a valid name." );

            shared_ptr<DetailLevelTestResult>
              spFailure = _spTestResult->AddSingleResult( L"REQ_EP_161",
							  explain,
							  TestResult::FAIL,
							  node );

            spFailure->AddDetail( L"AUID: " + AxStringUtil::uid2Str( axDefObj.GetAUID() ) );
        
            return false;
        }
        else
        {
            throw ex;
        }
    }
    return true;
}

bool EPNameVisitor::VisitComposition( Node& node,
				      const AxString& type,
				      const AxString& reqId, AxCompositionMob& axCompMob )
{
    AxString nodeName;

    try
    {
        nodeName = axCompMob.GetName();
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {
            AxString explain( type + L" does not have a valid name." );

            shared_ptr<DetailLevelTestResult>
              spFailure = _spTestResult->AddSingleResult( reqId,
							  explain,
							  TestResult::FAIL,
							  node );
            
            spFailure->AddDetail( L"Mob ID: " + AxStringUtil::mobid2Str( axCompMob.GetMobID() ) );
            
            return false;
        }
        else
        {
            throw ex;
        }
    }
    
    if ( _compositionNames.find( nodeName ) != _compositionNames.end() )
    {
        _compositionNames[nodeName] = _compositionNames[nodeName] + 1;
    }
    else
    {
        _compositionNames[nodeName] = 1;
    }
    
    return true;
    
}

bool EPNameVisitor::VisitNonComposition( Node& node, const AxString& type, const AxString& reqId, AxMob& axMob )
{
    AxString nodeName;

    try
    {
        nodeName = axMob.GetName();
    }
    catch ( const AxExHResult& ex )
    {
        if ( ex.getHResult() == AAFRESULT_PROP_NOT_PRESENT )
        {
          AxString explain( type + L" does not have a valid name." );

          shared_ptr<DetailLevelTestResult>
            spFailure = _spTestResult->AddSingleResult( reqId,
							explain,
							TestResult::FAIL,
							node );

            spFailure->AddDetail( L"Mob ID: " + AxStringUtil::mobid2Str( axMob.GetMobID() ) );

            return false;
        }
        else
        {
            throw ex;
        }
    }
    
    return true;
}

void EPNameVisitor::CheckForUniqueNames()
{
    //Check composition names for uniqueness
    this->CheckForUniqueNames( _topLevelNames, L"REQ_EP_027", EPTopLevelComposition::GetName() );
    this->CheckForUniqueNames( _lowerLevelNames, L"REQ_EP_032", EPLowerLevelComposition::GetName() );
}

void EPNameVisitor::CheckForUniqueNames( NameSet& names, const AxString& reqId, const AxString& type )
{
    NameSet::const_iterator iter;
    for ( iter = names.begin(); iter != names.end(); iter++ )
    {
        if ( _compositionNames[*iter] != 1 )
        {
            AxString explain( type + L" \"" + *iter + L"\" does not have a unique composition name." );
            _spTestResult->AddUnassociatedSingleResult( reqId, explain, TestResult::FAIL );
        }
    }
}

} // end of namespace aafanalyzer

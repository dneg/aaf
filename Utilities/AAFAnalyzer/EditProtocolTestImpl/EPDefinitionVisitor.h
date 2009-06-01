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

#ifndef __EPDefinitionVisitor_h_
#define __EPDefinitionVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//Ax files
#include <AxTypes.h>

//STL files
#include <set>
#include <map>

//Not in the aafanalyzer namespace so the forward declaration must be made
//outside of the namespace.
class AxMob;
class AxMobSlot;

namespace aafanalyzer {

using namespace boost;

class TestLevelTestResult;

class EPDefinitionVisitor : public EPTypedVisitor
{

  public:
  
    EPDefinitionVisitor( wostream& log,
			 shared_ptr<EdgeMap> spEdgeMap,
			 shared_ptr<TestLevelTestResult> spTestResult );
    
    virtual ~EPDefinitionVisitor();
   
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFOperationDef, EPEffect>& node );

    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFHeader>& node );
    
    void CheckForUnusedOperationDefinitions();
    void CheckLegacyData();
      
  private:
   
    // prohibited
    EPDefinitionVisitor();
    EPDefinitionVisitor( const EPDefinitionVisitor& );
    EPDefinitionVisitor& operator=( const EPDefinitionVisitor& );

    bool IsPre11();

    wostream& _log;
    shared_ptr<EdgeMap> _spEdgeMap;
    shared_ptr<TestLevelTestResult> _spTestResult;
    set<aafUID_t> _usedDefinitions;
    map<aafUID_t, pair<AxString,shared_ptr<Node> > > _registeredDefinitions;
    map<AxString, shared_ptr<Node> > _opDataDefCurrent;
    map<AxString, shared_ptr<Node> > _opDataDefLegacy;
    aafProductVersion_t _fileVersion;

  
};

} // end of namespace diskstream

#endif /*__EPDefinitionVisitor_h_*/

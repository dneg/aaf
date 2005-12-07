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

class DetailLevelTestResult;

class EPDefinitionVisitor : public EPTypedVisitor
{

  public:
  
    EPDefinitionVisitor( wostream& log, shared_ptr<EdgeMap> spEdgeMap );
    
    virtual ~EPDefinitionVisitor();
   
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFOperationDef, EPEffect>& node );

    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationGroup>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFHeader>& node );
    
    void CheckForUnusedOperationDefinitions();
    void CheckLegacyData();
    
    shared_ptr<DetailLevelTestResult> GetResult();
    
  private:
   
    wostream& _log;
    shared_ptr<EdgeMap> _spEdgeMap;
    shared_ptr<DetailLevelTestResult> _spResult;
    set<aafUID_t> _usedDefinitions;
    map<aafUID_t, AxString> _registeredDefinitions;
    set<AxString> _opDataDefCurrent;
    set<AxString> _opDataDefLegacy;
    aafProductVersion_t _fileVersion;

    // prohibited
    EPDefinitionVisitor();
    EPDefinitionVisitor( const EPDefinitionVisitor& );
    EPDefinitionVisitor& operator=( const EPDefinitionVisitor& );
  
};

} // end of namespace diskstream

#endif /*__EPDefinitionVisitor_h_*/

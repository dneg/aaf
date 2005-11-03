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

#ifndef __EPEditRateVisitor_h_
#define __EPEditRateVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <AAFTypedObjNode.h>
#include "AllowedEditRateTable.h"

//Test/Result files
#include <TestResult.h>

//Ax files
#include <AxTypes.h>

//STL files
#include <set>

//Not in the aafanalyzer namespace so the forward declaration must be made
//outside of the namespace.
class AxMobSlot;
class AxClassDef;
class AxDataDef;

namespace aafanalyzer {

using namespace boost;

class DetailLevelTestResult;

class EPEditRateVisitor : public EPTypedVisitor
{

  public:

    EPEditRateVisitor( wostream& log );
    
    virtual ~EPEditRateVisitor();

    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node );
       
    virtual bool EdgeVisit(AAFComponentReference& edge);
    virtual bool EdgeVisit(AAFSlotReference& edge);
    
    shared_ptr<DetailLevelTestResult> GetResult();
    
  private:
   
    wostream& _log;
    shared_ptr<DetailLevelTestResult> _spResult;
    AllowedEditRateTable _erTable;

    bool VisitMob( AxMob& axMob );
    bool TestEditRate( aafRational_t editRate, AxDataDef& axDataDef, const AxString& mobName, aafSlotID_t slotId );

    // prohibited
    EPEditRateVisitor();
    EPEditRateVisitor( const EPEditRateVisitor& );
    EPEditRateVisitor& operator=( const EPEditRateVisitor& );
  
};

} // end of namespace diskstream

#endif /*__EPEditRateVisitor_h_*/

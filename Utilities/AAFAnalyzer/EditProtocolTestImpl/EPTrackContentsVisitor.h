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

#ifndef __EPTrackContentsVisitor_h_
#define __EPTrackContentsVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//Ax files
#include <AxTypes.h>

//AAF files
#include <AAFClassDefUIDs.h>

//STL files
#include <set>

//Not in the aafanalyzer namespace so the forward declaration must be made
//outside of the namespace.
class AxClassDef;
class AxMob;
class AxMobSlot;

namespace aafanalyzer {

using namespace boost;

class DetailLevelTestResult;

class EPTrackContentsVisitor : public EPTypedVisitor
{

  public:
  
    EPTrackContentsVisitor( wostream& log );
    
    virtual ~EPTrackContentsVisitor();

    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node );
    
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node );
       
    virtual bool EdgeVisit(AAFComponentReference& edge);
    virtual bool EdgeVisit(AAFSlotReference& edge);
    
    shared_ptr<DetailLevelTestResult> GetResult();
    
  private:
   
    wostream& _log;
    shared_ptr<DetailLevelTestResult> _spResult;

    unsigned int CountSegments( AxMobSlot& track, aafUID_t segmentType );
    bool VisitEssenceTracks( AxMob& axMob, const AxString& type );

    // prohibited
    EPTrackContentsVisitor();
    EPTrackContentsVisitor( const EPTrackContentsVisitor& );
    EPTrackContentsVisitor& operator=( const EPTrackContentsVisitor& );
  
};

} // end of namespace diskstream

#endif /*__EPTrackContentsVisitor_h_*/

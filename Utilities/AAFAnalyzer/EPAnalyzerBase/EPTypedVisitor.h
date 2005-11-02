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

#ifndef __EPTYPEDVISITOR_h__
#define __EPTYPEDVISITOR_h__

//Edit Protocol Analyzer Base files
#include "EPTypedObjNode.h"

//AAF Analyzer Base files
#include <TypedVisitor.h>

//Ax files
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <set>

class AxMob;
class AxClassDef;
class AxMobSlot;

namespace aafanalyzer {

using namespace std;
using namespace boost;

class EPTopLevelComposition;
class EPLowerLevelComposition;
class EPSubClipComposition;
class EPAdjustedClipComposition;
class EPTemplateClip;
class EPClip;
class EPFileSource;
class EPRecordingSource;
class EPImportSource;
class EPTapeSource;
class EPFilmSource;
class TestInfo;
class DetailLevelTestResult;

class EPTypedVisitor : public TypedVisitor
{
    
  //Allow ancestor PreOrderVisit and PostOrderVisit functions to be called.
  using TypedVisitor::PreOrderVisit;
  using TypedVisitor::PostOrderVisit;
    
  public:
  
    typedef set<shared_ptr<AxMobSlot> > TrackSet;
  
    EPTypedVisitor();
    virtual ~EPTypedVisitor();

    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node);
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node);

    virtual bool PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node);
    virtual bool PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node);
    
  protected:

    //Functions commonly needed by EPTypedVisitors.  Put them in the base class
    //to avoid constantly re-implementing them.
    AxString GetMobName( AxMob& axMob, const AxString& type );
    bool IsType( AxClassDef& clsDef, aafUID_t type, aafUID_t parentType );
    shared_ptr<TrackSet> GetEssenceTracks( AxMob& axMob );
    
  private:
  
    // prohibited
    EPTypedVisitor( const EPTypedVisitor& );
    EPTypedVisitor& operator=( const EPTypedVisitor& );

};
    
} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/

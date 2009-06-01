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

#ifndef __EPNameVisitor_h_
#define __EPNameVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//Ax files
#include <AxMob.h>

//STL files
#include <map>
#include <set>

class AxDefObject;

namespace aafanalyzer {

class TestLevelTestResult;

using namespace boost;
using namespace std;

class EPNameVisitor : public EPTypedVisitor
{
  public:
  
    EPNameVisitor( wostream& log,
		   shared_ptr<EdgeMap> spEdgeMap,
		   shared_ptr<TestLevelTestResult> spTestResult );

    virtual ~EPNameVisitor();

    // Visit mobs to verify mob type name requirements.  We don't
    // generically visit all mobs (or all compositions mobs, master
    // mobs, source mobs) because the constraints on each are slightly
    // different. (e.g. the EP doesn't specify a valid name is
    // required on an import source but does require it on a tape
    // source.)
    // Each of these also checks the name constraints on the mob's
    // slots.
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>&     node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>&   node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>&      node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob,      EPTemplateClip>&            node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFMasterMob,      EPClip>&                    node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob,      EPRecordingSource>&         node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob,      EPTapeSource>&              node );
    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob,      EPFilmSource>&              node );

    // Capture all other mob types not in the list above.
    // Also verify they uniqueness of the slot names used in these
    // mobs.
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node);
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node);
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node);

    // Visit definition objects to verify definition name conventions.
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFDataDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFParameterDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFPluginDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFContainerDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFInterpolationDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFTaggedValueDefinition>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFOperationDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCodecDef>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFKLVDataDefinition>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFDefObject>& node );
    
    // Call this after a traversal to complete the analysis.
    void CheckForUniqueNames();

  private:

    typedef unsigned int Count;
    typedef map<AxString, Count> NameMap;
    typedef set<AxString> NameSet;

    wostream& _log;
    shared_ptr<EdgeMap> _spEdgeMap;
    
    shared_ptr<TestLevelTestResult> _spTestResult;
    NameMap _compositionNames;
    NameSet _topLevelNames;
    NameSet _lowerLevelNames;
    
    bool VisitComposition( Node& node, const AxString& type, const AxString& reqId, AxCompositionMob& axCompMob );
    bool VisitNonComposition( Node& node, const AxString& type, const AxString& reqId, AxMob& axMob );
    void CheckForUniqueNames( NameSet& names, const AxString& reqId, const AxString& type );
    void CheckForUniqueSlotNamesInMob( Node& node, IAAFMobSP spMob );
  
    // prohibited
    EPNameVisitor();
    EPNameVisitor( const EPNameVisitor& );
    EPNameVisitor& operator=( const EPNameVisitor& );
  
};

} // end of namespace diskstream

#endif /*__EPNameVisitor_h_*/

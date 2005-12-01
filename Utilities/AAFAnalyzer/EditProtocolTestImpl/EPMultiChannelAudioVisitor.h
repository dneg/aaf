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

#ifndef __EPMultiChannelAudioVisitor_h_
#define __EPMultiChannelAudioVisitor_h_

//Edit Protocol Analyzer Base files
#include <EPTypedVisitor.h>
#include <EPTypedObjNode.h>

//STL files
#include <set>

namespace aafanalyzer {

using namespace boost;

class DetailLevelTestResult;

class EPMultiChannelAudioVisitor : public EPTypedVisitor
{

  public:

    EPMultiChannelAudioVisitor( wostream& log );
    
    virtual ~EPMultiChannelAudioVisitor();

    virtual bool PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPMultiChannelAudioFileSource>& node );
    
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node );
    virtual bool PreOrderVisit( AAFTypedObjNode<IAAFSourceMob>& node );
       
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFCompositionMob>& node );
    virtual bool PostOrderVisit( AAFTypedObjNode<IAAFMasterMob>& node );
    
    virtual bool EdgeVisit( AAFMobReference& edge );
    
    shared_ptr<DetailLevelTestResult> GetResult();
    
  private:

    wostream& _log;
    shared_ptr<DetailLevelTestResult> _spResult;
    aafUInt32 _level;

    // prohibited
    EPMultiChannelAudioVisitor();
    EPMultiChannelAudioVisitor( const EPMultiChannelAudioVisitor& );
    EPMultiChannelAudioVisitor& operator=( const EPMultiChannelAudioVisitor& );
  
};

} // end of namespace diskstream

#endif /*__EPMultiChannelAudioVisitor_h_*/

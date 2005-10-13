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

//Edit Protocol Analyzer Base files
#include "EPTypedVisitor.h"

namespace {

using namespace aafanalyzer;

} // end of namespace

//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer
{

using namespace boost;

EPTypedVisitor::EPTypedVisitor()
{
}

EPTypedVisitor::~EPTypedVisitor()
{ 
}

/* 
 * Because we are using templates, a PreOrderVisit and a PostOrderVisit must
 * be suplied for every type of EPTypedObjNode.  If behaviour depends on the
 * type of the Mob and not the Edit Protocol material type, an identical
 * implementation would need to be provided for each material type.  Therefore,
 * make the default behaviour of all material type PreOrder and PostOrder visits
 * to be to forward on the call to a visit for the Mob type.  This behaviour
 * can be overriden in the concrete visitor.
 */

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFMasterMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFMasterMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFMasterMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFMasterMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PreOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PreOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPTopLevelComposition>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPLowerLevelComposition>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPSubClipComposition>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFCompositionMob, EPAdjustedClipComposition>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFCompositionMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFCompositionMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFMasterMob, EPTemplateClip>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFMasterMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFMasterMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFMasterMob, EPClip>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFMasterMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFMasterMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFileSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPRecordingSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPImportSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPTapeSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

bool EPTypedVisitor::PostOrderVisit( EPTypedObjNode<IAAFSourceMob, EPFilmSource>& node)
{
    shared_ptr<Node> spNode = node.GetSharedPointerToNode();
    shared_ptr<AAFTypedObjNode<IAAFSourceMob> > spBaseNode = dynamic_pointer_cast<AAFTypedObjNode<IAAFSourceMob> >(spNode);
    return this->PostOrderVisit( *spBaseNode );
}

} // end of namespace diskstream


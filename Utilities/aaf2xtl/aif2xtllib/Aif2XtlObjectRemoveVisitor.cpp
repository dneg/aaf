//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Siemens SBS Media
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <Aif2XtlObjectRemoveVisitor.h>

#include <Aif2XtlParseTreeNodeDecoration.h>

namespace {

class Aif2XtlMarkForRemoveInfo : public AifParseTreeNodeDecoration {
public:
	Aif2XtlMarkForRemoveInfo()
	{}
	
	virtual ~Aif2XtlMarkForRemoveInfo()
	{}
};

} // end of namespace

Aif2XtlObjectMarkForRemovalVisitor::Aif2XtlObjectMarkForRemovalVisitor( std::wostream& logStream )
: Aif2XtlParseTreeVisitor( logStream )
{}

Aif2XtlObjectMarkForRemovalVisitor::~Aif2XtlObjectMarkForRemovalVisitor()
{}

void Aif2XtlObjectMarkForRemovalVisitor::PreOrderVisit( Aif2XtlTaggedValue& node )
{
	// We don't want the node to be removed while it's parent is
	// traversing the child list.  Instead, decorate this node
	// with an Aif2XtlMarkForRemoveInfo.  A subsequent traveral
	// searchs it's children and removes any that are decorated.

	std::auto_ptr<Aif2XtlMarkForRemoveInfo> pMarkDecoration( new Aif2XtlMarkForRemoveInfo );

	node.PushDecoration( pMarkDecoration );
	
	// No point in visiting the children if this will be removed.
	node.SetVisitChildren(false);
}

void Aif2XtlObjectMarkForRemovalVisitor::PostOrderVisit( Aif2XtlTaggedValue& node )
{
	node.SetVisitChildren(true);
}

//========================================================================

Aif2XtlObjectRemoveMarkedVisitor::Aif2XtlObjectRemoveMarkedVisitor( std::wostream& logStream )
: Aif2XtlParseTreeVisitor( logStream )
{}

Aif2XtlObjectRemoveMarkedVisitor::~Aif2XtlObjectRemoveMarkedVisitor()
{}

void Aif2XtlObjectRemoveMarkedVisitor::PreOrderVisit( Aif2XtlParseTreeNode& node )
{
	int i;
	for ( i = 0; i < node.GetNumChildren(); ++i ) {
		AifParseTreeNode& childNode = node.GetChild(i);

		Aif2XtlMarkForRemoveInfo* pMark = 0;

		if ( node.IsDecorated( pMark ) ) {
			std::auto_ptr<AifParseTreeNode> pReleaseMe = node.ReleaseChild(i);
			// This changes the count. The easiest way to handle that is to
			// recurse to get the next marked child, then break.
			PreOrderVisit(node);
			break;			
		}
	}
}

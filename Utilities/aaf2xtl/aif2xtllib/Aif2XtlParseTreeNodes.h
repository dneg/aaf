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

#ifndef _Aif2XtlParseTreeNodes_h_
#define _Aif2XtlParseTreeNodes_h_

#include <AifParseTreeNode.h>
#include <AifParseTreeNodeFactory.h>

void Aif2XtlParseTreeNodesInit();

template <class IAAFType, class Aif2XtlNodeType>
class Aif2XtlStandardNodeFactory : public AifParseTreeNodeFactory {
public:

	Aif2XtlStandardNodeFactory()
		: AifParseTreeNodeFactory()
	{
		IAAFType* dummy = 0;
		Aif2XtlParseTreeNodeFactoryMap::GetInstance().RegisterFactory( AxAUID(dummy), this );
	}

	~Aif2XtlStandardNodeFactory()
	{}

	virtual std::auto_ptr<AifParseTreeNode> CreateNode( IAAFObjectSP spObj )
	{
		IAAFSmartPointer<IAAFType> sp;
		AxQueryInterface( spObj, sp );
		std::auto_ptr<AifParseTreeNode> node( new Aif2XtlNodeType( sp ) );
		return node;
	}
};

//----------------------------------------------------------------------------------------

class Aif2XtlParseTreeNode : public AifParseTreeNode {
public:
	Aif2XtlParseTreeNode();
	Aif2XtlParseTreeNode(IAAFObjectSP sp);
	virtual ~Aif2XtlParseTreeNode();
	virtual void Visit( AifParseTreeVisitor* pVisitor );
};

template <class Type>
class Aif2XtlStandardTypedNode : public Aif2XtlParseTreeNode {
public:
	Aif2XtlStandardTypedNode();
	virtual ~Aif2XtlStandardTypedNode();
	virtual void Visit( AifParseTreeVisitor* pVisitor );
};

// FIXME - Rename this "StandardAAFTyped"
template <class IAAFType>
class Aif2XtlStandardNode : public Aif2XtlParseTreeNode {
public:
	Aif2XtlStandardNode( IAAFSmartPointer<IAAFType> sp );
	virtual ~Aif2XtlStandardNode();
	IAAFSmartPointer<IAAFType> GetAif2XtlAAFObject();
	virtual void Visit( AifParseTreeVisitor* pVisitor );
private:
	IAAFSmartPointer<IAAFType> _sp;
};


// Extras:
//
// If Aif2XtlStandardNode is not used, then use this implementation of
// AifParseTreeNode::Visit(AifParseTreeVisitor*)
#define AIF2XTL_STANDARD_VISITOR_METHOD_DEFINITION(CLASS)									 \
void CLASS::Visit( AifParseTreeVisitor* pVisitor )									         \
{																							 \
	Aif2XtlParseTreeVisitor* pXtlVisitor = dynamic_cast<Aif2XtlParseTreeVisitor*>(pVisitor); \
	if (!pXtlVisitor) {																		 \
		AifParseTreeNode::Visit( pVisitor );												 \
		return;                                                                              \
	}																						 \
																							 \
	pXtlVisitor->PreOrderVisit( *this );													 \
	VisitChildren( pVisitor );																 \
	pXtlVisitor->PostOrderVisit( *this );													 \
}


typedef Aif2XtlStandardNode<IAAFCompositionMob>   Aif2XtlCompMobNode;
typedef Aif2XtlStandardNode<IAAFMasterMob>        Aif2XtlMasterMob;
typedef Aif2XtlStandardNode<IAAFTimelineMobSlot>  Aif2XtlTimelineMobSlot;
typedef Aif2XtlStandardNode<IAAFSequence>         Aif2XtlSequence;
typedef Aif2XtlStandardNode<IAAFSourceMob>        Aif2XtlSourceMob;
typedef Aif2XtlStandardNode<IAAFCDCIDescriptor>   Aif2XtlCDCIDescriptor;
typedef Aif2XtlStandardNode<IAAFWAVEDescriptor>   Aif2XtlWaveDescriptor;
typedef Aif2XtlStandardNode<IAAFNetworkLocator>   Aif2XtlNetworkLocator;
typedef Aif2XtlStandardNode<IAAFTransition>       Aif2XtlTransition;
typedef Aif2XtlStandardNode<IAAFTaggedValue>      Aif2XtlTaggedValue;
typedef Aif2XtlStandardNode<IAAFConstantValue>    Aif2XtlConstantValue;

// Aif2XtlGroup is a forward declaration of a non-existent class.  It
// is used simply to instantiate Aif2XtlStardardTypeNode, thus using
// the template to generate a unique type to represent the group node.
// It is inserted in the tree by Aif2XtlGroupVisitor.  The node
// factory has no knowledge of this node because it is not associated
// with an AAF object.
class Aif2XtlGroup;
typedef Aif2XtlStandardTypedNode<Aif2XtlGroup>		Aif2XtlGroupNode;

//----------------------------------------------------------------------------------------

// This node will visit all, or only one "selected" child.  This
// supports selecting of a single mob in a mob chiain.  The elements
// of a mob chain are added to the parse tree as children of a
// SourceClipNode by the Aif2XtlSourceClipVisitor, and a single node
// in the chain is selected by the Aif2XtlMobSelectVisitor.

class Aif2XtlSourceClipNode : public AifParseTreeNode {
public: 
	Aif2XtlSourceClipNode( IAAFSourceClipSP sp );
	virtual ~Aif2XtlSourceClipNode();

	virtual void Visit( AifParseTreeVisitor* pVisitor );

	IAAFSourceClipSP GetAif2XtlAAFObject();

	bool IsChildSelected();
	void  SetSelectedChild( int selectedChild );
	int GetSelectedChild();

private:
	IAAFSourceClipSP _sp;
	bool _isChildSelected;
	int _selectedChild;
};

//----------------------------------------------------------------------------------------

#endif

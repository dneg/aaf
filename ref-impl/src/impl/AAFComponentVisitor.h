#ifndef __AAFComponentVisitor_h__
#define __AAFComponentVisitor_h__

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
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



class ImplAAFCommentMarker;
class ImplAAFDescriptiveMarker;
class ImplAAFEdgecode;
class ImplAAFEssenceGroup;
class ImplAAFFiller;
class ImplAAFGPITrigger;
class ImplAAFNestedScope;
class ImplAAFOperationGroup;
class ImplAAFPulldown;
class ImplAAFScopeReference;
class ImplAAFSelector;
class ImplAAFSequence;
class ImplAAFSourceClip;
class ImplAAFTimecode;
class ImplAAFTimecodeStream;
class ImplAAFTimecodeStream12M;
class ImplAAFTransition;



// Class AAFComponentVisitor declares a visit operation for each Component
// referenced the given MobSlot.
//
// AAFComponentVisitor is a part of implementation of the Visitor pattern
// as described in "Design Patterns" by Gamma, Helm, Johnson, Vlissides 
// (1995, Addison-Wesley Pub Co, ISBN 0201633612 ).
//
class AAFComponentVisitor
{
    public:
        virtual ~AAFComponentVisitor() = 0;

        // Perform operation on the SourceClip being visited.
        virtual void VisitSourceClip        (ImplAAFSourceClip*) = 0;

        /*
        // TODO
        // For a complete implementation of the Visitor pattern each
        // concrete sub-class of the Component must have corresponding
        // Visit() method in the Visitor class, which it would call
        // from its Accept() method.

        virtual void VisitCommentMarker     (ImplAAFCommentMarker*) = 0;
        virtual void VisitDescriptiveMarker (ImplAAFDescriptiveMarker*) = 0;
        virtual void VisitEdgecode          (ImplAAFEdgecode*) = 0;
        virtual void VisitEssenceGroup      (ImplAAFEssenceGroup*) = 0;
        virtual void VisitFiller            (ImplAAFFiller*) = 0;
        virtual void VisitGPITrigger        (ImplAAFGPITrigger*) = 0;
        virtual void VisitNestedScope       (ImplAAFNestedScope*) = 0;
        virtual void VisitOperationGroup    (ImplAAFOperationGroup*) = 0;
        virtual void VisitPulldown          (ImplAAFPulldown*) = 0;
        virtual void VisitScopeReference    (ImplAAFScopeReference*) = 0;
        virtual void VisitSelector          (ImplAAFSelector*) = 0;
        virtual void VisitSequence          (ImplAAFSequence*) = 0;
        virtual void VisitTimecode          (ImplAAFTimecode*) = 0;
        virtual void VisitTimecodeStream    (ImplAAFTimecodeStream*) = 0;
        virtual void VisitTimecodeStream12M (ImplAAFTimecodeStream12M*) = 0;
        virtual void VisitTransition        (ImplAAFTransition*) = 0;
        */
};



inline AAFComponentVisitor::~AAFComponentVisitor()
{
}



#endif // ! __AAFComponentVisitor_h__


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

#ifndef _Aif2XtlEssencePreprocessVisitor_h_
#define _Aif2XtlEssencePreprocessVisitor_h_

#include <Aif2XtlParseTreeVisitor.h>
#include <Aif2XtlParseTreeNodes.h>

#include <AxSmartPointer.h>
#include <AxTypes.h>

class Aif2XtlParseTreeNodeFactory;

class Aif2XtlEssenceProcessVisitor : public Aif2XtlParseTreeVisitor {
public:
	Aif2XtlEssenceProcessVisitor( IAAFHeaderSP spHeader,
				      const AxString& aafFilename,
				      const AxString& extDirectory,
				      bool writeFile,
				      bool stripEssence,
				      Aif2XtlParseTreeNodeFactory& _nodeFactory,
					  std::wostream& logStream );

	virtual ~Aif2XtlEssenceProcessVisitor();

	virtual void PreOrderVisit( Aif2XtlSourceMob& node );
	virtual void PostOrderVisit( Aif2XtlSourceMob& node );

private:
	IAAFHeaderSP _spHeader;
	AxString _aafFilename;
	AxString _directory;
	bool _writeFile;
	bool _stripEssence;
	Aif2XtlParseTreeNodeFactory& _nodeFactory;
};

#endif


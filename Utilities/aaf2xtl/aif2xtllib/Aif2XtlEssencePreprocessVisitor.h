// Copyright 2003 BBC Technology

// $Id$
// $Name$

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


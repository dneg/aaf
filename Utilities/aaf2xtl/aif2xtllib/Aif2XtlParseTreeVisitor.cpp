// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <Aif2XtlParseTreeVisitor.h>

#include <Aif2XtlParseTreeNodes.h>

#include <iostream>

//====================================================================


Aif2XtlDumpVisitor::Aif2XtlDumpVisitor( std::wostream& os, std::wostream& logStream )
: Aif2XtlParseTreeVisitor( logStream ),
  _level( 0 ),
  _os( os )
{}

Aif2XtlDumpVisitor::~Aif2XtlDumpVisitor()
{}

void Aif2XtlDumpVisitor::indent( bool exceptionFlag )
{
	int level = _level;

	if ( exceptionFlag ) {
		_os << "* ";
	}

	_os << level << L"\t";

	while ( level-- )
	{
		_os << L"  ";
	}
}


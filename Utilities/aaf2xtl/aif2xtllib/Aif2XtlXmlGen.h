// Copyright 2003 BBC Technology

// $Id$
// $Name$

#ifndef _Aif2XtlXmlGen_h_h
#define _Aif2XtlXmlGen_h_h

// Get windows types vai the AAF headers.
#include <AAFTypes.h>

#include <string>
#include <memory>

class AifParseTree;
class AifSanteFeLoader;

// None of the methods of this class will throw an exception.  As
// such, they are suitable for calling directly from a com interface
// implementation.

class Aif2XtlXmlGen
{
 public:

  // Create an instance.  
  // This will catch any exception that may be thrown by the
  // constructor.
  // Caller owns the pointer written to ppInstance.
  // InFileName is an existing AAF file.
  // outStream is the output stream (e.g. an open ofstream or cout)

  static HRESULT __stdcall CreateInstance( const wchar_t* pInFileName,
					   const char* pAafComLib,
					   Aif2XtlXmlGen** ppInstance );


  // This version converts the file name to wide char.
  static HRESULT __stdcall CreateInstance( const char* pInFileName,
					   const char* pAafComLib,
					   Aif2XtlXmlGen** ppInstance );


  ~Aif2XtlXmlGen();

  // Option cuts only.  Default is false (i.e., process transitions)
  void SetCutsOnly( bool cutsOnly );

  // Load the file, parse the object tree, generate the xml output.
  HRESULT Execute( std::ostream& outStream, std::wostream& logStream );

  // Dump internal data structure.
  // Call this after Execute.
  void Dump( std::wostream& outStream );

 private:

  Aif2XtlXmlGen( const wchar_t* pInFileName,
				 const char* pAafComLib
 );

  // process transitions as cuts
  bool _cutsOnly;

  // extract essence data
  bool _extEssence;

  // Strip the essence data from the file and replace it with a
  // reference to an external file.
  bool _stripEssence;

  std::string _extDir;

  std::string _dllName;

  // AAF File.  Wide because that is what the SDK expects.
  std::basic_string<wchar_t> _inFileName;

  std::auto_ptr<AifParseTree> _apParseTree;

  std::auto_ptr<AifSanteFeLoader> _apLoader;
};

#endif

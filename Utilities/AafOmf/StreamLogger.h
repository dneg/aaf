// @doc INTERNAL
// @com This file implements the AAF informational logging utility. 
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#ifndef STREAMLOGGER_H_DEFINED
#define STREAMLOGGER_H_DEFINED (1)

#include "LoggerBase.h"

/****************************************************************
Name:
	class StreamLogger
Description:
	Logs messages to a given stream.
	
****************************************************************/
class StreamLogger : public LoggerBase
{
protected:
	FILE *iStream; // The file stream to log to. Can be stderr, stdout or a file.
public:
	StreamLogger( FILE *stream, unsigned level = 0, bool isMask = false );
	virtual ~StreamLogger( void );
	virtual unsigned SetLevel( unsigned level );
	virtual FILE* SetStream( FILE *stream );
	virtual void Print( unsigned level, const char *fmt, va_list args  );
};

inline StreamLogger::StreamLogger( FILE *stream, unsigned level, bool isMask )
	: iStream( stream ), LoggerBase( level, isMask ) 
{
}

inline StreamLogger::~StreamLogger( void )
{
}

/****************************************************************
Name:
	StreamLogger::SetStream
Description:
	Set the loggging stream.
Returns:
	The previous stream.
****************************************************************/
inline FILE *StreamLogger::SetStream( FILE *stream ) 
{ 
	FILE *temp = iStream;
	iStream = stream; 
	return temp; 
}

/****************************************************************
Name:
	class FileSteamLogger
Description:
	Logs messages to a file corresponding to a given 
	file name. 
	
****************************************************************/
class FileStreamLogger : public StreamLogger
{
public:
	FileStreamLogger(	const char *fileName, const char *mode = "w", 
						unsigned level = 0, bool isMask = false );
	virtual FileStreamLogger::~FileStreamLogger(void) ;
	class LogStreamNULL 
	{ 
	public:
		LogStreamNULL( void ) {} 
	};
};

/****************************************************************
Name:
	FileStreamLogger::FileStreamLogger
	FileStreamLogger::~FileStreamLogger
Description:
	Open up a file for logging with the given mode (constructor)
	Close the file when done   (destructor)
Returns:
	N/A
****************************************************************/

inline FileStreamLogger::FileStreamLogger( const char *fileName, const char *mode, unsigned level, bool isMask )
	: StreamLogger( fopen( fileName, mode ), level, isMask )
{
	if( iStream == NULL )
	{
		throw LogStreamNULL();
	}
}


inline FileStreamLogger::~FileStreamLogger(void) 
{ 
	if( iStream != 0 && iStream != stdout && iStream != stderr ) 
	{
		fclose( iStream ); 
	}
}

#endif // LOGGER_H_DEFINED

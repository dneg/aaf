/*
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aafmeta2dot.
 *
 *  aafmeta2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  aafmeta2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with aafmeta2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __LOGGING_H__
#define __LOGGING_H__

#include <iostream>

class Logging
{
 public:

  static void SetDebugLogStream( std::ostream *debugLogStream ) {
    _debugLogStream = debugLogStream;
  }

  static void SetErrorLogStream( std::ostream *errorLogStream ) {
    _errorLogStream = errorLogStream;
  }

  static void SetNullDebugLogStream() {
    _debugLogStream = &_nullStream;
  }

  static void SetNullErrorLogStream() {
    _errorLogStream = &_nullStream;
  }

  static std::ostream& DebugLogStream() {
    return *_debugLogStream;
  }

  static std::ostream& ErrorLogStream() {
    return *_errorLogStream;
  }

 private:
  static std::ostream* _debugLogStream;
  static std::ostream* _errorLogStream;

  static std::ostream _nullStream;
};


#endif // __LOGGING_H__


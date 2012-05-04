@echo off
rem  --------------------------------------------------------------------------
rem
rem   $Id$ $Name$
rem
rem   The contents of this file are subject to the AAF SDK Public Source
rem   License Agreement Version 2.0 (the "License"); You may not use this
rem   file except in compliance with the License.  The License is available
rem   in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
rem   Advanced Media Workflow Association, Inc., or its successor.
rem
rem   Software distributed under the License is distributed on an "AS IS"
rem   basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
rem   the License for the specific language governing rights and limitations
rem   under the License.  Refer to Section 3.3 of the License for proper use
rem   of this Exhibit.
rem
rem   WARNING:  Please contact the Advanced Media Workflow Association,
rem   Inc., for more information about any additional licenses to
rem   intellectual property covering the AAF Standard that may be required
rem   to create and distribute AAF compliant products.
rem   (http://www.amwa.tv/policies).
rem
rem   Copyright Notices:
rem   The Original Code of this file is Copyright 1998-2009, licensor of the
rem   Advanced Media Workflow Association.  All rights reserved.
rem
rem   The Initial Developer of the Original Code of this file and the
rem   licensor of the Advanced Media Workflow Association is
rem   Avid Technology.
rem   All rights reserved.
rem
rem  --------------------------------------------------------------------------

rem  ----------------------------------------------------------------
rem  File: update.cmd
rem
rem  Usage:  update.cmd <source> <destination>
rem
rem  This script checks if destination file is identical to
rem  the source file. If it's different or doesn't exist the source
rem  file gets copied to the destination file.
rem  ----------------------------------------------------------------

if not exist %2 goto update
fc /b %1 %2 | findstr "no differences encountered"
if errorlevel 1 goto update
echo %2 did not change
goto end
:update
echo Updating %2 ...
if exist %2 attrib -R %2
copy /v /b %1 %2
:end
@echo on



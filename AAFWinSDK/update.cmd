@echo off
rem  --------------------------------------------------------------------------
rem
rem   The contents of this file are subject to the AAF SDK Public
rem   Source License Agreement (the "License"); You may not use this file
rem   except in compliance with the License.  The License is available in
rem   AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
rem   Association or its successor.
rem   
rem   Software distributed under the License is distributed on an "AS IS"
rem   basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
rem   the License for the specific language governing rights and limitations
rem   under the License.
rem   
rem   The Original Code of this file is Copyright 1998-2001, Licensor of the
rem   AAF Association.
rem   
rem   The Initial Developer of the Original Code of this file and the
rem   Licensor of the AAF Association is Avid Technology.
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



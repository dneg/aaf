rem @echo off
if exist %2 attrib -R %2
copy /v /b %1 %2
rem @echo on

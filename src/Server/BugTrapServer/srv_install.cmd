@echo off
set NET_Version=v2.0.50727
set NET_Path=%WinDir%\Microsoft.NET\Framework\%NET_Version%

set currentDir=%cd%
cd ..\..\..\bin\srv\debug\
%NET_Path%\InstallUtil BugTrapServer.exe
cd %currentDir%

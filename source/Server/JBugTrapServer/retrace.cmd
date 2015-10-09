@echo off

call vars.cmd

set TRACEFILE=%1
if "%TRACEFILE%"=="" set TRACEFILE=BugTrapServerError.log
"%JAVABIN%\java.exe" -classpath %CLSPATH% -jar "%PGUARGBIN%\retrace.jar" BugTrapServer.map %TRACEFILE%

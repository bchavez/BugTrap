@echo off

del /f/q JBugTrapServer.jar
del /f/q JBugTrapServer.tmp.jar
del /f/q BugTrapServerError.log
del /f/q BugTrapServer.map
rd /s/q classes

@echo off

call vars.cmd

"%JAVABIN%\java.exe" -classpath JBugTrapServer.jar;%CLSPATH% BugTrapServer.ServerApp 2>>BugTrapServerError.log

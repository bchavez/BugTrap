@echo off

call vars.cmd

"%JAVABIN%\jdb.exe" -classpath JBugTrapServer.jar;%CLSPATH% -launch  BugTrapServer.ServerApp

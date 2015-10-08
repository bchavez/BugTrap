@echo off

call vars.cmd

rd /s/q HTML
"%JAVABIN%\javadoc.exe" -private -d HTML -classpath %CLSPATH% BugTrapServer

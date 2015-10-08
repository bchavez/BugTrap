@echo off

set JAVAHOME=%ProgramFiles%\Java
set JDK=jdk1.6.0_18
set JMAIL=javamail-1.4.3
set JAF=jaf-1.1.1
set JAVABIN=%JAVAHOME%\%JDK%\bin
set CLSPATH=".;%JAVAHOME%\%JDK%\jre\lib\rt.jar;%JAVAHOME%\%JMAIL%\mail.jar;%JAVAHOME%\%JAF%\activation.jar"

"%JAVABIN%\java.exe" -classpath JBugTrapServer.jar;%CLSPATH% BugTrapServer.ServerApp 2>>BugTrapServerError.log

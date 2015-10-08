@echo off

set JAVAHOME=%ProgramFiles%\Java
set JDK=jdk1.8.0
set JMAIL=javamail-1.4.3
set JAF=jaf-1.1.1
set PGUARD=proguard4.4
set JAVABIN=%JAVAHOME%\%JDK%\bin
set PGUARGBIN=%JAVAHOME%\%PGUARD%\lib
set CLSPATH=".;%JAVAHOME%\%JDK%\jre\lib\rt.jar;source\Server\JBugTrapServer\lib\%JMAIL%\mail.jar;source\Server\JBugTrapServer\lib\%JAF%\activation.jar"

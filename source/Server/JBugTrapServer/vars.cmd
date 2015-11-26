@echo off

set JAVAHOME=%ProgramFiles%\Java
set JDK=jdk1.7.0
set JMAIL=javamail-1.4.3
set JAF=jaf-1.1.1
set PGUARD=proguard4.11
set JAVABIN=%JAVAHOME%\%JDK%\bin
set PGUARGBIN=libs\%PGUARD%
set CLSPATH=".;%JAVAHOME%\%JDK%\jre\lib\rt.jar;libs\%JMAIL%\mail.jar;libs\%JAF%\activation.jar"

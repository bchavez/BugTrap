@echo off

call vars.cmd
call clean.cmd

set TARGET=Release
if not "%TARGET%"=="Release" set DOPTS=-g

md classes
"%JAVABIN%\javac.exe" -Xlint -deprecation -classpath %CLSPATH% %DOPTS% -d classes BugTrapServer\*.java
if errorlevel 1 goto end
cd classes
"%JAVABIN%\jar.exe" cvfm ..\JBugTrapServer.tmp.jar ..\manifest.mf BugTrapServer\*.class
if errorlevel 1 goto end
cd ..
if "%TARGET%"=="Release" (
"%JAVABIN%\java.exe" -classpath %CLSPATH% -jar "%PGUARGBIN%\proguard.jar" @JBugTrapServer.pg
del /f/q JBugTrapServer.tmp.jar
) else (
rename JBugTrapServer.tmp.jar JBugTrapServer.jar
)

rd /s/q classes
set TARGET=

:end

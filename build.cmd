@ECHO OFF
SETLOCAL

REM Uncomment to forcibly set the build version.
REM set FORCE_VERSION=0.0.4-alpha4

IF NOT DEFINED DevEnvDir (
    IF DEFINED vs120comntools ( 
        set envsetup=call "%vs120comntools%\..\..\VC\vcvarsall.bat"
    ) else (
        echo "VS120COMNTOOLS is not defined, aborting."
        exit /b 1
    )
)

mkdir __package

rem Install additional libraries (vc_mbcsmfc + boost.regex)
powershell -file install-dependencies.ps1

rem Restore NuGet packages
nuget restore source\BugTrap.vs2013.sln

rem Add boost to include and library paths
set "INCLUDE=C:\Libraries\boost;%INCLUDE%"
set "LIB=C:\Libraries\boost\stage\lib;%LIB%"

rem Additional arguments required for msbuild to use INCLUDE and LIB 
set "MSB_ADD_ARGS=-p:useenv=true"

rem 32-bit client
%envsetup% x86

echo "Building BugTrap [Debug|Win32]"
msbuild source\BugTrap.vs2013.sln -p:Configuration=Debug -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [Release|Win32]"
msbuild source\BugTrap.vs2013.sln -p:Configuration=Release -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [.NET Debug|Win32]"
msbuild source\BugTrap.vs2013.sln -p:Configuration=".NET Debug" -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [.NET Release|Win32]"
msbuild source\BugTrap.vs2013.sln -p:Configuration=".NET Release" -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [Unicode Debug|Win32]"
msbuild source\BugTrap.vs2013.sln -p:Configuration="Unicode Debug" -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [Unicode Release|Win32]"
msbuild source\BugTrap.vs2013.sln -p:Configuration="Unicode Release" -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

rem 64-bit client
%envsetup% x86_amd64

echo "Building BugTrap [Debug|x64]"
msbuild source\BugTrap.vs2013.sln -p:Configuration=Debug -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [Release|x64]"
msbuild source\BugTrap.vs2013.sln -p:Configuration=Release -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [.NET Debug|x64]"
msbuild source\BugTrap.vs2013.sln -p:Configuration=".NET Debug" -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [.NET Release|x64]"
msbuild source\BugTrap.vs2013.sln -p:Configuration=".NET Release" -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [Unicode Debug|x64]"
msbuild source\BugTrap.vs2013.sln -p:Configuration="Unicode Debug" -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap [Unicode Release|x64]"
msbuild source\BugTrap.vs2013.sln -p:Configuration="Unicode Release" -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

rem Examples
%envsetup% x86

echo "Building BugTrap.Examples [Debug|Win32]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Debug -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Release|Win32]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Release -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Debug|Win32]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Debug" -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Release|Win32]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Release" -p:Platform=Win32 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

%envsetup% x86_amd64

echo "Building BugTrap.Examples [Debug|x64]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Debug -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Release|x64]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Release -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Debug|x64]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Debug" -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Release|x64]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Release" -p:Platform=x64 %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

%envsetup% x86

echo "Building BugTrap.Examples [Debug|.NET x86]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Debug -p:Platform=".NET x86" %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Release|.NET x86]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Release -p:Platform=".NET x86" %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Debug|.NET x86]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Debug" -p:Platform=".NET x86" %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Release|.NET x86]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Release" -p:Platform=".NET x86" %MSB_ADD_ARGS%
if %errorlevel% neq 0 exit /b %errorlevel%

rem Windows server
echo "Building BugTrapServer [Debug|Any CPU]"
msbuild source\BugTrapServer.vs2013.sln -p:Configuration=Debug -p:Platform="Any CPU"
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrapServer [Release|Any CPU]"
msbuild source\BugTrapServer.vs2013.sln -p:Configuration=Release -p:Platform="Any CPU"
if %errorlevel% neq 0 exit /b %errorlevel%

robocopy bin\srv __package\Server\BugTrapServer\ /S

rem Windows web server
echo "Building BugTrapWebServer [Debug|Any CPU]"
msbuild source\BugTrapWebServer.vs2013.sln -p:Configuration=Debug -p:Platform="Any CPU"
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrapWebServer [Release|Any CPU]"
msbuild source\BugTrapWebServer.vs2013.sln -p:Configuration=Release -p:Platform="Any CPU"
if %errorlevel% neq 0 exit /b %errorlevel%

robocopy source\Server\BugTrapWebServer __package\Server\BugTrapWebServer\ /S


rem Java server
cd source\Server\JBugTrapServer\
call make.cmd
robocopy .\ ..\..\..\__package\Server\JBugTrapServer\ JBugTrapServer.jar BugTrapServer.config start.cmd
robocopy libs\ ..\..\..\__package\Server\JBugTrapServer\ /S /XD proguard*
cd ..\..\..

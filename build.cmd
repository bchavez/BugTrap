@ECHO OFF
SETLOCAL

REM Uncomment to forcibly set the build version.
REM set FORCE_VERSION=0.0.4-alpha4

IF NOT DEFINED DevEnvDir (
	IF DEFINED vs120comntools ( 
		CALL "%vs120comntools%\vsvars32.bat"
	)
)

rem Install additional libraries (vc_mbcsmfc)
powershell -file install-dependencies.ps1

rem Restore NuGet packages
nuget restore source\BugTrap.vs2013.sln

rem 32-bit client
echo ">>>>>>>>>>>>>>>>>>>>>>>> DEBUG 32"
msbuild source\BugTrap.vs2013.sln -p:Configuration=Debug -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> RELEASE 32"
msbuild source\BugTrap.vs2013.sln -p:Configuration=Release -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> .NET DEBUG 32"
msbuild source\BugTrap.vs2013.sln -p:Configuration=".NET Debug" -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> .NET RELEASE 32"
msbuild source\BugTrap.vs2013.sln -p:Configuration=".NET Release" -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

rem 64-bit client
echo ">>>>>>>>>>>>>>>>>>>>>>>> DEBUG 64"
msbuild source\BugTrap.vs2013.sln -p:Configuration=Debug -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> RELEASE 64"
msbuild source\BugTrap.vs2013.sln -p:Configuration=Release -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> .NET DEBUG 64"
msbuild source\BugTrap.vs2013.sln -p:Configuration=".NET Debug" -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> .NET RELEASE 64"
msbuild source\BugTrap.vs2013.sln -p:Configuration=".NET Release" -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

rem Examples
echo "Building BugTrap.Examples [Debug|Win32]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Debug -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Release|Win32]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Release -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Debug|Win32]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Debug" -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Release|Win32]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Release" -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Debug|x64]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Debug -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Release|x64]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Release -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Debug|x64]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Debug" -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Release|x64]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Release" -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Debug|.NET x86]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Debug -p:Platform=".NET x86"
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Release|.NET x86]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration=Release -p:Platform=".NET x86"
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Debug|.NET x86]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Debug" -p:Platform=".NET x86"
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrap.Examples [Unicode Release|.NET x86]"
msbuild source\BugTrap.Examples.vs2013.sln -p:Configuration="Unicode Release" -p:Platform=".NET x86"
if %errorlevel% neq 0 exit /b %errorlevel%

rem Windows server
echo "Building BugTrapServer [Debug|Any CPU]"
msbuild source\BugTrapServer.vs2013.sln -p:Configuration=Debug -p:Platform="Any CPU"
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrapServer [Release|Any CPU]"
msbuild source\BugTrapServer.vs2013.sln -p:Configuration=Release -p:Platform="Any CPU"
if %errorlevel% neq 0 exit /b %errorlevel%

rem Windows web server
echo "Building BugTrapWebServer [Debug|Any CPU]"
msbuild source\BugTrapWebServer.vs2013.sln -p:Configuration=Debug -p:Platform="Any CPU"
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building BugTrapWebServer [Release|Any CPU]"
msbuild source\BugTrapWebServer.vs2013.sln -p:Configuration=Release -p:Platform="Any CPU"
if %errorlevel% neq 0 exit /b %errorlevel%

rem Java server
rem XXX: add java server build script

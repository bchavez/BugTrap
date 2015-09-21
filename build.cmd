@ECHO OFF
SETLOCAL

REM Uncomment to forcibly set the build version.
REM set FORCE_VERSION=0.0.4-alpha4

IF NOT DEFINED DevEnvDir (
	IF DEFINED vs120comntools ( 
		CALL "%vs120comntools%\vsvars32.bat"
	)
)

echo ">>>>>>>>>>>>>>>>>>>>>>>> DEBUG 32"
msbuild src\BugTrap.vs2013.sln -p:Configuration=Debug -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> RELEASE 32"
msbuild src\BugTrap.vs2013.sln -p:Configuration=Release -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> .NET DEBUG 32"
msbuild src\BugTrap.vs2013.sln -p:Configuration=".NET Debug" -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> .NET RELEASE 32"
msbuild src\BugTrap.vs2013.sln -p:Configuration=".NET Release" -p:Platform=Win32
if %errorlevel% neq 0 exit /b %errorlevel%


echo ">>>>>>>>>>>>>>>>>>>>>>>> DEBUG 64"
msbuild src\BugTrap.vs2013.sln -p:Configuration=Debug -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> RELEASE 64"
msbuild src\BugTrap.vs2013.sln -p:Configuration=Release -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> .NET DEBUG 64"
msbuild src\BugTrap.vs2013.sln -p:Configuration=".NET Debug" -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%

echo ">>>>>>>>>>>>>>>>>>>>>>>> .NET RELEASE 64"
msbuild src\BugTrap.vs2013.sln -p:Configuration=".NET Release" -p:Platform=x64
if %errorlevel% neq 0 exit /b %errorlevel%
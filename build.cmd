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


rem Java server
cd source\Server\JBugTrapServer\
make.cmd

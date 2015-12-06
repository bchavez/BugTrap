[![Build status](https://ci.appveyor.com/api/projects/status/73nrgph9by0pgeb0?svg=true)](https://ci.appveyor.com/project/bchavez/bugtrap) [![Release](https://img.shields.io/github/release/bchavez/BugTrap.svg)](https://github.com/bchavez/BugTrap/releases) [![License](https://img.shields.io/github/license/bchavez/BugTrap.svg)](https://raw.githubusercontent.com/bchavez/BugTrap/master/LICENSE)

# BugTrap

BugTrap is a tool to catch unhandled exceptions in **unmanaged** and **managed** .NET code. BugTrap also supports sending crash reports to a remote server for analysis.

The original author, Maksim Pyatkovskiy, has a great [article about BugTrap on CodeProject](http://www.codeproject.com/Articles/14618/Catch-All-Bugs-with-BugTrap) that goes into detail about how BugTrap is used.

The BugTrap source code and binaries in this repository are granted by the original author under the MIT license. In other words, you're free to use BugTrap in commercial and non-commercial applications.

## Download & Install
Check the releases section in this repository for the latest builds:
[**BugTrap.zip**](https://github.com/bchavez/BugTrap/releases) contains all BugTrap components required for Win32/x64 projects:

* `BugTrap[U][D][N][-x64].dll` - BugTrap DLL module.
* `BugTrap[U][D][N][-x64].lib` - BugTrap library file used by linker.
  * **`[U]`** - Unicode aware version has 'U' letter. ANSI version doesn't have 'U' letter.
  * **`[D]`** - Debug version has 'D' letter. Release version doesn't have 'D' letter.
  * **`[N]`** - managed (.NET) version has 'N' letter. Native version doesn't have 'N' letter.
  * **`[-x64]`** - 64 bit version for AMD-64 platform has '-x64' suffix. x86 version doesn't have this suffix.
* `dbghelp.dll` - DbgHelp library (see ["BugTrap Developer's Guide"](https://raw.githubusercontent.com/bchavez/BugTrap/master/doc/BugTrap.pdf)                                      for details).
* `BugTrap.h` - Header file with BugTrap API definitions.
* `BTTrace.h` - C++ wrapper of custom logging functions.
* `BTAtlWindow.h` - ATL/WTL exception handlers.
* `BTMfcWindow.h` - MFC exception handlers.
* `CrashExplorer.exe` - MAP file analyzer.
* `BugTrap.chm` - BugTrap 1.x Specification.

Please see ["BugTrap Developer's Guide"](https://raw.githubusercontent.com/bchavez/BugTrap/master/doc/BugTrap.pdf) for additional information about file types used by BugTrap for Win32/x64.

### BugTrap Server Application
Inside [**BugTrap.zip**](https://github.com/bchavez/BugTrap/releases), you'll also find the following server applications for server side acceptance of crash reports:
* `Server\BugTrapServer` - BugTrap Server Windows Service in C#
* `Server\BugTrapWebServer` - BugTrap Web Server in ASP.NET
* `Server\JBugTrapServer` - BugTrap Server in Java

You can choose any server technology to begin accepting crash reports from BugTrap clients. Open and edit `*.config` files for various configuration parameters.

## Screenshots
![Screenshot](https://raw.githubusercontent.com/bchavez/BugTrap/master/doc/Screenshot2.png)
![Screenshot](https://raw.githubusercontent.com/bchavez/BugTrap/master/doc/Screenshot3.png)
![Screenshot](https://raw.githubusercontent.com/bchavez/BugTrap/master/doc/Screenshot4.png)


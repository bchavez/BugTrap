[![Build status](https://ci.appveyor.com/api/projects/status/73nrgph9by0pgeb0?svg=true)](https://ci.appveyor.com/project/bchavez/bugtrap)

# BugTrap

BugTrap is a tool to catch unhandled exceptions in **unmanaged** and **managed** .NET code. BugTrap also supports sending crash reports to a remote server for analysis.

The original author, Maksim Pyatkovskiy, has a great [article about BugTrap on CodeProject](http://www.codeproject.com/Articles/14618/Catch-All-Bugs-with-BugTrap) that goes into detail about how BugTrap is used.

The original author's website (and source code) went offline sometime in 2012 ([via WayBack Machine](http://web.archive.org/web/20120405043411/http://www.intellesoft.net/downloads.php)). This repository was originally a snapshot of the latest version of the source-code I could find before the author's website disappeared. Since then, many contributors have continued to improve BugTrap.

The original source-code licence for BugTrap is the [GNU Lesser General Public License (LGPL)](http://web.archive.org/web/20120402075507/http://www.intellesoft.net/bugtrap-license.php?BugTrapSrc.zip). In other words, you're free to use BugTrap in commercial and non-commercial applications.

## Download & Install
Check the releases section in this repository for the latest builds:
[**BugTrap.zip**](https://github.com/bchavez/BugTrap/releases) contains all BugTrap components required for Win32/x64 projects:

* `BugTrap[U][D][N][-x64].dll` - BugTrap DLL module.
* `BugTrap[U][D][N][-x64].lib` - BugTrap library file used by linker.
  * **`[U]`** - Unicode aware version has 'U' letter. ANSI version doesn't have 'U' letter.
  * **`[D]`** - Debug version has 'D' letter. Release version doesn't have 'D' letter.
  * **`[N]`** - managed (.NET) version has 'N' letter. Native version doesn't have 'N' letter.
  * **`[-x64]`** - 64 bit version for AMD-64 platform has '-x64' suffix. x86 version doesn't have this suffix.
* `dbghelp.dll` - DbgHelp library (see ["BugTrap Developer's Guide"](https://raw.githubusercontent.com/bchavez/BugTrap/master/docs/BugTrap.pdf)                                      for details).
* `BugTrap.h` - Header file with BugTrap API definitions.
* `BTTrace.h` - C++ wrapper of custom logging functions.
* `BTAtlWindow.h` - ATL/WTL exception handlers.
* `BTMfcWindow.h` - MFC exception handlers.
* `CrashExplorer.exe` - MAP file analyzer.
* `BugTrap.chm` - BugTrap 1.x Specification.

Please see ["BugTrap Developer's Guide"](https://raw.githubusercontent.com/bchavez/BugTrap/master/docs/BugTrap.pdf) for additional information about file types used by BugTrap for Win32/x64.

## Screenshots
![Screenshot](https://raw.githubusercontent.com/bchavez/BugTrap/master/docs/Screenshot2.png)
![Screenshot](https://raw.githubusercontent.com/bchavez/BugTrap/master/docs/Screenshot3.png)
![Screenshot](https://raw.githubusercontent.com/bchavez/BugTrap/master/docs/Screenshot4.png)


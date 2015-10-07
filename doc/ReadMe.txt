This folder contains all BugTrap components required for Win32 projects:
------------------------------------------------------------------------------------
BugTrap[U][D][N][-x64].dll         - BugTrap DLL module.
BugTrap[U][D][N][-x64].lib         - BugTrap library file used by linker.
dbghelp.dll                        - DbgHelp library (see "BugTrap Developer's Guide"
                                     for details).
BugTrap.h                          - Header file with BugTrap API definitions.
BTTrace.h                          - C++ wrapper of custom logging functions.
BTAtlWindow.h                      - ATL/WTL exception handlers.
BTMfcWindow.h                      - MFC exception handlers.
CrashExplorer.exe                  - MAP file analyzer.
BugTrap.chm                        - BugTrap 1.0 Specification.
------------------------------------------------------------------------------------
File names may vary by the following signs:
  [U]    - Unicode aware version has 'U' letter. ANSI version doesn't have 'U' letter.
  [D]    - Debug version has 'D' letter. Release version doesn't have 'D' letter.
  [N]    - managed (.NET) version has 'N' letter. Native version doesn't have 'N' letter.
  [-x64] - 64 bit version for AMD-64 platform has '-x64' suffix. x86 version doesn't have this suffix.

Please see "BugTrap Developer's Guide" for additional information about file types
used by BugTrap for Win32.
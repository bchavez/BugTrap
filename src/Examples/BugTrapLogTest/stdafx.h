// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <shlwapi.h>
#include <eh.h>                 // include set_terminate() declaration

// BugTrap includes //////////////////////////////////////////////////////////////

// Include main BugTrap header.
#include <BugTrap.h>

// Link with one of BugTrap libraries.
#if defined _M_IX86
 #ifdef _UNICODE
  #pragma comment(lib, "BugTrapU.lib")
 #else
  #pragma comment(lib, "BugTrap.lib")
 #endif
#elif defined _M_X64
 #ifdef _UNICODE
  #pragma comment(lib, "BugTrapU-x64.lib")
 #else
  #pragma comment(lib, "BugTrap-x64.lib")
 #endif
#else
 #error CPU architecture is not supported.
#endif

// Enable Common Controls support

#if defined _M_IX86
 #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
 #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
 #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
 #pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

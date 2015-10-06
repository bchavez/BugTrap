// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// Include standard Windows headers

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <tchar.h>
#include <eh.h>                 // include set_terminate() declaration

#pragma unmanaged

// BugTrap includes //////////////////////////////////////////////////////////////

// Include main BugTrap header.
#include <BugTrap.h>

// Link with one of BugTrap libraries.
#if defined _M_IX86
 #pragma comment(lib, "BugTrapN.lib")
#elif defined _M_X64
 #pragma comment(lib, "BugTrapN-x64.lib")
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

#pragma managed

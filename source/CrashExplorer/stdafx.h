// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER					// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0501			// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT			// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0501		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINDOWS			// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0501	// Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE				// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0600		// Change this to the appropriate value to target IE 5.0 or later.
#endif

#ifndef _RICHEDIT_VER			// Allow use of features specific to RichEdit 1.0 or later.
#define _RICHEDIT_VER 0x0210	// Change this to the appropriate value to target RichEdit 2.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit
#define _SECURE_ATL     1                   // use secure sprintf functions

#include <atlbase.h>
#include <atlstr.h>
#include <atltypes.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlcom.h>
#include <atlcomcli.h>
#include <atltypes.h>
#include <atlconv.h>
#include <atlfile.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlscrl.h>
#include <atlcrack.h>
#include <atlcoll.h>
#include <atlenc.h>
#include <msxml6.h>
#include <dbghelp.h>

// STL headers

#include <new>
#include <memory>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

// Boost headers

#include <boost/regex.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

// Leak watcher

#include "LeakWatcher.h"

// Custom definitions

#ifndef IDC_HAND
 #define IDC_HAND	MAKEINTRESOURCE(3249)
#endif

#define countof(array) (sizeof(array) / sizeof((array)[0]))

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

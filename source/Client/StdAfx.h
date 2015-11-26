/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Precomplied header file.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#ifdef _MANAGED
 #pragma unmanaged              // Compile all code as unmanaged by default
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

/// Comment out this if you don't want to use VDMDBG at all
//#define USE_VDMDBG
/// Comment out this if you don't want to read image header
//#define READ_IMAGEHEADER

#ifdef _WIN64
 #undef USE_VDMDBG
#endif

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <winsock2.h>
#include <shellapi.h>
#include <psapi.h>
#include <commctrl.h>
#include <commdlg.h>
#include <stdlib.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <dbghelp.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <mapi.h>
#include <lmcons.h>
#include <wininet.h>
#include <process.h>
#include <zmouse.h>
#include <limits.h>
#include <zip.h>
#include <stdio.h>
#include <new.h>

#ifdef USE_VDMDBG
 #include <vdmdbg.h>
#endif

struct _IMAGELIST { }; // unresolved typeref token

//#define DEBUG_WITH_BOUNDSCHECKER

#ifdef DEBUG_WITH_BOUNDSCHECKER
 #include <crtdbg.h>
 #pragma warning (disable : 4702) // unreachable code
 #define DEBUG_NEW new
#else
 #include "LeakWatcher.h"
#endif

#include <malloc.h>

#ifndef MAXSIZE_T
 #define MAXSIZE_T		((SIZE_T)~((SIZE_T)0))
#endif

#ifndef MAXSSIZE_T
 #define MAXSSIZE_T		((SSIZE_T)(MAXSIZE_T >> 1))
#endif

#ifndef MINSSIZE_T
 #define MINSSIZE_T		((SSIZE_T)~MAXSSIZE_T)
#endif

#ifndef MAXUINT
 #define MAXUINT		((UINT)~((UINT)0))
#endif

#ifndef MAXINT
 #define MAXINT			((INT)(MAXUINT >> 1))
#endif

#ifndef MININT
 #define MININT			((INT)~MAXINT)
#endif

#define countof(array) (sizeof(array) / sizeof((array)[0]))

#if defined _M_IX86
 #ifdef _MANAGED
  #ifdef _DEBUG
   #pragma comment(lib, "zlibMSD.lib")
  #else
   #pragma comment(lib, "zlibMS.lib")
  #endif
 #else
  #ifdef _DEBUG
   #pragma comment(lib, "zlibSD.lib")
  #else
   #pragma comment(lib, "zlibS.lib")
  #endif
 #endif
#elif defined _M_X64
 #ifdef _MANAGED
  #ifdef _DEBUG
   #pragma comment(lib, "zlibMSD-x64.lib")
  #else
   #pragma comment(lib, "zlibMS-x64.lib")
  #endif
 #else
  #ifdef _DEBUG
   #pragma comment(lib, "zlibSD-x64.lib")
  #else
   #pragma comment(lib, "zlibS-x64.lib")
  #endif
 #endif
#else
 #error CPU architecture is not supported.
#endif

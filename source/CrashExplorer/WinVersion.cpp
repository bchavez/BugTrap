/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Get Windows version.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "stdafx.h"
#include "WinVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// Cached Windows version info.
OSVERSIONINFO CWinVersion::m_osvi;

/**
 * @return current windows version.
 */
OSVERSIONINFO& CWinVersion::GetVersionInfo()
{
	if (! m_osvi.dwOSVersionInfoSize)
	{
		m_osvi.dwOSVersionInfoSize = sizeof(m_osvi);
		::GetVersionEx(&m_osvi);
	}
	return m_osvi;
}

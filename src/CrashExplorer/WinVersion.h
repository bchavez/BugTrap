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

#pragma once

class CWinVersion
{
public:
	/// Get current windows version.
	static OSVERSIONINFO& GetVersionInfo(void);
	/// Return true if current OS platform is NT-based.
	static bool IsWinNTPlatform(void);
	/// Return true if current OS version is Windows NT or newer.
	static bool IsWinNT(void);
	/// Return true if current OS version is Windows 2000 or newer.
	static bool IsWin2K(void);
	/// Return true if current OS version is Windows XP or newer.
	static bool IsWinXP(void);
	/// Return true if current OS version is Windows Vista or newer.
	static bool IsWinVista(void);

private:
	/// Cached Windows version info.
	static OSVERSIONINFO m_osvi;
};

/**
 * @return true if current OS platform is NT-based.
 */
inline bool CWinVersion::IsWinNTPlatform(void)
{
	return (CWinVersion::GetVersionInfo().dwPlatformId >= VER_PLATFORM_WIN32_NT);
}

/**
 * @return true if current OS version is Windows NT or newer.
 */
inline bool CWinVersion::IsWinNT(void)
{
	OSVERSIONINFO& osvi = CWinVersion::GetVersionInfo();
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 4);
}

/**
 * @return true if current OS version is Windows 2000 or newer.
 */
inline bool CWinVersion::IsWin2K(void)
{
	OSVERSIONINFO& osvi = CWinVersion::GetVersionInfo();
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 5);
}

/**
 * @return true if current OS version is Windows XP or newer.
 */
inline bool CWinVersion::IsWinXP(void)
{
	OSVERSIONINFO& osvi = CWinVersion::GetVersionInfo();
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 5 && osvi.dwMinorVersion >= 1);
}

/**
 * @return true if current OS version is Windows Vista or newer.
 */
inline bool CWinVersion::IsWinVista(void)
{
	OSVERSIONINFO& osvi = CWinVersion::GetVersionInfo();
	return (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 6);
}

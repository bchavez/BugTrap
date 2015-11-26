/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Base class for custom log file.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "LogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @param dwInitialLogSizeInBytes - initial log file size.
 */
CLogFile::CLogFile(void) : m_StrStream(1024)
{
	*m_szLogFileName = _T('\0');
	m_dwLogEchoMode = BTLE_NONE;
	m_dwLogFlags = BTLF_NONE;
	m_eLogLevel = BTLL_ALL;
	InitializeCriticalSection(&m_csLogFile);
}

/**
 * @return pointer to custom log file name.
 */
PCTSTR CLogFile::GetLogFileName(void)
{
	if (*m_szLogFileName == _T('\0'))
		CompleteLogFileName(NULL);
	return m_szLogFileName;
}

/**
 * @param eLogLevel - log level number.
 * @return log level prefix.
 */
PCTSTR CLogFile::GetLogLevelPrefix(BUGTRAP_LOGLEVEL eLogLevel)
{
	switch (eLogLevel)
	{
	case BTLL_ERROR:     return _T("ERROR");
	case BTLL_WARNING:   return _T("WARNING");
	case BTLL_IMPORTANT: return _T("IMPORTANT");
	case BTLL_INFO:      return _T("INFO");
	case BTLL_VERBOSE:   return _T("VERBOSE");
	default:             return NULL;
	}
}

/**
 * @return output console handle.
 */
HANDLE CLogFile::GetConsoleHandle(void) const
{
	HANDLE hConsole;
	if (m_dwLogEchoMode & BTLE_STDOUT)
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	else if (m_dwLogEchoMode & BTLE_STDERR)
		hConsole = GetStdHandle(STD_ERROR_HANDLE);
	else
		hConsole = NULL;
	return hConsole;
}

/**
 * @param hConsole - console handle.
 */
void CLogFile::WriteTextToConsole(HANDLE hConsole)
{
	DWORD dwTextLength, dwWritten;
	PCTSTR pszText = m_StrStream;
#ifndef _UNICODE
	UINT uConsoleCP = GetConsoleOutputCP();
	if (uConsoleCP != CP_ACP)
	{
		DWORD dwTextSizeW = MultiByteToWideChar(CP_ACP, 0, pszText, -1, NULL, 0);
		if (m_ConsoleBufferW.GetSize() < dwTextSizeW)
		{
			dwTextSizeW *= 2;
			if (! m_ConsoleBufferW.SetSize(dwTextSizeW))
				return;
		}
		MultiByteToWideChar(CP_ACP, 0, pszText, -1, m_ConsoleBufferW.GetData(), dwTextSizeW);
		DWORD dwTextSizeA = WideCharToMultiByte(uConsoleCP, 0, m_ConsoleBufferW.GetData(), -1, NULL, 0, NULL, NULL);
		if (m_ConsoleBufferA.GetSize() < dwTextSizeA)
		{
			dwTextSizeA *= 2;
			if (! m_ConsoleBufferA.SetSize(dwTextSizeA))
				return;
		}
		dwTextLength = WideCharToMultiByte(uConsoleCP, 0, m_ConsoleBufferW.GetData(), -1, m_ConsoleBufferA.GetData(), dwTextSizeA, NULL, NULL);
		WriteConsoleA(hConsole, m_ConsoleBufferA.GetData(), dwTextLength - 1, &dwWritten, NULL);
	}
	else
	{
		dwTextLength = (DWORD)m_StrStream.GetLength();
		WriteConsoleA(hConsole, pszText, dwTextLength, &dwWritten, NULL);
	}
#else
	dwTextLength = (DWORD)m_StrStream.GetLength();
	WriteConsole(hConsole, pszText, dwTextLength, &dwWritten, NULL);
#endif
}

/**
 * @param pSystemTime - pointer to system time.
 * @param pszTimeStatistics - time statistics buffer.
 * @param dwTimeStatisticsSize - size of time statistics buffer.
 */
void CLogFile::GetTimeStatistics(const SYSTEMTIME* pSystemTime, PTSTR pszTimeStatistics, DWORD dwTimeStatisticsSize)
{
	_stprintf_s(pszTimeStatistics, dwTimeStatisticsSize,
	            _T("%04d/%02d/%02d %02d:%02d:%02d"),
	            pSystemTime->wYear, pSystemTime->wMonth, pSystemTime->wDay, pSystemTime->wHour, pSystemTime->wMinute, pSystemTime->wSecond);
}

/**
 * @param pSystemTime - pointer to system time.
 * @param eLogLevel - log level number.
 * @param pszEntry - log entry text.
 */
void CLogFile::FillEntryText(BUGTRAP_LOGLEVEL eLogLevel, const SYSTEMTIME* pSystemTime, PCTSTR pszEntry)
{
	_ASSERTE(pszEntry != NULL);
	m_StrStream.Reset();
	if (m_dwLogFlags & BTLF_SHOWTIMESTAMP)
	{
		TCHAR szTimeStatistics[32];
		GetTimeStatistics(pSystemTime, szTimeStatistics, countof(szTimeStatistics));
		m_StrStream << _T('[') << szTimeStatistics << _T("] ");
	}
	if (m_dwLogFlags & BTLF_SHOWLOGLEVEL)
	{
		PCTSTR pszLogLevelPrefix = GetLogLevelPrefix(eLogLevel);
		if (pszLogLevelPrefix)
			m_StrStream << pszLogLevelPrefix << _T(": ");
	}
	m_StrStream << pszEntry << _T("\r\n");
}

/**
 * @param pszFormat - format expression.
 * @param argList - variable argument list.
 * @return true if string was formatted.
 */
BOOL CLogFile::FormatBufferV(PCTSTR pszFormat, va_list argList)
{
	DWORD dwFormatBufferSize = m_FormatBuffer.GetSize();
	if (dwFormatBufferSize == 0)
	{
		dwFormatBufferSize = 256;
		if (! m_FormatBuffer.SetSize(dwFormatBufferSize))
			return FALSE;
	}
	for (;;)
	{
		int iResult = _vsntprintf_s(m_FormatBuffer.GetData(), dwFormatBufferSize, _TRUNCATE, pszFormat, argList);
		if (iResult >= 0)
			return TRUE;
		dwFormatBufferSize *= 2;
		if (! m_FormatBuffer.SetSize(dwFormatBufferSize))
			return FALSE;
		_ASSERTE(m_FormatBuffer.GetSize() >= dwFormatBufferSize);
	}
}

/**
 * @param pszFormat - format expression.
 * @param argList - variable argument list.
 * @return true if string was formatted.
 */
BOOL CLogFile::FormatBufferF(PCTSTR pszFormat, ...)
{
	va_list argList;
	va_start(argList, pszFormat);
	BOOL bResult = FormatBufferV(pszFormat, argList);
	va_end(argList);
	return bResult;
}

/**
 * @param eLogLevel - log level number.
 * @param eEntryMode - entry mode.
 * @param rcsConsoleAccess - provides synchronous access to the console.
 * @param pszFormat - format string.
 * @return true if operation was completed successfully.
 */
BOOL CLogFile::WriteLogEntryF(BUGTRAP_LOGLEVEL eLogLevel, ENTRY_MODE eEntryMode, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszFormat, ...)
{
	va_list argList;
	va_start(argList, pszFormat);
	BOOL bResult = WriteLogEntryV(eLogLevel, eEntryMode, rcsConsoleAccess, pszFormat, argList);
	va_end(argList);
	return bResult;
}

/**
 * @param eLogLevel - log level number.
 * @param eEntryMode - entry mode.
 * @param rcsConsoleAccess - provides synchronous access to the console.
 * @param pszFormat - format string.
 * @param argList - variable argument list.
 * @return true if operation was completed successfully.
 */
BOOL CLogFile::WriteLogEntryV(BUGTRAP_LOGLEVEL eLogLevel, ENTRY_MODE eEntryMode, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszFormat, va_list argList)
{
	BUGTRAP_LOGLEVEL eLogFileLevel = GetLogLevel();
	if (eLogLevel > eLogFileLevel)
		return TRUE;
	BOOL bResult = FormatBufferV(pszFormat, argList);
	if (bResult)
		bResult = WriteLogEntry(eLogLevel, eEntryMode, rcsConsoleAccess, m_FormatBuffer.GetData());
	return bResult;
}

/**
 * @param eLogLevel - log level number.
 * @param pSystemTime - system time.
 * @param rcsConsoleAccess - provides synchronous access to the console.
 * @param pszEntry - log entry text.
 * @return true if text was written to console.
 */
BOOL CLogFile::WriteLogEntryToConsole(BUGTRAP_LOGLEVEL eLogLevel, const SYSTEMTIME* pSystemTime, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszEntry)
{
	DWORD dwLogEchoMode = GetLogEchoMode();
	HANDLE hConsole = GetConsoleHandle();
	BOOL bLogEcho = (dwLogEchoMode & BTLE_DBGOUT) != 0;
	if (hConsole || bLogEcho)
	{
		FillEntryText(eLogLevel, pSystemTime, pszEntry);
		EnterCriticalSection(&rcsConsoleAccess);
		if (hConsole)
			WriteTextToConsole(hConsole);
		if (bLogEcho)
			WriteTextToDebugConsole();
		LeaveCriticalSection(&rcsConsoleAccess);
		return TRUE;
	}
	else
		return FALSE;
}

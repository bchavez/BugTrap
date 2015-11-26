/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Log stream class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "LogStream.h"
#include "TextFormat.h"

/**
 * @return true if operation was completed successfully.
 */
BOOL CLogStream::LoadEntries(void)
{
	_ASSERTE(m_hFile == INVALID_HANDLE_VALUE);
	if (m_hFile != INVALID_HANDLE_VALUE)
		return FALSE;
	PCTSTR pszLogFileName = GetLogFileName();
	m_hFile = CreateFile(pszLogFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	DWORD dwFileSize = GetFileSize(m_hFile, NULL);
	if (dwFileSize == 0)
	{
		DWORD dwWritten;
		if (! WriteFile(m_hFile, g_arrUTF8Preamble, sizeof(g_arrUTF8Preamble), &dwWritten, NULL))
			goto error;
	}
	else
	{
		if (SetFilePointer(m_hFile, 0, NULL, FILE_END) == INVALID_SET_FILE_POINTER)
			goto error;
	}
	return TRUE;

error:
	Close();
	return FALSE;
}

/**
 * @return true if operation was completed successfully.
 */
BOOL CLogStream::ClearEntries(void)
{
	_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	if (SetFilePointer(m_hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return FALSE;
	if (! SetEndOfFile(m_hFile))
		return FALSE;
	return TRUE;
}

void CLogStream::Close(void)
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

/**
 * @param eLogLevel - log level number.
 * @param eEntryMode - entry mode.
 * @param rcsConsoleAccess - provides synchronous access to the console.
 * @param pszEntry - log entry text.
 * @return true if operation was completed successfully.
 */
BOOL CLogStream::WriteLogEntry(BUGTRAP_LOGLEVEL eLogLevel, ENTRY_MODE eEntryMode, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszEntry)
{
	_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);
	if (m_hFile == INVALID_HANDLE_VALUE)
		return FALSE;
	_ASSERTE(eEntryMode == EM_APPEND);
	if (eEntryMode != EM_APPEND)
		return FALSE;
	BUGTRAP_LOGLEVEL eLogFileLevel = GetLogLevel();
	if (eLogLevel <= eLogFileLevel)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		if (! WriteLogEntryToConsole(eLogLevel, &st, rcsConsoleAccess, pszEntry))
			FillEntryText(eLogLevel, &st, pszEntry);
		EncodeEntryText();
		const BYTE* pBuffer = m_MemStream.GetBuffer();
		if (pBuffer == NULL)
			return FALSE;
		DWORD dwLength = (DWORD)m_MemStream.GetLength();
		_ASSERTE(dwLength > 0);
		DWORD dwWritten;
		return WriteFile(m_hFile, pBuffer, dwLength, &dwWritten, NULL);
	}
	return TRUE;
}

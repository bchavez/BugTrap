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

#pragma once

#include "BugTrap.h"
#include "BugTrapUtils.h"
#include "StrStream.h"
#include "Buffer.h"

/**
 * @brief Base class for custom log file.
 */
class CLogFile
{
public:
	/// Entry mode.
	enum ENTRY_MODE
	{
		/// Records are appended to the end of the file.
		EM_APPEND,
		/// Records are inserted at the beginning of the file.
		EM_INSERT
	};

	/// Initialize the object.
	CLogFile(void);
	/// Destroy the object.
	virtual ~CLogFile(void);
	/// Get custom log file name.
	PCTSTR GetLogFileName(void);
	/// Set custom log file name.
	void SetLogFileName(PCTSTR pszLogFileName);
	/// Get number of records in custom log file.
	virtual DWORD GetLogSizeInEntries(void) const;
	/// Set number of records in custom log file.
	virtual BOOL SetLogSizeInEntries(DWORD dwLogSizeInEntries);
	/// Get maximum log file size in bytes.
	virtual DWORD GetLogSizeInBytes(void) const;
	/// Set maximum log file size in bytes.
	virtual BOOL SetLogSizeInBytes(DWORD dwLogSizeInBytes);
	/// Return true if time stamp is added to every log entry.
	DWORD GetLogFlags(void) const;
	/// Set true if time stamp is added to every log entry.
	BOOL SetLogFlags(DWORD dwLogFlags);
	/// Return minimal log level accepted by tracing functions.
	BUGTRAP_LOGLEVEL GetLogLevel(void) const;
	/// Set minimal log level accepted by tracing functions.
	BOOL SetLogLevel(BUGTRAP_LOGLEVEL eLogLevel);
	/// Get echo mode.
	DWORD GetLogEchoMode(void);
	/// Set echo mode.
	BOOL SetLogEchoMode(DWORD dwLogEchoMode);
	/// Take possession of the log file.
	void CaptureObject(void);
	/// Release possession of the log file.
	void ReleaseObject(void);
	/// Load Entries into memory.
	virtual BOOL LoadEntries(void) = 0;
	/// Save entries into disk.
	virtual BOOL SaveEntries(BOOL bCrash) = 0;
	/// Clear log entries.
	virtual BOOL ClearEntries(void) = 0;
	/// Close log file.
	virtual void Close(void) = 0;
	/// Add new log entry.
	virtual BOOL WriteLogEntry(BUGTRAP_LOGLEVEL eLogLevel, ENTRY_MODE eEntryMode, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszEntry) = 0;
	/// Add new log entry.
	BOOL WriteLogEntryF(BUGTRAP_LOGLEVEL eLogLevel, ENTRY_MODE eEntryMode, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszFormat, ...);
	/// Add new log entry.
	BOOL WriteLogEntryV(BUGTRAP_LOGLEVEL eLogLevel, ENTRY_MODE eEntryMode, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszFormat, va_list argList);

protected:
	/// Get default log file extension.
	virtual PCTSTR GetLogFileExtension(void) const = 0;
	/// Get last entry text.
	PCTSTR GetEntryText(void) const;
	/// Get log level prefix.
	static PCTSTR GetLogLevelPrefix(BUGTRAP_LOGLEVEL eLogLevel);
	/// Get time statistics.
	static void GetTimeStatistics(const SYSTEMTIME* pSystemTime, PTSTR pszTimeStatistics, DWORD dwTimeStatisticsSize);
	/// Fill entry text.
	void FillEntryText(BUGTRAP_LOGLEVEL eLogLevel, const SYSTEMTIME* pSystemTime, PCTSTR pszEntry);
	/// Write text to console.
	void WriteTextToConsole(HANDLE hConsole);
	/// Write text to debug console.
	void WriteTextToDebugConsole(void);
	/// Get output console handle.
	HANDLE GetConsoleHandle(void) const;
	/// Write log entry to the console.
	BOOL WriteLogEntryToConsole(BUGTRAP_LOGLEVEL eLogLevel, const SYSTEMTIME* pSystemTime, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszEntry);

private:
	/// Protects the class from being accidentally copied.
	CLogFile(const CLogFile& rLogFile);
	/// Protects the class from being accidentally copied.
	CLogFile& operator=(const CLogFile& rLogFile);
	/// Set log file name to default value.
	BOOL CompleteLogFileName(PCTSTR pszLogFileName);
	/// Fill buffer with formatted string.
	BOOL FormatBufferV(PCTSTR pszFormat, va_list argList);
	/// Fill buffer with formatted string.
	BOOL FormatBufferF(PCTSTR pszFormat, ...);

	/// Custom log file name.
	TCHAR m_szLogFileName[MAX_PATH];
	/// True if date and time stamp is added to every log entry.
	DWORD m_dwLogFlags;
	/// Echo mode.
	DWORD m_dwLogEchoMode;
	/// Minimal log level accepted by tracing functions.
	BUGTRAP_LOGLEVEL m_eLogLevel;
	/// Synchronization object.
	CRITICAL_SECTION m_csLogFile;
	/// Pre-allocated buffer for log entry text.
	CStrStream m_StrStream;
	/// Pre-allocated buffer for format string.
	CDynamicBuffer<TCHAR> m_FormatBuffer;
#ifndef _UNICODE
	/// Pre-allocated buffer for encoded console message.
	CDynamicBuffer<CHAR> m_ConsoleBufferA;
	/// Pre-allocated buffer for encoded console message.
	CDynamicBuffer<WCHAR> m_ConsoleBufferW;
#endif
};

inline CLogFile::~CLogFile(void)
{
	DeleteCriticalSection(&m_csLogFile);
}

/**
 *	@return maximum number of records in log file.
 */
inline DWORD CLogFile::GetLogSizeInEntries(void) const
{
	return 0;
}

/**
 * @param dwLogSizeInEntries - maximum number of records in log file.
 * @return true if operation is accepted.
 */
inline BOOL CLogFile::SetLogSizeInEntries(DWORD /*dwLogSizeInEntries*/)
{
	return FALSE;
}

/**
 * @return maximum log file size in bytes.
 */
inline DWORD CLogFile::GetLogSizeInBytes(void) const
{
	return 0;
}

/**
 * @param dwLogSizeInBytes - maximum log file size in bytes.
 * @return true if operation is accepted.
 */
inline BOOL CLogFile::SetLogSizeInBytes(DWORD /*dwLogSizeInBytes*/)
{
	return FALSE;
}

inline void CLogFile::CaptureObject(void)
{
	EnterCriticalSection(&m_csLogFile);
}

inline void CLogFile::ReleaseObject(void)
{
	LeaveCriticalSection(&m_csLogFile);
}

/**
 * @return current set of log flags.
 */
inline DWORD CLogFile::GetLogFlags(void) const
{
	return m_dwLogFlags;
}

/**
 * @param dwLogFlags - set of log flags.
 * @return true if operation was completed successfully.
 */
inline BOOL CLogFile::SetLogFlags(DWORD dwLogFlags)
{
	m_dwLogFlags = dwLogFlags;
	return TRUE;
}

/**
 * @return minimal log level accepted by tracing functions.
 */
inline BUGTRAP_LOGLEVEL CLogFile::GetLogLevel(void) const
{
	return m_eLogLevel;
}

/**
 * @param eLogLevel - minimal logl level accepted by tracing functions.
 * @return true if operation was completed successfully.
 */
inline BOOL CLogFile::SetLogLevel(BUGTRAP_LOGLEVEL eLogLevel)
{
	m_eLogLevel = eLogLevel;
	return TRUE;
}

/**
 * @return current echo mode.
 */
inline DWORD CLogFile::GetLogEchoMode(void)
{
	return m_dwLogEchoMode;
}

/**
 * @param dwLogEchoMode - new echo mode.
 * @return true if operation was completed successfully.
 */
inline BOOL CLogFile::SetLogEchoMode(DWORD dwLogEchoMode)
{
	m_dwLogEchoMode = dwLogEchoMode;
	return TRUE;
}

inline void CLogFile::WriteTextToDebugConsole(void)
{
	OutputDebugString(m_StrStream);
}

/**
 * @return last entry text.
 */
inline PCTSTR CLogFile::GetEntryText(void) const
{
	return (PCTSTR)m_StrStream;
}

/**
 * @param pszLogFileName - log file name.
 * @return true if log file name was created and false otherwise.
 */
inline BOOL CLogFile::CompleteLogFileName(PCTSTR pszLogFileName)
{
	return GetCompleteLogFileName(m_szLogFileName, pszLogFileName, GetLogFileExtension());
}

/**
 * @param pszLogFileName - pointer to custom log file name.
 */
inline void CLogFile::SetLogFileName(PCTSTR pszLogFileName)
{
	CompleteLogFileName(pszLogFileName);
}

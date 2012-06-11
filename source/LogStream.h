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

#pragma once

#include "LogFile.h"
#include "MemStream.h"
#include "Encoding.h"

/**
 * Log stream class.
 */
class CLogStream : public CLogFile
{
public:
	/// Object constructor,
	CLogStream(void);
	/// Object destructor.
	virtual ~CLogStream(void);
	/// Load Entries into memory.
	virtual BOOL LoadEntries(void);
	/// Save entries into disk.
	virtual BOOL SaveEntries(BOOL bCrash);
	/// Clear log entries.
	virtual BOOL ClearEntries(void);
	/// Add new log entry.
	virtual BOOL WriteLogEntry(BUGTRAP_LOGLEVEL eLogLevel, ENTRY_MODE eEntryMode, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszEntry);
	/// Close log file.
	virtual void Close(void);

protected:
	/// Get default log file extension.
	virtual PCTSTR GetLogFileExtension(void) const;

private:
	/// Protects the class from being accidentally copied.
	CLogStream(const CLogStream& rLogFile);
	/// Protects the class from being accidentally copied.
	CLogStream& operator=(const CLogStream& rLogFile);

	/// Encode entry text.
	void EncodeEntryText(void);

	/// Log file handle.
	HANDLE m_hFile;
	/// Encoder object pre-allocated for the log.
	CUTF8EncStream m_EncStream;
	/// Pre-allocated buffer for encoded log entry text.
	CMemStream m_MemStream;
};

inline CLogStream::CLogStream(void) : m_MemStream(1024), m_EncStream(&m_MemStream)
{
	m_hFile = INVALID_HANDLE_VALUE;
}

inline CLogStream::~CLogStream(void)
{
	Close();
}

/**
 * @param bCrash - true if the crash has been detected.
 * @return true if operation was completed successfully.
 */
inline BOOL CLogStream::SaveEntries(BOOL /*bCrash*/)
{
	return FALSE;
}

/**
 * @return log file extension.
 */
inline PCTSTR CLogStream::GetLogFileExtension(void) const
{
	return _T(".txt");
}

inline void CLogStream::EncodeEntryText(void)
{
	m_EncStream.Reset();
	PCTSTR pszEntry = GetEntryText();
	m_EncStream.WriteUTF8Bin(pszEntry);
}

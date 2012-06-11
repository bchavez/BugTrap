/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Base class for in-memory log file.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "LogFile.h"

/**
 * @brief Base class for in-memory log file.
 */
class CInMemLogFile : public CLogFile
{
public:
	/// Initialize the object.
	CInMemLogFile(DWORD dwInitialLogSizeInBytes);
	/// Destroy the object.
	virtual ~CInMemLogFile(void);
	/// Get number of records in custom log file.
	virtual DWORD GetLogSizeInEntries(void) const;
	/// Set number of records in custom log file.
	virtual BOOL SetLogSizeInEntries(DWORD dwLogSizeInEntries);
	/// Get maximum log file size in bytes.
	virtual DWORD GetLogSizeInBytes(void) const;
	/// Set maximum log file size in bytes.
	virtual BOOL SetLogSizeInBytes(DWORD dwLogSizeInBytes);
	/// Clear log entries.
	virtual BOOL ClearEntries(void);
	/// Close log file.
	virtual void Close(void);
	/// Get number of entries in a log.
	DWORD GetNumEntries(void) const;
	/// Get number of bytes in a log.
	DWORD GetNumBytes(void) const;

protected:
	/// Log entry data.
	struct CLogEntry
	{
		/// Pointer to the previous log entry.
		CLogEntry* m_pPrevEntry;
		/// Pointer to the next log entry.
		CLogEntry* m_pNextEntry;
		/// Buffer size in bytes.
		DWORD m_dwSize;
	};

	/// Get pointer to the first log entry.
	CLogEntry* GetFirstEntry(void) const;
	/// Get pointer to the last log entry.
	CLogEntry* GetLastEntry(void) const;
	/// Add log entry to the head.
	void AddToHead(CLogEntry* pLogEntry);
	/// Add log entry to the tail.
	void AddToTail(CLogEntry* pLogEntry);
	/// Remove the entry from the head.
	void DeleteHead(void);
	/// Remove the entry from the tail.
	void DeleteTail(void);
	/// Remove extra head entries.
	void FreeHead(void);
	/// Remove extra tail entries.
	void FreeTail(void);
	/// Free log entries.
	void FreeEntries(void);

private:
	/// Protects the class from being accidentally copied.
	CInMemLogFile(const CInMemLogFile& rLogFile);
	/// Protects the class from being accidentally copied.
	CInMemLogFile& operator=(const CInMemLogFile& rLogFile);

	/// Number of entries kept in memory.
	DWORD m_dwNumEntries;
	/// Size of log in bytes.
	DWORD m_dwNumBytes;
	/// Initial log size in bytes.
	DWORD m_dwInitialLogSizeInBytes;
	/// Maximum number of records in custom log file.
	DWORD m_dwLogSizeInEntries;
	/// Maximum log file size in bytes.
	DWORD m_dwLogSizeInBytes;
	/// Pointer to the first log entry.
	CLogEntry* m_pFirstEntry;
	/// Pointer to the last log entry.
	CLogEntry* m_pLastEntry;
};

/**
 *	@return maximum number of records in log file.
 */
inline DWORD CInMemLogFile::GetLogSizeInEntries(void) const
{
	return m_dwLogSizeInEntries;
}

/**
 * @param dwLogSizeInEntries - maximum number of records in log file.
 * @return true if operation was accepted.
 */
inline BOOL CInMemLogFile::SetLogSizeInEntries(DWORD dwLogSizeInEntries)
{
	m_dwLogSizeInEntries = dwLogSizeInEntries;
	return TRUE;
}

/**
 * @return maximum log file size in bytes.
 */
inline DWORD CInMemLogFile::GetLogSizeInBytes(void) const
{
	return m_dwLogSizeInBytes;
}

/**
 * @param dwLogSizeInBytes - maximum log file size in bytes.
 * @return true if operation was accepted.
 */
inline BOOL CInMemLogFile::SetLogSizeInBytes(DWORD dwLogSizeInBytes)
{
	m_dwLogSizeInBytes = dwLogSizeInBytes;
	return TRUE;
}

/**
 * @return pointer to the first log entry.
 */
inline CInMemLogFile::CLogEntry* CInMemLogFile::GetFirstEntry(void) const
{
	return m_pFirstEntry;
}

/**
 * @return pointer to the last log entry.
 */
inline CInMemLogFile::CLogEntry* CInMemLogFile::GetLastEntry(void) const
{
	return m_pLastEntry;
}

/**
 * @return number of entries in a log.
 */
inline DWORD CInMemLogFile::GetNumEntries(void) const
{
	return m_dwNumEntries;
}

/**
 * @return number of bytes in a log.
 */
inline DWORD CInMemLogFile::GetNumBytes(void) const
{
	return m_dwNumBytes;
}

inline CInMemLogFile::~CInMemLogFile(void)
{
	FreeEntries();
}

/*
 * @return true if operation was completed successfully.
 */
inline BOOL CInMemLogFile::ClearEntries(void)
{
	FreeEntries();
	return TRUE;
}

inline void CInMemLogFile::Close(void)
{
}

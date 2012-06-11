/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Text log file.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "InMemLogFile.h"
#include "Encoding.h"
#include "MemStream.h"
#include "TextFormat.h"

/**
 * @brief Text log file.
 */
class CTextLogFile : public CInMemLogFile
{
public:
	/// Initialize the object.
	CTextLogFile(void);
	/// Load entries into memory.
	virtual BOOL LoadEntries(void);
	/// Save entries into disk.
	virtual BOOL SaveEntries(BOOL bCrash);
	/// Add new log entry.
	virtual BOOL WriteLogEntry(BUGTRAP_LOGLEVEL eLogLevel, ENTRY_MODE eEntryMode, CRITICAL_SECTION& rcsConsoleAccess, PCTSTR pszEntry);

private:
	/// Protects the class from being accidentally copied.
	CTextLogFile(const CTextLogFile& rLogFile);
	/// Protects the class from being accidentally copied.
	CTextLogFile& operator=(const CTextLogFile& rLogFile);

	/// Log entry data.
	struct CTextLogEntry : public CInMemLogFile::CLogEntry
	{
#pragma warning(push)
#pragma warning(disable : 4200) // nonstandard extension used : zero-sized array in struct/union
		/// Data buffer.
		BYTE m_pbData[0];
#pragma warning(pop)
	};

	/// Get default log file extension.
	virtual PCTSTR GetLogFileExtension(void) const;
	/// Allocate log entry.
	CTextLogEntry* AllocLogEntry(const BYTE* pbData, DWORD dwSize, BOOL bAddCrLf);
	/// Add log entry to the head.
	BOOL AddToHead(BOOL bAddCrLf);
	/// Add log entry to the tail.
	BOOL AddToTail(BOOL bAddCrLf);
	/// Add log entry to the head.
	BOOL AddToHead(const BYTE* pbData, DWORD dwSize, BOOL bAddCrLf);
	/// Add log entry to the tail.
	BOOL AddToTail(const BYTE* pbData, DWORD dwSize, BOOL bAddCrLf);
	/// Encode entry text.
	void EncodeEntryText(void);

	/// Encoder object pre-allocated for the log.
	CUTF8EncStream m_EncStream;
	/// Pre-allocated buffer for encoded log entry text.
	CMemStream m_MemStream;
};

inline CTextLogFile::CTextLogFile(void) : CInMemLogFile(sizeof(g_arrUTF8Preamble)), m_MemStream(1024), m_EncStream(&m_MemStream)
{
}

/**
 * @return log file extension.
 */
inline PCTSTR CTextLogFile::GetLogFileExtension(void) const
{
	return _T(".txt");
}

inline void CTextLogFile::EncodeEntryText(void)
{
	m_EncStream.Reset();
	PCTSTR pszEntry = GetEntryText();
	m_EncStream.WriteUTF8Bin(pszEntry);
}

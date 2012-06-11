/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Base stream interface.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

class CBaseStream
{
public:
	/// List of optional, but supported features.
	enum STREAM_FEATURES
	{
		/// Optional functions are not supported.
		SF_NONE           = 0x00,
		/// IsOpen() is supported.
		SF_ISOPEN         = 0x01,
		/// Close() is supported.
		SF_CLOSE          = 0x02,
		/// GetName() is supported.
		SF_GETNAME        = 0x04,
		/// GetLastError() is supported.
		SF_GETLASTERROR   = 0x08,
		/// GetLength() is supported.
		SF_GETLENGTH      = 0x10,
		/// SetLength() is supported.
		SF_SETLENGTH      = 0x20,
		/// GetPosition() is supported.
		SF_GETPOSITION    = 0x40,
		/// SetPosition() is supported.
		SF_SETPOSITION    = 0x80
	};

	/// Destroy the object.
	virtual ~CBaseStream(void);
	/// Get list of supported features. This function is required.
	virtual unsigned GetFeatures(void) const;
	/// Return true if stream is open.
	virtual bool IsOpen(void) const;
	/// Closes the stream. This function is optional.
	virtual void Close(void);
	/// Get stream name. This function is optional.
	virtual bool GetName(PTSTR pszName, size_t nNameSize) const;
	/// Get last IO error code. This function is optional.
	virtual long GetLastError(void) const;
	/// Return number of bytes in the stream. This function is optional.
	virtual size_t GetLength(void) const;
	/// Get current position. This function is optional.
	virtual size_t GetPosition(void) const;
	/// Set current position. This function is optional.
	virtual size_t SetPosition(ptrdiff_t nOffset, int nMoveMethod);
};

inline CBaseStream::~CBaseStream(void)
{
}

/**
 * @return true if stream is open.
 */
inline bool CBaseStream::IsOpen(void) const
{
	return true;
}

/**
 * @return bit mask of supported features.
 */
inline unsigned CBaseStream::GetFeatures(void) const
{
	return SF_NONE;
}

inline void CBaseStream::Close(void)
{
}

/**
 * @param pszName - stream name buffer.
 * @param nNameSize - size of stream name buffer.
 * @return true if name was retrieved.
 */
inline bool CBaseStream::GetName(PTSTR pszName, size_t nNameSize) const
{
	if (nNameSize > 0)
		*pszName = _T('0');
	return false;
}

/**
 * @return number of bytes in the stream.
 */
inline size_t CBaseStream::GetLength(void) const
{
	return MAXSIZE_T;
}

/**
 * @return current stream position.
 */
inline size_t CBaseStream::GetPosition(void) const
{
	return MAXSIZE_T;
}

/**
 * @param nOffset - offset from start point.
 * @param nMoveMethod - start point.
 * @return new position value.
 */
inline size_t CBaseStream::SetPosition(ptrdiff_t /*nOffset*/, int /*nMoveMethod*/)
{
	return MAXSIZE_T;
}

/**
 * @return latest IO error code.
 */
inline long CBaseStream::GetLastError(void) const
{
	return NOERROR;
}

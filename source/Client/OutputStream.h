/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Output stream.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "BaseStream.h"

class CInputStream;

/// Output stream.
class COutputStream : public virtual CBaseStream
{
public:
	/// Set data length.
	virtual size_t SetLength(size_t nLength);
	/// Write one byte to the stream. This function is required.
	virtual bool WriteByte(unsigned char bValue) = 0;
	/// Write multiple values to the buffer. This function is optional.
	virtual size_t WriteByte(unsigned char bValue, size_t nCount);
	/// Write array of bytes to the stream. This function is optional.
	virtual size_t WriteBytes(const unsigned char* arrBytes, size_t nCount);
	/// Concatenate two streams. This function is optional.
	virtual size_t WriteStream(CInputStream* pInputStream);
};

/**
 * @param nLength - new stream length.
 * @return number of bytes in the stream.
 */
inline size_t COutputStream::SetLength(size_t /*nLength*/)
{
	return MAXSIZE_T;
}

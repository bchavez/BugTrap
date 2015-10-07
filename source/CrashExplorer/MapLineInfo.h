/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: MAP line information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "BaseLineInfo.h"

/// MAP line information.
class CMapLineInfo : public CBaseLineInfo
{
public:
	/// Initialize empty object.
	CMapLineInfo();
	/// Initialize the object.
	CMapLineInfo(PVOID ptrAddress, UINT uSection, PVOID ptrOffset, UINT uLineNumber);
	/**
	 * @brief Get code section.
	 * @return code section.
	 */
	UINT GetSection() const { return m_uSection; }
	/**
	 * @brief Get line offset.
	 * @return line offset.
	 */
	PVOID GetOffset() const { return m_ptrOffset; }
	/**
	 * @brief Get line number.
	 * @return line number.
	 */
	UINT GetLineNumber() const { return m_uLineNumber; }
	/**
	 * @brief Get line address.
	 * @return line address.
	 */
	virtual PVOID GetAddress() const { return m_ptrAddress; }
	/**
	 * @brief Get line number.
	 * @return line number.
	 */
	virtual UINT GetNumber() const { return m_uLineNumber; }

private:
	/// Line address.
	PVOID m_ptrAddress;
	/// Code section.
	UINT m_uSection;
	/// Line offset.
	PVOID m_ptrOffset;
	/// Line number.
	UINT m_uLineNumber;
};

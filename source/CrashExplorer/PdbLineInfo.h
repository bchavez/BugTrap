/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: PDB line information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "BaseLineInfo.h"

/// PDB line information.
class CPdbLineInfo : public CBaseLineInfo
{
public:
	/// Initialize empty object.
	CPdbLineInfo();
	/// Initialize the object.
	CPdbLineInfo(PVOID ptrAddress, UINT uNumber);
	/**
	 * @brief Get line address.
	 * @return line address.
	 */
	virtual PVOID GetAddress() const { return m_ptrAddress; }
	/**
	 * @brief Get line number.
	 * @return line number.
	 */
	virtual UINT GetNumber() const { return m_uNumber; }

private:
	/// Line address.
	PVOID m_ptrAddress;
	/// Line number.
	UINT m_uNumber;
};

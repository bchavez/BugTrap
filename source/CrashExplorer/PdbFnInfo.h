/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: PDB function information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "BaseFnInfo.h"

/// PDB function information.
class CPdbFnInfo : public CBaseFnInfo
{
public:
	/// Initialize empty object.
	CPdbFnInfo();
	/// Initialize the object.
	CPdbFnInfo(PVOID ptrAddress, const std::string& strName);
	/**
	 * @brief Get function name.
	 * @return function name.
	 */
	virtual std::string GetName() const { return m_strName; }
	/**
	 * @brief Get function address.
	 * @return function address.
	 */
	virtual PVOID GetAddress() const { return m_ptrAddress; }

private:
	/// Function address.
	PVOID m_ptrAddress;
	/// Function name.
	std::string m_strName;
};

inline CPdbFnInfo::CPdbFnInfo() :
			m_ptrAddress(NULL)
{
}

/**
 * @param ptrAddress - function address.
 * @param strName - function name.
 */
inline CPdbFnInfo::CPdbFnInfo(PVOID ptrAddress, const std::string& strName) :
			m_ptrAddress(ptrAddress),
			m_strName(strName)
{
}

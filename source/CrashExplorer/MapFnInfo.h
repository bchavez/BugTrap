/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: MAP function information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "BaseFnInfo.h"

/// MAP function information.
class CMapFnInfo : public CBaseFnInfo
{
public:
	/// Initialize empty object.
	CMapFnInfo();
	/// Initialize the object.
	CMapFnInfo(PVOID ptrAddress,
			   UINT uSection,
			   PVOID ptrOffest,
			   const std::string& strFunction,
			   PVOID ptrRvaBase,
			   const std::string& strLibrary,
			   const std::string& strObject);

	/**
	 * @brief Get section data.
	 * @return section data.
	 */
	UINT GetSection() const { return m_uSection; }
	/**
	 * @brief Get function offset.
	 * @return function offset.
	 */
	PVOID GetOffset() const { return m_ptrOffset; }
	/**
	 * @brief Get decorated function name.
	 * @return decorated function name.
	 */
	const std::string& GetDecoratedFunctionName() const { return m_strFunction; }
	/**
	 * @brief Get undecorated function name.
	 * @return undecorated function name.
	 */
	std::string GetUndecoratedFunctionName() const;
	/**
	 * @brief Get RVA+base address.
	 * @return RVA+base address.
	 */
	PVOID GetRvaBase() const { return m_ptrRvaBase; }
	/**
	 * @brief Get library name.
	 * @return library name.
	 */
	const std::string& GetLibrary() const { return m_strLibrary; }
	/**
	 * @brief Get object file name.
	 * @return object file name.
	 */
	const std::string& GetObject() const { return m_strObject; }
	/**
	 * @brief Get function address.
	 * @return function address.
	 */
	virtual PVOID GetAddress() const { return m_ptrAddress; }
	/**
	 * @brief Get function name.
	 * @return function name.
	 */
	virtual std::string GetName() const { return GetUndecoratedFunctionName(); }

private:
	/// Function address.
	PVOID m_ptrAddress;
	/// Section data.
	UINT m_uSection;
	/// Function offset.
	PVOID m_ptrOffset;
	/// Decorated function name.
	std::string m_strFunction;
	/// RVA base address.
	PVOID m_ptrRvaBase;
	/// Library name.
	std::string m_strLibrary;
	/// Object file name.
	std::string m_strObject;
};

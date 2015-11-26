/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Base function information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

/// Base function information.
class CBaseFnInfo
{
public:
	/// Destroy the object.
	virtual ~CBaseFnInfo() { }
	/// Get function name.
	virtual std::string GetName() const = 0;
	/// Get function address.
	virtual PVOID GetAddress() const = 0;
};

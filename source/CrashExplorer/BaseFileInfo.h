/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Base file information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

/// Base file information.
class CBaseFileInfo
{
public:
	/// Destroy the object.
	virtual ~CBaseFileInfo() { }
	/// Get file name.
	virtual const std::string& GetFileName() const = 0;
};

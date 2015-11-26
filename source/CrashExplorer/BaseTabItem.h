/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Base tab item.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "RuntimeClass.h"

class CBaseTabItem : public CObject, public CWindow
{
	DECLARE_ABSTRACT_RUNTIME_CLASS(CBaseTabItem)
public:
	/// Create tab item window.
	virtual BOOL CreateItem(HWND hwndParent) = 0;
	/// Get page title.
	virtual void GetItemTitle(CString& strTitle) = 0;
	/// Save tab contents.
	virtual void SaveState(void) = 0;
	/// Restore tab contents.
	virtual void LoadState(void) = 0;
};

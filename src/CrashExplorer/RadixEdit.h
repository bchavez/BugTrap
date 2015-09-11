/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Radix aware editor.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "NumEdit.h"
#include "RadixMenu.h"

/// Radix aware editor.
class CRadixEdit
{
public:
	/// Attach objects to controls.
	void Attach(HWND hwndEdit, HWND hwndRadixMenu);
	/// Get editor.
	CNumEdit& GetNumEdit();
	/// Get editor.
	const CNumEdit& GetNumEdit() const;
	/// Get radix menu.
	CRadixMenu& GetRadixMenu();
	/// Get radix menu.
	const CRadixMenu& GetRadixMenu() const;

protected:
	/// Editor object.
	CNumEdit m_txtNumEdit;
	/// Radix menu.
	CRadixMenu m_btnRadixMenu;
};

/**
 * @return the editor.
 */
inline CNumEdit& CRadixEdit::GetNumEdit()
{
	return m_txtNumEdit;
}

/**
 * @return the editor.
 */
inline const CNumEdit& CRadixEdit::GetNumEdit() const
{
	return m_txtNumEdit;
}

/**
 * @return radix menu.
 */
inline CRadixMenu& CRadixEdit::GetRadixMenu()
{
	return m_btnRadixMenu;
}

/**
 * @return radix menu.
 */
inline const CRadixMenu& CRadixEdit::GetRadixMenu() const
{
	return m_btnRadixMenu;
}

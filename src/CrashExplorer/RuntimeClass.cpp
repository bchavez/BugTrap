/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Runtime class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "stdafx.h"
#include "RuntimeClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// Instance of root runtime class.
CRuntimeClass<CObject> CObject::m_class(NULL);

/**
 * @param pBaseClass - pointer to base runtime class.
 * @return true if this class is defined from the base class.
 */
bool CBaseRuntimeClass::IsDerivedFrom(const CBaseRuntimeClass* pBaseClass) const
{
	const CBaseRuntimeClass* pClass = this;
	do
	{
		if (pClass == pBaseClass)
			return true;
		pClass = pClass->m_pBaseClass;
	}
	while (pClass != NULL);
	return false;
}

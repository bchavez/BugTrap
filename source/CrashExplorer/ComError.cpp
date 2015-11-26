/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: COM exception clsss.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "stdafx.h"
#include "resource.h"
#include "ComError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @param hResult - error code.
 * @param bUseErrorInfo - true if exception object should get error description from IErrorInfo.
 */
com_error::com_error(HRESULT hResult, bool bUseErrorInfo)
{
	m_hResult = hResult;
	if (bUseErrorInfo)
	{
		CComPtr<IErrorInfo> pErrorInfo;
		if (SUCCEEDED(GetErrorInfo(0, &pErrorInfo)))
		{
			CComBSTR bstrError;
			pErrorInfo->GetDescription(&bstrError);
			if (bstrError != NULL)
			{
				m_strErrorMessage = bstrError;
				return;
			}
		}
	}
	PSTR pszBuffer = NULL;
	if (FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, hResult, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), (PSTR)&pszBuffer, 0, NULL) > 0)
	{
		m_strErrorMessage = pszBuffer;
		if (pszBuffer != NULL)
		{
			LocalFree(pszBuffer);
			return;
		}
	}
	m_strErrorMessage.Format(IDS_ERROR_UNDEFINEDERROR, hResult);
}

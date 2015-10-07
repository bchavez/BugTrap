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

#pragma once

/// Exception class.
class com_error : public std::exception
{
public:
	/// Initialize the object.
	com_error(HRESULT hResult, bool bUseErrorInfo = false);
	/// Initialize the object.
	com_error(const char* pszErrorMessage);
	/// Initialize the object.
	com_error(HRESULT hResult, const char* pszErrorMessage);
	/// Return error message.
	virtual const char* what() const;
	/// Get error code.
	HRESULT hres() const;

private:
	/// Error code.
	HRESULT m_hResult;
	/// Error message.
	CStringA m_strErrorMessage;
};

/**
 * @return error message.
 */
inline const char* com_error::what() const
{
	return m_strErrorMessage;
}

/**
 * @return error code.
 */
inline HRESULT com_error::hres() const
{
	return m_hResult;
}

/**
 * @param pszErrorMessage - error message.
 */
inline com_error::com_error(const char* pszErrorMessage)
{
	m_hResult = -1;
	m_strErrorMessage = pszErrorMessage;
}

/**
 * @param hResult - error code.
 * @param pszErrorMessage - error message.
 */
inline com_error::com_error(HRESULT hResult, const char* pszErrorMessage)
{
	m_hResult = hResult;
	m_strErrorMessage = pszErrorMessage;
}

/**
 * Check result code and generate an exception in case of the error.
 * @param lResult - operation result.
 */
inline void CHECK_WIN32RESULT(LONG lResult)
{
	if (lResult != NOERROR)
		throw com_error(HRESULT_FROM_WIN32(lResult), false);
}

/**
 * Check result code and generate an exception in case of the error.
 * @param hResult - operation result.
 * @param bUseErrorInfo - true if exception object should get error description from IErrorInfo.
 */
inline void CHECK_HRESULT(HRESULT hResult, bool bUseErrorInfo = false)
{
	if (FAILED(hResult))
		throw com_error(hResult, bUseErrorInfo);
}

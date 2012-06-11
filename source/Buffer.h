/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Dynamic buffer.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

/// Structure holds either dynamically allocated or static buffer.
template <typename TYPE, DWORD dwStatBufferSize = 256>
class CMixedBuffer
{
public:
	/// Initialize structure elements.
	CMixedBuffer(void);
	/// Deallocate dynamic buffer if necessary.
	~CMixedBuffer(void);
	/// Allocate dynamic buffer.
	BOOL SetSize(DWORD dwBufferSize);
	/// Get buffer data.
	TYPE* GetData(void);
	/// Get buffer data.
	const TYPE* GetData(void) const;
	/// Get buffer size.
	DWORD GetSize(void) const;

private:
	/// Protect object from being accidentally copied.
	CMixedBuffer(const CMixedBuffer& rBuffer);
	/// Protect object from being accidentally copied.
	CMixedBuffer& operator=(const CMixedBuffer& rBuffer);

	/// Initialize static buffer.
	void InitStatBuffer(void);
	/// Static buffer.
	TYPE m_arrStatBuffer[dwStatBufferSize];
	/// Pointer to dynamic buffer.
	TYPE* m_pDynBuffer;
	/// Size of dynamic buffer.
	DWORD m_dwDynBufferSize;
};

template <typename TYPE, DWORD dwStatBufferSize>
inline CMixedBuffer<TYPE, dwStatBufferSize>::CMixedBuffer(void)
{
	InitStatBuffer();
}

template <typename TYPE, DWORD dwStatBufferSize>
inline CMixedBuffer<TYPE, dwStatBufferSize>::~CMixedBuffer(void)
{
	delete[] m_pDynBuffer;
}

/**
 * @return pointer to buffer data.
 */
template <typename TYPE, DWORD dwStatBufferSize>
inline TYPE* CMixedBuffer<TYPE, dwStatBufferSize>::GetData(void)
{
	return (m_pDynBuffer != NULL ? m_pDynBuffer : m_arrStatBuffer);
}

/**
 * @return pointer to buffer data.
 */
template <typename TYPE, DWORD dwStatBufferSize>
inline const TYPE* CMixedBuffer<TYPE, dwStatBufferSize>::GetData(void) const
{
	return (m_pDynBuffer != NULL ? m_pDynBuffer : m_arrStatBuffer);
}

/**
 * @return buffer size.
 */
template <typename TYPE, DWORD dwStatBufferSize>
inline DWORD CMixedBuffer<TYPE, dwStatBufferSize>::GetSize(void) const
{
	return (m_pDynBuffer != NULL ? m_dwDynBufferSize : countof(m_arrStatBuffer));
}

template <typename TYPE, DWORD dwStatBufferSize>
inline void CMixedBuffer<TYPE, dwStatBufferSize>::InitStatBuffer(void)
{
	m_pDynBuffer = NULL;
	m_dwDynBufferSize = 0;
}

/**
 * @param dwBufferSize - allocate size of buffer.
 * @return true if dynamic buffer was successfully allocated.
 */
template <typename TYPE, DWORD dwStatBufferSize>
BOOL CMixedBuffer<TYPE, dwStatBufferSize>::SetSize(DWORD dwBufferSize)
{
	if (dwBufferSize == 0)
	{
		delete[] m_pDynBuffer;
		InitStatBuffer();
		return TRUE;
	}
	if (m_pDynBuffer != NULL)
	{
		if (m_dwDynBufferSize >= dwBufferSize)
			return TRUE;
	}
	else
	{
		if (countof(m_arrStatBuffer) >= dwBufferSize)
			return TRUE;
	}
	TYPE* pDynBuffer = new TYPE[dwBufferSize];
	if (pDynBuffer == NULL)
		return FALSE;
	delete[] m_pDynBuffer;
	m_pDynBuffer = pDynBuffer;
	m_dwDynBufferSize = dwBufferSize;
	return TRUE;
}

/// Dynamically allocated buffer.
template <typename TYPE>
class CDynamicBuffer
{
public:
	/// Initialize structure elements.
	CDynamicBuffer(void);
	/// Initialize structure elements.
	CDynamicBuffer(DWORD dwBufferSize);
	/// Deallocate dynamic buffer if necessary.
	~CDynamicBuffer(void);
	/// Allocate dynamic buffer.
	BOOL SetSize(DWORD dwBufferSize);
	/// Get buffer data.
	TYPE* GetData(void);
	/// Get buffer data.
	const TYPE* GetData(void) const;
	/// Get buffer size.
	DWORD GetSize(void) const;

private:
	/// Protect object from being accidentally copied.
	CDynamicBuffer(const CDynamicBuffer& rBuffer);
	/// Protect object from being accidentally copied.
	CDynamicBuffer& operator=(const CDynamicBuffer& rBuffer);
	/// Allocate buffer.
	BOOL AllocBuffer(DWORD dwBufferSize);
	/// Initialize empty buffer.
	void InitBuffer(void);
	/// Free buffer.
	void FreeBuffer(void);

	/// Pointer to dynamic buffer.
	TYPE* m_pBuffer;
	/// Size of dynamic buffer.
	DWORD m_dwBufferSize;
};

template <typename TYPE>
void CDynamicBuffer<TYPE>::InitBuffer(void)
{
	m_pBuffer = NULL;
	m_dwBufferSize = 0;
}

template <typename TYPE>
void CDynamicBuffer<TYPE>::FreeBuffer(void)
{
	delete[] m_pBuffer;
	InitBuffer();
}

template <typename TYPE>
BOOL CDynamicBuffer<TYPE>::AllocBuffer(DWORD dwBufferSize)
{
	TYPE* pBuffer = new TYPE[dwBufferSize];
	if (pBuffer == NULL)
		return FALSE;
	delete[] m_pBuffer;
	m_pBuffer = pBuffer;
	m_dwBufferSize = dwBufferSize;
	return TRUE;
}

template <typename TYPE>
inline CDynamicBuffer<TYPE>::CDynamicBuffer(void)
{
	InitBuffer();
}

template <typename TYPE>
inline CDynamicBuffer<TYPE>::CDynamicBuffer(DWORD dwBufferSize)
{
	InitBuffer();
	if (dwBufferSize > 0)
		AllocBuffer(dwBufferSize);
}

template <typename TYPE>
inline CDynamicBuffer<TYPE>::~CDynamicBuffer(void)
{
	delete[] m_pBuffer;
}

/**
 * @return pointer to buffer data.
 */
template <typename TYPE>
inline TYPE* CDynamicBuffer<TYPE>::GetData(void)
{
	return m_pBuffer;
}

/**
 * @return pointer to buffer data.
 */
template <typename TYPE>
inline const TYPE* CDynamicBuffer<TYPE>::GetData(void) const
{
	return m_pBuffer;
}

/**
 * @return buffer size.
 */
template <typename TYPE>
inline DWORD CDynamicBuffer<TYPE>::GetSize(void) const
{
	return m_dwBufferSize;
}

/**
 * @param dwBufferSize - size of dynamic buffer.
 * @return true if dynamic buffer was successfully allocated.
 */
template <typename TYPE>
BOOL CDynamicBuffer<TYPE>::SetSize(DWORD dwBufferSize)
{
	if (dwBufferSize == 0)
	{
		FreeBuffer();
		return TRUE;
	}
	if (m_dwBufferSize >= dwBufferSize)
		return TRUE;
	return AllocBuffer(dwBufferSize);
}

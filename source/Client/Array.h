/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009ß IntelleSoft.
 * All rights reserved.
 *
 * Description: Simple dynamic array.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "ColHelper.h"

/// Simple dynamic array.
template
<
	typename DATA_TYPE,
	class INSTANCE_TRAITS = CObjectTraits<DATA_TYPE>,
	class COMPARE_TRAITS = CCompareTraits<DATA_TYPE>
>
class CArray
{
public:
	enum
	{
		/// Default array size.
		DEFAULT_SIZE = 10
	};

	/// Initialize the object.
	explicit CArray(size_t nSize = DEFAULT_SIZE);
	/// Destroy the object.
	~CArray(void);
	/// Makes a copy of array.
	CArray(const CArray& rArray);
	/// Makes a copy of array.
	CArray& operator=(const CArray& rArray);
	/// Append another array.
	void Append(const CArray& rArray);
	/// Ensures that the array has enough space.
	void EnsureSize(size_t nSize, bool bAdaptiveGrowth = true);
	/// Set number of array item.
	void SetCount(size_t nCount);
	/// Get number of items in the array.
	size_t GetCount(void) const;
	/// Get buffer size (in elements).
	size_t GetSize(void) const;
	/// Add new item.
	DATA_TYPE& AddItem(void);
	/// Add new item.
	void AddItem(const DATA_TYPE& rItem);
	/// Insert an element at a specified index; grow the array if necessary.
	void InsertItem(size_t nItemPos, const DATA_TYPE& rItem);
	/// Insert an element at a specified index; grow the array if necessary.
	DATA_TYPE& InsertItem(size_t nItemPos);
	/// Insert an element according to the array sort order; grow the array if necessary.
	size_t InsertOrderedItem(const DATA_TYPE& rItem, bool bAscending = true, bool bAllowDuplicates = true);
	/// Delete the item.
	void DeleteItem(size_t nItemPos);
	/// Delete all items.
	void DeleteAll(bool bFree = false);
	/// Sets an element at a given position.
	void SetAt(size_t nItemPos, const DATA_TYPE& rItem);
	/// Returns element at a given position.
	const DATA_TYPE& GetAt(size_t nItemPos) const;
	/// Return true if array contains no elements.
	bool IsEmpty(void) const;
	/// Sort the contents of the array.
	void QSort(bool bAscending = true);
	/// Search elements in a sorted array for the specified data.
	size_t BSearch(const DATA_TYPE& rKey, bool bAscending = true) const;
	/// Search elements in an unsorted array for the specified data.
	size_t LSearch(const DATA_TYPE& rKey) const;
	/// Set or get the element.
	DATA_TYPE& operator[](size_t nItemPos);
	/// Get the element.
	const DATA_TYPE& operator[](size_t nItemPos) const;
	/// Get array buffer.
	operator DATA_TYPE*(void);
	/// Get array buffer.
	operator const DATA_TYPE*(void) const;

private:
	/// Validate item index.
	void ValidateIndex(size_t nItemPos) const;
	/// Compare two elements.
	typedef int (*TCompareFunc)(const DATA_TYPE& rData1, const DATA_TYPE& rData2);
	/// Compare two elements.
	static int CompareAsc(const DATA_TYPE& rData1, const DATA_TYPE& rData2);
	/// Compare two elements.
	static int CompareDesc(const DATA_TYPE& rData1, const DATA_TYPE& rData2);
	/// Sort the contents of the array.
	static void PrivQSort(DATA_TYPE* arrData, TCompareFunc pfnCompare, size_t nLowPos, size_t nHighPos);

	/// Array data.
	DATA_TYPE* m_arrData;
	/// Number of items in the array.
	size_t m_nCount;
	/// Size of array.
	size_t m_nSize;
};

/**
 * @param nSize - the number of initially allocated elements.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::CArray(size_t nSize)
{
	m_nSize = nSize;
	m_nCount = 0;
	if (nSize > 0)
	{
		m_arrData = (DATA_TYPE*)new BYTE[nSize * sizeof(DATA_TYPE)];
		if (m_arrData == NULL)
		{
			m_nSize = 0;
			RaiseException(STATUS_NO_MEMORY, 0, 0, NULL);
		}
	}
	else
		m_arrData = NULL;
}

/**
 * @param rArray - object to be copied.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::CArray(const CArray& rArray)
{
	m_nSize = m_nCount = rArray.m_nCount;
	if (m_nSize)
	{
		m_arrData = (DATA_TYPE*)new BYTE[m_nSize * sizeof(DATA_TYPE)];
		if (m_arrData == NULL)
		{
			m_nSize = m_nCount = 0;
			RaiseException(STATUS_NO_MEMORY, 0, 0, NULL);
		}
		for (size_t i = 0; i < m_nCount; ++i)
		{
			INSTANCE_TRAITS::Constructor(m_arrData[i]);
			INSTANCE_TRAITS::Assignment(m_arrData[i], rArray.m_arrData[i]);
		}
	}
	else
		m_arrData = NULL;
}

/**
 * @param rArray - object to be copied.
 * @return reference to itself.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>& CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::operator=(const CArray& rArray)
{
	if (this != &rArray)
	{
		SetCount(rArray.m_nCount);
		for (size_t i = 0; i < m_nCount; ++i)
			INSTANCE_TRAITS::Assignment(m_arrData[i], rArray.m_arrData[i]);
	}
	return *this;
}

template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::~CArray(void)
{
	DeleteAll(true);
}

/**
 * @param rArray - another array.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::Append(const CArray& rArray)
{
	size_t nTotalCount = m_nCount + rArray.m_nCount;
	EnsureSize(nTotalCount, false);
	for (size_t nSrcPos = 0, nDstPos = m_nCount; nDstPos < nTotalCount; ++nSrcPos, ++nDstPos)
	{
		INSTANCE_TRAITS::Constructor(m_arrData[nDstPos]);
		INSTANCE_TRAITS::Assignment(m_arrData[nDstPos], rArray.m_arrData[nSrcPos]);
	}
	m_nCount = nTotalCount;
}

/**
 * @param nSize - checked array size.
 * @param bAdaptiveGrowth - true for adaptive growth.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::EnsureSize(size_t nSize, bool bAdaptiveGrowth)
{
	if (m_nSize < nSize)
	{
		if (bAdaptiveGrowth)
		{
			if (nSize <= 10)
				nSize = 10;
			else if (nSize <= 100)
				nSize += nSize;
			else
				nSize += nSize / 2;
		}
		DATA_TYPE* arrData = (DATA_TYPE*)new BYTE[nSize * sizeof(DATA_TYPE)];
		if (arrData == NULL)
			RaiseException(STATUS_NO_MEMORY, 0, 0, NULL);
		if (m_arrData)
		{
			CopyMemory(arrData, m_arrData, m_nCount * sizeof(DATA_TYPE));
			delete[] (PBYTE)m_arrData;
		}
		m_arrData = arrData;
		m_nSize = nSize;
	}
}

/**
 * @param nCount - the new array size (number of elements).
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::SetCount(size_t nCount)
{
	EnsureSize(nCount, false);
	if (nCount < m_nCount)
	{
		for (size_t i = nCount; i < m_nCount; ++i)
			INSTANCE_TRAITS::Destructor(m_arrData[i]);
	}
	else if (nCount > m_nCount)
	{
		for (size_t i = m_nCount; i < nCount; ++i)
			INSTANCE_TRAITS::Constructor(m_arrData[i]);
	}
	m_nCount = nCount;
}

/**
 * @param rItem - the element to be added to this array.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::AddItem(const DATA_TYPE& rItem)
{
	INSTANCE_TRAITS::Assignment(AddItem(), rItem);
}

/**
 * @param nItemPos - index of an element.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::ValidateIndex(size_t nItemPos) const
{
	_ASSERTE(nItemPos < m_nCount);
	if (nItemPos >= m_nCount)
		RaiseException(STATUS_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
}

/**
 * @return a reference to newly inserted element.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
DATA_TYPE& CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::AddItem(void)
{
	EnsureSize(m_nCount + 1, true);
	INSTANCE_TRAITS::Constructor(m_arrData[m_nCount]);
	return m_arrData[m_nCount++];
}

/**
 * @param nItemPos - an integer index of deleted element.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::DeleteItem(size_t nItemPos)
{
	ValidateIndex(nItemPos);
	INSTANCE_TRAITS::Destructor(m_arrData[nItemPos]);
	MoveMemory(m_arrData + nItemPos, m_arrData + nItemPos + 1, (--m_nCount - nItemPos) * sizeof(DATA_TYPE));
}

/**
 * @param bFree - pass true of release array memory.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::DeleteAll(bool bFree)
{
	SetCount(0);
	if (bFree)
	{
		delete[] (PBYTE)m_arrData;
		m_arrData = NULL;
		m_nSize = 0;
	}
}

/**
 * @param nItemPos - an integer index of the insertion point.
 * @param rItem - the element to be placed in this array.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::InsertItem(size_t nItemPos, const DATA_TYPE& rItem)
{
	INSTANCE_TRAITS::Assignment(InsertItem(nItemPos), rItem);
}

/**
 * @param rItem - the element to be placed in this array.
 * @param bAscending - sort direction.
 * @param bAllowDuplicates - true if duplicates are allowed.
 * @return position of the inserted item.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline size_t CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::InsertOrderedItem(const DATA_TYPE& rItem, bool bAscending, bool bAllowDuplicates)
{
	size_t nInsertPos = BSearch(rItem, bAscending);
	if (nInsertPos & ~MAXSSIZE_T)
		nInsertPos = ~nInsertPos;
	else if (! bAllowDuplicates)
		return nInsertPos;
	InsertItem(nInsertPos, rItem);
	return nInsertPos;
}

/**
 * @param nItemPos - an integer index of the insertion point.
 * @return a reference to newly inserted element.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
DATA_TYPE& CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::InsertItem(size_t nItemPos)
{
	_ASSERTE(nItemPos <= m_nCount);
	if (nItemPos >= m_nCount + 1)
		RaiseException(STATUS_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
	size_t nSize = m_nCount + 1;
	if (m_nSize < nSize)
	{
		nSize += nSize / 2;
		DATA_TYPE* arrData = (DATA_TYPE*)new BYTE[nSize * sizeof(DATA_TYPE)];
		if (arrData == NULL)
			RaiseException(STATUS_NO_MEMORY, 0, 0, NULL);
		CopyMemory(arrData, m_arrData, nItemPos * sizeof(DATA_TYPE));
		CopyMemory(arrData + nItemPos + 1, m_arrData + nItemPos, (m_nCount - nItemPos) * sizeof(DATA_TYPE));
		delete[] (PBYTE)m_arrData;
		m_arrData = arrData;
		m_nSize = nSize;
	}
	else
	{
		MoveMemory(m_arrData + nItemPos + 1, m_arrData + nItemPos, (m_nCount - nItemPos) * sizeof(DATA_TYPE));
	}
	++m_nCount;
	INSTANCE_TRAITS::Constructor(m_arrData[nItemPos]);
	return m_arrData[nItemPos];
}

/**
 * @param nItemPos - the index of an element.
 * @return the reference of an element at the specified position.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline DATA_TYPE& CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::operator[](size_t nItemPos)
{
	ValidateIndex(nItemPos);
	return m_arrData[nItemPos];
}

/**
 * @param nItemPos - the index of an element.
 * @return the constant reference of an element at the specified position.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline const DATA_TYPE& CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::operator[](size_t nItemPos) const
{
	ValidateIndex(nItemPos);
	return m_arrData[nItemPos];
}

/**
 * @return the number of elements in the array.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline size_t CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::GetCount(void) const
{
	return m_nCount;
}

/**
 * @return allocated size of the array.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline size_t CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::GetSize(void) const
{
	return m_nSize;
}

/**
 * @return pointer to the address of the first element.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::operator DATA_TYPE*(void)
{
	return m_arrData;
}

/**
 * @return pointer to the address of the first element.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::operator const DATA_TYPE*(void) const
{
	return m_arrData;
}

/**
 * @return true if the array contains no elements.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline bool CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::IsEmpty(void) const
{
	return (m_nCount == 0);
}

/**
 * @param nItemPos - position of the element.
 * @param rItem - new data value.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::SetAt(size_t nItemPos, const DATA_TYPE& rItem)
{
	ValidateIndex(nItemPos);
	INSTANCE_TRAITS::Assignment(m_arrData[nItemPos], rItem);
}

/**
 * @param nItemPos - position of the element.
 * @return element value.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline const DATA_TYPE& CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::GetAt(size_t nItemPos) const
{
	ValidateIndex(nItemPos);
	return m_arrData[nItemPos];
}

/**
 * @param rData1 - pointer to the first compared element.
 * @param rData2 - pointer to the second compared element.
 * @return comparison result.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline int CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::CompareAsc(const DATA_TYPE& rData1, const DATA_TYPE& rData2)
{
	return COMPARE_TRAITS::OrderedCompare(rData1, rData2);
}

/**
 * @param rData1 - pointer to the first compared element.
 * @param rData2 - pointer to the second compared element.
 * @return comparison result.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline int CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::CompareDesc(const DATA_TYPE& rData1, const DATA_TYPE& rData2)
{
	return COMPARE_TRAITS::OrderedCompare(rData2, rData1);
}

/**
 * @param bAscending - sort direction.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
inline void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::QSort(bool bAscending)
{
	TCompareFunc pfnCompare = bAscending ? &CompareAsc : &CompareDesc;
	PrivQSort(m_arrData, pfnCompare, 0, m_nCount - 1);
}

/**
 * @param arrData - array data.
 * @param pfnCompare - pointer to comparison function.
 * @param nLowPos - leftmost array index.
 * @param nHighPos - rightmost array index.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
void CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::PrivQSort(DATA_TYPE* arrData, TCompareFunc pfnCompare, size_t nLowPos, size_t nHighPos)
{
	if (nLowPos < nHighPos)
	{
		size_t nLeft = nLowPos;
		size_t nRight = nHighPos;
		DATA_TYPE& DataPivot = arrData[nLowPos];
		while (nLeft <= nRight)
		{
			while (nLeft <= nRight && (*pfnCompare)(DataPivot, arrData[nLeft]) > 0)
				++nLeft;
			while (nLeft <= nRight && (*pfnCompare)(DataPivot, arrData[nRight]) < 0)
				--nRight;
			if (nLeft > nRight)
				break;
			DATA_TYPE DataTemp;
			INSTANCE_TRAITS::Assignment(DataTemp, arrData[nLeft]);
			INSTANCE_TRAITS::Assignment(arrData[nLeft], arrData[nRight]);
			INSTANCE_TRAITS::Assignment(arrData[nRight], DataTemp);
			++nLeft;
			--nRight;
		}
		PrivQSort(arrData, pfnCompare, nLowPos, nRight);
		PrivQSort(arrData, pfnCompare, nLeft, nHighPos);
	}
}

/**
 * @param rKey - data to search for.
 * @param bAscending - sort direction.
 * @return element number where the data was found or negative index if no data found.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
size_t CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::BSearch(const DATA_TYPE& rKey, bool bAscending) const
{
	TCompareFunc pfnCompare = bAscending ? &CompareAsc : &CompareDesc;
	size_t nLowPos = 0, nHighPos = 0, nMiddlePos = 0;
	while (nLowPos <= nHighPos)
	{
		nMiddlePos = (nLowPos + nHighPos) / 2;
		int nResult = (*pfnCompare)(m_arrData[nMiddlePos], rKey);
		if (nResult < 0)
			nLowPos = ++nMiddlePos;
		else if (nResult > 0)
			nHighPos = nMiddlePos - 1;
		else
			return nMiddlePos;
	}
	return ~nMiddlePos;
}

/**
 * @param rKey - data to search for.
 * @return element number where the data was found or -1 if no data found.
 */
template <typename DATA_TYPE, class INSTANCE_TRAITS, class COMPARE_TRAITS>
size_t CArray<DATA_TYPE, INSTANCE_TRAITS, COMPARE_TRAITS>::LSearch(const DATA_TYPE& rKey) const
{
	for (size_t i = 0; i < m_nCount; ++i)
	{
		if (COMPARE_TRAITS::Compare(m_arrData[i], rKey))
			return i;
	}
	return MAXSIZE_T;
}

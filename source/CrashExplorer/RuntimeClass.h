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

#pragma once

#define _DECLARE_RUNTIME_CLASS() \
public: \
	static const CBaseRuntimeClass* GetThisClass() \
	{ return &m_class; } \
	virtual const CBaseRuntimeClass* GetRuntimeClass() const \
	{ return &m_class; }

#define DECLARE_ABSTRACT_RUNTIME_CLASS(CLASS) \
	_DECLARE_RUNTIME_CLASS() \
private: \
	static CAbstractRuntimeClass< CLASS > m_class; \

#define DECLARE_RUNTIME_CLASS(CLASS) \
	_DECLARE_RUNTIME_CLASS() \
private: \
	static CRuntimeClass< CLASS > m_class; \

#define IMPLEMENT_ABSTRACT_RUNTIME_CLASS(CLASS, BASE_CLASS) \
	CAbstractRuntimeClass< CLASS > CLASS::m_class(RUNTIME_CLASS(BASE_CLASS));

#define IMPLEMENT_RUNTIME_CLASS(CLASS, BASE_CLASS) \
	CRuntimeClass< CLASS > CLASS::m_class(RUNTIME_CLASS(BASE_CLASS));

#define RUNTIME_CLASS(CLASS) \
	CLASS::GetThisClass()

#define STATIC_DOWNCAST(CLASS, OBJECT) \
	RUNTIME::StaticDownCast< CLASS >(RUNTIME_CLASS(CLASS), OBJECT)

#define DYNAMIC_DOWNCAST(CLASS, OBJECT) \
	RUNTIME::DynamicDownCast< CLASS >(RUNTIME_CLASS(CLASS), OBJECT)

/// Base object class.
class CObject;

/// Runtime class.
class CBaseRuntimeClass
{
public:
	/// Initialize the runtime class.
	CBaseRuntimeClass(const CBaseRuntimeClass* pBaseClass);
	/// Creates an object during run time.
	virtual CObject* CreateObject() const = 0;
	/// Return pointer to the instance of base runtime class.
	const CBaseRuntimeClass* GetBaseClass() const;
	/// Determines if the class is derived from the specified class.
	bool IsDerivedFrom(const CBaseRuntimeClass* pBaseClass) const;

private:
	/// Pointer to base runtime class.
	const CBaseRuntimeClass* m_pBaseClass;
};

/// Abstract runtime class.
template <class CLASS>
class CAbstractRuntimeClass : public CBaseRuntimeClass
{
public:
	/// Initialize the runtime class.
	CAbstractRuntimeClass(const CBaseRuntimeClass* pBaseClass);
	/// Creates an object during run time.
	virtual CObject* CreateObject() const;
};

/// Runtime class.
template <class CLASS>
class CRuntimeClass : public CBaseRuntimeClass
{
public:
	/// Initialize the runtime class.
	CRuntimeClass(const CBaseRuntimeClass* pBaseClass);
	/// Creates an object during run time.
	virtual CObject* CreateObject() const;
};

/// Base object class.
class CObject
{
public:
	/// Destroy the object.
	virtual ~CObject();
	/// Tests this object's relationship to a given class.
	bool IsKindOf(const CBaseRuntimeClass* pClass) const;

private:
	/// Instance of root runtime class.
	DECLARE_RUNTIME_CLASS(CObject)
};

inline CObject::~CObject()
{
}

/**
 * @param pBaseClass - pointer to the base class.
 */
inline CBaseRuntimeClass::CBaseRuntimeClass(const CBaseRuntimeClass* pBaseClass)
{
	m_pBaseClass = pBaseClass;
}

/**
 * @return pointer to instance of the base runtime class.
 */
inline const CBaseRuntimeClass* CBaseRuntimeClass::GetBaseClass() const
{
	return m_pBaseClass;
}

/**
 * @return true if object is derived from a given class.
 */
inline bool CObject::IsKindOf(const CBaseRuntimeClass* pClass) const
{
	return GetRuntimeClass()->IsDerivedFrom(pClass);
}

/**
 * @param pBaseClass - pointer to the base class.
 */
template <class CLASS>
inline CAbstractRuntimeClass<CLASS>::CAbstractRuntimeClass(const CBaseRuntimeClass* pBaseClass) : CBaseRuntimeClass(pBaseClass)
{
}

/**
 * @return a pointer to the newly created object.
 */
template <class CLASS>
inline CObject* CAbstractRuntimeClass<CLASS>::CreateObject() const
{
	return NULL;
}

/**
 * @param pBaseClass - pointer to the base class.
 */
template <class CLASS>
inline CRuntimeClass<CLASS>::CRuntimeClass(const CBaseRuntimeClass* pBaseClass) : CBaseRuntimeClass(pBaseClass)
{
}

/**
 * @return a pointer to the newly created object.
 */
template <class CLASS>
inline CObject* CRuntimeClass<CLASS>::CreateObject() const
{
	return new CLASS();
}

namespace RUNTIME
{
	/**
	 * Casts @a pObject to a pointer to a @a CLASS object.
	 * @param pClass - runtime type information.
	 * @param pObject - the pointer to be cast to a pointer to a class_name object.
	 * @return pointer of required data type.
	 */
	template <class CLASS>
	inline CLASS* StaticDownCast(const CBaseRuntimeClass* pClass, CObject* pObject)
	{
		pClass;
		_ASSERTE(pObject == NULL || pObject->IsKindOf(pClass));
		return static_cast<CLASS*>(pObject);
	}

	/**
	 * Casts @a pObject to a pointer to a @a CLASS object.
	 * @param pClass - runtime type information.
	 * @param pObject - the pointer to be cast to a pointer to a class_name object.
	 * @return pointer of required data type.
	 */
	template <class CLASS>
	inline const CLASS* StaticDownCast(const CBaseRuntimeClass* pClass, const CObject* pObject)
	{
		pClass;
		_ASSERTE(pObject == NULL || pObject->IsKindOf(pClass));
		return static_cast<const CLASS*>(pObject);
	}

	/**
	 * Casts @a pObject to a pointer to a @a CLASS object.
	 * @param pClass - runtime type information.
	 * @param pObject - the pointer to be cast to a pointer to a class_name object.
	 * @return pointer of required data type.
	 */
	template <class CLASS>
	inline CLASS* DynamicDownCast(const CBaseRuntimeClass* pClass, CObject* pObject)
	{
		return (pObject != NULL && pObject->IsKindOf(pClass) ? static_cast<CLASS*>(pObject) : NULL);
	}

	/**
	 * Casts @a pObject to a pointer to a @a CLASS object.
	 * @param pClass - runtime type information.
	 * @param pObject - the pointer to be cast to a pointer to a class_name object.
	 * @return pointer of required data type.
	 */
	template <class CLASS>
	inline const CLASS* DynamicDownCast(const CBaseRuntimeClass* pClass, const CObject* pObject)
	{
		return (pObject != NULL && pObject->IsKindOf(pClass) ? static_cast<const CLASS*>(pObject) : NULL);
	}
}

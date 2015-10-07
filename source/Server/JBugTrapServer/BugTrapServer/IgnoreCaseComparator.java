/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Comparator for ignoring case distinctions.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

package BugTrapServer;

import java.util.*;

/** Comparator for ignoring case distinctions. */
final class IgnoreCaseComparator implements Comparator<String> {
	/** Private object constructor. */
	private IgnoreCaseComparator() {
	}

	/**
	 * Compare two objects. This function correctly handlers null references.
	 * @param str1 - first object.
	 * @param str2 - second object.
	 * @return comparison result.
	 */
	public int compare(String str1, String str2) {
		return str1.compareToIgnoreCase(str2);
	}

	/**
	 * Get shared instance of the comparator object.
	 * @return shared instance of the object.
	 */
	public static IgnoreCaseComparator getInstance() {
		return instance;
	}

	/** Comparator instance. */
	private static IgnoreCaseComparator instance = new IgnoreCaseComparator();
}

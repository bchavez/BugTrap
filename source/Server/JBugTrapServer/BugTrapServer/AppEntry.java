/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Application entry info block.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

package BugTrapServer;

/** Application entry info block. */
class AppEntry implements Comparable<AppEntry> {
	/** Application name. */
	public String name;
	/** Application version. */
	public String version;

	/** Initialize the object. */
	public AppEntry() {
		this.name = null;
		this.version = null;
	}

	/**
	 * Initialize the object.
	 * @param name - application name.
	 */
	public AppEntry(String name) {
		this.name = name;
		this.version = null;
	}

	/**
	 * Initialize the object.
	 * @param name - application name.
	 * @param version - application version.
	 */
	public AppEntry(String name, String version) {
		this.name = name;
		this.version = version;
	}

	/**
	 * Compare two objects. This function correctly handlers null references.
	 * @param obj1 - first object.
	 * @param obj2 - second object.
	 * @return comparison result.
	 */
	private static < T extends Comparable<T> > int compareObjects(T obj1, T obj2)
	{
		if (obj1 == null && obj2 == null)
			return 0;
		else if (obj1 == null)
			return -1;
		else if (obj2 == null)
			return +1;
		else
			return obj1.compareTo(obj2);
	}

	/**
	 * Compares the current instance with another object of the same type.
	 * @param appEntry - an object to compare with this instance.
	 * @return a 32-bit signed integer that indicates the relative order of the comparands.
	 */
	public int compareTo(AppEntry appEntry) {
		int result = compareObjects(this.name, appEntry.name);
		if (result == 0)
			result = compareObjects(this.version, appEntry.version);
		return result;
	}

	/**
	 * Serves as a hash function for a particular type, suitable for use in hashing algorithms and data structures like a hash table.
	 * @return a hash code for the current Object.
	 */
	public int hashCode() {
		int hashCode = 0;
		if (this.name != null)
			hashCode ^= this.name.hashCode();
		if (this.version != null)
			hashCode ^= this.version.hashCode();
		return hashCode;
	}

	/**
	 * Determines whether the specified Object is equal to the current Object.
	 * @param obj - the Object to compare with the current Object.
	 * @return true if the specified Object is equal to the current Object; otherwise, false.
	 */
	public boolean equals(Object obj) {
		return (this.compareTo((AppEntry)obj) == 0);
	}
}

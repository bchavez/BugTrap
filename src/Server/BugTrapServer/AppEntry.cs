/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Application entry.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.Xml.Serialization;

namespace BugTrapServer
{
	/// <summary>
	/// Application entry info block.
	/// </summary>
	[Serializable, XmlRoot("application")]
	public sealed class AppEntry : IComparable
	{
		/// <summary>
		/// Application name.
		/// </summary>
		[XmlText(Type = typeof(string))]
		public string Name;

		/// <summary>
		/// Application version.
		/// </summary>
		public string Version;

		/// <summary>
		/// Initialize the object.
		/// </summary>
		public AppEntry()
		{
			this.Name = null;
			this.Version = null;
		}

		/// <summary>
		/// Initialize the object.
		/// </summary>
		/// <param name="name">Application name.</param>
		public AppEntry(string name)
		{
			this.Name = name;
			this.Version = null;
		}

		/// <summary>
		/// Initialize the object.
		/// </summary>
		/// <param name="name">Application name.</param>
		/// <param name="version">Application version.</param>
		public AppEntry(string name, string version)
		{
			this.Name = name;
			this.Version = version;
		}

		/// <summary>
		/// Compare two objects.
		/// </summary>
		/// <remarks>This function correctly handlers null references.</remarks>
		/// <param name="obj1">First object.</param>
		/// <param name="obj2">Second object.</param>
		/// <returns>Comparison result.</returns>
		private static int CompareObjects(IComparable obj1, IComparable obj2)
		{
			if (obj1 == null && obj2 == null)
				return 0;
			else if (obj1 == null)
				return -1;
			else if (obj2 == null)
				return +1;
			else
				return obj1.CompareTo(obj2);
		}

		/// <summary>
		/// Compares the current instance with another object of the same type.
		/// </summary>
		/// <param name="obj">An object to compare with this instance.</param>
		/// <returns>A 32-bit signed integer that indicates the relative order of the comparands.</returns>
		int IComparable.CompareTo(object obj)
		{
			AppEntry appEntry = obj as AppEntry;
			if (appEntry == null)
				return +1;
			int result = CompareObjects(this.Name, appEntry.Name);
			if (result == 0)
				result = CompareObjects(this.Version, appEntry.Version);
			return result;
		}

		/// <summary>
		/// Determines whether the specified Object is equal to the current Object.
		/// </summary>
		/// <param name="obj">The Object to compare with the current Object.</param>
		/// <returns>True if the specified Object is equal to the current Object; otherwise, false.</returns>
		public override bool Equals(object obj)
		{
			IComparable compObj = this;
			return (compObj.CompareTo(obj) == 0);
		}

		/// <summary>
		/// Serves as a hash function for a particular type, suitable for use in hashing algorithms and data structures like a hash table.
		/// </summary>
		/// <returns>A hash code for the current Object.</returns>
		public override int GetHashCode()
		{
			int hashCode = 0;
			if (this.Name != null)
				hashCode ^= this.Name.GetHashCode();
			if (this.Version != null)
				hashCode ^= this.Version.GetHashCode();
			return hashCode;
		}
	}
}

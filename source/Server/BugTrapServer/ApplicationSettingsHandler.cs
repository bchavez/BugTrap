/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Custom XML section handler.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

using System;
using System.Configuration;
using System.Xml;
using System.Xml.Serialization;

namespace BugTrapServer
{
	/// <summary>
	/// Implements IConfigurationSectionHandler interface for application lists.
	/// </summary>
	public class ApplicationSettingsHandler : IConfigurationSectionHandler
	{
		/// <summary>
		/// Implemented by all configuration section handlers to parse the XML of the configuration section.
		/// The returned object is added to the configuration collection and is accessed by GetConfig.
		/// </summary>
		/// <param name="parent">The configuration settings in a corresponding parent configuration section.</param>
		/// <param name="configContext">An HttpConfigurationContext when Create is called from the ASP.NET configuration system.
		/// Otherwise, this parameter is reserved and is a null reference (Nothing in Visual Basic).</param>
		/// <param name="section">The XmlNode that contains the configuration information from the configuration file.
		/// Provides direct access to the XML contents of the configuration section.</param>
		/// <returns>A configuration object.</returns>
		object IConfigurationSectionHandler.Create(object parent, object configContex, XmlNode section)
		{
			XmlSerializer ser = new XmlSerializer(typeof(ApplicationSettings));
			XmlNodeReader reader = new XmlNodeReader(section);
			return ser.Deserialize(reader);
		}
	}
}

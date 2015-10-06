/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Web server installation script.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

function HandleError(err) {
	WScript.Echo(err.number + ': ' + (err.description != '' ? err.description : 'Unknown error'));
	WScript.Quit();
}

try {
	// Global file system object
	var fso = new ActiveXObject('Scripting.FileSystemObject');
	// Global registry provider
	var HKEY_LOCAL_MACHINE = 0x80000002;
	var wbemLoc = new ActiveXObject('WbemScripting.SWbemLocator');
	var wbemSrv = wbemLoc.ConnectServer(null, 'root\\default');
	var regProv = wbemSrv.Get('StdRegProv');
}
catch (err) {
	HandleError(err);
}

function combinePath(str1, str2) {
	if (str1.length > 0 && str1.charAt(str1.length - 1) == '\\') {
		return (str1 + str2);
	}
	else {
		return (str1 + '\\' + str2);
	}
}

function getStringValue(key, subKeyName, valueName) {
	var menthod = regProv.Methods_.Item("GetStringValue");
	var inParams = menthod.InParameters.SpawnInstance_();
	inParams.hDefKey = key;
	inParams.sSubKeyName = subKeyName;
	inParams.sValueName = valueName;
	var outParams = regProv.ExecMethod_(menthod.Name, inParams);
	return (outParams.ReturnValue == 0 ? outParams.sValue : null);
}

function enumKey(key, subKeyName) {
	var menthod = regProv.Methods_.Item("EnumKey");
	var inParams = menthod.InParameters.SpawnInstance_();
	inParams.hDefKey = key;
	inParams.sSubKeyName = subKeyName;
	var outParams = regProv.ExecMethod_(menthod.Name, inParams);
	return (outParams.ReturnValue == 0 ? outParams.sNames.toArray() : null);
}

function enumValues(key, subKeyName) {
	var menthod = regProv.Methods_.Item("EnumValues");
	var inParams = menthod.InParameters.SpawnInstance_();
	inParams.hDefKey = key;
	inParams.sSubKeyName = subKeyName;
	var outParams = regProv.ExecMethod_(menthod.Name, inParams);
	return (outParams.ReturnValue == 0 ? outParams.sNames.toArray() : null);
}

function createKey(key, subKeyName, valueName, value) {
	var menthod = regProv.Methods_.Item("CreateKey");
	var inParams = menthod.InParameters.SpawnInstance_();
	inParams.hDefKey = key;
	inParams.sSubKeyName = subKeyName;
	var outParams = regProv.ExecMethod_(menthod.Name, inParams);
	return (outParams.ReturnValue == 0);
}

function setStringValue(key, subKeyName, valueName, value) {
	var menthod = regProv.Methods_.Item("SetStringValue");
	var inParams = menthod.InParameters.SpawnInstance_();
	inParams.hDefKey = key;
	inParams.sSubKeyName = subKeyName;
	inParams.sValueName = valueName;
	inParams.sValue = value;
	var outParams = regProv.ExecMethod_(menthod.Name, inParams);
	return (outParams.ReturnValue == 0);
}

function getDotNetFrameworkPath() {
	var dotNetFrameworkRegPath = 'SOFTWARE\\Microsoft\\.NETFramework';
	var dotNetFrameworkRegInstallValue = getStringValue(HKEY_LOCAL_MACHINE, dotNetFrameworkRegPath, 'InstallRoot');
	if (dotNetFrameworkRegInstallValue != null) {
		var dotNetFrameworkPolicyRegPath = dotNetFrameworkRegPath + '\\policy';
		var policies = enumKey(HKEY_LOCAL_MACHINE, dotNetFrameworkPolicyRegPath);
		var regExp = /^v(\d)\.(\d)$/;
		for (var i = 0; i < policies.length; ++i) {
			policy = policies[i];
			var results = policy.match(regExp);
			if (results != null) {
				var majorVer = results[1];
				var minorVer = results[2];
				if (majorVer >= 2) {
					var dotNetFrameworkPolicyVerRegPath = dotNetFrameworkPolicyRegPath + '\\' + policy;
					var values = enumValues(HKEY_LOCAL_MACHINE, dotNetFrameworkPolicyVerRegPath, values);
					for (var j = 0; j < values.length; ++j) {
						dotNetFrameworkDir = combinePath(dotNetFrameworkRegInstallValue, policy + '.' + values[j]);
						if (fso.FolderExists(dotNetFrameworkDir)) {
							return dotNetFrameworkDir;
						}
					}
				}
			}
		}
	}
	return null;
}

//try {
	var virtDirName = 'BugTrapWebServer';
	// Construct source path
	var sourcePath = WScript.ScriptFullName;
	sourcePath = sourcePath.substr(0, sourcePath.lastIndexOf('\\'));
	// Initialize IIS objects
	var IIS = new ActiveXObject('IISNamespace');
	var webService = IIS.GetObject('IIsWebService', 'localhost/W3SVC');
	var webServer = webService.GetObject('IIsWebServer', '1');
	var webRoot = webServer.GetObject('IIsWebVirtualDir', 'Root');
	// Construct destination path
	var destPath = webRoot.Path;
    // setup environment variable for deployment
    var sh = WScript.CreateObject("WScript.Shell");
    var sysEnv = sh.Environment("SYSTEM");
    sysEnv("BT_WEBSRV_ROOT") = destPath;
	// Check if web site files should be copied or just registered in place
	var copyFiles = sourcePath.substr(0, destPath.length).toLowerCase() != destPath.toLowerCase();
	if (copyFiles) {
		// Append default Web application name to the path
		destPath = combinePath(destPath, virtDirName);
	}
	else {
		// Use existing path unmodified
		destPath = sourcePath;
	}
	// Unregister old Web application (if exists)
	try {
		var webSite = webRoot.GetObject('IIsWebVirtualDir', virtDirName);
		webSite.DeleteObject(0);
	}
	catch (err) {
		// Web application may not be registered - ignore error in this case
	}
	if (copyFiles) {
		if (fso.FolderExists(destPath)) {
            fso.DeleteFolder(destPath, true);
		}
        WScript.Sleep(1);
		fso.CreateFolder(destPath);
	}
	// Register new Web application directory in IIS
	var webSite = webRoot.Create('IIsWebVirtualDir', virtDirName);
	webSite.AccessRead = true;
	webSite.AccessScript = true;
	webSite.EnableDefaultDoc = false;
	webSite.AppAllowDebugging = false;
	webSite.AppAllowClientDebug = false;
	webSite.AspAllowSessionState = false;
	webSite.ContentIndexed = false;
	webSite.AppIsolated = 1; // Isolated process
	webSite.Path = destPath;
	webSite.AppCreate(true);
	webSite.AppFriendlyName = virtDirName;
	webSite.SetInfo();
	// Get location of .NET Framework
	var dotNetFrameworkDir = getDotNetFrameworkPath();
	if (dotNetFrameworkDir != null) {
		// Register new event source
		var eventLogEntryRegPath = 'SYSTEM\\CurrentControlSet\\Services\\Eventlog\\Application\\' + virtDirName;
		if (createKey(HKEY_LOCAL_MACHINE, eventLogEntryRegPath)) {
			setStringValue(HKEY_LOCAL_MACHINE, eventLogEntryRegPath, 'EventMessageFile', combinePath(dotNetFrameworkDir, 'EventLogMessages.dll'));
		}
	}
//}
//catch (err) {
//	HandleError(err);
//}

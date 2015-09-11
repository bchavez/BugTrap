#include "StdAfx.h"

#ifdef _MANAGED

#pragma managed

using namespace System::Reflection;
using namespace System::Runtime::CompilerServices;
using namespace System::Runtime::InteropServices;

// General Information about an assembly is controlled through the following
// set of attributes. Change these attribute values to modify the information
// associated with an assembly.
[assembly: AssemblyTitle("BugTrap dynamic link library")];
[assembly: AssemblyDescription("BugTrap - software error reporting tool")];
[assembly: AssemblyConfiguration("")];
[assembly: AssemblyCompany("IntelleSoft")];
[assembly: AssemblyProduct("BugTrap")];
[assembly: AssemblyCopyright("Copyright © 2005-2009 IntelleSoft")];
[assembly: AssemblyTrademark("")];
[assembly: AssemblyCulture("")];

// Setting ComVisible to false makes the types in this assembly not visible
// to COM components.  If you need to access a type in this assembly from
// COM, set the ComVisible attribute to true on that type.
[assembly: ComVisible(false)];

// The following GUID is for the ID of the typelib if this project is exposed to COM
[assembly: Guid("B3DB7206-529E-4000-B549-CA23E08A37F0")];

// Version information for an assembly consists of the following four values:
//
//      Major Version
//      Minor Version
//      Build Number
//      Revision
//
[assembly: AssemblyVersion("1.3.3718.38316")];
[assembly: AssemblyFileVersion("1.3.3718.38316")];

using namespace System::Security::Permissions;

[assembly: SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];

#pragma unmanaged

#endif // _MANAGED

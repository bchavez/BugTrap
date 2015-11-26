/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: .NET symbolic engine.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#ifdef _MANAGED

#include "BugTrapNet.h"

#pragma managed

using namespace System;
using namespace System::Threading;
using namespace System::Diagnostics;
using namespace System::Text;
using namespace System::Collections;
using namespace System::Reflection;

namespace IntelleSoft
{
	namespace BugTrap
	{
		private ref class StackFrameEnumerator
		{
		public:
			StackFrameEnumerator(Thread^ thread);
			StackFrameEnumerator(Exception^ exception);
			void InitStackTrace(void);
			StackFrame^ GetFirstStackTraceEntry(void);
			StackFrame^ GetNextStackTraceEntry(void);
			property System::Exception^ Exception
			{
				System::Exception^ get(void);
			}

		private:
			initonly StackTrace^ stackTrace;
			initonly System::Exception^ exception;
			int frameIndex;
		};

		inline StackFrameEnumerator::StackFrameEnumerator(Thread^ thread)
		{
			this->stackTrace = gcnew StackTrace(thread, true);
			this->frameIndex = int::MaxValue;
		}

		inline StackFrameEnumerator::StackFrameEnumerator(System::Exception^ exception)
		{
			this->stackTrace = gcnew StackTrace(exception, true);
			this->frameIndex = int::MaxValue;
			this->exception = exception;
		}

		inline void StackFrameEnumerator::InitStackTrace(void)
		{
			this->frameIndex = 0;
		}

		inline StackFrame^ StackFrameEnumerator::GetFirstStackTraceEntry(void)
		{
			this->InitStackTrace();
			return this->GetNextStackTraceEntry();
		}

		inline System::Exception^ StackFrameEnumerator::Exception::get(void)
		{
			return this->exception;
		}

		private ref class AssemblyEnumerator
		{
		public:
			AssemblyEnumerator(void);
			void InitAssemblies(void);
			Assembly^ GetFirstAssembly(void);
			Assembly^ GetNextAssembly(void);

		private:
			initonly array<Assembly^>^ assemblies;
			IEnumerator^ assemblyEnumerator;
		};

		inline void AssemblyEnumerator::InitAssemblies(void)
		{
			this->assemblyEnumerator->Reset();
		}

		inline Assembly^ AssemblyEnumerator::GetFirstAssembly(void)
		{
			this->InitAssemblies();
			return this->GetNextAssembly();
		}
	}
}

#pragma unmanaged

#endif // _MANAGED

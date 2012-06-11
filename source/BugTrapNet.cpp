/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: .NET interface to BugTrap.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "BugTrapNet.h"

#ifdef _MANAGED

#pragma managed

using namespace System::Text;

namespace IntelleSoft
{
	namespace BugTrap
	{
		Object^ LogFilesEnumerator::Current::get(void)
		{
			if (this->index < 0 || this->index >= LogFiles::Count)
				throw gcnew InvalidOperationException();
			return LogFiles::GetEntry(this->index);
		}

		bool LogFilesEnumerator::MoveNext(void)
		{
			int count = LogFiles::Count;
			if (this->index >= count)
				return false;
			return (++this->index < count);
		}

		Object^ LogFiles::GetLogFileEntry(INT_PTR index, bool getByIndex)
		{
			BUGTRAP_LOGTYPE type;
			DWORD size, result;
			result = BT_GetLogFileEntry(index, getByIndex, &type, NULL, NULL);
			if (result != ERROR_SUCCESS)
				throw gcnew Win32Exception(result);
			switch (type)
			{
			case BTLT_LOGFILE:
				{
					BUGTRAP_LOGFILEENTRY entry;
					size = sizeof(entry);
					result = BT_GetLogFileEntry(index, getByIndex, &type, &size, &entry);
					if (result != ERROR_SUCCESS)
						throw gcnew Win32Exception(result);
					return gcnew LogFileEntry(gcnew String(entry.szLogFileName));
				}
			case BTLT_REGEXPORT:
				{
					BUGTRAP_REGEXPORTENTRY entry;
					size = sizeof(entry);
					result = BT_GetLogFileEntry(index, getByIndex, &type, &size, &entry);
					if (result != ERROR_SUCCESS)
						throw gcnew Win32Exception(result);
					return gcnew RegExportEntry(gcnew String(entry.szLogFileName), gcnew String(entry.szRegKey));
				}
			default:
				throw gcnew InvalidOperationException();
			}
		}

		void ExceptionHandler::HandleException(System::Exception^ exception, Object^ sender, UnhandledExceptionEventArgs^ args)
		{
			Monitor::Enter(syncObj);
			try
			{
				Exception = exception;
				Sender = sender;
				Arguments = args;
				BT_CallNetFilter();
			}
			finally
			{
				Exception = nullptr;
				Sender = nullptr;
				Arguments = nullptr;
				Monitor::Exit(syncObj);
			}
		}

		void ExceptionHandler::HandleException(System::Exception^ exception)
		{
			UnhandledExceptionEventArgs^ args = gcnew UnhandledExceptionEventArgs(exception, ExceptionType::DomainException);
			HandleException(exception, nullptr, args);
		}

		void ExceptionHandler::ReadVersionInfo(AssemblyName^ assemblyName)
		{
			ExceptionHandler::AppName = assemblyName->Name;
			Version^ version = assemblyName->Version;
			ExceptionHandler::AppVersion = version != nullptr ? version->ToString() : nullptr;
		}

		void ExceptionHandler::InstallHandler(void)
		{
			BT_InstallSehFilter();
			AppDomain::CurrentDomain->UnhandledException += gcnew UnhandledExceptionEventHandler(OnUnhandledException);
			Application::ThreadException += gcnew ThreadExceptionEventHandler(OnThreadException);
		}

		void ExceptionHandler::UninstallHandler(void)
		{
			AppDomain::CurrentDomain->UnhandledException -= gcnew UnhandledExceptionEventHandler(OnUnhandledException);
			Application::ThreadException -= gcnew ThreadExceptionEventHandler(OnThreadException);
			BT_UninstallSehFilter();
		}

		void ExceptionHandler::SaveSnapshot(System::Exception^ exception, String^ fileName)
		{
			try
			{
				Exception = exception;
				SaveSnapshot(fileName);
			}
			finally
			{
				Exception = nullptr;
			}
		}

		void ExceptionHandler::SendSnapshot(System::Exception^ exception)
		{
			try
			{
				Exception = exception;
				SendSnapshot();
			}
			finally
			{
				Exception = nullptr;
			}
		}

		void ExceptionHandler::MailSnapshot(System::Exception^ exception)
		{
			try
			{
				Exception = exception;
				MailSnapshot();
			}
			finally
			{
				Exception = nullptr;
			}
		}

		void LogFile::PrvOpen(String^ fileName, LogFormatType logFormat)
		{
			pin_ptr<const wchar_t> wstrFileName(PtrToStringChars(fileName));
			this->handle = (IntPtr)BT_OpenLogFile(wstrFileName, (BUGTRAP_LOGFORMAT)logFormat);
			if (this->handle == IntPtr::Zero)
				throw gcnew IOException();
		}

		void LogFile::Close(void)
		{
			if (this->handle != IntPtr::Zero)
			{
				ValidateIoResult(BT_CloseLogFile((INT_PTR)this->handle));
				Detach();
			}
		}

		void LogFile::Attach(IntPtr handle)
		{
			ValidateHandle();
			this->handle = handle;
		}

		void LogFile::Flush(void)
		{
			ValidateHandle();
			ValidateIoResult(BT_FlushLogFile((INT_PTR)this->handle));
		}

		String^ LogFile::FileName::get(void)
		{
			ValidateHandle();
			return gcnew String(BT_GetLogFileName((INT_PTR)this->handle));
		}

		int LogFile::LogSizeInEntries::get(void)
		{
			ValidateHandle();
			return BT_GetLogSizeInEntries((INT_PTR)this->handle);
		}

		void LogFile::LogSizeInEntries::set(int value)
		{
			ValidateHandle();
			ValidateIoResult(BT_SetLogSizeInEntries((INT_PTR)this->handle, value));
		}

		int LogFile::LogSizeInBytes::get(void)
		{
			ValidateHandle();
			return BT_GetLogSizeInBytes((INT_PTR)this->handle);
		}

		void LogFile::LogSizeInBytes::set(int value)
		{
			ValidateHandle();
			ValidateIoResult(BT_SetLogSizeInBytes((INT_PTR)this->handle, value));
		}

		LogFlagsType LogFile::LogFlags::get(void)
		{
			ValidateHandle();
			return (LogFlagsType)BT_GetLogFlags((INT_PTR)this->handle);
		}

		void LogFile::LogFlags::set(LogFlagsType value)
		{
			ValidateHandle();
			ValidateIoResult(BT_SetLogFlags((INT_PTR)this->handle, (DWORD)value));
		}

		LogLevelType LogFile::LogLevel::get(void)
		{
			ValidateHandle();
			return (LogLevelType)BT_GetLogLevel((INT_PTR)this->handle);
		}

		void LogFile::LogLevel::set(LogLevelType value)
		{
			ValidateHandle();
			ValidateIoResult(BT_SetLogLevel((INT_PTR)this->handle, (BUGTRAP_LOGLEVEL)value));
		}

		LogEchoType LogFile::LogEchoMode::get(void)
		{
			ValidateHandle();
			return (LogEchoType)BT_GetLogEchoMode((INT_PTR)this->handle);
		}

		void LogFile::LogEchoMode::set(LogEchoType value)
		{
			ValidateHandle();
			ValidateIoResult(BT_SetLogEchoMode((INT_PTR)this->handle, (DWORD)value));
		}

		void LogFile::Clear(void)
		{
			ValidateHandle();
			ValidateIoResult(BT_ClearLog((INT_PTR)this->handle));
		}

		void LogFile::Insert(LogLevelType logLevel, String^ entry)
		{
			ValidateHandle();
			pin_ptr<const wchar_t> wstrEntry(PtrToStringChars(entry));
			ValidateIoResult(BT_InsLogEntry((INT_PTR)this->handle, (BUGTRAP_LOGLEVEL)logLevel, wstrEntry));
		}

		void LogFile::Insert(LogLevelType logLevel, String^ format, ... array<Object^>^ args)
		{
			ValidateHandle();
			StringBuilder^ entry = gcnew StringBuilder();
			entry->AppendFormat(format, args);
			pin_ptr<const wchar_t> wstrEntry(PtrToStringChars(entry->ToString()));
			ValidateIoResult(BT_InsLogEntry((INT_PTR)this->handle, (BUGTRAP_LOGLEVEL)logLevel, wstrEntry));
		}

		void LogFile::Append(LogLevelType logLevel, String^ entry)
		{
			ValidateHandle();
			pin_ptr<const wchar_t> wstrEntry(PtrToStringChars(entry));
			ValidateIoResult(BT_AppLogEntry((INT_PTR)this->handle, (BUGTRAP_LOGLEVEL)logLevel, wstrEntry));
		}

		void LogFile::Append(LogLevelType logLevel, String^ format, ... array<Object^>^ args)
		{
			ValidateHandle();
			StringBuilder^ entry = gcnew StringBuilder();
			entry->AppendFormat(format, args);
			pin_ptr<const wchar_t> wstrEntry(PtrToStringChars(entry->ToString()));
			ValidateIoResult(BT_AppLogEntry((INT_PTR)this->handle, (BUGTRAP_LOGLEVEL)logLevel, wstrEntry));
		}
	}
}

#pragma unmanaged

#endif // _MANAGED

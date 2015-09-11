// BugTrapLogTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define NUM_THREADS  10
#define NUM_MESSAGES 1000

BTTrace g_Log;
volatile LONG g_nStartedThreadCount;
volatile LONG g_nDeadThreadCount;
HANDLE g_hThreadDeathEvent;
HANDLE g_hThreadStartEvent;
HANDLE g_hThreadsReadyEvenet;

static void ThreadFunc(void* args)
{
	int nThreadID = PtrToInt(args);

	g_Log.AppendF(_T("Thread %d: Entering ThreadFunc() function"), nThreadID);

	InterlockedDecrement(&g_nStartedThreadCount);
	SetEvent(g_hThreadStartEvent);

	WaitForSingleObject(g_hThreadsReadyEvenet, INFINITE);

	// Generating some activity...
	for (int nIteration = 1; nIteration <= NUM_MESSAGES; ++nIteration)
	{
		g_Log.AppendF(_T("Thread %d: Iteration %d"), nThreadID, nIteration);
		Sleep(0); // Enforce thread context switching
	}

	g_Log.AppendF(_T("Thread %d: Leaving ThreadFunc() function"), nThreadID);

	InterlockedDecrement(&g_nDeadThreadCount);
	SetEvent(g_hThreadDeathEvent);
}

static BOOL TestLogging(PCTSTR pszLogFileName, BUGTRAP_LOGFORMAT eLogFormat)
{
	_tprintf(_T("Generating \"%s\" ..."), pszLogFileName);

	TCHAR szLogFilePath[MAX_PATH];
	GetModuleFileName(NULL, szLogFilePath, ARRAYSIZE(szLogFilePath));
	PathRemoveFileSpec(szLogFilePath);
	TCHAR szFullLogFileName[MAX_PATH];
	PathCombine(szFullLogFileName, szLogFilePath, pszLogFileName);

	if (! g_Log.Open(szFullLogFileName, eLogFormat))
	{
		_tprintf(_T("ERROR: can't open or create log file \"%s\".\n")
			_T("Make sure you have enough permissions to create a file in \"%s\".\n"),
			pszLogFileName,
			szLogFilePath);
		return FALSE;
	}

	g_Log.SetLogFlags(BTLF_SHOWTIMESTAMP);
	g_Log.Clear();

	g_Log.AppendF(_T("Entering TestLogging(\"%s\") function"), pszLogFileName);

	g_hThreadsReadyEvenet = CreateEvent(NULL, TRUE, FALSE, NULL); // non-signaled, manual-reset event
	g_hThreadStartEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // non-signaled, auto-reset event
	g_hThreadDeathEvent = CreateEvent(NULL, FALSE, FALSE, NULL); // non-signaled, auto-reset event

	// Starting worker threads...
	for (int nThreadID = 1; nThreadID <= NUM_THREADS; ++nThreadID)
	{
		InterlockedIncrement(&g_nStartedThreadCount);
		InterlockedIncrement(&g_nDeadThreadCount);
		_beginthread(ThreadFunc, 0, IntToPtr(nThreadID));
	}

	do
		WaitForSingleObject(g_hThreadStartEvent, INFINITE);
	while (InterlockedExchangeAdd(&g_nStartedThreadCount, 0) > 0);
	DWORD dwStartTicks = GetTickCount();
	SetEvent(g_hThreadsReadyEvenet);
	do
		WaitForSingleObject(g_hThreadDeathEvent, INFINITE);
	while (InterlockedExchangeAdd(&g_nDeadThreadCount, 0) > 0);
	DWORD dwEndTicks = GetTickCount();

	CloseHandle(g_hThreadStartEvent);
	CloseHandle(g_hThreadDeathEvent);
	CloseHandle(g_hThreadsReadyEvenet);

	g_Log.Append(_T("Leaving TestLogging() function"));
	g_Log.Close();

	_tprintf(_T(" %lu ms\n"), dwEndTicks - dwStartTicks);
	return TRUE;
}

void _tmain()
{
	_tprintf(
		_T("TEST LOG GENERATOR\n")
		_T("Number of simultaneous threads: %d\n")
		_T("Number of messages per thread:  %d\n")
		_T("\n"),
		NUM_THREADS,
		NUM_MESSAGES);

	if (! TestLogging(_T("text_mode.log"), BTLF_TEXT))
		return;
	if (! TestLogging(_T("stream_mode.log"), BTLF_STREAM))
		return;

	_tprintf(_T("\n"));
}

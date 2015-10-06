#include "stdafx.h"
#include "UnmanagedThread.h"

#pragma unmanaged

static void ThreadFunc(void* /*args*/)
{
	// Throwing access violation
	int* ptr = 0;
	*ptr = 0;
}

void StartUnmanagedThread()
{
	// Starting worker thread
	_beginthread(ThreadFunc, 0, NULL);
}

#pragma managed

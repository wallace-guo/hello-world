#include "test_tls.h"
#include <stdio.h>
#include <windows.h>
#include <process.h>

DWORD g_tlsUsedTime;
// get thread time use tls
UINT __stdcall ThreadFunc(LPVOID)
{
	int i;

	DWORD dwStart = ::GetTickCount();
	::TlsSetValue(g_tlsUsedTime, (LPVOID)dwStart);

	// thread running 
	i = 10000*10000;
	while(i--) { }
	
	DWORD dwElapsed = ::GetTickCount();
	dwElapsed = dwElapsed - (DWORD)::TlsGetValue(g_tlsUsedTime);

	//print time
	printf(" This thread is coming to end. Thread ID: %-5d, Used Time: %d \n", 
		::GetCurrentThreadId(), dwElapsed);
	return 0;
}

void test_tls()
{
	
	UINT uId;
	int i;
	HANDLE h[10];

	// alloc
	g_tlsUsedTime = ::TlsAlloc(); 

	// created 10 threads
	for(i=0; i<10; i++)
	{
		h[i] = (HANDLE)::_beginthreadex(NULL, 0, ThreadFunc, NULL, 0, &uId);
	}
	for(i=0; i<10; i++)
	{
		::WaitForSingleObject(h[i], INFINITE);
		::CloseHandle(h[i]);
	}

	// free
	::TlsFree(g_tlsUsedTime);
}
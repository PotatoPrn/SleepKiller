#include "Class.h"

#include <TlHelp32.h>
#include <Windows.h>


bool SleepKiller::GetPID(const char* TargetProcess)
{
	HANDLE hProcessSnap;
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	do
	{
		if (_strcmpi(TargetProcess, pe32.szExeFile) == 0)
		{
			PID = pe32.th32ProcessID;
			CloseHandle(hProcessSnap);
			return true;
		}
	}
	while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return false;
}

#pragma once

#include <Windows.h>
#include <iostream>

class SleepKiller
{
public:
	DWORD PID;
	HANDLE pHandle;

	bool GetPID(const char* ProcessName);

	void* PatternScan(char* Module, char* Pattern, char* Mask);

	bool ResolveFunctionAddress();

	bool KillFunctions();

	struct
	{
		bool SleepFunction = false;
	} EnabledUtils;

	struct
	{
		uintptr_t SleepAddress;
	} FunctionAddress;


	BOOL MemoryPatch(BYTE* Dst, BYTE* Src, unsigned int Size)
	{
		DWORD ProcID;

		if (VirtualProtectEx(pHandle, Dst, Size, PAGE_EXECUTE_READWRITE, &ProcID) == 0)
		{
			return false;
		}

		if (WriteProcessMemory(pHandle, Dst, Src, Size, nullptr) == 0)
		{
			return false;
		}

		if (VirtualProtectEx(pHandle, Dst, Size, ProcID, &ProcID) == 0)
		{
			return false;
		}

		return true;
	}
};

extern SleepKiller Skiller;

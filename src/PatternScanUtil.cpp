#include <Windows.h>
#include <TlHelp32.h>

#include "Class.h"

void* PatternScanInternal(char* Base, size_t Size, char* Pattern, char* Mask)
{
	/* Base = Buffer to be scanned
	 * Size = size of buffer
	 */
	size_t PatternLen = strlen(Mask);

	for (unsigned int i = 0; i < Size - PatternLen; i++)
	{
		bool Found = true;

		for (unsigned int j = 0; j < PatternLen; j++)
		{
			if (Mask[j] != '?' && Pattern[j] != *(Base + i + j))
			{
				Found = false;
				break;
			}
		}
		if (Found)
		{
			return (void*)(Base + i);
		}
	}
	return nullptr;
}

void* ExtPScanWrapper(HANDLE hProcess, uintptr_t Begin, uintptr_t End, char* Pattern, char* Mask)
{
	uintptr_t CurrentChunk = Begin;
	SIZE_T BytesRead;

	while (CurrentChunk < End)
	{
		// 4096 is the max size of each mem page
		char Buffer[4096];

		DWORD OldProc;
		VirtualProtectEx(hProcess, (void*)CurrentChunk, sizeof(Buffer), PAGE_EXECUTE_READWRITE, &OldProc);
		ReadProcessMemory(hProcess, (void*)CurrentChunk, &Buffer, sizeof(Buffer), &BytesRead);
		VirtualProtectEx(hProcess, (void*)CurrentChunk, sizeof(Buffer), OldProc, &OldProc);

		if (BytesRead == 0)
		{
			return nullptr;
		}

		void* InternelAddress = PatternScanInternal((char*)&Buffer, BytesRead, Pattern, Mask);

		if (InternelAddress != nullptr)
		{
			uintptr_t OffsetFromBuffer = (uintptr_t)InternelAddress - (uintptr_t)&Buffer;
			return (void*)(CurrentChunk + OffsetFromBuffer);
		}
		else
		{
			CurrentChunk = CurrentChunk + BytesRead;
		}
	}
	return nullptr;
}

MODULEENTRY32 GetModuleEntry(DWORD PID, char* ModuleName)
{
	MODULEENTRY32 ModEntry = { 0 };

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, PID);

	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 curr = { 0 };

		curr.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &curr))
		{
			do
			{
				if (!_strcmpi(curr.szModule, ModuleName))
				{
					ModEntry = curr;
				}
			}
			while (Module32Next(hSnapshot, &curr));
		}
		CloseHandle(hSnapshot);
	}
	return ModEntry;
}


void* SleepKiller::PatternScan(char* Module, char* Pattern, char* Mask)
{
	MODULEENTRY32 ModEntry = GetModuleEntry(PID, Module);
	if (!ModEntry.th32ProcessID)
	{
		return nullptr;
	}
	uintptr_t ModStart = (uintptr_t)ModEntry.modBaseAddr;
	uintptr_t ModEnd = ModStart + ModEntry.modBaseSize;
	return ExtPScanWrapper(pHandle, ModStart, ModEnd, Pattern, Mask);
}

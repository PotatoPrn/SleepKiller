#include "Class.h"

#include <iostream>

bool SleepKiller::ResolveFunctionAddress()
{
	FunctionAddress.SleepAddress = (uintptr_t)PatternScan("kernelbase.dll", "\x8B\xFF\x55\x8B\xEC\x6A\x00\xFF\x75\x08\xE8\xFF\xFF\xFF\xFF\x5D\xC2\x04\x00", "xxxxxxxxxxx????xxxx");

	if (FunctionAddress.SleepAddress)
	{
		std::cout << "Sleep Address Found!" << " " << std::hex << FunctionAddress.SleepAddress << "\n";
		EnabledUtils.SleepFunction = true;
	}

	return true;
}


bool SleepKiller::KillFunctions()
{
	if (!MemoryPatch((BYTE*)FunctionAddress.SleepAddress, (BYTE*)"\xC2\x04\x00", 3))
	{
		std::cout << "Error Disabling Sleep Function" << "\n";
		return false;
	}

	std::cout << "Successfully Disabled Sleep Function :D" << "\n";

	return true;
}

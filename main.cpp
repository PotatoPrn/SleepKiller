#include <iostream>
#include <string>

#include "Class.h"

SleepKiller Skiller;

int main()
{
	std::string TargetProcess;

	std::cout << "What process do you wish to manage?" << "\n";
	std::getline(std::cin, TargetProcess);

	if (!Skiller.GetPID(TargetProcess.c_str()))
	{
		std::cout << "Error Retrieving PID for Process" << "\n";
		exit(1);
	}

	printf("Process ID: %x\n", Skiller.PID);

	Skiller.pHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, Skiller.PID);

	if (!Skiller.pHandle)
	{
		std::cout << "error Opening a handle for the selected application" << "\n";
		exit(1);
	}

	printf("Handle for Process is: %d\n", Skiller.pHandle);

	if (Skiller.ResolveFunctionAddress())
	{
		std::cout << "Functions Resolved, Killing Now..." << "\n";
		if (Skiller.KillFunctions())
		{
			std::cout << "Functions Killed, Exiting now..." << "\n";
			system("pause");
			return 0;
		}

	}

	std::cout << "Error killing functions exiting..." << "\n";
	system("pause");
	return 1;
}

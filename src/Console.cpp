#include <Console.hpp>
#include <cstdio>
#include <Windows.h>

namespace
{
	FILE* fp = nullptr;
}

bool NConsole::Initiate()
{
	return AllocConsole() != 0
		&& freopen_s(&fp, "CONOUT$", "w+", stdout) == 0;
}

void NConsole::Terminate()
{
	fclose(fp);
	FreeConsole();
}
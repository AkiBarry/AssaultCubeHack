#include <Windows.h>
#include "Console.hpp"
#include <tlhelp32.h>
#include "Hooker.hpp"
#include "Globals.hpp"
#include "Signatures.hpp"

bool Initiate();

void Terminate();

DWORD WINAPI DllMain(HINSTANCE hiInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason != DLL_PROCESS_ATTACH)
		return TRUE;

	NGlobals::dll_instance = hiInstance;

	if (!Initiate())
	{
		NConsole::Print("Failed");
		goto terminate;
	}

	HookGL();

terminate:
	Sleep(2000);

	Terminate();

	return TRUE;
}

bool Initiate()
{
	{
		char file_dir[MAX_PATH] = { 0 };

		if (GetModuleFileName(NGlobals::dll_instance, file_dir, MAX_PATH - 1) == 0)
			return false;

		NGlobals::dll_file_path = file_dir;
		const size_t slash_pos = NGlobals::dll_file_path.rfind('\\');
		NGlobals::dll_folder_path = NGlobals::dll_file_path.substr(0, slash_pos + 1);
		NGlobals::game_base_address = reinterpret_cast<ptr_t>(GetModuleHandle(nullptr));
	}
	

	return NConsole::Initiate()
		&& NHook::Initiate()
		//&& NLog::Initiate()
		&& NSignatures::Initiate()
		&& NCanvas::Initiate()
		&& NInput::Initiate(FindWindowA(nullptr, "AssaultCube"));
}

void Terminate()
{
	//NConsole::Terminate();
}
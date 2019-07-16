#include "Hook.hpp"

bool NHook::Initiate()
{
	return MH_Initialize() == MH_OK;
}

void NHook::Terminate()
{
	MH_Uninitialize();
}

CHook::~CHook()
{
	Terminate();
}

void CHook::Initiate()
{
	if (MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID *>(ppOriginal)) == MH_OK)
		is_hooked = true;
}

void CHook::Terminate()
{
	if (MH_RemoveHook(pTarget) == MH_OK)
		is_hooked = false;
}

void CHook::Enable()
{
	if (!is_hooked)
		Initiate();

	if (!is_hooked)
		return;

	if (!is_enabled)
	{
		if (MH_EnableHook(pTarget) == MH_OK)
			is_enabled = true;
	}
}

void CHook::Disable()
{
	if (is_hooked && is_enabled)
	{
		if (MH_DisableHook(pTarget) == MH_OK)
			is_enabled = false;
	}
}
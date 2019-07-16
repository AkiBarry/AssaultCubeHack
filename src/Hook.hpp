#pragma once

#include "MinHook.h"

namespace NHook
{
	bool Initiate();
	void Terminate();
}

class CHook
{
private:
	LPVOID		pTarget = nullptr;
	LPVOID		pDetour = nullptr;
	LPVOID *	ppOriginal = nullptr;
public:
	bool		is_hooked = false;
	bool		is_enabled = false;

	CHook() = default;

	template<class T>
	CHook(LPVOID target, LPVOID detour, T ** original) : pTarget(target), pDetour(detour), ppOriginal(reinterpret_cast<LPVOID *>(original)) {};

	~CHook();

	void Initiate();
	void Terminate();

	void Enable();
	void Disable();
};
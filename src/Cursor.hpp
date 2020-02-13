#pragma once
#include "UU.hpp"

namespace NCursor
{
	extern UU::CVector<float, 2> pos;
	extern bool is_active;

	bool Initiate();

	void LockScreen();
	void UnlockScreen();
}

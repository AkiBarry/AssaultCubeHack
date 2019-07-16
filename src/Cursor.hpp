#pragma once
#include "Vector.hpp"

namespace NCursor
{
	extern NMath::CVector<float, 2> pos;
	extern bool is_active;

	bool Initiate();

	void LockScreen();
	void UnlockScreen();
}

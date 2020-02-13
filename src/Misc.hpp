#pragma once
#include "UU.hpp"

namespace NMisc
{
	enum class W2S_RESULT
	{
		ON_SCREEN,
		IN_FRONT,
		BEHIND
	};
	
	W2S_RESULT WorldToScreen(UU::CVec3f three_dimensional_vec, UU::CVec2f & output_screen_vec);
}

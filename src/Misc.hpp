#pragma once
#include "Vector.hpp"

namespace NMisc
{
	enum class W2S_RESULT
	{
		ON_SCREEN,
		IN_FRONT,
		BEHIND
	};
	
	W2S_RESULT WorldToScreen(NMath::CVec3f three_dimensional_vec, NMath::CVec2f & output_screen_vec);
}

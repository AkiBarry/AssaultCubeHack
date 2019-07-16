#include "Misc.hpp"
#include "Globals.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace NMisc
{
	float w = 0.f;
}

NMisc::W2S_RESULT NMisc::WorldToScreen(NMath::CVec3f three_dimensional_vec, NMath::CVec2f & output_screen_vec)
{
	const glm::mat4x4 view = glm::make_mat4x4(NGlobals::GetViewMatrix());

	const glm::vec4 temp(three_dimensional_vec[0], three_dimensional_vec[1], three_dimensional_vec[2], 1.f);

	glm::vec4 return_value = view * temp;

	W2S_RESULT results = W2S_RESULT::ON_SCREEN;

	float w = return_value[3];

	if (w > 0.f)
	{
		const float inv_w = 1.f / w;
		return_value[0] *= inv_w;
		return_value[1] *= inv_w;

		if (NMath::Abs(return_value[0]) > 1.f || NMath::Abs(return_value[1]) > 1.f)
		{
			results = W2S_RESULT::IN_FRONT;
		}
	}
	else
	{
		const float abs_return_value_0 = NMath::Abs(return_value[0]);
		const float abs_return_value_1 = NMath::Abs(return_value[1]);

		if (abs_return_value_0 > abs_return_value_1)
		{
			return_value[0] = NMath::Sign(return_value[0]);
			return_value[1] /= abs_return_value_0;
		}
		else
		{
			return_value[0] /= abs_return_value_1;
			return_value[1] = NMath::Sign(return_value[1]);
		}

		results = W2S_RESULT::BEHIND;
	}

	float x = NGlobals::GameResolution()[0] * 0.5f;
	float y = NGlobals::GameResolution()[1] * 0.5f;

	x += 0.5f * return_value[0] * NGlobals::GameResolution()[0] + 0.5f;
	y -= 0.5f * return_value[1] * NGlobals::GameResolution()[1] + 0.5f;

	return_value[0] = x;
	return_value[1] = y;

	output_screen_vec = NMath::CVec2f(x, y);

	return results;
}

#pragma once

#define GLEW_STATIC

#include "UU.hpp"
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

namespace NCanvas
{
	bool Initiate();
	void Terminate();
	void InHookInitiate();

	void BeginScene();
	void EndScene();

	void BeginText();
	void EndText();

	void Begin2D();
	void End2D();

	void Begin3D();
	void End3D();

	namespace NDraw
	{
		//2D

		void Rect(UU::CVec2f position, UU::CVec2f size, UU::CColour colour);
		void OutlinedRect(UU::CVec2f position, UU::CVec2f size, UU::CColour colour);
		void Circle(UU::CVec2f position, float radius, UU::CColour colour);
		void OutlinedCircle(UU::CVec2f position, float radius, UU::CColour colour);
		void Poly(size_t num_points, UU::CVec2f *  positions, UU::CColour colour);
		void OutlinedPoly(size_t num_points, UU::CVec2f *  positions, UU::CColour colour);
		void Line(UU::CVec2f position1, UU::CVec2f position2, UU::CColour colour);
		void OutlinedLine(UU::CVec2f position1, UU::CVec2f position2, UU::CColour colour);
		void Text(std::string text, UU::CVec2f position, float size, UU::CColour colour);

		// 3D

		void Cuboid(UU::CVec3f position, UU::CVec3f size, UU::CColour colour);
		void OutlinedCuboid(UU::CVec3f position, UU::CVec3f size, UU::CColour colour);
		
		void Line(UU::CVec3f position1, UU::CVec3f position2, UU::CColour colour);
		void OutlinedLine(UU::CVec3f position1, UU::CVec3f position2, UU::CColour colour);
	}

	namespace NText
	{
		class CCharacter
		{
		public:
			CCharacter() = default;

			CCharacter(uint32_t _texture_id, UU::CVec2f _size, UU::CVec2f _bearing, float _advance) 
				: texture_id(_texture_id), size(_size), bearing(_bearing), advance(_advance) {}

			uint32_t					texture_id = 0;  // ID handle of the glyph texture
			UU::CVec2f	size = UU::CVec2f(0.f, 0.f);			// Size of glyph
			UU::CVec2f	bearing = UU::CVec2f(0.f, 0.f);		// Offset from baseline to left/top of glyph
			float						advance = 0;    // Offset to advance to next glyph
		};

		GLuint CompileShaders();

		extern FT_Library ft;
		extern FT_Face face;
		extern GLuint VAO, VBO;
		extern std::map<int32_t, CCharacter> characters;
		
	}
}

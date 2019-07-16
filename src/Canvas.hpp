#pragma once

#define GLEW_STATIC

#include "Colour.hpp"
#include "Vector.hpp"
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

		void Rect(NMath::CVec2f position, NMath::CVec2f size, CColour colour);
		void OutlinedRect(NMath::CVec2f position, NMath::CVec2f size, CColour colour);
		void Circle(NMath::CVec2f position, float radius, CColour colour);
		void OutlinedCircle(NMath::CVec2f position, float radius, CColour colour);
		void Poly(size_t num_points, NMath::CVec2f *  positions, CColour colour);
		void OutlinedPoly(size_t num_points, NMath::CVec2f *  positions, CColour colour);
		void Line(NMath::CVec2f position1, NMath::CVec2f position2, CColour colour);
		void OutlinedLine(NMath::CVec2f position1, NMath::CVec2f position2, CColour colour);
		void Text(std::string text, NMath::CVec2f position, float size, CColour colour);

		// 3D

		void Cuboid(NMath::CVec3f position, NMath::CVec3f size, CColour colour);
		void OutlinedCuboid(NMath::CVec3f position, NMath::CVec3f size, CColour colour);
		
		void Line(NMath::CVec3f position1, NMath::CVec3f position2, CColour colour);
		void OutlinedLine(NMath::CVec3f position1, NMath::CVec3f position2, CColour colour);
	}

	namespace NText
	{
		class CCharacter
		{
		public:
			CCharacter() = default;

			CCharacter(uint32_t _texture_id, NMath::CVec2f _size, NMath::CVec2f _bearing, float _advance) 
				: texture_id(_texture_id), size(_size), bearing(_bearing), advance(_advance) {}

			uint32_t					texture_id = 0;  // ID handle of the glyph texture
			NMath::CVec2f	size = NMath::CVec2f(0.f, 0.f);			// Size of glyph
			NMath::CVec2f	bearing = NMath::CVec2f(0.f, 0.f);		// Offset from baseline to left/top of glyph
			float						advance = 0;    // Offset to advance to next glyph
		};

		GLuint CompileShaders();

		extern FT_Library ft;
		extern FT_Face face;
		extern GLuint VAO, VBO;
		extern std::map<int32_t, CCharacter> characters;
		
	}
}

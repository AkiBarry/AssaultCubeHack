#pragma once

#define GLEW_STATIC

#include "UU.hpp"
#include <GL/glew.h>
#include <iostream>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

#include "Types.hpp"

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

	float GetDPI();

	namespace Draw
	{
		// Colour
		void SetColour(UU::CColour colour);
		UU::CColour& GetColour();
		
		//2D

		void FilledRect(UU::CVec2f position, UU::CVec2f size);
		void OutlinedRect(UU::CVec2f position, UU::CVec2f size);
		void FilledCircle(UU::CVec2f position, float radius);
		void OutlinedCircle(UU::CVec2f position, float radius);
		void FilledPoly(size_t num_points, UU::CVec2f *  positions);
		void OutlinedPoly(size_t num_points, UU::CVec2f *  positions);
		void Line(UU::CVec2f position1, UU::CVec2f position2);
		void OutlinedLine(UU::CVec2f position1, UU::CVec2f position2);
		void Text(std::string text, UU::CVec2f position, std::string font, uint_t size);

		// 3D

		void FilledCuboid(UU::CVec3f position, UU::CVec3f size);
		void OutlinedCuboid(UU::CVec3f position, UU::CVec3f size);
		
		void Line(UU::CVec3f position1, UU::CVec3f position2);
		void OutlinedLine(UU::CVec3f position1, UU::CVec3f position2);
	}

	namespace NText
	{
		class CCharacter
		{
		public:
			CCharacter() = default;

			CCharacter(uint32_t _texture_id, UU::CVec2ui _size, UU::CVec2i _bearing, long _advance) 
				: texture_id(_texture_id), size(_size), bearing(_bearing), advance(_advance) {}

			uint32_t					texture_id	= 0;					// ID handle of the glyph texture
			UU::CVec2ui					size		= UU::CVec2ui(0, 0);	// Size of glyph
			UU::CVec2i					bearing		= UU::CVec2i(0, 0);	// Offset from baseline to left/top of glyph
			long						advance		= 0;					// Offset to advance to next glyph
		};

		GLuint CompileShaders();

		FT_Face GetFontFace(std::string font);
		CCharacter& GetCharacter(FT_Face face, uint_t size, char32_t c);

		UU::CVec2l MeasureString(std::string text, std::string font, uint_t size);

		void PreloadFont(std::string font, uint_t size);
		
		extern FT_Library ft;
		extern std::map<std::string, FT_Face> font_faces;
		extern GLuint VAO, VBO;
		extern std::map<std::tuple<FT_Face, uint_t, char32_t>, CCharacter> characters;
	}
}

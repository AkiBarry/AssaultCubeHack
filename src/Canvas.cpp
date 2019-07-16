#include "Canvas.hpp"
#include "Globals.hpp"
#include "Console.hpp"
#include <Windows.h>
#include "GL/glew.h"
#include <gl/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

#include <string>
#include <sstream>
#include <vector>
#include <fstream>

namespace NCanvas
{
	namespace NText
	{
		FT_Library ft;
		FT_Face face;
		GLuint VAO, VBO;
		std::map<int32_t, CCharacter> characters;
		GLuint program;


		GLuint CompileShaders() 
		{
			static const GLchar * vertex_shader_source[] = {
			"#version 330 core															\n"
			"layout(location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>				\n"
			"out vec2 TexCoords;														\n"
			"																			\n"
			"uniform mat4 projection;													\n"
			"																			\n"
			"void main()																\n"
			"{																			\n"
			"	gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);					\n"
			"	TexCoords = vertex.zw;													\n"
			"}																			\n"
			};

			static const GLchar * fragment_shader_source[] = {
			"#version 330 core															\n"
			"in vec2 TexCoords;															\n"
			"out vec4 color;															\n"
			"																			\n"
			"uniform sampler2D text;													\n"
			"uniform vec4 textColor;													\n"
			"																			\n"
			"void main()																\n"
			"{																			\n"
			"	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);			\n"
			"	color = sampled * textColor;											\n"
			"																			\n"
			"}"
			};


			GLint success;

			const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
			glCompileShader(vertex_shader);

			glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				NConsole::Println("ERROR::SHADER_COMPILATION_ERROR");
			}

			const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
			glCompileShader(fragment_shader);

			glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				NConsole::Println("ERROR::SHADER_COMPILATION_ERROR");
			}

			const GLuint program = glCreateProgram();
			glAttachShader(program, vertex_shader);
			glAttachShader(program, fragment_shader);
			glLinkProgram(program);

			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);

			return program;
		}
	}
}

bool NCanvas::Initiate()
{
	return true;
}

void NCanvas::Terminate()
{
}

void NCanvas::InHookInitiate()
{
	glewInit();

	NText::program = NText::CompileShaders();

	if (FT_Init_FreeType(&NText::ft))
		NConsole::Println("ERROR::FREETYPE: Could not init FreeType Library");

	if (FT_New_Face(NText::ft, (NGlobals::dll_folder_path + "fonts\\consolas.ttf").c_str(), 0, &NText::face))
		NConsole::Println("ERROR::FREETYPE: Failed to load font");

	FT_Set_Pixel_Sizes(NText::face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(NText::face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			NText::face->glyph->bitmap.width,
			NText::face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			NText::face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		NText::CCharacter character(texture,
			NMath::CVec2f(NText::face->glyph->bitmap.width, NText::face->glyph->bitmap.rows),
			NMath::CVec2f(NText::face->glyph->bitmap_left, NText::face->glyph->bitmap_top),
			static_cast<float>(NText::face->glyph->advance.x));

		NText::characters.insert(std::pair<GLchar, NText::CCharacter>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	FT_Done_Face(NText::face);
	FT_Done_FreeType(NText::ft);

	glGenVertexArrays(1, &NText::VAO);
	glGenBuffers(1, &NText::VBO);
	glBindVertexArray(NText::VAO);
	glBindBuffer(GL_ARRAY_BUFFER, NText::VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void NCanvas::BeginScene()
{
	static bool first_time = true;

	if (first_time)
	{
		InHookInitiate();
		first_time = false;
	}

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glDepthMask(GL_FALSE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DITHER);
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);
	glHint(GL_POINT_SMOOTH, GL_DONT_CARE);
	glHint(GL_LINE_SMOOTH, GL_DONT_CARE);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
	glDisable(GL_MULTISAMPLE_ARB);

	glLineWidth(1.f);

	glMatrixMode(GL_PROJECTION);
	{
		glPushMatrix();
		glLoadIdentity();
	}

	glMatrixMode(GL_MODELVIEW);
	{
		glPushMatrix();
		glLoadIdentity();
	}
}

void NCanvas::EndScene()
{
	glMatrixMode(GL_PROJECTION);
	{
		glPopMatrix();
	}

	glMatrixMode(GL_MODELVIEW);
	{
		glPopMatrix();
	}

	glPopAttrib();

	glEnable(GL_TEXTURE_2D);
}

void NCanvas::Begin2D()
{
	glMatrixMode(GL_PROJECTION);
	{
		glOrtho(-0.5, NGlobals::GameResolution()[0] - 0.5, NGlobals::GameResolution()[1] - 0.5, -0.5, -1.0, 1.0);
	}
}

void NCanvas::End2D()
{
	glMatrixMode(GL_PROJECTION);
	{
		glLoadIdentity();
	}
}

void NCanvas::Begin3D()
{
	glMatrixMode(GL_MODELVIEW);
	{
		glLoadMatrixf(NGlobals::GetViewMatrix());
	}
}
void NCanvas::End3D()
{
	glMatrixMode(GL_MODELVIEW);
	{
		glLoadIdentity();
	}
}

void NCanvas::NDraw::Rect(NMath::CVec2f position, NMath::CVec2f size, CColour colour)
{
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	position -= NMath::CVec2f(0.f, 1.f);

	glBegin(GL_QUADS);

	glVertex2i(static_cast<GLint>(position[0]), static_cast<GLint>(position[1]));
	glVertex2i(static_cast<GLint>(position[0] + size[0]), static_cast<GLint>(position[1]));
	glVertex2i(static_cast<GLint>(position[0] + size[0]), static_cast<GLint>(position[1] + size[1]));
	glVertex2i(static_cast<GLint>(position[0]), static_cast<GLint>(position[1] + size[1]));

	glEnd();
}

void NCanvas::NDraw::OutlinedRect(NMath::CVec2f position, NMath::CVec2f size, CColour colour)
{
	size -= NMath::CVec2f(1.f, 1.f);

	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_LINE_LOOP);

	glVertex2i(static_cast<GLint>(position[0]), static_cast<GLint>(position[1]));
	glVertex2i(static_cast<GLint>(position[0] + size[0]), static_cast<GLint>(position[1]));
	glVertex2i(static_cast<GLint>(position[0] + size[0]), static_cast<GLint>(position[1] + size[1]));
	glVertex2i(static_cast<GLint>(position[0]), static_cast<GLint>(position[1] +size[1]));

	glEnd();
}

void NCanvas::NDraw::Circle(NMath::CVec2f position, float radius, CColour colour)
{
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_TRIANGLE_FAN);

	for (float ang = 0; ang < 2.f * NMath::FLT_PI; ang += 2.f * NMath::FLT_PI / ( 2 * NMath::FLT_PI * radius))
	{
		glVertex2i(static_cast<GLint>(position[0] + radius * NMath::Cos(ang)), static_cast<GLint>(position[1] + radius * NMath::Sin(ang)));
	}

	glEnd();
}

void NCanvas::NDraw::OutlinedCircle(NMath::CVec2f position, float radius, CColour colour)
{
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_LINE_LOOP);

	for (float ang = 0; ang < 2.f * NMath::FLT_PI ; ang += 2.f * NMath::FLT_PI / (2 * NMath::FLT_PI * radius))
	{
		glVertex2i(static_cast<GLint>(position[0] + radius * NMath::Cos(ang)), static_cast<GLint>(position[1] + radius * NMath::Sin(ang)));
	}

	glEnd();
}

void NCanvas::NDraw::Poly(size_t num_points, NMath::CVec2f * positions, CColour colour)
{
	if (num_points < 3)
		return;

	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_POLYGON);
	{
		for (size_t i = 0; i < num_points; ++i)
			glVertex2i(static_cast<GLint>(positions[i][0]), static_cast<GLint>(positions[i][1]));
	}
	glEnd();
}

void NCanvas::NDraw::OutlinedPoly(size_t num_points, NMath::CVec2f * positions, CColour colour)
{
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_LINE_LOOP);
	{
		for (size_t i = 0; i < num_points; ++i)
			glVertex2i(static_cast<GLint>(positions[i][0]), static_cast<GLint>(positions[i][1]));
	}
	glEnd();
}

void NCanvas::NDraw::Line(NMath::CVec2f position1, NMath::CVec2f position2, CColour colour)
{
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_LINES);

	glVertex2i(static_cast<GLint>(position1[0]), static_cast<GLint>(position1[1]));
	glVertex2i(static_cast<GLint>(position2[0]), static_cast<GLint>(position2[1]));

	glEnd();
}

void NCanvas::NDraw::OutlinedLine(NMath::CVec2f position1, NMath::CVec2f position2, CColour colour)
{
	glLineWidth(3.f);
	glColor4ub(0, 0, 0, colour.a);

	glBegin(GL_LINES);

	glVertex2i(static_cast<GLint>(position1[0]), static_cast<GLint>(position1[1]));
	glVertex2i(static_cast<GLint>(position2[0]), static_cast<GLint>(position2[1]));

	glEnd();

	glLineWidth(1.f);
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_LINES);

	glVertex2i(static_cast<GLint>(position1[0]), static_cast<GLint>(position1[1]));
	glVertex2i(static_cast<GLint>(position2[0]), static_cast<GLint>(position2[1]));

	glEnd();
}

void NCanvas::NDraw::Text(std::string text, NMath::CVec2f position, float size, CColour colour)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_DITHER);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	size /= 48.f;

	static const glm::mat4 projection = glm::ortho(0.0f, NGlobals::GameResolution()[0], NGlobals::GameResolution()[1], 0.0f);
	glUseProgram(NText::program);
	glUniformMatrix4fv(glGetUniformLocation(NText::program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DITHER);

	glUniform4f(glGetUniformLocation(NText::program, "textColor"), colour.r / 255.f, colour.g / 255.f, colour.b / 255.f, colour.a / 255.f);
	//glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(NText::VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); ++c)
	{
		NText::CCharacter ch = NText::characters[*c];

		GLfloat xpos = position[0] + ch.bearing[0] * size;
		GLfloat ypos = position[1] - ch.bearing[1] * size;

		if (xpos > NGlobals::GameResolution()[0])
			break;

		GLfloat w = ch.size[0] * size;
		GLfloat h = ch.size[1] * size;

		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos,       0.0, 0.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 },
			{ xpos,     ypos + h,   0.0, 1.0 },

			{ xpos,		ypos,       0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos + w,	ypos + h,   1.0, 1.0 }

		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.texture_id);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, NText::VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		position[0] += (ch.advance / 64.f) * size; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_DITHER);

	glUseProgram(0);

	glPopAttrib();
}

void NCanvas::NDraw::Cuboid(NMath::CVec3f position, NMath::CVec3f size, CColour colour)
{
	glLineWidth(1.f);
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	auto temp = position - NMath::CVec3f(size[0] / 2.f, size[1] / 2.f, 0.f);

	NMath::CVec3f corners[8] = {
		temp, temp + NMath::CVec3f(size[0], 0.f, 0.f),  temp + NMath::CVec3f(size[0], size[1], 0.f), temp + NMath::CVec3f(0.f, size[1], 0.f),
		temp + NMath::CVec3f(0.f, 0.f, size[2]), temp + NMath::CVec3f(size[0], 0.f, size[2]),  temp + size, temp + NMath::CVec3f(0.f, size[1], size[2]),
	};

	// Bottom

	glBegin(GL_QUADS);

	glVertex3fv(corners[0].Base());
	glVertex3fv(corners[3].Base());
	glVertex3fv(corners[2].Base());
	glVertex3fv(corners[1].Base());

	glEnd();

	// Top

	glBegin(GL_QUADS);

	glVertex3fv(corners[4].Base());
	glVertex3fv(corners[5].Base());
	glVertex3fv(corners[6].Base());
	glVertex3fv(corners[7].Base());

	glEnd();

	// Sides

	glBegin(GL_QUADS);

	glVertex3fv(corners[0].Base());
	glVertex3fv(corners[4].Base());
	glVertex3fv(corners[7].Base());
	glVertex3fv(corners[3].Base());

	glEnd();

	glBegin(GL_QUADS);

	glVertex3fv(corners[7].Base());
	glVertex3fv(corners[6].Base());
	glVertex3fv(corners[2].Base());
	glVertex3fv(corners[3].Base());

	glEnd();

	glBegin(GL_QUADS);

	glVertex3fv(corners[5].Base());
	glVertex3fv(corners[4].Base());
	glVertex3fv(corners[0].Base());
	glVertex3fv(corners[1].Base());

	glEnd();

	glBegin(GL_QUADS);

	glVertex3fv(corners[6].Base());
	glVertex3fv(corners[5].Base());
	glVertex3fv(corners[1].Base());
	glVertex3fv(corners[2].Base());

	glEnd();
}

void NCanvas::NDraw::OutlinedCuboid(NMath::CVec3f position, NMath::CVec3f size, CColour colour)
{
	glLineWidth(1.f);
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	const auto temp = position - NMath::CVec3f(size[0] / 2.f, size[1] / 2.f, 0.f);

	NMath::CVec3f corners[8] = {
		temp, temp + NMath::CVec3f(size[0], 0.f, 0.f),  temp + NMath::CVec3f(size[0], size[1], 0.f), temp + NMath::CVec3f(0.f, size[1], 0.f),
		temp + NMath::CVec3f(0.f, 0.f, size[2]), temp + NMath::CVec3f(size[0], 0.f, size[2]),  temp + size, temp + NMath::CVec3f(0.f, size[1], size[2]),
	};

	glBegin(GL_LINE_STRIP);

	glVertex3fv(corners[0].Base());
	glVertex3fv(corners[4].Base());
	glVertex3fv(corners[5].Base());
	glVertex3fv(corners[6].Base());

	glEnd();

	glBegin(GL_LINE_STRIP);

	glVertex3fv(corners[4].Base());
	glVertex3fv(corners[7].Base());
	glVertex3fv(corners[6].Base());
	glVertex3fv(corners[2].Base());

	glEnd();

	glBegin(GL_LINE_STRIP);

	glVertex3fv(corners[7].Base());
	glVertex3fv(corners[3].Base());
	glVertex3fv(corners[0].Base());
	glVertex3fv(corners[1].Base());

	glEnd();

	glBegin(GL_LINE_STRIP);

	glVertex3fv(corners[3].Base());
	glVertex3fv(corners[2].Base());
	glVertex3fv(corners[1].Base());
	glVertex3fv(corners[5].Base());

	glEnd();
}

void NCanvas::NDraw::Line(NMath::CVec3f position1, NMath::CVec3f position2, CColour colour)
{
	glLineWidth(1.f);
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_LINES);

	glVertex3f(position1[0], position1[1], position1[2]);
	glVertex3f(position2[0], position2[1], position2[2]);

	glEnd();
}

void NCanvas::NDraw::OutlinedLine(NMath::CVec3f position1, NMath::CVec3f position2, CColour colour)
{
	glLineWidth(3.f);
	glColor4ub(0, 0, 0, 255);

	glBegin(GL_LINES);

	glVertex3f(position1[0], position1[1], position1[2]);
	glVertex3f(position2[0], position2[1], position2[2]);

	glEnd();

	glLineWidth(1.f);
	glColor4ub(colour.r, colour.g, colour.b, colour.a);

	glBegin(GL_LINES);

	glVertex3f(position1[0], position1[1], position1[2]);
	glVertex3f(position2[0], position2[1], position2[2]);

	glEnd();
}
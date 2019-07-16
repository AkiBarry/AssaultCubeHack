#pragma once
#include <Windows.h>
#include "Canvas.hpp"
#include "Globals.hpp"
#include <detours.h>
#include <gl/GL.h>
#include "Cheat.hpp"
#include "MinHook.h"
#include "Input.hpp"
#include "Hook.hpp"

using tglBegin = void (APIENTRY *)(GLenum);
using twglSwapBuffers = BOOL(__stdcall *) (_In_ HDC hDc);

twglSwapBuffers owglSwapBuffers;
twglSwapBuffers nwglSwapBuffers;

tglBegin oglBegin;
tglBegin nglBegin;

CHook temp;

static bool first_time = true;

inline void FirstTime()
{

}

inline BOOL CheatLoop(_In_ HDC hDc)
{
	if (first_time)
	{
		FirstTime();
		first_time = false;
	}

	NCheat::Aimbot();

	NCheat::Movement();

	NCanvas::BeginScene();

	NCheat::Visuals();

	NCheat::ESP();

	NCanvas::EndScene();

	NInput::Update();

	return nwglSwapBuffers(hDc);
}

static void HookGL()
{
	const HMODULE hOpenGL = GetModuleHandle("opengl32.dll");

	if (hOpenGL)
	{
		owglSwapBuffers = reinterpret_cast<twglSwapBuffers>(GetProcAddress(hOpenGL, "wglSwapBuffers"));

		temp = CHook(owglSwapBuffers, &CheatLoop, &nwglSwapBuffers);

		temp.Enable();
	}
}
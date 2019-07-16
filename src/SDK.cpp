#include "SDK.hpp"
#include <Windows.h>
#include "Globals.hpp"
#include "Canvas.hpp"
#include <iomanip>

struct STraceResult
{
	NMath::CVec3f end;
	bool collided;
};


bool NSDK::SPlayer::IsVisible() const
{
	STraceResult trace_result;
	NMath::CVec3f from = NGlobals::LocalPlayer().cam_position;
	NMath::CVec3f to = cam_position;

	DWORD traceLine = 0x48A310;
	
	DWORD * local_player_ptr = *reinterpret_cast<DWORD **>(NGlobals::game_base_address + 0x109B74);

	__asm
	{
		push 0;
		push 0;
		push local_player_ptr
		push to[8]
		push to[4]
		push to[0]
		push from[8]
		push from[4]
		push from[0]
		lea eax, [trace_result]
		call traceLine;
		add esp, 36
	}

	return !trace_result.collided;
}

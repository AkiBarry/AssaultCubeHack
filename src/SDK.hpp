#pragma once
#include "Vector.hpp"

namespace NSDK
{

	// TYPE enum { ENT_PLAYER = 0, ENT_BOT, ENT_CAMERA, ENT_BOUNCE };
	// STATE enum { CS_ALIVE = 0, CS_DEAD, CS_SPAWNING, CS_LAGGED, CS_EDITING, CS_SPECTATE };
	// TEAM enum { TEAM_CLA = 0, TEAM_RVSF, TEAM_CLA_SPECT, TEAM_RVSF_SPECT, TEAM_SPECT, TEAM_NUM, TEAM_ANYACTIVE };
#pragma pack(push,1)
	class SPlayer
	{
	public:

		bool IsVisible() const;

		char							pad_0x0000[0x4]; //0x0000
		NMath::CVec3f					cam_position; //0x0004 
		NMath::CVec3f					velocity; //0x0010 
		NMath::CVec3f					velocity_t; //0x001C 
		NMath::CVec3f					delta_position; //0x0028 
		NMath::CVec3f					position; //0x0034 
		NMath::CAngle<float, 3, false>	view_angles; //0x0040 
		float							pitch_velocity; //0x004C 
		float							max_speed; //0x0050 
		int32_t							time_in_air; //0x0054 
		float							radius; //0x0058 
		float							height; //0x005C 
		float							max_height; //0x0060 
		float							above_eye; //0x0064 
		bool							in_water; //0x0068 
		bool							on_floor; //0x0069 
		bool							on_ladder; //0x006A 
		bool							jump_next; //0x006B 
		bool							crouched; //0x006C 
		bool							crouched_in_air; //0x006D 
		bool							try_crouch; //0x006E 
		bool							can_collide; //0x006F 
		bool							stuck; //0x0070 
		bool							scoping; //0x0071 
		bool							shoot; //0x0072 
		uint8_t							pad_0x0073[0x1]; //0x0073
		int32_t							last_jump; //0x0074 
		float							last_jump_height; //0x0078 
		int32_t							last_splash; //0x007C 
		int8_t							forward_move; //0x0080 
		int8_t							side_move; //0x0081 
		uint8_t							state; //0x0082 
		uint8_t							type; //0x0083 
		float							eye_height_velocity; //0x0084 
		int32_t							last_position; //0x0088 
		bool							k_left; //0x008C 
		bool							k_right; //0x008D 
		bool							k_forward; //0x008E 
		bool							k_backward; //0x008F 
		uint8_t							pad_0x0090[0x68]; //0x0090
		int32_t							health; //0x00F8 
		int32_t							armour; //0x00FC 
		int32_t							primary; //0x0100 
		int32_t							next_primary; //0x0104 
		int32_t							gun_select; //0x0108 
		bool							akimbo; //0x010C 
		uint8_t							pad_0x010D[0x3]; //0x010D
		int32_t							knife_reserve_ammo; //0x0110 
		int32_t							pistol_reserve_ammo; //0x0114 
		int32_t							carbine_reserve_ammo; //0x0118 
		int32_t							shotgun_reserve_ammo; //0x011C 
		int32_t							smg_reserve_ammo; //0x0120 
		int32_t							sniper_reserve_ammo; //0x0124 
		int32_t							ar_reserve_ammo; //0x0128 
		int32_t							broken_reserve_ammo; //0x012C 
		int32_t							grenade_reserve_ammo; //0x0130 
		int32_t							akimbo_reserve_ammo; //0x0134 
		int32_t							knife_clip_ammo; //0x0138 
		int32_t							pistol_clip_ammo; //0x013C 
		int32_t							carbine_clip_ammo; //0x0140 
		int32_t							shotgun_clip_ammo; //0x0144 
		int32_t							smg_clip_ammo; //0x0148 
		int32_t							sniper_clip_ammo; //0x014C 
		int32_t							ar_clip_ammo; //0x0150 
		int32_t							broken_clip_ammo; //0x0154 
		int32_t							grenade_clip_ammo; //0x0158 
		int32_t							akimbo_clip_ammo; //0x015C 
		int32_t							knife_wait_time;		//0x0160 
		int32_t							pistol_wait_time;		//0x0164 
		int32_t							carbine_wait_time;		//0x0168 
		int32_t							shotgun_wait_time;		//0x016C 
		int32_t							smg_wait_time;			//0x0170 
		int32_t							sniper_wait_time;		//0x0174 
		int32_t							ar_wait_time;			//0x0178 
		int32_t							broken_wait_time;		//0x017C 
		int32_t							grenade_wait_time;		//0x0180 
		int32_t							akimbo_wait_time;		//0x0184 
		uint8_t							pad_0x0188[0x50];		//0x0188
		int32_t							current_model_number;	//0x01D8 
		int32_t							next_model_number;		//0x01DC 
		uint8_t							pad_0x01E0[0x1C];		//0x01E0
		int32_t							frags;					//0x01FC 
		char							pad_0x0200[0x4];		//0x0200
		int32_t							deaths;					//0x0204 
		char							pad_0x0208[0x4];		//0x0208
		int32_t							team_kills;				//0x020C 
		char							pad_0x0210[0x14];		//0x0210
		bool							attacking;				//0x0224 
		char							name[16];				//0x0225 
		uint8_t							pad_0x0235[0xF7];		//0x0235
		int8_t							team;					//0x032C
	}; //Size=0x032D


#pragma pack(pop)
}

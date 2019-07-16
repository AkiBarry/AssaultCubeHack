#include "Globals.hpp"
#include "Signatures.hpp"
#include <gl/GL.h>
#include "Console.hpp"
#include "SDK.hpp"

HINSTANCE	NGlobals::dll_instance = nullptr;
std::string NGlobals::dll_file_path;
std::string NGlobals::dll_folder_path;
ptr_t		NGlobals::game_base_address;


NMath::CVec2f NGlobals::GameResolution()
{
	GLfloat viewport[4];

	glGetFloatv(GL_VIEWPORT, viewport);

	return NMath::CVec2f(viewport[2], viewport[3]);
}

NSDK::SPlayer & NGlobals::LocalPlayer()
{
	return **reinterpret_cast<NSDK::SPlayer **>(game_base_address + 0x10f4f4); // Alternate: 0x109B74
}

NSDK::SPlayer ** NGlobals::GetEntityList()
{
	
	return *reinterpret_cast<NSDK::SPlayer ***>(game_base_address + 0x10F4F8);
}

int32_t NGlobals::GetEntityListSize()
{
	return *reinterpret_cast<int32_t *>(game_base_address + 0x10F500);
}

float * NGlobals::GetViewMatrix()
{
	return reinterpret_cast<float *>(game_base_address + 0x101AE8);
}

float & NGlobals::GetFov()
{
	return *reinterpret_cast<float *>(game_base_address + 0x10F1C4);
}

// GLOBAL VARIABLES

#define DEFINE_GLOBAL_VAR(type, name, data) CVariable<type> NVars::name = CVariable<type>(data, #name)

DEFINE_GLOBAL_VAR(bool,						aimbot_enabled,						true);
DEFINE_GLOBAL_VAR(bool,						aimbot_vis_check,					true);
DEFINE_GLOBAL_VAR(TARGET_ALGORITHM,			aimbot_target_algorithm,			TARGET_ALGORITHM::DISTANCE);
DEFINE_GLOBAL_VAR(bool,						aimbot_ignore_team,					true);
DEFINE_GLOBAL_VAR(bool,						aimbot_on_key,						true);
DEFINE_GLOBAL_VAR(float,					aimbot_smooth,						0.5f);
DEFINE_GLOBAL_VAR(SMOOTH_TYPE,				aimbot_smooth_type,					SMOOTH_TYPE::LINEAR);
DEFINE_GLOBAL_VAR(bool,						aimbot_silent,						false);
DEFINE_GLOBAL_VAR(SILENT_TYPE,				aimbot_silent_type,					SILENT_TYPE::IMPERFECT);
DEFINE_GLOBAL_VAR(bool,						aimbot_auto_shoot,					false);
DEFINE_GLOBAL_VAR(bool,						aimbot_auto_duck,					false);
DEFINE_GLOBAL_VAR(bool,						aimbot_body_aim,					false);
DEFINE_GLOBAL_VAR(bool,						aimbot_locked,						false);

DEFINE_GLOBAL_VAR(bool,						esp_enabled,						true);
DEFINE_GLOBAL_VAR(bool,						esp_beacon,							false);
DEFINE_GLOBAL_VAR(bool,						esp_halo,							false);
DEFINE_GLOBAL_VAR(bool,						esp_glow,							false);
DEFINE_GLOBAL_VAR(bool,						esp_box,							true);
DEFINE_GLOBAL_VAR(bool,						esp_filled_box,						true);
DEFINE_GLOBAL_VAR(BOX_DIMENSION,			esp_box_dimensions,					BOX_DIMENSION::THREE);
DEFINE_GLOBAL_VAR(BOX_TYPE,					esp_box_type,						BOX_TYPE::STATIC);
DEFINE_GLOBAL_VAR(bool,						esp_health_bar,						false);
DEFINE_GLOBAL_VAR(bool,						esp_armour_bar,						false);
DEFINE_GLOBAL_VAR(bool,						esp_aim_position,					false);
DEFINE_GLOBAL_VAR(bool,						esp_skeleton,						false);
DEFINE_GLOBAL_VAR(bool,						esp_name,							false);
DEFINE_GLOBAL_VAR(bool,						esp_health,							false);
DEFINE_GLOBAL_VAR(bool,						esp_armour,							false);
DEFINE_GLOBAL_VAR(bool,						esp_weapon,							false);
DEFINE_GLOBAL_VAR(bool,						esp_distance,						false);
DEFINE_GLOBAL_VAR(bool,						esp_barrel,							false);
DEFINE_GLOBAL_VAR(bool,						esp_hitbox,							false);
DEFINE_GLOBAL_VAR(bool,						esp_filled_hitbox,					false);
DEFINE_GLOBAL_VAR(bool,						esp_snapline,						false);
DEFINE_GLOBAL_VAR(SNAPLINE_POSITION,		esp_snapline_position,				SNAPLINE_POSITION::CENTER);

DEFINE_GLOBAL_VAR(bool,						esp_chams,							false);
DEFINE_GLOBAL_VAR(bool,						esp_chams_XQZ,						false);
DEFINE_GLOBAL_VAR(bool,						esp_chams_type,						false);

DEFINE_GLOBAL_VAR(bool,						visuals_enable,						false);
DEFINE_GLOBAL_VAR(bool,						visuals_field_of_view,				false);
DEFINE_GLOBAL_VAR(float,					visuals_field_of_view_angle,		170.f);
DEFINE_GLOBAL_VAR(bool,						visuals_third_person,				false);
DEFINE_GLOBAL_VAR(bool,						visuals_ASUS_walls,					false);
DEFINE_GLOBAL_VAR(bool,						visuals_crosshair,					false);
DEFINE_GLOBAL_VAR(CROSSHAIR_TYPE,			visuals_crosshair_type,				CROSSHAIR_TYPE::GENERIC);
DEFINE_GLOBAL_VAR(bool,						visuals_watermark,					true);

DEFINE_GLOBAL_VAR(bool,						radar_enable,						true);
DEFINE_GLOBAL_VAR(float,					radar_range,						100.f);
DEFINE_GLOBAL_VAR(bool,						radar_rotate,						false);
DEFINE_GLOBAL_VAR(bool,						radar_show_north,					true);
DEFINE_GLOBAL_VAR(bool,						radar_show_fov,						true);
DEFINE_GLOBAL_VAR(bool,						radar_show_players,					true);
DEFINE_GLOBAL_VAR(bool,						radar_show_team,					true);
DEFINE_GLOBAL_VAR(bool,						radar_show_ents,					true);
DEFINE_GLOBAL_VAR(bool,						radar_show_weapons,					true);
DEFINE_GLOBAL_VAR(bool,						radar_circular,						false);

DEFINE_GLOBAL_VAR(bool,						miscs_enable,						true);
DEFINE_GLOBAL_VAR(bool,						miscs_bunny_hop,					true);
DEFINE_GLOBAL_VAR(bool,						miscs_edge_jump,					false);
DEFINE_GLOBAL_VAR(bool,						miscs_fly,							false);
DEFINE_GLOBAL_VAR(bool,						miscs_auto_pistol,					true);
DEFINE_GLOBAL_VAR(bool,						miscs_auto_aim,						false);
DEFINE_GLOBAL_VAR(bool,						miscs_no_spread,					false);
DEFINE_GLOBAL_VAR(bool,						miscs_no_recoil,					false);
DEFINE_GLOBAL_VAR(bool,						miscs_namestealer,					false);
DEFINE_GLOBAL_VAR(uint32_t,					miscs_namestealer_interval,			10);

// MENU ITEMS

#define DEFINE_MENU_ITEM(type, name) NMenu::type * NMenuItems::name = nullptr

DEFINE_MENU_ITEM(CWindow,					main_window);

DEFINE_MENU_ITEM(CFrame,					aimbot_frame);
DEFINE_MENU_ITEM(CFrame,					esp_frame);
DEFINE_MENU_ITEM(CFrame,					visuals_frame);
DEFINE_MENU_ITEM(CFrame,					miscs_frame);

DEFINE_MENU_ITEM(CButton,					aimbot_frame_select);
DEFINE_MENU_ITEM(CButton,					esp_frame_select);
DEFINE_MENU_ITEM(CButton,					visuals_frame_select);
DEFINE_MENU_ITEM(CButton,					miscs_frame_select);

DEFINE_MENU_ITEM(CCheckBox,					aimbot_enabled);
DEFINE_MENU_ITEM(CCheckBox,					aimbot_vis_check);
//DEFINE_MENU_ITEM(TARGET_ALGORITHM,		aimbot_target_algorithm);
DEFINE_MENU_ITEM(CCheckBox,					aimbot_ignore_team);
DEFINE_MENU_ITEM(CCheckBox,					aimbot_on_key);
//DEFINE_MENU_ITEM(float,					aimbot_smooth);
//DEFINE_MENU_ITEM(SMOOTH_TYPE,				aimbot_smooth_type);
DEFINE_MENU_ITEM(CCheckBox,					aimbot_silent);
//DEFINE_MENU_ITEM(SILENT_TYPE,				aimbot_silent_type);
DEFINE_MENU_ITEM(CCheckBox,					aimbot_auto_shoot);
DEFINE_MENU_ITEM(CCheckBox,					aimbot_auto_duck);
DEFINE_MENU_ITEM(CCheckBox,					aimbot_body_aim);
DEFINE_MENU_ITEM(CCheckBox,					aimbot_locked);

DEFINE_MENU_ITEM(CWindow,					radar_window);
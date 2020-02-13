#pragma once
#include <Windows.h>
#include <string>
#include "UU.hpp"
#include "SDK.hpp"
#include "Types.hpp"
#include "Variable.hpp"
#include "Menu.hpp"

namespace NGlobals
{
	extern HINSTANCE dll_instance;

	extern std::string dll_file_path;

	extern std::string dll_folder_path;

	extern ptr_t game_base_address;

	UU::CVec2f GameResolution();

	NSDK::SPlayer & LocalPlayer();

	NSDK::SPlayer ** GetEntityList();

	int32_t GetEntityListSize();
	
	float * GetViewMatrix();

	float & GetFov();
}

enum class TARGET_ALGORITHM
{
	NEXT_SHOT,
	DISTANCE,
	ANGLE,
	SCALAR_PROJECTION
};

enum class SMOOTH_TYPE
{
	ZENO,
	LINEAR,
	ZENO_WITH_MIN
};

enum class SILENT_TYPE
{
	PERFECT,
	IMPERFECT
};

enum class BOX_DIMENSION
{
	TWO,
	THREE
};

enum class BOX_TYPE
{
	STATIC,
	DYNAMIC,
	BONES
};

enum class SNAPLINE_POSITION
{
	BOTTOM,
	CENTER
};

enum class CHAMS_TYPE
{
	WIREFRAME,
	SOLID,
	FLAT
};

enum class CROSSHAIR_TYPE
{
	GENERIC,
	SIMPLE
};

#define DECLARE_GLOBAL_VAR(type, name) extern CVariable<type> name

namespace NVars
{
	DECLARE_GLOBAL_VAR(bool,					aimbot_enabled);
	DECLARE_GLOBAL_VAR(bool,					aimbot_vis_check);
	DECLARE_GLOBAL_VAR(TARGET_ALGORITHM,		aimbot_target_algorithm);
	DECLARE_GLOBAL_VAR(bool,					aimbot_ignore_team);
	DECLARE_GLOBAL_VAR(bool,					aimbot_on_key);
	DECLARE_GLOBAL_VAR(float,					aimbot_smooth);
	DECLARE_GLOBAL_VAR(SMOOTH_TYPE,				aimbot_smooth_type);
	DECLARE_GLOBAL_VAR(bool,					aimbot_silent);
	DECLARE_GLOBAL_VAR(SILENT_TYPE,				aimbot_silent_type);
	DECLARE_GLOBAL_VAR(bool,					aimbot_auto_shoot);
	DECLARE_GLOBAL_VAR(bool,					aimbot_auto_duck);
	DECLARE_GLOBAL_VAR(bool,					aimbot_body_aim);
	DECLARE_GLOBAL_VAR(bool,					aimbot_locked);
			
	DECLARE_GLOBAL_VAR(bool,					esp_enabled);
	DECLARE_GLOBAL_VAR(bool,					esp_beacon);
	DECLARE_GLOBAL_VAR(bool,					esp_halo);
	DECLARE_GLOBAL_VAR(bool,					esp_glow);
	DECLARE_GLOBAL_VAR(bool,					esp_box);
	DECLARE_GLOBAL_VAR(bool,					esp_filled_box);
	DECLARE_GLOBAL_VAR(BOX_DIMENSION,			esp_box_dimensions);
	DECLARE_GLOBAL_VAR(BOX_TYPE,				esp_box_type);
	DECLARE_GLOBAL_VAR(bool,					esp_health_bar);
	DECLARE_GLOBAL_VAR(bool,					esp_armour_bar);
	DECLARE_GLOBAL_VAR(bool,					esp_aim_position);
	DECLARE_GLOBAL_VAR(bool,					esp_skeleton);
	DECLARE_GLOBAL_VAR(bool,					esp_name);
	DECLARE_GLOBAL_VAR(bool,					esp_health);
	DECLARE_GLOBAL_VAR(bool,					esp_armour);
	DECLARE_GLOBAL_VAR(bool,					esp_weapon);
	DECLARE_GLOBAL_VAR(bool,					esp_distance);
	DECLARE_GLOBAL_VAR(bool,					esp_barrel);
	DECLARE_GLOBAL_VAR(bool,					esp_hitbox);
	DECLARE_GLOBAL_VAR(bool,					esp_filled_hitbox);
	DECLARE_GLOBAL_VAR(bool,					esp_snapline);
	DECLARE_GLOBAL_VAR(SNAPLINE_POSITION,		esp_snapline_position);
			
	DECLARE_GLOBAL_VAR(bool,					esp_chams);
	DECLARE_GLOBAL_VAR(bool,					esp_chams_XQZ);
	DECLARE_GLOBAL_VAR(bool,					esp_chams_type);
			 
	DECLARE_GLOBAL_VAR(bool,					visuals_enable);
	DECLARE_GLOBAL_VAR(bool,					visuals_field_of_view);
	DECLARE_GLOBAL_VAR(float,					visuals_field_of_view_angle);
	DECLARE_GLOBAL_VAR(bool,					visuals_third_person);
	DECLARE_GLOBAL_VAR(bool,					visuals_ASUS_walls);
	DECLARE_GLOBAL_VAR(bool,					visuals_crosshair);
	DECLARE_GLOBAL_VAR(CROSSHAIR_TYPE,			visuals_crosshair_type);
	DECLARE_GLOBAL_VAR(bool,					visuals_watermark);

	DECLARE_GLOBAL_VAR(bool,					radar_enable);
	DECLARE_GLOBAL_VAR(float,					radar_range);
	DECLARE_GLOBAL_VAR(bool,					radar_rotate);
	DECLARE_GLOBAL_VAR(bool,					radar_show_north);
	DECLARE_GLOBAL_VAR(bool,					radar_show_fov);
	DECLARE_GLOBAL_VAR(bool,					radar_show_players);
	DECLARE_GLOBAL_VAR(bool,					radar_show_team);
	DECLARE_GLOBAL_VAR(bool,					radar_show_ents);
	DECLARE_GLOBAL_VAR(bool,					radar_show_weapons);
	DECLARE_GLOBAL_VAR(bool,					radar_circular);
			 
	DECLARE_GLOBAL_VAR(bool,					miscs_enable);
	DECLARE_GLOBAL_VAR(bool,					miscs_bunny_hop);
	DECLARE_GLOBAL_VAR(bool,					miscs_edge_jump);
	DECLARE_GLOBAL_VAR(bool,					miscs_fly);
	DECLARE_GLOBAL_VAR(bool,					miscs_auto_pistol);
	DECLARE_GLOBAL_VAR(bool,					miscs_auto_aim);
	DECLARE_GLOBAL_VAR(bool,					miscs_no_spread);
	DECLARE_GLOBAL_VAR(bool,					miscs_no_recoil);
	DECLARE_GLOBAL_VAR(bool,					miscs_namestealer);
	DECLARE_GLOBAL_VAR(uint32_t,				miscs_namestealer_interval);
}

#define DECLARE_MENU_ITEM(type, name) extern NMenu::type * name

namespace NMenuItems
{
	DECLARE_MENU_ITEM(CWindow,					main_window);

	DECLARE_MENU_ITEM(CFrame,					aimbot_frame);
	DECLARE_MENU_ITEM(CFrame,					esp_frame);
	DECLARE_MENU_ITEM(CFrame,					visuals_frame);
	DECLARE_MENU_ITEM(CFrame,					miscs_frame);

	DECLARE_MENU_ITEM(CButton,					aimbot_frame_select);
	DECLARE_MENU_ITEM(CButton,					esp_frame_select);
	DECLARE_MENU_ITEM(CButton,					visuals_frame_select);
	DECLARE_MENU_ITEM(CButton,					miscs_frame_select);

	DECLARE_MENU_ITEM(CCheckBox,				aimbot_enabled);
	DECLARE_MENU_ITEM(CCheckBox,				aimbot_vis_check);
	//DECLARE_MENU_ITEM(TARGET_ALGORITHM,		aimbot_target_algorithm);
	DECLARE_MENU_ITEM(CCheckBox,				aimbot_ignore_team);
	DECLARE_MENU_ITEM(CCheckBox,				aimbot_on_key);
	//DECLARE_MENU_ITEM(float, aimbot_smooth);
	//DECLARE_MENU_ITEM(SMOOTH_TYPE, aimbot_smooth_type);
	DECLARE_MENU_ITEM(CCheckBox,				aimbot_silent);
	//DECLARE_MENU_ITEM(SILENT_TYPE, aimbot_silent_type);
	DECLARE_MENU_ITEM(CCheckBox,				aimbot_auto_shoot);
	DECLARE_MENU_ITEM(CCheckBox,				aimbot_auto_duck);
	DECLARE_MENU_ITEM(CCheckBox,				aimbot_body_aim);
	DECLARE_MENU_ITEM(CCheckBox,				aimbot_locked);

	DECLARE_MENU_ITEM(CSlider<float>,			esp_test);

	DECLARE_MENU_ITEM(CWindow,					radar_window);
}
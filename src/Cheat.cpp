#include "Cheat.hpp"
#include "Canvas.hpp"
#include "Globals.hpp"
#include "Console.hpp"
#include "Input.hpp"
#include "Misc.hpp"
#include "Menu.hpp"

void NCheat::Aimbot()
{
	if (!NVars::aimbot_enabled)
		return;

	if (!NInput::IsKeyDown('F') && NVars::aimbot_on_key)
		return;

	if (reinterpret_cast<size_t>(NGlobals::GetEntityList()) < NGlobals::game_base_address)
		return;

	for (int i = 0; i < NGlobals::GetEntityListSize(); ++i)
	{
		if (reinterpret_cast<size_t>(NGlobals::GetEntityList()[i]) <= NGlobals::game_base_address)
			continue;

		const NSDK::SPlayer current_entity = *NGlobals::GetEntityList()[i];

		if (!current_entity.IsVisible())
			continue;

		if (current_entity.health <= 0)
			continue;

		if (current_entity.team == NGlobals::LocalPlayer().team && NVars::aimbot_ignore_team)
			continue;
		
		const auto diff_vector = (current_entity.cam_position + NMath::CVec3f(0.f, 0.f, 0.5f)) - NGlobals::LocalPlayer().cam_position;

		NMath::CAngle<float, 3, false> temp_angle = diff_vector.ToCAngle<float, false>();

		NGlobals::LocalPlayer().view_angles[0] = temp_angle[1] + 90.f;
		NGlobals::LocalPlayer().view_angles[1] = temp_angle[0];

		NVars::aimbot_locked = true;

		if (NVars::aimbot_auto_shoot)
			NGlobals::LocalPlayer().attacking = true;

		return;
	}
	NVars::aimbot_locked = false;
	NGlobals::LocalPlayer().attacking = false;
}

void NCheat::ESP()
{
	if (NVars::esp_enabled)
	{
		if (reinterpret_cast<size_t>(NGlobals::GetEntityList()) >= NGlobals::game_base_address)
		{
			for (int i = 0; i < NGlobals::GetEntityListSize(); ++i)
			{
				if (reinterpret_cast<size_t>(NGlobals::GetEntityList()[i]) <= NGlobals::game_base_address)
					continue;

				const NSDK::SPlayer current_entity = *NGlobals::GetEntityList()[i];

				if (current_entity.health <= 0)
					continue;

				CColour ent_colour = (NVars::aimbot_ignore_team && current_entity.team == NGlobals::LocalPlayer().team) ? CColour::Purple : CColour::Orange;

				NMath::CVec3f min_3d = current_entity.position - NMath::CVec3f(current_entity.radius, current_entity.radius, 0.f);
				NMath::CVec3f max_3d = current_entity.position + NMath::CVec3f(current_entity.radius, current_entity.radius, current_entity.height);

				NMath::CVec3f corners_3d[8] = {
					min_3d,
					NMath::CVec3f(min_3d[0], max_3d[1], min_3d[2]),
					NMath::CVec3f(max_3d[0], min_3d[1], min_3d[2]),
					NMath::CVec3f(max_3d[0], max_3d[1], min_3d[2]),
					NMath::CVec3f(min_3d[0], min_3d[1], max_3d[2]),
					NMath::CVec3f(min_3d[0], max_3d[1], max_3d[2]),
					NMath::CVec3f(max_3d[0], min_3d[1], max_3d[2]),
					max_3d
				};

				NMath::CVec2f corners_2d[8];

				bool draw_box = false;

				for (int j = 0; j < 8; ++j)
				{
					if (NMisc::WorldToScreen(corners_3d[j], corners_2d[j]) != NMisc::W2S_RESULT::BEHIND)
						draw_box = true;
				}

				NMath::CVec2f min_2d(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
				NMath::CVec2f max_2d = -min_2d;

				for (int j = 0; j < 8; ++j)
				{
					min_2d = corners_2d[j].Min(min_2d);
					max_2d = corners_2d[j].Max(max_2d);
				}

				NMath::CVec2f size_2d = max_2d - min_2d;

				switch (NVars::esp_box_dimensions)
				{
					case BOX_DIMENSION::TWO:
					{
						if (!draw_box)
							break;
						
						NCanvas::Begin2D();
						{
							NCanvas::NDraw::Rect(min_2d, size_2d, ent_colour.WithAlpha(50));
							NCanvas::NDraw::OutlinedRect(min_2d, size_2d, ent_colour);

							NCanvas::NDraw::OutlinedRect(min_2d + NMath::CVec2f(-7.f, 0.f), NMath::CVec2f(5.f, size_2d[1]), CColour::Black);
							NCanvas::NDraw::OutlinedRect(min_2d + NMath::CVec2f(-6.f, 1.f), NMath::CVec2f(3.f, size_2d[1] - 2.f), CColour(40, 120, 250));
							NCanvas::NDraw::Line(min_2d + NMath::CVec2f(-5.f, 2.f), min_2d + NMath::CVec2f(-5.f, size_2d[1] - 4.f), CColour::Black);

							NCanvas::NDraw::Rect(min_2d + NMath::CVec2f(-6.f,
								1.f + (100.f - current_entity.health) / 100.f * (size_2d[1] - 2.f)), NMath::CVec2f(3.f, current_entity.health / 100.f * (size_2d[1] - 2.f)),
								CHSB(current_entity.health / 300.f, 1.f, 1.f).ToColour());

							NMath::CVec2f pos_2d;

							NMisc::WorldToScreen(current_entity.position, pos_2d);

							NCanvas::NDraw::OutlinedLine(NGlobals::GameResolution() / 2.f, pos_2d, CColour::White);
						}
						NCanvas::End2D();

						break;
					}
					case BOX_DIMENSION::THREE:
					{
						NCanvas::Begin3D();
						{
							NCanvas::NDraw::Cuboid(NGlobals::GetEntityList()[i]->position, NMath::CVec3f(current_entity.radius * 2.f,
								current_entity.radius * 2.f, current_entity.height), ent_colour.WithAlpha(50));

							NCanvas::NDraw::OutlinedCuboid(NGlobals::GetEntityList()[i]->position, NMath::CVec3f(current_entity.radius * 2.f,
								current_entity.radius * 2.f, current_entity.height), ent_colour);
						}
						NCanvas::End3D();

						break;
					}
				}
			}
		}
	}

	NCanvas::Begin2D();
	{
		if (NInput::WasKeyReleased(VK_INSERT))
		{
			NInput::ToggleInputBlock();
			NInput::ToggleMouseLock();
			NMenuItems::main_window->enabled = !NInput::mouse_locked;
		}

		NMenu::BeginScene();
		{
			NMenu::UpdateAndDrawAll();
		}
		NMenu::EndScene();


		if (!NInput::mouse_locked)
		{
			const NMath::CVec2f temp = NInput::GetMousePos();

			NMath::CVec2f cursor_arr[3] = {
				temp,
				temp + NMath::CVec2f(12.f, 15.f),
				temp + NMath::CVec2f(0.f, 20.f)
			};

			NCanvas::NDraw::Poly(3, cursor_arr, CColour::White);
			NCanvas::NDraw::OutlinedPoly(3, cursor_arr, CColour::Black);
		}

	}
	NCanvas::End2D();
}

void NCheat::Visuals()
{
	if (NVars::visuals_field_of_view)
		NGlobals::GetFov() = NVars::visuals_field_of_view_angle;
}

void NCheat::Movement()
{
	if (NInput::input_blocked)
		return;

	if (NVars::miscs_auto_pistol && !(NVars::aimbot_locked && NVars::aimbot_auto_shoot))
	{
		NGlobals::LocalPlayer().attacking = NInput::IsKeyDown(VK_LBUTTON);
	}

	if (NVars::miscs_bunny_hop)
		if (NInput::IsKeyDown(VK_SPACE))
			if (!NGlobals::LocalPlayer().on_floor)
				NGlobals::LocalPlayer().jump_next = true;

	if (NVars::miscs_fly)
		NGlobals::LocalPlayer().jump_next = NGlobals::LocalPlayer().on_floor = NInput::IsKeyDown(VK_SPACE);
}

bool NCheat::Terminate()
{
	return false;
}

void frame_select_function(NMenu::CFrame * prev_frame, NMenu::CButton * prev_frame_select, NMenu::CFrame* new_frame, NMenu::CButton* new_frame_select)
{
	prev_frame->enabled = false;
	prev_frame_select->SetColour(CColour(60, 160, 220));
	new_frame->enabled = true;
	new_frame_select->SetColour(CColour(255, 255, 255));
}

bool NCheat::Initiate()
{
	static float num_frames = 4;

	static NMenu::CFrame* active_frame;
	static NMenu::CButton* active_frame_select;

	NMenuItems::main_window = NMenu::CreateItem<NMenu::CWindow>();
	NMenuItems::main_window->SetPosition(NMath::CVec2f(50.f, 50.f));
	NMenuItems::main_window->SetSize(NMath::CVec2f(300.f, 500.f));
	NMenuItems::main_window->SetColour(CColour(80, 180, 240));
	NMenuItems::main_window->SetText("Inertia");
	NMenuItems::main_window->enabled = false;

	/* Frames */

	NMenuItems::aimbot_frame = NMenu::CreateItem<NMenu::CFrame>(NMenuItems::main_window); active_frame = NMenuItems::aimbot_frame;
	NMenuItems::aimbot_frame->SetPosition(NMath::CVec2f(0.f, 25.f));
	NMenuItems::aimbot_frame->SetSize(NMenuItems::main_window->GetSize() - NMath::CVec2f(0.f, 25.f));
	NMenuItems::aimbot_frame->SetColour(CColour(200, 200, 200));

	NMenuItems::esp_frame = NMenu::CreateItem<NMenu::CFrame>(NMenuItems::main_window);
	NMenuItems::esp_frame->SetPosition(NMath::CVec2f(0.f, 25.f));
	NMenuItems::esp_frame->SetSize(NMenuItems::main_window->GetSize() - NMath::CVec2f(0.f, 25.f));
	NMenuItems::esp_frame->SetColour(CColour(200, 200, 200));
	NMenuItems::esp_frame->enabled = false;

	NMenuItems::visuals_frame = NMenu::CreateItem<NMenu::CFrame>(NMenuItems::main_window);
	NMenuItems::visuals_frame->SetPosition(NMath::CVec2f(0.f, 25.f));
	NMenuItems::visuals_frame->SetSize(NMenuItems::main_window->GetSize() - NMath::CVec2f(0.f, 25.f));
	NMenuItems::visuals_frame->SetColour(CColour(200, 200, 200));
	NMenuItems::visuals_frame->enabled = false;

	NMenuItems::miscs_frame = NMenu::CreateItem<NMenu::CFrame>(NMenuItems::main_window);
	NMenuItems::miscs_frame->SetPosition(NMath::CVec2f(0.f, 25.f));
	NMenuItems::miscs_frame->SetSize(NMenuItems::main_window->GetSize() - NMath::CVec2f(0.f, 25.f));
	NMenuItems::miscs_frame->SetColour(CColour(200, 200, 200));
	NMenuItems::miscs_frame->enabled = false;

	/* Frame Selects */

	NMenuItems::aimbot_frame_select = NMenu::CreateItem<NMenu::CButton>(NMenuItems::main_window); active_frame_select = NMenuItems::aimbot_frame_select;
	NMenuItems::aimbot_frame_select->SetPosition(NMath::CVec2f(300.f * (0.f / num_frames), 0.f));
	NMenuItems::aimbot_frame_select->SetSize(NMath::CVec2f(300.f / num_frames + 1.f, 26.f));
	NMenuItems::aimbot_frame_select->SetColour(CColour(255, 255, 255));
	NMenuItems::aimbot_frame_select->SetText("Aimbot");
	NMenuItems::aimbot_frame_select->OnClick = [&]
	{
		frame_select_function(active_frame, active_frame_select, NMenuItems::aimbot_frame, NMenuItems::aimbot_frame_select);
		active_frame = NMenuItems::aimbot_frame;
		active_frame_select = NMenuItems::aimbot_frame_select;
	};

	NMenuItems::esp_frame_select = NMenu::CreateItem<NMenu::CButton>(NMenuItems::main_window);
	NMenuItems::esp_frame_select->SetPosition(NMath::CVec2f(300.f * (1.f / num_frames), 0.f));
	NMenuItems::esp_frame_select->SetSize(NMath::CVec2f(300.f / num_frames + 1.f, 26.f));
	NMenuItems::esp_frame_select->SetColour(CColour(60, 160, 220));
	NMenuItems::esp_frame_select->SetText("ESP");
	NMenuItems::esp_frame_select->OnClick = [&]
	{
		frame_select_function(active_frame, active_frame_select, NMenuItems::esp_frame, NMenuItems::esp_frame_select);
		active_frame = NMenuItems::esp_frame;
		active_frame_select = NMenuItems::esp_frame_select;
	};

	NMenuItems::visuals_frame_select = NMenu::CreateItem<NMenu::CButton>(NMenuItems::main_window);
	NMenuItems::visuals_frame_select->SetPosition(NMath::CVec2f(300.f * (2.f / num_frames), 0.f));
	NMenuItems::visuals_frame_select->SetSize(NMath::CVec2f(300.f / num_frames + 1.f, 26.f));
	NMenuItems::visuals_frame_select->SetColour(CColour(60, 160, 220));
	NMenuItems::visuals_frame_select->SetText("Visuals");
	NMenuItems::visuals_frame_select->OnClick = [&]
	{
		frame_select_function(active_frame, active_frame_select, NMenuItems::visuals_frame, NMenuItems::visuals_frame_select);
		active_frame = NMenuItems::visuals_frame;
		active_frame_select = NMenuItems::visuals_frame_select;
	};

	NMenuItems::miscs_frame_select = NMenu::CreateItem<NMenu::CButton>(NMenuItems::main_window);
	NMenuItems::miscs_frame_select->SetPosition(NMath::CVec2f(300.f * (3.f / num_frames), 0.f));
	NMenuItems::miscs_frame_select->SetSize(NMath::CVec2f(300.f / num_frames + 1.f, 26.f));
	NMenuItems::miscs_frame_select->SetColour(CColour(60, 160, 220));
	NMenuItems::miscs_frame_select->SetText("Miscellaneous");
	NMenuItems::miscs_frame_select->OnClick = [&]
	{
		frame_select_function(active_frame, active_frame_select, NMenuItems::miscs_frame, NMenuItems::miscs_frame_select);
		active_frame = NMenuItems::miscs_frame;
		active_frame_select = NMenuItems::miscs_frame_select;
	};

	NMenuItems::aimbot_enabled = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_enabled->SetPosition(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_enabled->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_enabled->SetVariable(&NVars::aimbot_enabled);
	NMenuItems::aimbot_enabled->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_enabled->SetText("Enable");

	NMenuItems::aimbot_vis_check = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_vis_check->SetPosition(NMath::CVec2f(10.f, 30.f));
	NMenuItems::aimbot_vis_check->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_vis_check->SetVariable(&NVars::aimbot_vis_check);
	NMenuItems::aimbot_vis_check->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_vis_check->SetText("Vis Check");

	NMenuItems::aimbot_ignore_team = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_ignore_team->SetPosition(NMath::CVec2f(10.f, 50.f));
	NMenuItems::aimbot_ignore_team->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_ignore_team->SetVariable(&NVars::aimbot_ignore_team);
	NMenuItems::aimbot_ignore_team->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_ignore_team->SetText("Ignore Team");

	NMenuItems::aimbot_on_key = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_on_key->SetPosition(NMath::CVec2f(10.f, 70.f));
	NMenuItems::aimbot_on_key->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_on_key->SetVariable(&NVars::aimbot_on_key);
	NMenuItems::aimbot_on_key->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_on_key->SetText("On Key");

	NMenuItems::aimbot_silent = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_silent->SetPosition(NMath::CVec2f(10.f, 90.f));
	NMenuItems::aimbot_silent->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_silent->SetVariable(&NVars::aimbot_silent);
	NMenuItems::aimbot_silent->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_silent->SetText("Silent");

	NMenuItems::aimbot_auto_shoot = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_auto_shoot->SetPosition(NMath::CVec2f(10.f, 110.f));
	NMenuItems::aimbot_auto_shoot->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_auto_shoot->SetVariable(&NVars::aimbot_auto_shoot);
	NMenuItems::aimbot_auto_shoot->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_auto_shoot->SetText("Auto Shoot");

	NMenuItems::aimbot_auto_duck = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_auto_duck->SetPosition(NMath::CVec2f(10.f, 130.f));
	NMenuItems::aimbot_auto_duck->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_auto_duck->SetVariable(&NVars::aimbot_auto_duck);
	NMenuItems::aimbot_auto_duck->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_auto_duck->SetText("Auto Duck");

	NMenuItems::aimbot_body_aim = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_body_aim->SetPosition(NMath::CVec2f(10.f, 150.f));
	NMenuItems::aimbot_body_aim->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_body_aim->SetVariable(&NVars::aimbot_body_aim);
	NMenuItems::aimbot_body_aim->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_body_aim->SetText("Body Aim");

	NMenuItems::aimbot_locked = NMenu::CreateItem<NMenu::CCheckBox>(NMenuItems::aimbot_frame);
	NMenuItems::aimbot_locked->SetPosition(NMath::CVec2f(10.f, 170.f));
	NMenuItems::aimbot_locked->SetSize(NMath::CVec2f(10.f, 10.f));
	NMenuItems::aimbot_locked->SetVariable(&NVars::aimbot_locked);
	NMenuItems::aimbot_locked->SetColour(CColour(0, 0, 0));
	NMenuItems::aimbot_locked->SetText("Locked");

	NMenuItems::radar_window = NMenu::CreateItem<NMenu::CWindow>();
	NMenuItems::radar_window->SetWindowPosition(NMath::CVec2f(0.f, 0.f));
	NMenuItems::radar_window->SetSize(NMath::CVec2f(200.f, 200.f));
	NMenuItems::radar_window->SetColour(CColour(80, 180, 240));
	NMenuItems::radar_window->SetText("Radar");
	NMenuItems::radar_window->ShowCloseButton(false);
	NMenuItems::radar_window->enabled = true;
	NMenuItems::radar_window->window_frame->Paint = []
	{
		const NMath::CVec2f radar_abs_pos = NMenuItems::radar_window->window_frame->GetAbsPosition();
		const NMath::CVec2f radar_size_div_two = NMenuItems::radar_window->window_frame->GetSize() / 2.f;

		if (NVars::radar_circular)
		{
			NCanvas::NDraw::OutlinedCircle(radar_abs_pos + radar_size_div_two, 100.f, CColour::Black);
			NCanvas::NDraw::Circle(radar_abs_pos + radar_size_div_two, 99.f, CColour::White);
		}
		else
		{
			NMenuItems::radar_window->window_frame->Draw();
		}

		const float fov_div_two = NMath::DegToRad(NGlobals::GetFov()) / 2.f;
		const float angle = NMath::DegToRad(NGlobals::LocalPlayer().view_angles[0]) + NMath::FLT_PI;

		float fov_rotation_angle = 0.f;

		if (!NVars::radar_rotate)
		{
			fov_rotation_angle = angle;
		}

		const float pos_angle = fov_div_two + fov_rotation_angle;
		const float neg_angle = -fov_div_two + fov_rotation_angle;

		float pos_fov_line_scaler = 1.f;
		float neg_fov_line_scaler = 1.f;

		if (!NVars::radar_circular)
		{
			pos_fov_line_scaler = NMath::Min(NMath::Abs(1.f / NMath::Cos(pos_angle)), NMath::Abs(1.f / NMath::Sin(pos_angle)));
			neg_fov_line_scaler = NMath::Min(NMath::Abs(1.f / NMath::Cos(neg_angle)), NMath::Abs(1.f / NMath::Sin(neg_angle)));
		}

		// FOV Lines

		NCanvas::NDraw::Line(
			radar_abs_pos + NMenuItems::radar_window->window_frame->GetSize() / 2.f,
			radar_abs_pos + NMenuItems::radar_window->window_frame->GetSize() / 2.f + NMath::CVec2f(NMath::Sin(pos_angle),
				-NMath::Cos(pos_angle)) * 100.f * pos_fov_line_scaler,
			CColour::Black
		);

		NCanvas::NDraw::Line(
			radar_abs_pos + radar_size_div_two,
			radar_abs_pos + radar_size_div_two + NMath::CVec2f(NMath::Sin(neg_angle),
				-NMath::Cos(neg_angle)) * 100.f * neg_fov_line_scaler,
			CColour::Black
		);

		if (reinterpret_cast<size_t>(NGlobals::GetEntityList()) >= NGlobals::game_base_address)
		{
			for (int i = 0; i < NGlobals::GetEntityListSize(); ++i)
			{
				if (reinterpret_cast<size_t>(NGlobals::GetEntityList()[i]) <= NGlobals::game_base_address)
					continue;

				const NSDK::SPlayer current_entity = *NGlobals::GetEntityList()[i];

				if (current_entity.health <= 0)
					continue;

				NMath::CVec2f relative_pos = (NGlobals::LocalPlayer().position - current_entity.position).AsCVector<2>() / NVars::radar_range;

				if (NVars::radar_circular)
				{
					if(relative_pos.LengthSqr() > 1.f)
					{
						relative_pos.NormalizeInPlace();
					}

					relative_pos *= 100.f;
				}
				else
				{
					NMath::CVec2f possible_position = NMenuItems::radar_window->window_frame->GetSize() / 2.f - NMath::CVec2f(6.f, 6.f);

					relative_pos[0] *= possible_position[0];
					relative_pos[1] *= possible_position[1];

					NMath::CVec2f abs_relative_pos = NMath::CVec2f(NMath::Abs(relative_pos[0]), NMath::Abs(relative_pos[1]));

					if (NMath::Abs(relative_pos[0]) > possible_position[0] || NMath::Abs(relative_pos[1]) > possible_position[1])
					{
						if (abs_relative_pos[0] / possible_position[0] > abs_relative_pos[1] / possible_position[1])
						{
							relative_pos /= abs_relative_pos[0] / possible_position[0];
						}
						else
						{
							relative_pos /= abs_relative_pos[1] / possible_position[1];
						}
					}
				}

				if (NVars::radar_rotate)
				{
					relative_pos.RotateInPlace(NMath::CAngle<float, 1>(-angle));
				}

				NCanvas::NDraw::OutlinedRect(NMenuItems::radar_window->window_frame->GetAbsPosition() + (NMenuItems::radar_window->GetSize() / 2.f) + relative_pos - NMath::CVec2f(5.f, 5.f), NMath::CVec2f(10.f, 10.f), CColour(0, 0, 0));
				NCanvas::NDraw::Rect(NMenuItems::radar_window->window_frame->GetAbsPosition() + (NMenuItems::radar_window->GetSize() / 2.f) + relative_pos - NMath::CVec2f(4.f, 4.f), NMath::CVec2f(8.f, 8.f), CColour(200, 200, 200));
			}
		}
	};

	return true;
}
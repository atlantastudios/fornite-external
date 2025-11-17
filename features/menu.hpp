#pragma once
#include "../core/settings.hpp"
#include "../core/sdk.hpp"
#include "../imgui/imgui.h"
#include "../utils/Keybinds.h"

namespace menu {
	
	void render() {
		// FOV circle
		if (settings::aimbot::show_fov) {
			ImGui::GetForegroundDrawList()->AddCircle(
				ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2),
				settings::aimbot::fov,
				ImColor(250, 250, 250, 250), 100, 1.0f
			);
		}
		
		// Toggle menu
		if (GetAsyncKeyState(VK_INSERT) & 1) {
			settings::show_menu = !settings::show_menu;
		}
		
		if (!settings::show_menu) return;
		
		ImGui::SetNextWindowSize({ 850, 650 });
		ImGui::Begin("NiggaKillWare V1 [aimbot is broken right neow]", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		
		// Tabs
		if (ImGui::Button("Aimbot", { 200, 25 })) settings::tab = 0;
		ImGui::SameLine();
		if (ImGui::Button("Visuals", { 200, 25 })) settings::tab = 1;
		ImGui::SameLine();
		if (ImGui::Button("Misc", { 200, 25 })) settings::tab = 3;
		
		switch (settings::tab) {
			case 0: // Aimbot
			{
				ImGui::Checkbox("Enable Aimbot", &settings::aimbot::enable);
				if (settings::aimbot::enable) {
					ImGui::Text("Locks camera to enemy head while keybind is held");
					ImGui::SliderFloat("Smoothness", &settings::aimbot::smoothness, 1.0f, 20.0f, "%.1f");
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Higher = smoother but slower aim");
				}
				ImGui::Checkbox("Visible Check", &settings::aimbot::visible_check);
				ImGui::Checkbox("Show Fov", &settings::aimbot::show_fov);
				if (settings::aimbot::show_fov) {
					ImGui::SliderFloat("Fov Size", &settings::aimbot::fov, 50, 300, "%.0f");
				}
				ImGui::Text("Aimbot Key");
				HotkeyButton(settings::aimbot::current_key, ChangeKey, keystatus);
				break;
			}
			case 1: // Visuals
			{
				ImGui::Checkbox("Enable", &settings::visuals::enable);
				ImGui::Checkbox("Cornered Box", &settings::visuals::CorneredBox);
				ImGui::Checkbox("Fill Box", &settings::visuals::fill_box);
				ImGui::Checkbox("Fill Box WColor", &settings::visuals::fill_wcbox);
				ImGui::Checkbox("Skeleton", &settings::visuals::skeleton);
				ImGui::Checkbox("Box", &settings::visuals::Box);
				ImGui::Checkbox("Line", &settings::visuals::line);
				ImGui::Checkbox("Distance", &settings::visuals::distance);
				break;
			}
			case 3: // Misc
			{
				ImGui::Checkbox("WaterMark", &settings::Features::Watermark);
				ImGui::Checkbox("Overlay FPS", &settings::Features::fpswater);
				ImGui::Checkbox("Info", &settings::Features::info);
				ImGui::Checkbox("Crosshair", &settings::Features::crosshair);
				ImGui::Checkbox("Radar", &settings::radar::radar);
				if (ImGui::Button("Unload", { 120, 20 })) exit(0);
				break;
			}
		}
		
		ImGui::End();
	}
}


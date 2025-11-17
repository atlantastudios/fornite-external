#pragma once
#include "../core/sdk.hpp"
#include "../core/settings.hpp"
#include "../cache/player_cache.hpp"
#include "../imgui/imgui.h"

namespace radar_feature {
	
	void render() {
		if (!settings::radar::radar) return;
		
		static ImVec2 radarSize = { 200, 200 };
		static ImVec2 radarPos = { 30, (ImGui::GetIO().DisplaySize.y / 4) - (radarSize.y / 2) + 180 };
		static int RedDotSize = 3;
		
		ImGui::SetNextWindowSize(radarSize);
		ImGui::SetNextWindowPos(radarPos);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(10, 10, 10, 155));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("##Radar", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);
		
		ImVec2 winpos = radarPos;
		ImVec2 winsize = radarSize;
		
		// Draw cross
		if (settings::radar::cross) {
			ImGui::GetWindowDrawList()->AddLine(
				ImVec2(winpos.x + winsize.x * 0.5f, winpos.y),
				ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y),
				ImGui::GetColorU32(ImGuiCol_Border), 1.0f
			);
			ImGui::GetWindowDrawList()->AddLine(
				ImVec2(winpos.x, winpos.y + winsize.y * 0.5f),
				ImVec2(winpos.x + winsize.x, winpos.y + winsize.y * 0.5f),
				ImGui::GetColorU32(ImGuiCol_Border), 1.0f
			);
		}
		
		// Draw local player
		if (settings::radar::local_player) {
			ImGui::GetWindowDrawList()->AddCircleFilled(
				ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y * 0.5f),
				2.0f, ImColor(255, 255, 255, 255), 64
			);
		}
		
		// Draw enemies using cached data
		float dotsize = RedDotSize;
		if (dotsize < 1.0f) dotsize = 1.0f;
		if (dotsize > 10.0f) dotsize = 10.0f;
		
		float range = (float)(settings::radar::range / 5) * 1000.0f;
		
		for (const auto& cached : player_cache::cached_players) {
			if (!cached.is_valid) continue;
			
			Vector3 bottom3d = cached.bottom_3d;
			Vector3 vAngle = cache::local_camera.rotation;
			auto fYaw = vAngle.y * M_PI / 180.0f;
			
			float dx = bottom3d.x - cache::local_camera.location.x;
			float dy = bottom3d.y - cache::local_camera.location.y;
			float fsin_yaw = sinf(fYaw);
			float fminus_cos_yaw = -cosf(fYaw);
			float x = dy * fminus_cos_yaw + dx * fsin_yaw;
			x = -x;
			float y = dx * fminus_cos_yaw - dy * fsin_yaw;
			
			// Clamp to radar range
			if (fabs(x) > range || fabs(y) > range) {
				if (y > x) {
					if (y > -x) {
						x = range * x / y;
						y = range;
					} else {
						y = -range * y / x;
						x = -range;
					}
				} else {
					if (y > -x) {
						y = range * y / x;
						x = range;
					} else {
						x = -range * x / y;
						y = -range;
					}
				}
			}
			
			int ScreenX = (int)(winpos.x + winsize.x * 0.5f + (x / range * winsize.x));
			int ScreenY = (int)(winpos.y + winsize.y * 0.5f + (y / range * winsize.y));
			
			// Clamp to radar bounds
			if (ScreenX < winpos.x) ScreenX = (int)winpos.x;
			if (ScreenX > winpos.x + winsize.x) ScreenX = (int)(winpos.x + winsize.x);
			if (ScreenY < winpos.y) ScreenY = (int)winpos.y;
			if (ScreenY > winpos.y + winsize.y) ScreenY = (int)(winpos.y + winsize.y);
			
			ImGui::GetWindowDrawList()->AddCircleFilled(
				ImVec2((float)ScreenX, (float)ScreenY),
				dotsize, ImColor(255, 0, 0, 255), 64
			);
		}
		
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}
}


#pragma once
#include "../core/settings.hpp"
#include "../core/sdk.hpp"
#include "../imgui/imgui.h"
#include "../utils/Keybinds.h"
#include <chrono>

namespace ui {
	
	void draw_crosshair() {
		if (!settings::Features::crosshair) return;
		
		ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
		ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float size = 10.0f;
		
		ImGui::GetForegroundDrawList()->AddLine(
			ImVec2(center.x - size, center.y), ImVec2(center.x + size, center.y),
			ImGui::ColorConvertFloat4ToU32(color), 2.0f
		);
		ImGui::GetForegroundDrawList()->AddLine(
			ImVec2(center.x, center.y - size), ImVec2(center.x, center.y + size),
			ImGui::ColorConvertFloat4ToU32(color), 2.0f
		);
	}
	
	void draw_watermark() {
		if (!settings::Features::Watermark) return;
		
		ImVec2 position = ImVec2(30, 30);
		ImColor color = ImColor(0.2f, 0.6f, 1.0f, 1.0f);
		const char* text = "SofMainFNBase [Free] [Public]";
		ImGui::GetForegroundDrawList()->AddText(position, color, text);
	}
	
	void draw_fps() {
		if (!settings::Features::fpswater) return;
		
		char fpsText[32];
		snprintf(fpsText, sizeof(fpsText), "FPS: %.2f", ImGui::GetIO().Framerate);
		ImVec2 position = ImVec2(45.0f, 60.0f);
		ImColor fpsColor = ImColor(0.2f, 0.6f, 1.0f, 1.0f);
		ImGui::GetForegroundDrawList()->AddText(position, fpsColor, fpsText);
	}
	
	void draw_info() {
		if (!settings::Features::info) return;
		
		static auto lastTime = std::chrono::high_resolution_clock::now();
		static int frameCount = 0;
		
		auto currentTime = std::chrono::high_resolution_clock::now();
		frameCount++;
		
		static float fps = 0.0f;
		if (std::chrono::duration<float>(currentTime - lastTime).count() >= 1.0f) {
			fps = static_cast<float>(frameCount) / std::chrono::duration<float>(currentTime - lastTime).count();
			frameCount = 0;
			lastTime = currentTime;
		}
		
		const char* watermarkText = "[Status: Undetected] - [Developer: SofMain] - [Free / Public ]";
		char fullText[512];
		snprintf(fullText, sizeof(fullText), "%s | FPS: %.2f", watermarkText, fps);
		ImGui::Begin("Info", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImVec2 position = ImVec2(35, 30);
		ImGui::SetCursorPos(position);
		ImGui::TextUnformatted(fullText);
		ImGui::End();
	}
	
	void render_all() {
		draw_crosshair();
		draw_watermark();
		draw_fps();
		draw_info();
	}
}


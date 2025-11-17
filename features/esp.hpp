#pragma once
#include "../core/sdk.hpp"
#include "../core/settings.hpp"
#include "../cache/player_cache.hpp"
#include "../imgui/imgui.h"

namespace esp {
	
	// Optimized drawing functions
	inline void draw_box(float x, float y, float w, float h, ImColor color, int thickness) {
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		draw_list->AddLine(ImVec2(x, y), ImVec2(x + w, y), color, thickness);
		draw_list->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + h), color, thickness);
		draw_list->AddLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, thickness);
		draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + h), color, thickness);
	}
	
	inline void draw_cornered_box(float x, float y, float w, float h, ImColor color, int thickness) {
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		float third_w = w / 3.0f;
		float third_h = h / 3.0f;
		
		// Top-left
		draw_list->AddLine(ImVec2(x, y), ImVec2(x, y + third_h), color, thickness);
		draw_list->AddLine(ImVec2(x, y), ImVec2(x + third_w, y), color, thickness);
		// Top-right
		draw_list->AddLine(ImVec2(x + w - third_w, y), ImVec2(x + w, y), color, thickness);
		draw_list->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + third_h), color, thickness);
		// Bottom-left
		draw_list->AddLine(ImVec2(x, y + h - third_h), ImVec2(x, y + h), color, thickness);
		draw_list->AddLine(ImVec2(x, y + h), ImVec2(x + third_w, y + h), color, thickness);
		// Bottom-right
		draw_list->AddLine(ImVec2(x + w - third_w, y + h), ImVec2(x + w, y + h), color, thickness);
		draw_list->AddLine(ImVec2(x + w, y + h - third_h), ImVec2(x + w, y + h), color, thickness);
	}
	
	inline void draw_filled_rect(float x, float y, float w, float h, ImU32 color) {
		ImGui::GetForegroundDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
	}
	
	inline void draw_line(Vector2 target, ImColor color) {
		ImGui::GetForegroundDrawList()->AddLine(
			ImVec2(settings::screen_center_x, settings::height),
			ImVec2(target.x, target.y),
			color, 1.0f
		);
	}
	
	inline void draw_distance(Vector2 location, float distance, ImColor color) {
		char dist[64];
		sprintf_s(dist, "%.fm", distance);
		ImVec2 text_size = ImGui::CalcTextSize(dist);
		ImGui::GetForegroundDrawList()->AddText(
			ImVec2(location.x - text_size.x / 2, location.y - text_size.y / 2),
			color, dist
		);
	}
	
	// Render skeleton (optimized - updates every frame for accuracy)
	void render_skeleton(const player_cache::CachedPlayer& cached) {
		if (!cached.is_valid || !cached.mesh) return;
		
		// Only render if skeleton ESP is enabled
		if (!settings::visuals::skeleton) return;
		
		// Get bones fresh every frame for accuracy
		const int bone_ids[] = { 66, 9, 10, 11, 38, 39, 40, 2, 71, 72, 78, 79, 75, 82, 67 };
		constexpr int bone_count = sizeof(bone_ids) / sizeof(bone_ids[0]);
		
		std::vector<Vector3> bones_3d(bone_count);
		std::vector<Vector2> bones_2d(bone_count);
		
		// Get all bone positions
		for (int i = 0; i < bone_count; i++) {
			bones_3d[i] = get_entity_bone(cached.mesh, bone_ids[i]);
			bones_2d[i] = project_world_to_screen(bones_3d[i]);
		}
		
		// Validate bones are on screen before rendering
		bool has_valid_bones = false;
		for (const auto& bone : bones_2d) {
			if (bone.x > 0 && bone.x < settings::width && bone.y > 0 && bone.y < settings::height) {
				has_valid_bones = true;
				break;
			}
		}
		
		if (!has_valid_bones) return;
		
		ImColor color = cached.is_visible ? ImColor(36, 207, 2, 250) : ImColor(250, 0, 0, 250);
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		
		// Draw skeleton lines
		if (bones_2d.size() >= 14) {
			// Left shoulder to right shoulder
			draw_list->AddLine(ImVec2(bones_2d[1].x, bones_2d[1].y), ImVec2(bones_2d[4].x, bones_2d[4].y), color, 2.0f);
			// Left arm
			draw_list->AddLine(ImVec2(bones_2d[1].x, bones_2d[1].y), ImVec2(bones_2d[2].x, bones_2d[2].y), color, 2.0f);
			draw_list->AddLine(ImVec2(bones_2d[2].x, bones_2d[2].y), ImVec2(bones_2d[3].x, bones_2d[3].y), color, 2.0f);
			// Right arm
			draw_list->AddLine(ImVec2(bones_2d[4].x, bones_2d[4].y), ImVec2(bones_2d[5].x, bones_2d[5].y), color, 2.0f);
			draw_list->AddLine(ImVec2(bones_2d[5].x, bones_2d[5].y), ImVec2(bones_2d[6].x, bones_2d[6].y), color, 2.0f);
			// Torso
			draw_list->AddLine(ImVec2(bones_2d[0].x, bones_2d[0].y), ImVec2(bones_2d[7].x, bones_2d[7].y), color, 2.0f);
			// Left leg
			draw_list->AddLine(ImVec2(bones_2d[7].x, bones_2d[7].y), ImVec2(bones_2d[8].x, bones_2d[8].y), color, 2.0f);
			draw_list->AddLine(ImVec2(bones_2d[8].x, bones_2d[8].y), ImVec2(bones_2d[9].x, bones_2d[9].y), color, 2.0f);
			draw_list->AddLine(ImVec2(bones_2d[9].x, bones_2d[9].y), ImVec2(bones_2d[12].x, bones_2d[12].y), color, 2.0f);
			// Right leg
			draw_list->AddLine(ImVec2(bones_2d[7].x, bones_2d[7].y), ImVec2(bones_2d[10].x, bones_2d[10].y), color, 2.0f);
			draw_list->AddLine(ImVec2(bones_2d[10].x, bones_2d[10].y), ImVec2(bones_2d[11].x, bones_2d[11].y), color, 2.0f);
			draw_list->AddLine(ImVec2(bones_2d[11].x, bones_2d[11].y), ImVec2(bones_2d[13].x, bones_2d[13].y), color, 2.0f);
		}
	}
	
	// Render ESP for a single player (optimized)
	void render_player_esp(const player_cache::CachedPlayer& cached) {
		if (!cached.is_valid || cached.head_2d.x == 0 && cached.head_2d.y == 0) return;
		
		float x = cached.head_2d.x - (cached.box_width / 2);
		float y = cached.head_2d.y;
		float w = cached.box_width;
		float h = cached.box_height;
		
		ImColor visible_color = ImColor(36, 207, 2, 250);
		ImColor hidden_color = ImColor(250, 0, 0, 250);
		ImColor color = cached.is_visible ? visible_color : hidden_color;
		
		// Box ESP
		if (settings::visuals::Box) {
			draw_box(x, y, w, h, color, 1);
			if (settings::visuals::fill_box) {
				draw_filled_rect(x, y, w, h, IM_COL32(0, 0, 0, 50));
			}
		}
		
		// Cornered Box
		if (settings::visuals::CorneredBox) {
			draw_cornered_box(x, y, w, h, color, 1);
			if (settings::visuals::fill_box) {
				draw_filled_rect(x, y, w, h, IM_COL32(0, 0, 0, 50));
			}
		}
		
		// Fill with color
		if (settings::visuals::fill_wcbox) {
			ImU32 fill_color = IM_COL32(
				(int)(color.Value.x * 255.0f),
				(int)(color.Value.y * 255.0f),
				(int)(color.Value.z * 255.0f),
				100
			);
			draw_filled_rect(x, y, w, h, fill_color);
		}
		
		// Line
		if (settings::visuals::line) {
			draw_line(cached.bottom_2d, color);
		}
		
		// Distance
		if (settings::visuals::distance) {
			draw_distance(cached.bottom_2d, cached.distance, ImColor(250, 250, 250, 250));
		}
		
		// Skeleton (only when enabled, cached)
		if (settings::visuals::skeleton) {
			render_skeleton(cached);
		}
	}
	
	// Render all ESP (optimized batch rendering)
	void render_all() {
		if (!settings::visuals::enable) return;
		
		for (const auto& cached : player_cache::cached_players) {
			if (cached.is_valid) {
				render_player_esp(cached);
			}
		}
	}
}


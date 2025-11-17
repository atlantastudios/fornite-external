#pragma once
#include "../core/sdk.hpp"
#include "../core/input.hpp"
#include "../core/settings.hpp"
#include "../cache/player_cache.hpp"

namespace aimbot_feature {
	
	void update() {
		if (!settings::aimbot::enable) return;
		
		if (GetAsyncKeyState(settings::aimbot::current_key)) {
			uintptr_t target_mesh = player_cache::get_closest_player_in_fov(settings::aimbot::fov);
			
			if (target_mesh) {
				// Find cached player data
				for (const auto& cached : player_cache::cached_players) {
					if (cached.mesh == target_mesh && cached.is_valid) {
						if (settings::aimbot::visible_check && !cached.is_visible) {
							return;
						}
						
						// Lock to head using driver mouse input
						input::lock_to_head(cached.head_3d, settings::aimbot::smoothness);
						break;
					}
				}
			}
		}
	}
}


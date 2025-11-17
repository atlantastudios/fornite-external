#pragma once
#include "../includes.hpp"
#include "../cache/player_cache.hpp"
#include "../features/esp.hpp"
#include "../features/aimbot.hpp"
#include "../features/exploits.hpp"
#include "game_data.hpp"

namespace game_loop {
	
	static bool debug_printed = false;
	static int debug_frame_count = 0;
	
	// Optimized main game loop
	void update() {
		// Update game data structures
		game_data::update();
		
		// Update player cache (handles all player data reading)
		player_cache::update_cache();
		
		// Update features
		aimbot_feature::update();
		exploits::update();
		
		// Render ESP (uses cached data)
		esp::render_all();
		
		// Debug output (every 60 frames)
		debug_frame_count++;
		if (!debug_printed || debug_frame_count % 60 == 0) {
			printf("\n[DEBUG] ========================================\n");
			printf("[DEBUG] Game Loop Debug Info (Frame %d):\n", debug_frame_count);
			printf("[DEBUG] ========================================\n");
			printf("[DEBUG] Base Address: 0x%llX\n", sofmainisud);
			printf("[DEBUG] UWORLD: 0x%llX\n", cache::uworld);
			printf("[DEBUG] Player Count: %d\n", cache::player_count);
			printf("[DEBUG] Cached Players: %zu\n", player_cache::cached_players.size());
			printf("[DEBUG] ========================================\n");
			debug_printed = true;
		}
	}
}


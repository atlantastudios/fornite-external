#pragma once
#include "sdk.hpp"
#include "offsets.hpp"

extern uintptr_t sofmainisud;

namespace game_data {
	
	// Update game data structures (optimized batch reads)
	void update() {
		if (!sofmainisud) return;
		
		// Batch read core structures
		cache::uworld = read<uintptr_t>(sofmainisud + UWORLD);
		if (!cache::uworld) return;
		
		cache::game_instance = read<uintptr_t>(cache::uworld + GAME_INSTANCE);
		if (!cache::game_instance) return;
		
		uintptr_t local_players_array = read<uintptr_t>(cache::game_instance + LOCAL_PLAYERS);
		if (!local_players_array) return;
		
		cache::local_players = read<uintptr_t>(local_players_array);
		if (!cache::local_players) return;
		
		cache::player_controller = read<uintptr_t>(cache::local_players + PLAYER_CONTROLLER);
		if (!cache::player_controller) return;
		
		cache::local_pawn = read<uintptr_t>(cache::player_controller + LOCAL_PAWN);
		
		// Update local player data
		if (cache::local_pawn != 0) {
			cache::root_component = read<uintptr_t>(cache::local_pawn + ROOT_COMPONENT);
			if (cache::root_component) {
				cache::relative_location = read<Vector3>(cache::root_component + RELATIVE_LOCATION);
			}
			cache::player_state = read<uintptr_t>(cache::local_pawn + PLAYER_STATE);
			if (cache::player_state) {
				cache::my_team_id = read<int>(cache::player_state + TEAM_INDEX);
			}
		}
		
		// Update game state
		cache::game_state = read<uintptr_t>(cache::uworld + GAME_STATE);
		if (cache::game_state) {
			cache::player_array = read<uintptr_t>(cache::game_state + PLAYER_ARRAY);
			cache::player_count = read<int>(cache::game_state + (PLAYER_ARRAY + sizeof(uintptr_t)));
		}
	}
}


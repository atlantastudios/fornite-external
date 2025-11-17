#pragma once
#include "../core/sdk.hpp"
#include "../core/offsets.hpp"
#include <vector>
#include <unordered_map>

extern uintptr_t sofmainisud;

namespace player_cache {

	struct CachedPlayer {
		uintptr_t player_state = 0;
		uintptr_t pawn_private = 0;
		uintptr_t mesh = 0;
		int team_id = 0;
		bool is_valid = false;
		bool is_visible = false;

		// Cached positions (updated every few frames)
		Vector3 head_3d = { 0, 0, 0 };
		Vector3 bottom_3d = { 0, 0, 0 };
		Vector2 head_2d = { 0, 0 };
		Vector2 bottom_2d = { 0, 0 };
		float distance = 0.0f;
		float box_width = 0.0f;
		float box_height = 0.0f;

		// Cached bones for skeleton (only when skeleton ESP is enabled)
		std::vector<Vector3> bones_3d;
		std::vector<Vector2> bones_2d;
		bool bones_cached = false;

		// Frame tracking
		int last_update_frame = 0;
		int last_visibility_check = 0;
		int last_bone_update = 0;
	};

	// Cache storage
	inline std::vector<CachedPlayer> cached_players;
	inline int current_frame = 0;

	// Update intervals (frames)
	constexpr int UPDATE_INTERVAL_POSITION = 1;      // Update positions every frame
	constexpr int UPDATE_INTERVAL_VISIBILITY = 2;    // Check visibility every 2 frames
	constexpr int UPDATE_INTERVAL_BONES = 1;         // Update bones every frame (only if skeleton enabled)

	// Update player cache (optimized batch reads)
	void update_cache() {
		cached_players.clear();
		current_frame++;

		// Update camera for radar and other features
		cache::local_camera = get_view_point();

		if (!cache::player_array || cache::player_count <= 0) {
			cached_players.clear();
			return;
		}

		// Resize cache if needed
		if (cached_players.size() < cache::player_count) {
			cached_players.resize(cache::player_count);
		}

		// Batch read all player states first
		std::vector<uintptr_t> player_states(cache::player_count);
		for (int i = 0; i < cache::player_count; i++) {
			player_states[i] = read<uintptr_t>(cache::player_array + (i * sizeof(uintptr_t)));
		}

		// Process each player
		for (int i = 0; i < cache::player_count; i++) {
			auto& cached = cached_players[i];
			uintptr_t player_state = player_states[i];

			if (!player_state) {
				cached.is_valid = false;
				continue;
			}

			// Read team ID
			int team_id = read<int>(player_state + TEAM_INDEX);
			if (team_id == cache::my_team_id) {
				cached.is_valid = false;
				continue;
			}

			// Read pawn
			uintptr_t pawn_private = read<uintptr_t>(player_state + PAWN_PRIVATE);
			if (!pawn_private || pawn_private == cache::local_pawn) {
				cached.is_valid = false;
				continue;
			}

			// Read mesh
			uintptr_t mesh = read<uintptr_t>(pawn_private + MESH);
			if (!mesh) {
				cached.is_valid = false;
				continue;
			}

			// Update cached data
			cached.player_state = player_state;
			cached.pawn_private = pawn_private;
			cached.mesh = mesh;
			cached.team_id = team_id;
			cached.is_valid = true;

			// Update positions (every frame for smooth ESP)
			if (current_frame - cached.last_update_frame >= UPDATE_INTERVAL_POSITION) {
				cached.head_3d = get_entity_bone(mesh, 110);
				cached.bottom_3d = get_entity_bone(mesh, 0);

				if (cached.head_3d.x != 0 || cached.head_3d.y != 0 || cached.head_3d.z != 0) {
					cached.head_2d = project_world_to_screen(Vector3(cached.head_3d.x, cached.head_3d.y, cached.head_3d.z + 20));
					cached.bottom_2d = project_world_to_screen(cached.bottom_3d);
					cached.distance = cache::relative_location.distance(cached.bottom_3d) / 100.0f;
					cached.box_height = abs(cached.head_2d.y - cached.bottom_2d.y);
					cached.box_width = cached.box_height * 0.50f;
				}

				cached.last_update_frame = current_frame;
			}

			// Update visibility (less frequently)
			if (current_frame - cached.last_visibility_check >= UPDATE_INTERVAL_VISIBILITY) {
				cached.is_visible = is_visible(mesh);
				cached.last_visibility_check = current_frame;
			}
		}
	}

	// Get closest player for aimbot
	uintptr_t get_closest_player_in_fov(float fov) {
		uintptr_t closest_mesh = 0;
		float closest_distance = FLT_MAX;

		for (const auto& cached : cached_players) {
			if (!cached.is_valid || !cached.mesh) continue;

			double dx = cached.head_2d.x - settings::screen_center_x;
			double dy = cached.head_2d.y - settings::screen_center_y;
			float dist = sqrtf((float)(dx * dx + dy * dy));

			if (dist <= fov && dist < closest_distance) {
				closest_distance = dist;
				closest_mesh = cached.mesh;
			}
		}

		return closest_mesh;
	}

	// Clear cache
	void clear() {
		cached_players.clear();
		current_frame = 0;
	}
}

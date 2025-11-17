//DONT TOUCH IF YOU DONT KNOW WHAT U DOING BOYS... I MEANNN... SOFMAIN WARNED YOUUU
#pragma once
#include <Windows.h>
#include <cmath>
#include "includes.hpp"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510
#endif

namespace input
{
	// Module handles for gui.hpp
	HMODULE user32 = 0;
	HMODULE win32u = 0;
	
	static BYTE ntusersendinput_bytes[30];
	
	// Initialize - needed for gui overlay and mouse input
	static BOOLEAN WINAPI init()
	{
		HMODULE user32_lib = LoadLibraryA("user32.dll");
		if (!user32_lib) return false;
		HMODULE win32u_lib = LoadLibraryA("win32u.dll");
		if (!win32u_lib) return false;
		user32 = GetModuleHandleA("user32.dll");
		if (!user32) return false;
		win32u = GetModuleHandleA("win32u.dll");
		if (!win32u) return false;
		
		// Get NtUserSendInput for mouse movement
		LPVOID ntusersendinput_addr = GetProcAddress(user32, "NtUserSendInput");
		if (!ntusersendinput_addr)
		{
			ntusersendinput_addr = GetProcAddress(win32u, "NtUserSendInput");
			if (!ntusersendinput_addr) return FALSE;
		}
		memcpy(ntusersendinput_bytes, ntusersendinput_addr, 30);
		
		return TRUE;
	}
	
	// Lock camera to target head using driver mouse input (UNDETECTED)
	static BOOLEAN WINAPI lock_to_head(Vector3 head_position, float smoothness)
	{
		if (!cache::player_controller || !cache::uworld) return FALSE;
		
		// Get camera location and rotation
		uintptr_t location_pointer = read<uintptr_t>(cache::uworld + CAMERA_LOCATION);
		uintptr_t rotation_pointer = read<uintptr_t>(cache::uworld + CAMERA_ROTATION);
		if (!location_pointer || !rotation_pointer) return FALSE;
		
		Vector3 camera_location = read<Vector3>(location_pointer);
		
		// Read current rotation
		double rot_a = read<double>(rotation_pointer);
		double rot_b = read<double>(rotation_pointer + ROTATION_OFFSET_B);
		double rot_c = read<double>(rotation_pointer + ROTATION_OFFSET_C);
		
		// Convert to degrees
		float currentYaw = (float)(atan2(rot_a * -1, rot_b) * (180.0 / M_PI)) * -1;
		float currentPitch = (float)(asin(rot_c) * (180.0 / M_PI));
		
		// Calculate direction to head
		Vector3 direction = head_position - camera_location;
		float distance = sqrtf(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);
		if (distance < 1.0f) return FALSE;
		
		// Normalize direction
		direction.x /= distance;
		direction.y /= distance;
		direction.z /= distance;
		
		// Calculate target angles
		float targetYaw = (float)(atan2(direction.y, direction.x) * (180.0 / M_PI));
		float targetPitch = (float)(asin(direction.z) * (180.0 / M_PI));
		
		// Calculate angle differences
		float deltaYaw = targetYaw - currentYaw;
		float deltaPitch = targetPitch - currentPitch;
		
		// Normalize to -180 to 180
		while (deltaYaw > 180.0f) deltaYaw -= 360.0f;
		while (deltaYaw < -180.0f) deltaYaw += 360.0f;
		
		// Apply smoothness
		float moveYaw = deltaYaw / smoothness;
		float movePitch = deltaPitch / smoothness;
		
		// Convert angle delta to mouse movement (approximate conversion)
		// Adjust sensitivity multiplier based on your in-game sensitivity
		float sensitivity_multiplier = 0.5f; // Adjust this to match your sensitivity
		int mouseX = (int)(moveYaw * sensitivity_multiplier);
		int mouseY = (int)(-movePitch * sensitivity_multiplier); // Negative because pitch is inverted
		
		// Move mouse using driver (kernel-mode, undetected)
		if (abs(mouseX) > 0 || abs(mouseY) > 0)
		{
			mem::move_mouse(mouseX, mouseY);
			return TRUE;
		}
		
		return TRUE;
	}
}

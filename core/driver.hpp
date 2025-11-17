#pragma once

#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
uintptr_t virtualaddy;

// Updated IOCTL codes to match kernel driver
#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x8A2F, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_ba CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9B3E, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_guarded_region CTL_CODE(FILE_DEVICE_UNKNOWN, 0x7C4D, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_mouse_input CTL_CODE(FILE_DEVICE_UNKNOWN, 0x6D5C, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0x4E7F9A2B  // Updated security key

// Mouse event flags (matching Windows MOUSEEVENTF_* constants)
#define MOUSEEVENTF_MOVE           0x0001
#define MOUSEEVENTF_LEFTDOWN       0x0002
#define MOUSEEVENTF_LEFTUP         0x0004
#define MOUSEEVENTF_RIGHTDOWN      0x0008
#define MOUSEEVENTF_RIGHTUP        0x0010
#define MOUSEEVENTF_MIDDLEDOWN     0x0020
#define MOUSEEVENTF_MIDDLEUP       0x0040
#define MOUSEEVENTF_WHEEL          0x0800
#define MOUSEEVENTF_HWHEEL         0x1000
#define MOUSEEVENTF_ABSOLUTE       0x8000


typedef struct _rw {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

typedef struct _mouse_input {
	INT32 security;
	INT32 flags;        // Button flags (MOUSEEVENTF_LEFTDOWN, MOUSEEVENTF_RIGHTDOWN, etc.)
	INT32 x;            // X delta or absolute position
	INT32 y;            // Y delta or absolute position
	INT32 data;         // Wheel data (0 if no wheel event)
	ULONG extraInfo;    // Additional information
} mouse_input, * pmouse_input;

namespace mem {
	HANDLE driver_handle;
	INT32 process_id;

	bool find_driver() {
		// Updated device name to match kernel driver obfuscation
		driver_handle = CreateFileW((L"\\\\.\\MsWinZonesCacheCounterMutexA"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
			return false;

		return true;
	}

	void read_physical(PVOID address, PVOID buffer, DWORD size) {
		_rw arguments = { 0 };

		arguments.security = code_security;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = FALSE;

		DeviceIoControl(driver_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	void write_physical(PVOID address, PVOID buffer, DWORD size) {
		_rw arguments = { 0 };

		arguments.security = code_security;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = TRUE;

		DeviceIoControl(driver_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	uintptr_t find_image() {
		uintptr_t image_address = { NULL };
		_ba arguments = { NULL };

		arguments.security = code_security;
		arguments.process_id = process_id;
		arguments.address = (ULONGLONG*)&image_address;

		DeviceIoControl(driver_handle, code_ba, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	uintptr_t get_guarded_region() {
		uintptr_t guarded_region_address = { NULL };
		_ga arguments = { NULL };

		arguments.security = code_security;
		arguments.address = (ULONGLONG*)&guarded_region_address;

		DeviceIoControl(driver_handle, code_get_guarded_region, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return guarded_region_address;
	}

	INT32 find_process(LPCTSTR process_name) {
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name)) {
					CloseHandle(hsnap);
					process_id = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);

		return { NULL };
	}

	// Kernel-mode mouse input injection - bypasses user-mode detection
	bool send_mouse_input(INT32 flags, INT32 x, INT32 y, INT32 data = 0, ULONG extraInfo = 0) {
		if (!driver_handle || driver_handle == INVALID_HANDLE_VALUE)
			return false;

		_mouse_input arguments = { 0 };
		arguments.security = code_security;
		arguments.flags = flags;
		arguments.x = x;
		arguments.y = y;
		arguments.data = data;
		arguments.extraInfo = extraInfo;

		DWORD bytes_returned = 0;
		return DeviceIoControl(driver_handle, code_mouse_input, &arguments, sizeof(arguments), nullptr, 0, &bytes_returned, NULL) != 0;
	}

	// Convenience functions for common mouse operations
	void move_mouse(INT32 x, INT32 y) {
		send_mouse_input(MOUSEEVENTF_MOVE, x, y);
	}

	void move_mouse_absolute(INT32 x, INT32 y) {
		send_mouse_input(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y);
	}

	void click_left() {
		send_mouse_input(MOUSEEVENTF_LEFTDOWN, 0, 0);
		send_mouse_input(MOUSEEVENTF_LEFTUP, 0, 0);
	}

	void click_right() {
		send_mouse_input(MOUSEEVENTF_RIGHTDOWN, 0, 0);
		send_mouse_input(MOUSEEVENTF_RIGHTUP, 0, 0);
	}

	void scroll_wheel(INT32 delta) {
		send_mouse_input(MOUSEEVENTF_WHEEL, 0, 0, delta);
	}
}

template <typename T>
T read(uint64_t address) {
	T buffer{ };
	mem::read_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

template <typename T>
T write(uint64_t address, T buffer) {

	mem::write_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

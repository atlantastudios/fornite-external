//SIMPLE ASS CODE FOR YALL TO UNDERSTAND EVERYTHING! IF YOU CANT UNDERSTAND YOU A GIGA PASTER

//PLEASE DONT SELL THIS SOURCE PLEASEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

//CAUSE I KNOW YOU A PASTER AND DONT KNOW ANYTHING JUST WANNA CHANGE MENU AND NAME AND MAKE THIS YOURS WITHOUT HAVING ANY KNOWLEDGE I AM LETTING YOU KNOW THAT IF YOU CHANGE DRIVER YOU WILL HAVE TO CHANGE SOME THINGS IN HERE AND IN game.hpp and in driver.hpp SO JUST KEEP THAT IN MIND!
#include "includes.hpp"
#include <iostream>
#include <thread>


//NO THIS HIGHLIGHTS ARE NOT CHATGPT, I MADE THEM TO HELP YALL YNS TO UNDERSTAND WHAT EACH SHIT DOES, SO THAT YALL CAN LEARN AND NOT JUST PASTE.


// DONT USE ANY SHITTY CR3 LOOP FIX OR WHATEVER UR CHEAT WILL LAG SO BAD. IF UR DRIVER NEED CR3 LOOP THEN USE OTHER DRIVER OR FIX IT.
void main()
{
	printf("Waiting for Fortnite...");

	while (game_wnd == 0)
	{
		Sleep(1);
		mem::process_id = mem::find_process(L"FortniteClient-Win64-Shipping.exe"); //waiting for proccess
		game_wnd = get_process_wnd(mem::process_id); //found the process
		Sleep(1);
	}

	



	system("cls");
	printf("SOFMAIN FN BASE V4");

	if (!input::init())
	{
		printf("The input was not initialized :("); //input wasnt initialized please go to definition and check //INPUT MEANS THE MOUSE AIM I USE
		Sleep(3000);
		exit(0);
	}

	if (!gui::init())
	{
		printf("The gui was not initialized :("); //init/gui wasnt initialized please go to definition and check // GUI MEANS THE OVERLAY I USE
		Sleep(3000);
		exit(0);
	}
	if (!mem::find_driver())
	{
		printf("The driver was not initialized :("); //driver wasnt initialized please go to definition and check or check ur shitty pc OR YOU JUST FCKED THE COMMS IDK JUST DOUBLE CHECK EVERYTHING FOR THIS ONE!
		Sleep(3000);
		exit(0);
	}
	printf("[DEBUG] Driver initialized successfully!\n");
	printf("[DEBUG] Driver Handle: 0x%p\n", mem::driver_handle);

	//DONT TOUCH HERE PASTER! TOUCH ONLY IF YOU WANNA CHANGE DRIVER OR IF YOU KNOW WHAT YOU ARE DOING! -SOFMAIN
	sofmainisud = mem::find_image(); //finding img / base

	if (!mem::driver_handle) //base_address //driver_handle
	{
		printf("The driver couldn't get the base address"); //Check ur driver base in the code if u get this error! Mine doesnt have a base_address so its driver_handle
		Sleep(3000);
		exit(0);
	}

	printf("[DEBUG] ========================================\n");
	printf("[DEBUG] Initialization Debug Info:\n");
	printf("[DEBUG] ========================================\n");
	printf("[DEBUG] Process ID: %d\n", mem::process_id);
	printf("[DEBUG] Base Address (sofmainisud): 0x%llX\n", sofmainisud);
	printf("[DEBUG] ========================================\n");
	printf("[DEBUG] Key Offsets Being Used:\n");
	printf("[DEBUG] ========================================\n");
	printf("[DEBUG] UWORLD: 0x%X\n", UWORLD);
	printf("[DEBUG] GAME_INSTANCE: 0x%X\n", GAME_INSTANCE);
	printf("[DEBUG] LOCAL_PLAYERS: 0x%X\n", LOCAL_PLAYERS);
	printf("[DEBUG] PLAYER_CONTROLLER: 0x%X\n", PLAYER_CONTROLLER);
	printf("[DEBUG] LOCAL_PAWN (ACKNOWLEDGED_PAWN): 0x%X\n", LOCAL_PAWN);
	printf("[DEBUG] PAWN_PRIVATE: 0x%X\n", PAWN_PRIVATE);
	printf("[DEBUG] ROOT_COMPONENT: 0x%X\n", ROOT_COMPONENT);
	printf("[DEBUG] RELATIVE_LOCATION: 0x%X\n", RELATIVE_LOCATION);
	printf("[DEBUG] PLAYER_STATE: 0x%X\n", PLAYER_STATE);
	printf("[DEBUG] TEAM_INDEX: 0x%X\n", TEAM_INDEX);
	printf("[DEBUG] GAME_STATE: 0x%X\n", GAME_STATE);
	printf("[DEBUG] PLAYER_ARRAY: 0x%X\n", PLAYER_ARRAY);
	printf("[DEBUG] MESH: 0x%X\n", MESH);
	printf("[DEBUG] CAMERA_LOCATION: 0x%X\n", CAMERA_LOCATION);
	printf("[DEBUG] CAMERA_ROTATION: 0x%X\n", CAMERA_ROTATION);
	printf("[DEBUG] VISIBLE_SECONDS: 0x%X\n", VISIBLE_SECONDS);
	printf("[DEBUG] BASEFOV (CAMERA_FOV): 0x%X\n", BASEFOV);
	printf("[DEBUG] ========================================\n");
	printf("[DEBUG] Expected UWORLD Address: 0x%llX\n", sofmainisud + UWORLD);
	printf("[DEBUG] ========================================\n");
	printf("[DEBUG] Testing memory reads...\n");
	
	// Test read from base address
	uintptr_t test_uworld = read<uintptr_t>(sofmainisud + UWORLD);
	printf("[DEBUG] UWORLD Read: 0x%llX (Valid: %s)\n", test_uworld, (test_uworld != 0 && test_uworld != 0xCCCCCCCCCCCCCCCC) ? "YES" : "NO");
	
	if (test_uworld != 0 && test_uworld != 0xCCCCCCCCCCCCCCCC)
	{
		uintptr_t test_game_instance = read<uintptr_t>(test_uworld + GAME_INSTANCE);
		printf("[DEBUG] Game Instance Read: 0x%llX (Valid: %s)\n", test_game_instance, (test_game_instance != 0 && test_game_instance != 0xCCCCCCCCCCCCCCCC) ? "YES" : "NO");
	}
	
	printf("[DEBUG] ========================================\n");
	printf("[DEBUG] Starting overlay...\n");
	printf("[DEBUG] ========================================\n");
	//here is for menu overlay blah blah
	create_overlay(); 
	directx_init();
	render_loop();

}



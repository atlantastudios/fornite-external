#pragma once
// Current Patch: v38.10
// Updated offsets to match correct values

#define UWORLD 0x173bdd18
#define GNAMES 0x167B6600
#define LOCAL_PLAYERS 0x38
#define PLAYER_CONTROLLER 0x30
#define PLAYERCAMERAMANAGER 0x360
#define ACKNOWLEDGED_PAWN 0x358  // LOCAL_PAWN (updated from 0x350)
#define PLAYER_STATE 0x2d0  // updated from 0x2B8
#define TEAM_INDEX 0x11a9
#define B_IS_DYING 0x728
#define B_IS_DBNO 0x841
#define PLATFORM 0x440  // updated from 0x438
#define TARGETED_FORT_PAWN 0x1900
#define KILL_SCORE 0x11A8
#define PLAYERNAME 0xA00  // updated from 0xB48
#define RANKED_PROGRESS 0xD8  // updated from 0xd0
#define PLAYER_AIM 0x2BC0
#define GAME_INSTANCE 0x248  // OwningGameInstance
#define GAME_STATE 0x1D0
#define PLAYER_ARRAY 0x2c8
#define ROOT_COMPONENT 0x1B0
#define PAWN_PRIVATE 0x328
#define AACTOR 0x128
#define PERSISTENT_LEVEL 0x40
#define MESH 0x330
#define BONE_ARRAY 0x5E8
#define BONE_ARRAY_CACHE 0x5F8  // bonecache
#define BONE_STRIDE 0x60  // bonestide
#define COMPONENT_TO_WORLD 0x1E0
#define RELATIVE_LOCATION 0x140
#define ADDITIONAL_AIM_OFFSET 0x2D20
#define CAMERA_FOV 0x3B4  // updated from BASEFOV 0x3AC
#define CAMERA_LOCATION 0x180
#define CAMERA_ROTATION 0x190
#define CURRENT_VEHICLE 0x2C18
#define CURRENT_WEAPON 0x990  // updated from 0xa80
#define WEAPON_DATA 0x5A0  // updated from 0x550
#define AMMO_COUNT 0x14d4
#define ITEM_NAME 0x40  // ItemName
#define PROJECTILE_SPEED 0x2488
#define PROJECTILE_GRAVITY 0x21d8
#define COMPONENT_VELOCITY 0x188
#define PRIMARY_PICKUP_ITEM_ENTRY 0x3A8
#define ITEM_DEFINITION 0x10
#define ITEM_RARITY 0xAA
#define TIER 0xA2
#define RARITY 0xAA
#define B_ALREADY_SEARCHED 0xD52
#define HABANERO_COMPONENT 0x940  // updated from 0x970
#define LEVELS 0x1E8

// Exploit/Feature specific offsets (may need verification)
#define MESH_ROTATION 0x150  // Used for spinbot/playerbackwards - VERIFY THIS OFFSET
#define FOV_CHANGER_1 0x2850  // FOV changer offset 1 - VERIFY THIS OFFSET
#define FOV_CHANGER_2 0x2854  // FOV changer offset 2 - VERIFY THIS OFFSET
#define FOV_CHANGER_3 0x2800  // FOV changer offset 3 - VERIFY THIS OFFSET
#define FOV_CHANGER_4 0x2804  // FOV changer offset 4 - VERIFY THIS OFFSET

// Rotation structure internal offsets (for FNRot struct)
#define ROTATION_OFFSET_B 0x20   // Internal FNRot struct offset
#define ROTATION_OFFSET_C 0x1D0  // Internal FNRot struct offset

// Legacy/compatibility defines
#define LOCAL_PAWN ACKNOWLEDGED_PAWN  // Use ACKNOWLEDGED_PAWN for new code
#define BASEFOV CAMERA_FOV  // Use CAMERA_FOV for new code
#define VISIBLE_SECONDS 0x1A0  // Keep for compatibility if used
#define LAST_SUBMIT_TIME 0x32C  // Keep for compatibility if used
#define LAST_SUBMIT_TIME_ON_SCREEN 0x2f0  // Keep for compatibility if used
#define VELOCITY COMPONENT_VELOCITY  // Use COMPONENT_VELOCITY for new code
#define DISPLAY_NAME ITEM_NAME  // Use ITEM_NAME for new code
#define WEAPON_NAME ITEM_NAME  // Use ITEM_NAME for new code
#define ISDYING B_IS_DYING  // Use B_IS_DYING for new code

//ALL OFFSETS HERE!
//THATS ALL THE OFFSETS YOU NEED FOR THAT SIMPLE ASS BASE
//IF YOU ADD MORE STUFF KEEP IN MIND THAT YOU MIGHT NEED MORE OFFSETS !
//LAST UPDATED: v38.10
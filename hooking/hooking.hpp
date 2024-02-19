#pragma once
#include "types.hpp"
#include "native/nativeinvoker.hpp"
#include "byte_patch.hpp"
#include "../game/enums.hpp"
#include "../gui/submenus/submenus.hpp"
#include <vector>

#pragma warning (push)
#pragma warning (disable: 4244)

typedef bool(__cdecl* IsDLCPresent)							(Hash dlcHash);
typedef uint32_t*(__cdecl* TextureFileRegister)				(int*, const char*, bool, const char*, bool);
typedef bool(__cdecl* GetEventData)							(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
typedef void(__cdecl* TriggerScriptEvent)					(int unk0, uint64_t* args, int argCount, int bitFlags);
using GetScriptHandlerIfNetworked							= void* (*) ();
typedef const char* (__cdecl* GetLabelText)					(void* this_, const char* label);
typedef bool(__cdecl* IncrementStatEvent)					(uint64_t net_event_struct, int64_t sender, int64_t a3);

class hooking
{
public:
	static uint64_t							world_ptr;
	static __int64**						global_ptr;
	static IsDLCPresent					    is_dlc_present;
	static TextureFileRegister				texture_file_register;
	static GetEventData						get_event_data;
	static TriggerScriptEvent				trigger_script_event;
	static GetScriptHandlerIfNetworked		get_script_handler_if_networked;
	static GetLabelText						get_label_text;
	static IncrementStatEvent				increment_stat_event;
	static byte_patch*						c4_spawn_limit;

	static NativeRegistrationNew**			m_registrationTable;

	static IsDLCPresent IsDLCPresentOriginal;
	static GetLabelText GetLabelTextOriginal;
	static GetEventData GetEventDataOriginal;
	static IncrementStatEvent IncrementStatEventOriginal;

	static void Init();
	static std::vector<LPVOID> GetMH_Hooked();
};
#pragma warning (pop)
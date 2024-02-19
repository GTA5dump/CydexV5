#include "hooking.hpp"
#include "../cheat/file_system.hpp"
#include "../cheat/log.hpp"
#include "native/natives.hpp"
#include "native/crossmap.hpp"
#include "../gui/GUI.hpp"
#include <MinHook.h>

using namespace Cheat;

IsDLCPresent														hooking::is_dlc_present;
TextureFileRegister													hooking::texture_file_register;
GetEventData														hooking::get_event_data;
TriggerScriptEvent													hooking::trigger_script_event;
GetLabelText														hooking::get_label_text;
GetScriptHandlerIfNetworked											hooking::get_script_handler_if_networked;
IncrementStatEvent													hooking::increment_stat_event;

uint64_t															hooking::world_ptr;
__int64**															hooking::global_ptr;
byte_patch*															hooking::c4_spawn_limit;

NativeRegistrationNew**												hooking::m_registrationTable;
std::vector<LPVOID>													MH_Hooked;

bool IsDLCPresentHooked												(std::uint32_t DLCHash);
bool GetEventDataHooked												(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount);
bool IncrementStatEventHooked										(uint64_t net_event_struct, int64_t sender, int64_t a3);
void* GetScriptHandlerIfNetworkedHooked								();
const char* GetLabelTextHooked										(void* this_, const char* label);

void hooking::Init()
{
	// Retrieving pointers
	LOG_DEBUG("Getting IDP pointer");
	hooking::is_dlc_present = static_cast<IsDLCPresent>(Memory::pattern("48 89 5C 24 ? 57 48 83 EC 20 81 F9 ? ? ? ?").count(1).get(0).get<void>(0));

	LOG_DEBUG("Getting GLT pointer");
	hooking::get_label_text = static_cast<GetLabelText>(Memory::pattern("48 89 5C 24 ? 57 48 83 EC 20 48 8B DA 48 8B F9 48 85 D2 75 44 E8").count(1).get(0).get<void>(0));
	
	LOG_DEBUG("Getting GSHIN pointer");
	hooking::get_script_handler_if_networked = static_cast<GetScriptHandlerIfNetworked>(Memory::pattern("40 53 48 83 EC 20 E8 ? ? ? ? 48 8B D8 48 85 C0 74 12 48 8B 10 48 8B C8").count(1).get(0).get<void>(0));
	
	LOG_DEBUG("Getting GED pointer");
	hooking::get_event_data = static_cast<GetEventData>(Memory::pattern("48 85 C0 74 14 4C 8B 10").count(1).get(0).get<void>(-28));

	LOG_DEBUG("Getting TSE pointer");
	hooking::trigger_script_event = static_cast<TriggerScriptEvent>(Memory::pattern("48 8B C4 48 89 58 08 48 89 68 10 48 89 70 18 48 89 78 20 41 56 48 81 EC ? ? ? ? 45 8B F0 41 8B F9").count(1).get(0).get<void>(0));
	
	LOG_DEBUG("Getting TFR pointer");
	hooking::texture_file_register = static_cast<TextureFileRegister>(Memory::pattern("48 89 5C 24 ? 48 89 6C 24 ? 48 89 7C 24 ? 41 54 41 56 41 57 48 83 EC 50 48 8B EA 4C 8B FA 48 8B D9 4D 85 C9").count(1).get(0).get<void>());

	LOG_DEBUG("Getting C4SL pointer");
	c4_spawn_limit = byte_patch::make(Memory::pattern("41 80 78 28 ? 0F 85 ? ? ? ? 49 8B 80").count(1).get(0).get<std::int8_t>(4), 99).get();

	LOG_DEBUG("Getting ISE pointer");
	hooking::increment_stat_event = static_cast<IncrementStatEvent>(Memory::pattern("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 55 41 56 41 57 48 8B EC 48 83 EC 60 8B 79 30").count(1).get(0).get<void>(0));

	char* c_location = nullptr;
	void* v_location = nullptr;

	// Load Vector3 Result Fix
	LOG_DEBUG("Getting VRF pointer");
	v_location = Memory::pattern("83 79 18 00 48 8B D1 74 4A FF 4A 18").count(1).get(0).get<void>(0);
	if (v_location != nullptr) scrNativeCallContext::SetVectorResults = (void(*)(scrNativeCallContext*))(v_location);

	// Load Native Registration Table
	LOG_DEBUG("Getting NRT pointer");
	c_location = Memory::pattern("76 32 48 8B 53 40 48 8D 0D").count(1).get(0).get<char>(9);
	if (c_location == nullptr) { LOG_DEBUG("Failed to load Native Registration Table"); } else { m_registrationTable = reinterpret_cast<decltype(m_registrationTable)>(c_location + *(int32_t*)c_location + 4); }

	// Load Game World Pointer
	LOG_DEBUG("Getting WLD pointer");
	c_location = Memory::pattern("48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07").count(1).get(0).get<char>(0);
	if (c_location == nullptr) { LOG_DEBUG("Failed to load World Pointer"); } else { hooking::world_ptr = reinterpret_cast<uint64_t>(c_location) + *reinterpret_cast<int*>(reinterpret_cast<uint64_t>(c_location) + 3) + 7; }

	// Get Global Pointer
	LOG_DEBUG("Getting GLB pointer");
	c_location = Memory::pattern("4C 8D 05 ? ? ? ? 4D 8B 08 4D 85 C9 74 11").count(1).get(0).get<char>(0);
	__int64 patternAddr = NULL;
	if (c_location == nullptr) { LOG_DEBUG("Failed to load Global Pointer"); } else { patternAddr = reinterpret_cast<decltype(patternAddr)>(c_location);	}
	hooking::global_ptr = (__int64**)(patternAddr + *(int*)(patternAddr + 3) + 7);

	// Initialize Natives
	LOG_DEBUG("Initialized GN");
	crossmap::initNativeMap();

	// Hook Game Functions
	LOG_DEBUG("Hook GED");
	auto status = MH_CreateHook(hooking::get_event_data, GetEventDataHooked, (void**)&GetEventDataOriginal);
	if ((status != MH_OK && status != MH_ERROR_ALREADY_CREATED) || MH_EnableHook(hooking::get_event_data) != MH_OK) { LOG_DEBUG("Failed to hook GET_EVENT_DATA"); }
	MH_Hooked.push_back(hooking::get_event_data);

	LOG_DEBUG("Hook GSHIN");
	status = MH_CreateHook(hooking::get_script_handler_if_networked, GetScriptHandlerIfNetworkedHooked, NULL);
	if (status != MH_OK || MH_EnableHook(hooking::get_script_handler_if_networked) != MH_OK) { LOG_DEBUG("Failed to hook GET_SCRIPT_HANDLER_IF_NETWORKED"); }
	MH_Hooked.push_back(hooking::get_script_handler_if_networked);

	LOG_DEBUG("Hook GLT");
	status = MH_CreateHook(hooking::get_label_text, GetLabelTextHooked, (void**)&GetLabelTextOriginal);
	if (status != MH_OK || MH_EnableHook(hooking::get_label_text) != MH_OK) { LOG_DEBUG("Failed to hook GET_LABEL_TEXT"); }
	MH_Hooked.push_back(hooking::get_label_text);

	LOG_DEBUG("Hook ISE");
	status = MH_CreateHook(hooking::increment_stat_event, IncrementStatEventHooked, (void**)&IncrementStatEventOriginal);
	if ((status != MH_OK && status != MH_ERROR_ALREADY_CREATED) || MH_EnableHook(hooking::increment_stat_event) != MH_OK) { LOG_DEBUG("Hooked Stats"); }
	MH_Hooked.push_back(hooking::increment_stat_event);

	LOG_DEBUG("Hook IDP");
	status = MH_CreateHook(hooking::is_dlc_present, IsDLCPresentHooked, (void**)&IsDLCPresentOriginal);
	if ((status != MH_OK && status != MH_ERROR_ALREADY_CREATED) || MH_EnableHook(hooking::is_dlc_present) != MH_OK) { LOG_DEBUG("Failed to hook IS_DLC_PRESENT"); }
	MH_Hooked.push_back(hooking::is_dlc_present);
}

std::vector<LPVOID> hooking::GetMH_Hooked()
{
	return MH_Hooked;
}
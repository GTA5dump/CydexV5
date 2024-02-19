#include "../hooking.hpp"
#include "../../cheat/common.hpp"
#include "../../cheat/log.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"

using namespace Cheat;

IncrementStatEvent hooking::IncrementStatEventOriginal = nullptr;
bool IncrementStatEventHooked(uint64_t net_event_struct, int64_t sender, int64_t a3)
{
	auto hash = *reinterpret_cast<DWORD*>(net_event_struct + 0x30);

	switch (hash)
	{
	case 0x9C6A0C42:
	case 0x62EB8C5A:
	case 0x0E7072CD:
	case 0x762F9994:
	case 0xB722D6C0:
	case 0x3CDB43E2:
	case 0xE8FB6DD5:
	case 0xF3DE4879:
	case 0x3B566D5C:
	case 0x368F6FD9:
	case 0xCBFD04A4:
	case 0x9F79BA0B:
		if (Features::ProtectionBlockReports)
		{
			LOG_DEBUG("INCREMENT_STAT: blocked report hash %i", hash);
			Game::notification::MinimapAdvanced((char*)"Blocked player report", (char*)"Textures", (char*)"AdvancedNotificationImage", false, 4, (char*)"Protection", (char*)"", 0.8f, (char*)"");
			return true;
		}
		break;
	}

	return hooking::IncrementStatEventOriginal(net_event_struct, sender, a3);
}
#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
int SetTimeHour = 0, SetTimeMinutes = 0, SetTimeSeconds = 0;
void GUI::Submenus::Time()
{
	GUI::Title("Time");
	if (GUI::Int("Hour", SetTimeHour, 0, 23, 1, "", SELECTABLE_DISABLE_SAVE | SELECTABLE_RETURN_VALUE_CHANGE)) { NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(SetTimeHour, CLOCK::GET_CLOCK_MINUTES(), CLOCK::GET_CLOCK_SECONDS()); }
	if (GUI::Int("Minutes", SetTimeMinutes, 0, 59, 1, "", SELECTABLE_DISABLE_SAVE | SELECTABLE_RETURN_VALUE_CHANGE)) { NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(CLOCK::GET_CLOCK_HOURS(), SetTimeMinutes, CLOCK::GET_CLOCK_SECONDS()); }
	if (GUI::Int("Seconds", SetTimeSeconds, 0, 59, 1, "", SELECTABLE_DISABLE_SAVE | SELECTABLE_RETURN_VALUE_CHANGE)) { NETWORK::NETWORK_OVERRIDE_CLOCK_TIME(CLOCK::GET_CLOCK_HOURS(), CLOCK::GET_CLOCK_MINUTES(), SetTimeSeconds); }
	GUI::Break("Current Time", SELECTABLE_CENTER_TEXT);
	std::string CurrentGameTimeString = "Game Time: ~c~" + std::to_string(CLOCK::GET_CLOCK_HOURS()) + ":" + std::to_string(CLOCK::GET_CLOCK_MINUTES()) + ":" + std::to_string(CLOCK::GET_CLOCK_SECONDS());
	GUI::Break(CurrentGameTimeString.c_str(), false);
	std::string CurrentSystemTimeString = "System Time: ~c~" + helper::ReturnDateTimeFormatAsString("%H:%M:%S");
	GUI::Break(CurrentSystemTimeString.c_str());
	GUI::Break("Miscellaneous", SELECTABLE_CENTER_TEXT);
	if (GUI::Toggle("Slow Motion", Features::SlowMotion, "Slows Down Game Time"))
	{
		MISC::SET_TIME_SCALE(Features::SlowMotion ? 0.2f : 1.f);
	}
	GUI::Toggle("Pause Time", Features::PauseTimeBool, "Pause Game Time");
}
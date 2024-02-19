#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::Radio()
{
	GUI::Title("Radio");
	if (GUI::Option("Skip Track", "")) { AUDIO::SKIP_RADIO_FORWARD(); }
	GUI::Toggle("Freeze Station", Features::FreezeStationBool, "");
	if (GUI::Toggle("Mobile Radio", Features::MobileRadio, ""))
	{
		AUDIO::SET_MOBILE_RADIO_ENABLED_DURING_GAMEPLAY(Features::MobileRadio);
	}
}
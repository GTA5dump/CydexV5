#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::SelectedPlayerRemote()
{
	GUI::Title("Script Events");
	if (GUI::Option("Single Player Kick", ""))
	{
		Game::gtao::tse::start_event(eRemoteEvents::KickToSP, Features::SelectedPlayer);
	}
	if (GUI::Option("Force Into Mission", ""))
	{
		Game::gtao::tse::start_event(eRemoteEvents::ForceIntoMission, Features::SelectedPlayer);
	}
	if (GUI::Option("CEO Kick", ""))
	{
		Game::gtao::tse::start_event(eRemoteEvents::CEOKick, Features::SelectedPlayer);
	}
	GUI::Break("Teleport", SELECTABLE_CENTER_TEXT);
	GUI::MenuOption("Property", SelectedPlayerPropertyTeleport);
	if (GUI::Option("Cayo Perico Island", ""))
	{
		Game::gtao::tse::start_event(eRemoteEvents::CayoPericoTeleport, Features::SelectedPlayer);
	}
}
#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::SelectedPlayer()
{
	GUI::Title(PLAYER::GET_PLAYER_NAME(Features::SelectedPlayer));
	if (GUI::Toggle("Spectate", Features::SpectatePlayer, "", SELECTABLE_DISABLE_SAVE))
	{
		NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(Features::SpectatePlayer, PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(Features::SelectedPlayer));
	}
	if (GUI::Option("Host Kick", !Game::gtao::is_freemode_script_host(PLAYER::PLAYER_ID()) ? "Unavailable: you are not the session host" : "", !Game::gtao::is_freemode_script_host(PLAYER::PLAYER_ID()) ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
		NETWORK::NETWORK_SESSION_KICK_PLAYER(Features::SelectedPlayer);
	GUI::MenuOption("Teleport", Submenus::SelectedPlayerTeleport);
	GUI::MenuOption("Friendly", Submenus::SelectedPlayerFriendly);
	GUI::MenuOption("Griefing", Submenus::SelectedPlayerGriefing);
	if (GUI::Option("View Social Club Profile", ""))
	{ 
		int playerHandle;
		NETWORK::NETWORK_HANDLE_FROM_PLAYER(Features::SelectedPlayer, &playerHandle, 13); 
		NETWORK::NETWORK_SHOW_PROFILE_UI(&playerHandle); 
	}
}
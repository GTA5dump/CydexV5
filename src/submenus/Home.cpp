#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::Home()
{
	GUI::Title("Cydex");
	GUI::MenuOption("Local", Submenus::Self);
	GUI::MenuOption("Weapon", Submenus::Weapon);
	GUI::MenuOption("Teleport", Submenus::Teleport);
	GUI::MenuOption("Vehicle", Submenus::Vehicle);
	GUI::MenuOption("Spawner", Submenus::Spawn);
	GUI::MenuOption("World", Submenus::World);
	GUI::MenuOption("Miscellaneous", Submenus::Miscellaneous);
	GUI::MenuOption("Online", Submenus::Session);
	GUI::MenuOption("Recovery", Submenus::Recovery);
	GUI::MenuOption("Protection", Submenus::Protection);
	GUI::MenuOption("Settings", Submenus::Settings);
}
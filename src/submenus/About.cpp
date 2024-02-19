#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::About()
{
	GUI::Title("About");
	GUI::Break("Version: 5.0");
	if (GUI::Option("Join Cydex Discord", ""))
	{
		system("start https://discord.gg/xghuHDtk6C");
		system("start https://cydex.glitch.me/");
	}
	if (GUI::Option("Unload Cydex", "Thanks for using Cydex :D"))
	{
		// Remove console window
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		FreeConsole();

		// Stop MainFiber
		core::c_running = false;
	}
}
#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"
#include "../../cheat/file_system.hpp"
#include "../../game/features/theme_loader.hpp"
#include <filesystem>

using namespace Cheat;
void GUI::Submenus::ThemeFiles()
{
	int index = 0;
	GUI::Title("Saved Themes");
	for (const auto& file : std::filesystem::directory_iterator(file_system::paths::ThemesDir))
	{
		if (GUI::Option(file.path().stem().string(), ""))
		{
			if (Features::theme_loader::Load(file.path().stem().string()))
			{
				Game::notification::Minimap((char*)"Theme Loaded");
			}
		}
		index++;
	}
	if (!index)
		GUI::Break("None have been saved");
}
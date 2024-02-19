#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"
#include "../../game/features/gxt_editor.hpp"

using namespace Cheat;
void GUI::Submenus::GXTEditor()
{
	GUI::Title("GXT Editor");
	if (GUI::Option("Add", "Set a GXT-string modification"))
	{
		std::string GXT, Custom;
		if (Game::keyboard::get_input("Enter GXT - Example: PM_ENTER_MP", GXT, 1, 50) == Game::keyboard::input_status::SUCCESS)
		{
			if (!UI::DOES_TEXT_LABEL_EXIST(GXT.c_str()))
				Game::notification::Minimap((char*)"~r~That is not a valid GXT");
			else if (GXT == "FMMC_KEY_TIP8" || GXT == "LETTERS_HELP2")
				Game::notification::Minimap((char*)"~r~That is a resevered GXT and cannot be used");
			else if (Game::keyboard::get_input("Enter desired string for GXT: " + GXT, Custom, 1, 50) == Game::keyboard::input_status::SUCCESS)
				Features::gxt_editor::create_custom(GXT, Custom);
		}
	}
	GUI::Break("Strings", SELECTABLE_CENTER_TEXT);
	if (!Features::gxt_editor::custom_strings.empty())
		for (auto& gxt_m : Features::gxt_editor::custom_strings)
		{
			if (GUI::Option(gxt_m.first, "String: " + gxt_m.second + "\nSelect to remove"))
			{
				Features::gxt_editor::remove_custom(gxt_m.first);
				GUI::currentOption = 1;
			}
		}
	else
		GUI::Break("None have been saved");
}
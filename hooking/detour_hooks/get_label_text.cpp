#include "../../cheat/fibermain.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../hooking/native/natives.hpp"
#include "../../game/features/gxt_editor.hpp"

using namespace Cheat;

std::string Game::InGameHelpTextMessage;
GetLabelText hooking::GetLabelTextOriginal = nullptr;
const char* GetLabelTextHooked(void* this_, const char* label)
{
	// Modify FMMC_KEY_TIP8 string for DISPLAY_ONSCREEN_KEYBOARD title text
	if (!Game::keyboard::keyboard_title.empty())
		if (label == "FMMC_KEY_TIP8")
			return helper::StringToChar(Game::keyboard::keyboard_title);

	// Modify LETTERS_HELP2 string for DISPLAY_HELP_TEXT_THIS_FRAME text
	if (!Game::InGameHelpTextMessage.empty())
		if (label == "LETTERS_HELP2")
			return helper::StringToChar(Game::InGameHelpTextMessage);

	// GXT Editor
	const char* modified_string;
	if (Features::gxt_editor::get_string(label, modified_string))
	{
		return modified_string;
	}
	
	return hooking::GetLabelTextOriginal(this_, label);
}
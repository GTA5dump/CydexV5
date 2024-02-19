#include "../GUI.hpp"
#include "../gui_controls.hpp"
#include "../submenus/submenus.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;

void GUI::Title(std::string title)
{
	// Header background
	if (ShowHeaderBackground)
		DrawRectInGame({ PrimaryColor.r, PrimaryColor.g, PrimaryColor.b, HeaderBackgroundTransparency }, { guiX, GUI::guiY - SelectableHeight - 0.181f }, { guiWidth, SelectableHeight + 0.045f });
	
	// Header texture
	if (ShowHeaderTexture)
		DrawSpriteInGame("Textures", "HeaderDefaultTransparent", guiX, GUI::guiY - SelectableHeight - 0.181f, guiWidth, SelectableHeight + 0.045f, 0, 255, 255, 255, HeaderTextureTransparency);
	
	// Draw title text & rectangle
	DrawTextInGame(title, TextColorAndFont, { GUI::guiX, GUI::guiY - 0.174f }, { 0.40f, 0.38f }, true, true);
	DrawRectInGame({ 24,11,39,255 }, { guiX, GUI::guiY - 0.1585f }, { guiWidth, SelectableHeight });

	if (gui_controls::cursor_nav_state)
	{
		DrawSpriteInGame("commonmenu", "arrowleft", GUI::guiX - (GUI::guiWidth * 48 / 100), GUI::guiY - 0.160f, 0.015f, 0.025f, 0, 255, 255, 255, 255);
		DrawTextInGame(GUI::currentMenu == Submenus::Home ? "Close" : "Back", TextColorAndFont, { GUI::guiX - (GUI::guiWidth * 45 / 100), GUI::guiY - 0.174f }, { 0.35f, 0.37f }, false, true);
	}

	// Hide some in-game elements and disable various controls while menu gui is open
	UI::HIDE_HELP_TEXT_THIS_FRAME();
	CAM::SET_CINEMATIC_BUTTON_ACTIVE(false);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_CHARACTER_WHEEL, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_MULTIPLAYER_INFO, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_PHONE, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_VEH_CIN_CAM, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_MAP_POI, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_VEH_HEADLIGHT, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK1, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_MELEE_ATTACK2, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_REPLAY_START_STOP_RECORDING, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_REPLAY_START_STOP_RECORDING_SECONDARY, true);
	PAD::DISABLE_CONTROL_ACTION(2, INPUT_VEH_PREV_RADIO, true);
	
	// Set current submenu
	CurrentSubmenu = title;
}
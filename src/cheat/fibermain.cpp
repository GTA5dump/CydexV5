#include "fibermain.hpp"
#include "cheat.hpp"
#include "entry_point.hpp"
#include "file_system.hpp"
#include "version.hpp"
#include "update.hpp"
#include "config.hpp"
#include "../hooking/globalhandle.hpp"
#include "../hooking/offsets.hpp"
#include "../hooking/native/natives.hpp"
#include "../gui/submenus/submenus.hpp"
#include "../gui/gui_controls.hpp"
#include "../game/game.hpp"
#include "../game/features.hpp"
#include "../game/features/gxt_editor.hpp"
#include <MinHook.h>


using namespace Cheat;

HANDLE fibermain::handle = nullptr;
DWORD fibermain::wake_time = NULL;

void fibermain::func_(LPVOID lpParameter)
{
	// Check for availability of newer cheat version
	LOG_INFO("Checking for newer version");
	int LocalVersion = build_info::VersionInt;
	int LatestVersion = update_check::GetLatest();
	if (LatestVersion)
	{
		if (LocalVersion < LatestVersion)
		{
			update_check::IsAvailable = true;
			LOG_WARN("A newer version of the cheat is available on GitHub");
		}
		else
		{
			LOG_DEBUG("No newer cheat version available");
		}
	}

	// Create Menu Selectable Arrow Animation Thread
	std::thread MenuSelectableAnimationThreadHandle([]()
		{
			while (core::c_running)
			{
				if (GUI::menuLevel > 0)
				{
					GUI::MenuOptionArrowAnimationState = !GUI::MenuOptionArrowAnimationState;
					Sleep(GUI::MenuArrowAnimationDelay);
				}
			}
		});
	MenuSelectableAnimationThreadHandle.detach();

	// Load configuration file
	config::Load();

	// Load texture file
	GUI::LoadTextureFile();

	// Load 'multiplayer vehicles in Single Player' bypass
	globalHandle(GLOBAL_SP_DESPAWN_BYPASS).As<bool>() = true;
	LOG_DEBUG("Loaded SPVSB");

	// Fetch default HUD colors
	for (int i = 0; i <= game_data::HUDColors.size(); i++)
	{
		game_data::DefaultHUDColorsS data{};
		UI::GET_HUD_COLOUR(i, &data.R, &data.G, &data.B, &data.A);
		game_data::DefaultHUDColors.push_back(data);
	}
	LOG_DEBUG("Fetched DHC");

	// Load saved HUD colors
	if (std::filesystem::exists(file_system::paths::HUDColorsFile))
	{
		int SavedHUDColorsIndex = 0;
		for (auto const& HUDColorComponentName : game_data::HUDColors)
		{
			std::string Red = file_system::ini_file::get(file_system::paths::HUDColorsFile, HUDColorComponentName, "r");
			std::string Green = file_system::ini_file::get(file_system::paths::HUDColorsFile, HUDColorComponentName, "g");
			std::string Blue = file_system::ini_file::get(file_system::paths::HUDColorsFile, HUDColorComponentName, "b");
			std::string Alpha = file_system::ini_file::get(file_system::paths::HUDColorsFile, HUDColorComponentName, "a");

			if (!Red.empty() && !Green.empty() && !Blue.empty() && !Alpha.empty())
			{
				UI::REPLACE_HUD_COLOUR_WITH_RGBA(SavedHUDColorsIndex, helper::StringToInt(Red), helper::StringToInt(Green), helper::StringToInt(Blue), helper::StringToInt(Alpha));
				LOG_DEBUG("Loaded CHC {0}", HUDColorComponentName.c_str());
			}
			SavedHUDColorsIndex++;
		}
	}

	// GXT Editor
	Features::gxt_editor::load_custom_file();
	LOG_DEBUG("Loaded GXT strings");

	// Cheat init completed
	core::init_complete = true;
	LOG_INFO("Cheat init completed");

	// Main cheat loop within the created fiber - this runs in the IS_DLC_PRESENT hook (thus, game natives can be called only from within this loop)
	while (core::c_running)
	{
		// Core component loops
		Features::Loop();			// Responsible for looping most cheat-technical and game-feature related functions
		GUI::Submenus::Loop();		// Responsible for looping the submenu logical; this is also responsible for rendering the menu GUI title
		GUI::Loop();				// Responsible for some core GUI elements; currently only the menu bottom
		GUI::gui_controls::Loop();	// Responsible for control inputs (keyboard and mouse)

		// Switch Fiber
		SwitchToFiber(fibermain::handle);
	}

	// Create shutdown thread
	auto unload = [](LPVOID data) -> DWORD
	{ 
		// Disable & remove MinHook hooks
		for (int i = 0; i < hooking::GetMH_Hooked().size(); i++)
		{
			MH_DisableHook(hooking::GetMH_Hooked()[i]);
			MH_RemoveHook(hooking::GetMH_Hooked()[i]);
		}

		// Undo all byte patching
		byte_patch::restore_all();

		// Make sure all pending/queued log entries are written to file
		log::GetLogger()->flush();

		// Uninit MinHook
		MH_Uninitialize();

		// Unload DLL from game process
		FreeLibraryAndExitThread(entry_point::module_handle, EXIT_SUCCESS);
	};
	CreateThread(NULL, NULL, unload, NULL, NULL, NULL);
}

void fibermain::pause( bool show_msg, int duration)
{
	if (show_msg)
		GUI::DrawTextInGame("One moment please", { 255, 255, 255, 255, FontPricedown }, { 0.525f, 0.400f }, { 1.5f, 1.5f }, true, true);

	if (duration > 0)
		wake_time = timeGetTime() + duration;

	SwitchToFiber(fibermain::handle);
}
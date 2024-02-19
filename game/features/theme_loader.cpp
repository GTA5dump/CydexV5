#include "theme_loader.hpp"
#include "../../cheat/file_system.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/features.hpp"
#include "../../game/game.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include <string>
#include <filesystem>

namespace Cheat
{
	namespace Features
	{
		const std::string theme_loader::version = "4.0";
		std::string theme_loader::Current;
		void theme_loader::Create(std::string Name)
		{
			std::string TargetThemePath = file_system::paths::ThemesDir + "\\" + Name + ".ini";

			file_system::ini_file::write(version, TargetThemePath, "theme", "version");

			file_system::ini_file::write_bool(GUI::DisableMenuGUIOpenCloseFade, TargetThemePath, "Menu", "open/close_fade_disabled");
			file_system::ini_file::write(std::to_string(GUI::guiWidth), TargetThemePath, "Menu", "menu_gui_width");

			file_system::ini_file::write(std::to_string(GUI::guiX), TargetThemePath, "Positioning", "menu_gui_x");
			file_system::ini_file::write(std::to_string(GUI::guiY), TargetThemePath, "Positioning", "menu_gui_y");

			file_system::ini_file::write(std::to_string(GUI::SelectableInfoBoxX), TargetThemePath, "Positioning", "selectable_information_box_x");
			file_system::ini_file::write(std::to_string(GUI::SelectableInfoBoxY), TargetThemePath, "Positioning", "selectable_information_box_y");
			file_system::ini_file::write(std::to_string(GUI::SelectableHeight), TargetThemePath, "Positioning", "selectable_height");

			file_system::ini_file::write_bool(GUI::ShowHeaderTexture, TargetThemePath, "Header", "header_texture");
			file_system::ini_file::write_bool(GUI::ShowHeaderBackground, TargetThemePath, "Header", "header_background");

			file_system::ini_file::write_bool(Features::RGBDiscoBool, TargetThemePath, "Color", "rgb_disco");
			if (!Features::RGBDiscoBool)
			{
				file_system::ini_file::write(std::to_string(GUI::PrimaryColor.r), TargetThemePath, "Color", "primary_color_r");
				file_system::ini_file::write(std::to_string(GUI::PrimaryColor.g), TargetThemePath, "Color", "primary_color_g");
				file_system::ini_file::write(std::to_string(GUI::PrimaryColor.b), TargetThemePath, "Color", "primary_color_b");

				file_system::ini_file::write(std::to_string(GUI::TextColorAndFont.r), TargetThemePath, "Color", "text_color_r");
				file_system::ini_file::write(std::to_string(GUI::TextColorAndFont.g), TargetThemePath, "Color", "text_color_g");
				file_system::ini_file::write(std::to_string(GUI::TextColorAndFont.b), TargetThemePath, "Color", "text_color_b");
			}

			file_system::ini_file::write(std::to_string(GUI::TextColorAndFont.f), TargetThemePath, "Font", "text_font");

			Game::notification::Minimap((char*)"Theme Saved");
		}
		bool theme_loader::Load(std::string Name)
		{
			std::string TargetThemePath = file_system::paths::ThemesDir + "\\" + Name + ".ini";

			if (!std::filesystem::exists(TargetThemePath))
				return Remove(Name);

			// Load current and write to config for auto load
			theme_loader::Current = Name;
			file_system::ini_file::write(Features::theme_loader::Current, file_system::paths::ConfigFile, "theme", "current");

			try
			{
				GUI::DisableMenuGUIOpenCloseFade = helper::StringToBool(file_system::ini_file::get(TargetThemePath, "Menu", "open/close_fade_disabled"));
				GUI::guiWidth = std::stof(file_system::ini_file::get(TargetThemePath, "Menu", "menu_gui_width"));

				GUI::guiX = std::stof(file_system::ini_file::get(TargetThemePath, "Positioning", "menu_gui_x"));
				GUI::guiY = std::stof(file_system::ini_file::get(TargetThemePath, "Positioning", "menu_gui_y"));

				GUI::SelectableInfoBoxX = std::stof(file_system::ini_file::get(TargetThemePath, "Positioning", "selectable_information_box_x"));
				GUI::SelectableInfoBoxY = std::stof(file_system::ini_file::get(TargetThemePath, "Positioning", "selectable_information_box_y"));
				GUI::SelectableHeight = std::stof(file_system::ini_file::get(TargetThemePath, "Positioning", "selectable_height"));

				GUI::ShowHeaderTexture = helper::StringToBool(file_system::ini_file::get(TargetThemePath, "Header", "header_texture"));
				GUI::ShowHeaderBackground = helper::StringToBool(file_system::ini_file::get(TargetThemePath, "Header", "header_background"));

				Features::RGBDiscoBool = helper::StringToBool(file_system::ini_file::get(TargetThemePath, "Color", "rgb_disco"));

				if (!Features::RGBDiscoBool)
				{
					GUI::PrimaryColor.r = helper::StringToInt(file_system::ini_file::get(TargetThemePath, "Color", "primary_color_r"));
					GUI::PrimaryColor.g = helper::StringToInt(file_system::ini_file::get(TargetThemePath, "Color", "primary_color_g"));
					GUI::PrimaryColor.b = helper::StringToInt(file_system::ini_file::get(TargetThemePath, "Color", "primary_color_b"));

					GUI::TextColorAndFont.r = helper::StringToInt(file_system::ini_file::get(TargetThemePath, "Color", "text_color_r"));
					GUI::TextColorAndFont.g = helper::StringToInt(file_system::ini_file::get(TargetThemePath, "Color", "text_color_g"));
					GUI::TextColorAndFont.b = helper::StringToInt(file_system::ini_file::get(TargetThemePath, "Color", "text_color_b"));
				}

				GUI::TextColorAndFont.f = helper::StringToInt(file_system::ini_file::get(TargetThemePath, "Font", "text_font"));
			}
			catch (...) { return false; }

			// Theme version check
			if (file_system::ini_file::get(TargetThemePath, "theme", "version") != version)
			{
				Game::notification::Minimap((char*)"Theme outdated - please recreate it");
				return false;
			}

			return true;
		}
		bool theme_loader::Remove(std::string Name)
		{
			// Clear current theme
			theme_loader::Current.clear();
			file_system::ini_file::remove_key(file_system::paths::ConfigFile, "theme", "current");

			// Remove file
			if (std::remove(helper::StringToChar(file_system::paths::ThemesDir + "\\" + Name + ".ini")))
			{
				return true;
			}

			return false;
		}
	}
}
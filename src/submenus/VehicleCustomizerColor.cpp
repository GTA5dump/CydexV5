#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
int VehiclePrimaryColorRed, VehiclePrimaryColorGreen, VehiclePrimaryColorBlue, VehicleSecondaryColorRed, VehicleSecondaryColorGreen, VehicleSecondaryColorBlue;
void GUI::Submenus::VehicleCustomizerColor()
{
	GUI::Title("Custom Color");
	GUI::Int("Primary Color: Red", VehiclePrimaryColorRed, 0, 255, 1, "", SELECTABLE_RETURN_VALUE_CHANGE);
	GUI::Int("Primary Color: Green", VehiclePrimaryColorGreen, 0, 255, 1, "", SELECTABLE_RETURN_VALUE_CHANGE);
	GUI::Int("Primary Color: Blue", VehiclePrimaryColorBlue, 0, 255, 1, "", SELECTABLE_RETURN_VALUE_CHANGE);
	if (GUI::Option("Set Primary Color", ""))
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			::Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, VehiclePrimaryColorRed, VehiclePrimaryColorGreen, VehiclePrimaryColorBlue);
		}
		else
		{
			Game::notification::Minimap((char*)"~r~Player is not in a vehicle");
		}
	}
	GUI::Int("Secondary Color: Red", VehicleSecondaryColorRed, 0, 255, 1, "", SELECTABLE_RETURN_VALUE_CHANGE);
	GUI::Int("Secondary Color: Green", VehicleSecondaryColorGreen, 0, 255, 1, "", SELECTABLE_RETURN_VALUE_CHANGE);
	GUI::Int("Secondary Color: Blue", VehicleSecondaryColorBlue, 0, 255, 1, "", SELECTABLE_RETURN_VALUE_CHANGE);
	if (GUI::Option("Set Secondary Color", ""))
	{
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0))
		{
			::Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0);
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, VehicleSecondaryColorRed, VehicleSecondaryColorGreen, VehicleSecondaryColorBlue);
		}
		else
		{
			Game::notification::Minimap((char*)"~r~Player is not in a vehicle");
		}
	}
}
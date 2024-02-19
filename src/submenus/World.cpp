#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"
#include "../../hooking/globalhandle.hpp"
#include "../../hooking/offsets.hpp"

using namespace Cheat;
void GUI::Submenus::World()
{
	GUI::Title("World");
	GUI::MenuOption("Time", Submenus::Time);
	GUI::MenuOption("Weather", Submenus::Weather);
	GUI::MenuOption("Nearby Vehicles", Submenus::NearbyVehicles);
	GUI::MenuOption("Nearby Peds", Submenus::NearbyPeds);
	if (GUI::Toggle("Snow", Features::WorldSnow, "GTAO Only", !NETWORK::NETWORK_IS_SESSION_STARTED() ? SELECTABLE_DISABLED : SELECTABLE_DUMMY))
	{
		globalHandle(GLOBAL_SNOW[0]).At(GLOBAL_SNOW[1]).As<bool>() = Features::WorldSnow;
	}
	if (GUI::Toggle("No Gravity", Features::NoGravity, ""))
	{
		if (Features::NoGravity)
			MISC::SET_GRAVITY_LEVEL(3);
		else
			MISC::SET_GRAVITY_LEVEL(0);
	}
	if (GUI::Toggle("Blackout", Features::WorldBlackout, "Disable all map lighting"))
	{
		GRAPHICS::SET_ARTIFICIAL_LIGHTS_STATE(Features::WorldBlackout);
	}
	if (GUI::Option("Clear Area", "Clear area of vehicles, objects etc"))
	{
		Vector3 MyPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false);
		MISC::CLEAR_AREA(MyPos.x, MyPos.y, MyPos.z, 250, true, false, false, false);
		MISC::CLEAR_AREA_OF_COPS(MyPos.x, MyPos.y, MyPos.z, 250, 0);
		MISC::CLEAR_AREA_OF_OBJECTS(MyPos.x, MyPos.y, MyPos.z, 250, 0);
		MISC::CLEAR_AREA_OF_VEHICLES(MyPos.x, MyPos.y, MyPos.z, 250, false, false, false, false, false, false, 0);
		MISC::CLEAR_AREA_OF_PEDS(MyPos.x, MyPos.y, MyPos.z, 250, 0);
		MISC::CLEAR_AREA_OF_PROJECTILES(MyPos.x, MyPos.y, MyPos.z, 250, false);

		// Seperate Remove Nearby Vehicles: for spawned vehicles
		const int ElementAmount = 10;
		const int ArrSize = ElementAmount * 2 + 2;

		::Vehicle* vehs = new ::Vehicle[ArrSize];
		vehs[0] = ElementAmount;
		int VehFound = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), vehs);

		for (int i = 0; i < VehFound; i++)
		{
			int OffsetID = i * 2 + 2;
			if (vehs[OffsetID] != PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false))
			{
				Game::entity::request_network_control(vehs[OffsetID]);
				ENTITY::DELETE_ENTITY(&vehs[OffsetID]);
			}
		}
		delete[] vehs;
		Game::notification::Minimap(helper::StringToChar("Area Cleared"));
	}
}
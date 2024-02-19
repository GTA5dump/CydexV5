#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../cheat/fibermain.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
int FakeWantedLevelInteger = 0;
void GUI::Submenus::Miscellaneous()
{
	GUI::Title("Miscellaneous");
	GUI::MenuOption("Report Statistics", Submenus::ReportStatistics);
	GUI::MenuOption("Disables", Submenus::Disables);
	GUI::MenuOption("GXT Editor", Submenus::GXTEditor);
	GUI::MenuOption("In-game Television", Submenus::Television);
	GUI::MenuOption("Radio", Submenus::Radio);
	GUI::MenuOption("Extra-sensory Perception", Submenus::ESP);
	GUI::MenuOption("Head-up Display", Submenus::HUD);
	GUI::MenuOption("IPL Loader", Submenus::IPLLoader);
	if (GUI::Int("Fake Wanted Level", FakeWantedLevelInteger, 0, 6, 1, "", SELECTABLE_DISABLE_SAVE | SELECTABLE_RETURN_VALUE_CHANGE))
	{
		MISC::SET_FAKE_WANTED_LEVEL(FakeWantedLevelInteger);
	}
	if (GUI::Toggle("No-Clip", Features::NoClipBool, ""))
	{
		if (!Features::NoClipBool)
		{
			Entity target = !PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false) ? PLAYER::PLAYER_PED_ID() : PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
			PLAYER::DISABLE_PLAYER_FIRING(PLAYER::PLAYER_ID(), false);
			ENTITY::SET_ENTITY_COLLISION(target, true, true);
			ENTITY::FREEZE_ENTITY_POSITION(target, false);
		}
	}
	GUI::Toggle("Jump Around Mode", Features::JumpAroundModeBool, "Nearby vehicles will 'jump around'");
	GUI::Toggle("Show Joining Players Notification", Features::ShowJoiningPlayersNotification, "");
	GUI::Toggle("Show FPS", Features::ShowFPSBool, "");
	if (GUI::Option("Drive To Waypoint", "A NPC drives you to the set waypoint"))
	{
		int WaypointHandle = UI::GET_FIRST_BLIP_INFO_ID(SpriteWaypoint);
		if (UI::DOES_BLIP_EXIST(WaypointHandle))
		{
			std::string VehicleName = "MARSHALL";
			Vector3 WayPointVector = UI::GET_BLIP_COORDS(WaypointHandle);
			STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY(helper::StringToChar(VehicleName)));
			while (!STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY(helper::StringToChar(VehicleName)))) { fibermain::pause(); }
			Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0);
			::Vehicle VehicleHandle = VEHICLE::CREATE_VEHICLE(MISC::GET_HASH_KEY(helper::StringToChar(VehicleName)), pos.x, pos.y, pos.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), 1, 1, false);
			if (VehicleHandle != 0)
			{
				Ped Driver = PED::CREATE_RANDOM_PED_AS_DRIVER(VehicleHandle, false);
				PED::SET_PED_INTO_VEHICLE(Driver, VehicleHandle, -1);
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), VehicleHandle, 0);
				TASK::TASK_VEHICLE_DRIVE_TO_COORD(Driver, VehicleHandle, WayPointVector.x, WayPointVector.y, WayPointVector.z, 40, 1, ENTITY::GET_ENTITY_MODEL(VehicleHandle), 7, 6, -1);
				VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(VehicleHandle, "CRUSADER");
				Game::notification::Minimap((char*)"NPC Driver Spawned");
			}
		}
		else { Game::notification::Minimap((char*)"Please set a waypoint first to use this feature"); }
	}
	if (GUI::Option("Get Empty Session", "Get Empty (Public) Session")) { Sleep(10000); }
	if (GUI::Option("Exit to Single Player", "")) { if (NETWORK::NETWORK_IS_SESSION_STARTED()) { NETWORK::_SHUTDOWN_AND_LOAD_MOST_RECENT_SAVE(); } }
}
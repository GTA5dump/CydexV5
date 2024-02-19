#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::NearbyVehicles()
{
	GUI::Title("Nearby Vehicles");
	if (GUI::Option("Explode Vehicles", "Explode nearby vehicles"))
	{
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
				Vector3 coords = ENTITY::GET_ENTITY_COORDS(vehs[OffsetID], false);
				FIRE::ADD_EXPLOSION(coords.x, coords.y, coords.z, 0, 1000.f, true, false, 0.f, false);
			}
		}
		delete[] vehs;
	}
	if (GUI::Option("Delete Vehicles", "Delete nearby vehicles"))
	{
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
				ENTITY::SET_ENTITY_COORDS(vehs[OffsetID], 6400.f, 6400.f, 0.f, false, false, false, false);
			}
		}
		delete[] vehs;
	}
	if (GUI::Option("Boost Vehicles", "Speed boost nearby vehicles"))
	{
		const int numElements = 10;
		const int arrSize = numElements * 2 + 2;
		::Vehicle veh[arrSize];
		veh[0] = numElements;
		int count = PED::GET_PED_NEARBY_VEHICLES(PLAYER::PLAYER_PED_ID(), veh);
		if (veh != NULL)
		{
			for (int i = 1; i <= count; i++)
			{
				int offsettedID = i;
				if (veh[offsettedID] != NULL && ENTITY::DOES_ENTITY_EXIST(veh[offsettedID]))
				{
					Game::entity::request_network_control(veh[offsettedID]);
					VEHICLE::SET_VEHICLE_FORWARD_SPEED(veh[offsettedID], 10000.0f);
				}
			}
		}
	}
}
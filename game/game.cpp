#include "game.hpp"
#include "../cheat/common.hpp"
#include "../cheat/cheat.hpp"
#include "../cheat/fibermain.hpp"
#include "../gui/GUI.hpp"
#include "../hooking/globalhandle.hpp"
#include "../hooking/offsets.hpp"
#include "../hooking/native/natives.hpp"
#include "../game/features.hpp"
#include "notification.hpp"
#include "data/game_data.hpp"
#include <regex>

namespace Cheat
{
	namespace Game
	{
		std::string keyboard::keyboard_title;
		keyboard::input_status keyboard::get_input(std::string Title, std::string& Input, int MinLength, int MaxLength)
		{
			// Temporarily modify the FMMC_KEY_TIP8 GXT string to show our own custom text
			keyboard_title = Title;

			// Show keyboard
			MISC::DISPLAY_ONSCREEN_KEYBOARD(0, "FMMC_KEY_TIP8", "", "", "", "", "", MaxLength);
			
			// Wait for user input or for user to cancel input
			while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) { fibermain::pause(false); }
				
			// Undo GXT modification
			keyboard_title.clear();
			
			// Return CANCELLED if the user cancelled input
			if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
			{
				return input_status::CANCELLED;
			}

			// Get inputted data
			Input = MISC::GET_ONSCREEN_KEYBOARD_RESULT();

			// Make sure the input is not too short  - this removes leading, trailing and double/extra spaces
			Input = std::regex_replace(Input, std::regex("^ +| +$|( ) +"), "$1");
			if (Input.length() < MinLength)
			{
				Input.clear();
				return input_status::TOO_SHORT;
			}

			// Return SUCCESS
			return input_status::SUCCESS;
		}
		void vehicle::repair(Vehicle vehicle)
		{
			// Entity health restore
			ENTITY::SET_ENTITY_HEALTH(vehicle, ENTITY::GET_ENTITY_MAX_HEALTH(vehicle), 0);
			
			// Vehicle engine
			VEHICLE::SET_VEHICLE_ENGINE_HEALTH(vehicle, 1000.f);

			// Set vehicle fixed 
			VEHICLE::SET_VEHICLE_FIXED(vehicle);
		}
		void vehicle::clean(Vehicle vehicle)
		{
			// Remove all dirt
			VEHICLE::SET_VEHICLE_DIRT_LEVEL(vehicle, 0.f);
		}
		std::vector <Vehicle> vehicle::UpDownGradeVehicleBlist = { 0x6838FC1D, 0x5C6C00B4 };
		void vehicle::up_downgrade(Vehicle vehicle, bool IsUpgrade, bool Notify)
		{
			/*
			SET_VEHICLE_MOD causes a game crash on certain recently added 'next-gen' DLC vehicles
			A IS_VEHICLE_MOD_GEN9_EXCLUSIVE native is available to check for this but it does not appear to be working correctly (from what was tested)
			So, at least for now, we simply refuse to up or downgrade any of these vehicles
			*/
			for (auto const& b : UpDownGradeVehicleBlist)
			{
				if (VEHICLE::IS_VEHICLE_MODEL(vehicle, b))
				{
					if (Notify)
						Game::notification::Minimap((char*)"~r~This vehicle cannot be up or downgraded");
					return;
				}
			}

			// Max up or downgrade target vehicle
			VEHICLE::SET_VEHICLE_MOD_KIT(vehicle, 0);
			for (int i = 0; i < 50; i++)
			{
				if (IsUpgrade)
					VEHICLE::SET_VEHICLE_MOD(vehicle, i, VEHICLE::GET_NUM_VEHICLE_MODS(vehicle, i) - 1, false);
				else
					VEHICLE::REMOVE_VEHICLE_MOD(vehicle, i);
			}
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, 18, IsUpgrade); // Turbo
			VEHICLE::TOGGLE_VEHICLE_MOD(vehicle, 22, IsUpgrade); // Xeon headlights

			if (Notify)
				Game::notification::MinimapAdvanced((char*)"Vehicle max up/downgraded", (char*)"Textures", (char*)"AdvancedNotificationImage", false, 4, (char*)"Los Santos Customs", (char*)"", 1.0, (char*)"");
		}
		std::vector<Vehicle> vehicle::previously_spawned;
		bool vehicle::spawn(Hash vehicle)
		{
			if (!STREAMING::IS_MODEL_A_VEHICLE(vehicle))
				return false;
				
			bool IsInVehicle = PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false);
			bool SetVelocity = false;

			// Load model
			while (!STREAMING::HAS_MODEL_LOADED(vehicle))
			{
				STREAMING::REQUEST_MODEL(vehicle);
				fibermain::pause(false);
			}

			// Get velocity
			Vector3 VelocityCurrentVehicle;
			if (Features::VehicleSpawnerDeleteOldVehicle && Features::VehicleSpawnerSpawnInsideVehicle && IsInVehicle)
			{
				VelocityCurrentVehicle = ENTITY::GET_ENTITY_VELOCITY(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));
				SetVelocity = true;
			}

			// Delete current
			if (Features::VehicleSpawnerDeleteOldVehicle && IsInVehicle)
				delete_v(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false));

			// Spawn aircraft
			Vector3 NewVehiclePosition = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0f, 5.0f, 0.0f);
			if (Features::VehicleSpawnerSpawnAirVehicleAir && (VEHICLE::IS_THIS_MODEL_A_PLANE(vehicle) || VEHICLE::IS_THIS_MODEL_A_HELI(vehicle)))
				NewVehiclePosition.z += 100.f;

			// Create vehicle
			Vehicle NewVehicleHandle = VEHICLE::CREATE_VEHICLE(vehicle, NewVehiclePosition.x, NewVehiclePosition.y, NewVehiclePosition.z, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()), true, true, false);
			if (NewVehicleHandle)
			{
				// Fade in effect - only works in GTAO
				NETWORK::NETWORK_FADE_IN_ENTITY(NewVehicleHandle, false, 0);

				if (Features::VehicleSpawnerSpawnInsideVehicle)
					PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), NewVehicleHandle, -1);

				if (Features::VehicleSpawnerSpawnWithGodmode)
					ENTITY::SET_ENTITY_INVINCIBLE(NewVehicleHandle, true);

				if (Features::VehicleSpawnerSpawnMaxUpgraded)
					up_downgrade(NewVehicleHandle, true, false);

				if (Features::VehicleSpawnerLicensePlateVectorPosition != 0)
				{
					char* LicensePlateString = (char*)"";
					if (Features::VehicleSpawnerLicensePlateVectorPosition == 2)
					{
						LicensePlateString = helper::StringToChar(Features::VehicleSpawnerCustomLicensePlateTextString);
					}
					VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(NewVehicleHandle, LicensePlateString);
				}

				VEHICLE::SET_VEHICLE_IS_STOLEN(NewVehicleHandle, false);

				VEHICLE::SET_VEHICLE_ENGINE_ON(NewVehicleHandle, true, true, false);

				if (SetVelocity)
					ENTITY::SET_ENTITY_VELOCITY(NewVehicleHandle, VelocityCurrentVehicle.x, VelocityCurrentVehicle.y, VelocityCurrentVehicle.z);

				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(NewVehicleHandle, true, true);

				NETWORK::SET_NETWORK_ID_EXISTS_ON_ALL_MACHINES(NETWORK::VEH_TO_NET(NewVehicleHandle), true);

				DECORATOR::DECOR_SET_INT(NewVehicleHandle, "MPBitset", 0);

				ENTITY::_SET_ENTITY_CLEANUP_BY_ENGINE(NewVehicleHandle, true);

				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(vehicle);

				previously_spawned.push_back(NewVehicleHandle);
			}
			return true;
		}
		bool vehicle::delete_v(Vehicle Vehicle)
		{
			if (ENTITY::DOES_ENTITY_EXIST(Vehicle))
			{
				entity::request_network_control(Vehicle);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Vehicle, true, true);
				VEHICLE::DELETE_VEHICLE(&Vehicle);
				return true;
			}
			return false;
		}
		int gtao::get_rp(int level)
		{
			return globalHandle(GLOBAL_LEVEL_TO_RP).At(1, level).As<int>();
		}
		bool gtao::is_friend(Player player)
		{
			int handle[76];
			NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
			if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
				return NETWORK::NETWORK_IS_FRIEND(&handle[0]);
			return false;
		}
		bool gtao::in_crew(Player player)
		{
			int handle[76];
			NETWORK::NETWORK_HANDLE_FROM_PLAYER(player, &handle[0], 13);
			if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
				return NETWORK::NETWORK_CLAN_PLAYER_IS_ACTIVE((Any*)handle);
			return false;
		}
		bool gtao::is_freemode_script_host(Player Player)
		{
			if (Player == NETWORK::NETWORK_GET_HOST_OF_SCRIPT("freemode", -1, 0))
				return true;
			return false;
		}
		int gtao::get_rockstar_id(Player Player)
		{
			int handle[76];
			NETWORK::NETWORK_HANDLE_FROM_PLAYER(Player, &handle[0], 13);
			if (NETWORK::NETWORK_IS_HANDLE_VALID(&handle[0], 13))
				return helper::StringToInt(NETWORK::NETWORK_MEMBER_ID_FROM_GAMER_HANDLE(&handle[0]));
			else
				return 0;
		}
		void gtao::set_skill_stat(std::string Skill, int Level)
		{
			STATS::STAT_SET_INT(MISC::GET_HASH_KEY(helper::StringToChar(get_current_character() + "_SCRIPT_INCREASE_" + Skill)), Level, true);
		}
		int gtao::get_skill_stat(std::string Skill)
		{
			int SkillLevelPercentage = 0;
			STATS::STAT_GET_INT(MISC::GET_HASH_KEY(helper::StringToChar(get_current_character() + "_SCRIPT_INCREASE_" + Skill)), &SkillLevelPercentage, -1);
			return SkillLevelPercentage;
		}
		void gtao::change_session(gtao::session_types type)
		{
			if (type >= 0 && type <= 12)
			{
				globalHandle(1575017).As<int>() = (int)type;
				globalHandle(1574589).As<int>() = 1;
				fibermain::pause(false, 50);
				globalHandle(1574589).As<int>() = 0;
				GUI::CloseMenuGUI();
			}
		}
		void gtao::set_rank(int Rank)
		{
			if (Rank > 0 && Rank <= 8000)
			{
				STATS::STAT_SET_INT(MISC::GET_HASH_KEY(helper::StringToChar(get_current_character() + "_CHAR_SET_RP_GIFT_ADMIN")), gtao::get_rp(Rank), true);
				Game::notification::Minimap((char*)"Join a new GTAO session for the new Rank to be applied");
			}
			else
			{
				Game::notification::Minimap((char*)"Invalid Rank Inputted");
			}
		}
		std::string gtao::get_current_character(bool NumberOnly)
		{
			int CharacterID;
			STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_LAST_MP_CHAR"), &CharacterID, -1);
			if (NumberOnly)
				return std::to_string(CharacterID);

			return "MP" + std::to_string(CharacterID);
		}
		int gtao::get_amount_killed()
		{
			int k;
			STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_KILLS_PLAYERS"), &k, -1);
			return k;
		}
		int gtao::get_amount_died()
		{
			int d;
			STATS::STAT_GET_INT(MISC::GET_HASH_KEY("MPPLY_DEATHS_PLAYER"), &d, -1);
			return d;
		}
		void gtao::tse::start_event(eRemoteEvents event_id, Player TargetPlayer, int property_teleport_index)
		{
			if (NETWORK::NETWORK_IS_SESSION_STARTED() && NETWORK::NETWORK_IS_PLAYER_ACTIVE(TargetPlayer))
			{
				if (event_id == eRemoteEvents::KickToSP)
				{
					uint64_t arguments_aray[4] = { (uint64_t)eRemoteEvents::KickToSP, static_cast<uint64_t>(TargetPlayer), 0, 0 };
					SCRIPT::TRIGGER_SCRIPT_EVENT(1, arguments_aray, sizeof(arguments_aray) / sizeof(arguments_aray[0]), 1 << TargetPlayer);
				}
				else if (event_id == eRemoteEvents::PropertyTeleport)
				{
					if (property_teleport_index != -1)
					{
						uint64_t teleport[9] = { (uint64_t)eRemoteEvents::PropertyTeleport, static_cast<uint64_t>(TargetPlayer), 0, static_cast<uint64_t>(-1), 1, static_cast<uint64_t>(property_teleport_index), 0, 0, 0 };
						SCRIPT::TRIGGER_SCRIPT_EVENT(1, teleport, 9, (1 << TargetPlayer));
					}
				}
				else if (event_id == eRemoteEvents::CayoPericoTeleport)
				{
					uint64_t arguments_aray[2] = { (uint64_t)eRemoteEvents::CayoPericoTeleport, static_cast<uint64_t>(TargetPlayer) };
					SCRIPT::TRIGGER_SCRIPT_EVENT(1, arguments_aray, sizeof(arguments_aray) / sizeof(arguments_aray[0]), 1 << TargetPlayer);
				}
				else if (event_id == eRemoteEvents::ForceIntoMission)
				{
					uint64_t arguments_aray[3] = { (uint64_t)eRemoteEvents::ForceIntoMission, static_cast<uint64_t>(TargetPlayer), 0 };
					SCRIPT::TRIGGER_SCRIPT_EVENT(1, arguments_aray, sizeof(arguments_aray) / sizeof(arguments_aray[0]), 1 << TargetPlayer);
				}
				else if (event_id == eRemoteEvents::CEOKick)
				{
					uint64_t arguments_aray[2] = { (uint64_t)eRemoteEvents::CEOKick, static_cast<uint64_t>(TargetPlayer) };
					SCRIPT::TRIGGER_SCRIPT_EVENT(1, arguments_aray, sizeof(arguments_aray) / sizeof(arguments_aray[0]), 1 << TargetPlayer);
				}
			}
		}
		float math::MSToKMH(float MS)
		{
			return roundf(MS * 3.6);
		}
		float math::MSToMPH(float MS)
		{
			return roundf(MS * 2.2);
		}
		float math::KMHToMS(float MS)
		{
			return roundf(MS * 0.27);
		}
		float math::MPHToMS(float MS)
		{
			return roundf(MS * 0.44);
		}
		Vector3 vector::multiply(Vector3 vector, float inc)
		{
			vector.x *= inc;
			vector.y *= inc;
			vector.z *= inc;
			vector._paddingx *= inc;
			vector._paddingy *= inc;
			vector._paddingz *= inc;
			return vector;
		}
		Vector3 vector::add_two(Vector3 vectorA, Vector3 vectorB)
		{
			Vector3 result;
			result.x = vectorA.x;
			result.y = vectorA.y;
			result.z = vectorA.z;
			result.x += vectorB.x;
			result.y += vectorB.y;
			result.z += vectorB.z;
			return result;
		}
		float vector::get_distance_between_two(Vector3 a, Vector3 b)
		{
			return MISC::GET_DISTANCE_BETWEEN_COORDS(a.x, a.y, a.z, b.x, b.y, b.z, true);
		}
		Vector3 vector::get_cam_direction()
		{
			Vector3 Cam = CAM::GET_GAMEPLAY_CAM_ROT(0);
			float radiansZ = Cam.z * 0.0174532924f;
			float radiansX = Cam.x * 0.0174532924f;
			float num = abs((float)cos((double)radiansX));
			Vector3 dir;
			dir.x = (float)((double)((float)(-(float)sin((double)radiansZ))) * (double)num);
			dir.y = (float)((double)((float)cos((double)radiansZ)) * (double)num);
			dir.z = (float)sin((double)radiansX);
			return dir;
		}
		void weapon::give_all(Ped Ped)
		{
			for (auto const& i : game_data::WeaponsHashList)
			{
				WEAPON::GIVE_WEAPON_TO_PED(Ped, i.WeaponHash, 9999, false, false);
			}
		}
		void weapon::max_upgrade_all(Ped Ped)
		{
			for (auto const& i : game_data::MaxUpgradeWeaponComponents)
			{
				WEAPON::GIVE_WEAPON_COMPONENT_TO_PED(Ped, i.WeaponHash, MISC::GET_HASH_KEY(helper::StringToChar(i.UpgradeHash)));
			}
		}
		bool entity::remove_attached_entity(Ped Ped, Hash Model)
		{
			Vector3 PedCoords = ENTITY::GET_ENTITY_COORDS(Ped, false);
			Object Object = OBJECT::GET_CLOSEST_OBJECT_OF_TYPE(PedCoords.x, PedCoords.y, PedCoords.z, 4.0f, Model, false, false, true);
			if (ENTITY::IS_ENTITY_ATTACHED_TO_ENTITY(Object, Ped))
			{
				request_network_control(Object);
				ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Object, true, true);
				ENTITY::DELETE_ENTITY(&Object);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&Object);
				return true;
			}
			return false;
		}
		void entity::apply_force(Entity Entity, float x, float y, float z)
		{
			request_network_control(Entity);
			ENTITY::APPLY_FORCE_TO_ENTITY(Entity, 1, x, y, z, 0, 0, 0, 0, true, true, true, false, true);
		}
		bool entity::is_in_interior(Entity Entity)
		{
			if (INTERIOR::GET_INTERIOR_FROM_ENTITY(Entity) == 0)
				return false;
			return true;
		}
		void entity::request_network_control(Entity Entity)
		{
			int EntityTick = 0;
			int IdTick = 0;
			while (!NETWORK::NETWORK_HAS_CONTROL_OF_ENTITY(Entity) && EntityTick <= 25)
			{
				NETWORK::NETWORK_REQUEST_CONTROL_OF_ENTITY(Entity);
				EntityTick++;
			}
			if (NETWORK::NETWORK_IS_SESSION_STARTED())
			{
				int netID = NETWORK::NETWORK_GET_NETWORK_ID_FROM_ENTITY(Entity);
				while (!NETWORK::NETWORK_HAS_CONTROL_OF_NETWORK_ID(netID) && IdTick <= 25)
				{
					NETWORK::NETWORK_REQUEST_CONTROL_OF_NETWORK_ID(netID);
					IdTick++;
				}
				NETWORK::SET_NETWORK_ID_CAN_MIGRATE(netID, true);
			}
		}
		void entity::attach_to_entity(Entity Entity, Hash Object)
		{
			int attachobj[100];
			int nuattach = 1;
			Vector3 pos = ENTITY::GET_ENTITY_COORDS(Entity, false);
			if (STREAMING::IS_MODEL_VALID(Object))
			{
				while (!STREAMING::HAS_MODEL_LOADED(Object))
				{
					STREAMING::REQUEST_MODEL(Object);
					fibermain::pause(false);
				}
					
				attachobj[nuattach] = OBJECT::CREATE_OBJECT(Object, pos.x, pos.y, pos.z, 1, 1, 1);
				if (ENTITY::DOES_ENTITY_EXIST(attachobj[nuattach]))
				{
					ENTITY::ATTACH_ENTITY_TO_ENTITY(attachobj[nuattach], Entity, 31086, 0, 0, 0, 0, 0, 0, true, false, false, false, 2, true);
					nuattach++;
					if (nuattach >= 101) { nuattach = 1; }
				}
			}
		}
		bool teleport::to_blip(Blip blip_sprite, int blip_color)
		{
			auto BlipHandle = UI::GET_FIRST_BLIP_INFO_ID(blip_sprite);
			if (UI::DOES_BLIP_EXIST(BlipHandle))
			{
				if (blip_color == -1 || UI::GET_BLIP_COLOUR(BlipHandle) == blip_color)
				{
					Game::teleport::to_coords(PLAYER::PLAYER_PED_ID(), UI::GET_BLIP_INFO_ID_COORD(BlipHandle), true, false);
					return true;
				}
			}
			return false;
		}
		bool teleport::to_objective()
		{
			std::map<int, int> blips = {
				{ eBlipSprite::SpriteStandard, eBlipColor::ColorYellow },
				{ eBlipSprite::SpriteStandard, eBlipColor::ColorYellowMission2 },
				{ eBlipSprite::SpriteRaceFinish, 0 },
				{ eBlipSprite::SpriteStandard, eBlipColor::ColorGreen },
				{ eBlipSprite::SpriteStandard, eBlipColor::ColorBlue },
				{ eBlipSprite::SpriteCrateDrop, 0 }
			};

			for (auto& blip : blips)
			{
				if (teleport::to_blip(blip.first, blip.second != 0 ? blip.second : -1))
					return true;
			}
			return false;
		}
		void teleport::to_coords(Entity e, Vector3 coords, bool AutoCorrectGroundHeight, bool IgnoreCurrentPedVehicle)
		{
			Entity TargetEntity = e;
			bool TeleportTransition = Features::TeleportTransition && TargetEntity == PLAYER::PLAYER_PED_ID();
			if (TeleportTransition)
				STREAMING::_SWITCH_OUT_PLAYER(TargetEntity, 0, 2);

			if (ENTITY::IS_AN_ENTITY(TargetEntity))
			{
				entity::request_network_control(TargetEntity);

				if (PED::IS_PED_IN_ANY_VEHICLE(TargetEntity, false) && !IgnoreCurrentPedVehicle)
				{
					TargetEntity = PED::GET_VEHICLE_PED_IS_USING(TargetEntity);
				}

				if (!AutoCorrectGroundHeight)
				{
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(TargetEntity, coords.x, coords.y, coords.z, false, false, true);
				}
				else
				{
					bool GroundFound = false;
					static std::array<float, 21> GroundCheckHeight = { 0.0f, 50.0f, 100.0f, 150.0f, 200.0f, 250.0f, 300.0f, 350.0f, 400.0f, 450.0f, 500.0f, 550.0f, 600.0f, 650.0f, 700.0f, 750.0f, 800.0f, 850.0f, 900.0f, 950.0f, 1000.00f };

					for (const float& CurrentHeight : GroundCheckHeight)
					{
						ENTITY::SET_ENTITY_COORDS_NO_OFFSET(TargetEntity, coords.x, coords.y, CurrentHeight, false, false, true);
						fibermain::pause(false, 50);
						if (MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, CurrentHeight, &coords.z, false, false))
						{
							GroundFound = true;
							coords.z += 3.0f;
							break;
						}
					}

					if (!GroundFound)
					{
						Vector3 ClosestRoadCoord;
						if (PATHFIND::GET_CLOSEST_ROAD(coords.x, coords.y, coords.z, 1.0f, 1,
							&ClosestRoadCoord, &ClosestRoadCoord, NULL, NULL, NULL, NULL))
						{
							coords = ClosestRoadCoord;
						}
						Game::notification::Subtitle((char*)"~r~Ground not found, teleported to nearby road", 4000);
					}
					ENTITY::SET_ENTITY_COORDS_NO_OFFSET(TargetEntity, coords.x, coords.y, coords.z, false, false, true);
				}
				if (TeleportTransition)
					STREAMING::_SWITCH_IN_PLAYER(TargetEntity);
			}
		}
		void ped::play_animation(Ped TargetPed, char* AnimationName, char* AnimationID, bool Controllable)
		{
			entity::request_network_control(TargetPed);
			while (!STREAMING::HAS_ANIM_DICT_LOADED((AnimationName))) 
			{ 
				STREAMING::REQUEST_ANIM_DICT(AnimationName);
				fibermain::pause(false);
			}
			TASK::TASK_PLAY_ANIM(TargetPed, AnimationName, AnimationID, 8.0f, 0.0f, -1, Controllable ? ANIM_FLAG_UPPERBODY | ANIM_FLAG_ENABLE_PLAYER_CONTROL | ANIM_FLAG_STOP_LAST_FRAME : 0, 0, false, false, false);
		}
		void ped::change_player_model(Hash Ped)
		{
			if (STREAMING::IS_MODEL_A_PED(Ped))
			{
				while (!STREAMING::HAS_MODEL_LOADED(Ped)) { STREAMING::REQUEST_MODEL(Ped); fibermain::pause(false); }
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), Ped);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				PED::CLEAR_ALL_PED_PROPS(PLAYER::PLAYER_PED_ID());
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Ped);
			}
		}
	}
}

void Cheat::Game::ShowTeleportLocationsMenu(std::vector<game_data::TeleportLocation> category)
{
	for (int i = 0; i < category.size(); i++)
	{
		Vector3 Coords;
		Coords.x = category[i].coordinateX;
		Coords.y = category[i].coordinateY;
		Coords.z = category[i].coordinateZ;
		if (GUI::Option(category[i].Name, ""))
		{
			Game::teleport::to_coords(PLAYER::PLAYER_PED_ID(), Coords, false, false);
		}
	}
}
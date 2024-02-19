#pragma once
#include "../hooking/types.hpp"
#include "enums.hpp"
#include "../game/data/game_data.hpp"

namespace Cheat
{
	namespace Game
	{
		// Keyboard
		class keyboard
		{
		public:
			static std::string keyboard_title;
			enum class input_status
			{
				SUCCESS,
				CANCELLED,
				TOO_SHORT
			};
			static input_status get_input(std::string Title, std::string& Input, int MinLength, int MaxLength);
		};

		// Vehicle
		class vehicle
		{
		private:
			static std::vector <Vehicle> UpDownGradeVehicleBlist;
		public:
			static std::vector<Vehicle> previously_spawned;
			static bool spawn(Hash vehicle);
			static void repair(Vehicle vehicle);
			static void clean(Vehicle vehicle);
			static void up_downgrade(Vehicle vehicle, bool IsUpgrade, bool Notify);
			static bool delete_v(Vehicle Vehicle);
		};

		// GTA Online
		class gtao
		{
		public:
			enum session_types
			{
				SessionTypeJoinPublic,
				SessionTypeNewPublic,
				SessionTypeClosedCrew,
				SessionTypeCrew,
				SessionTypeClosedFriend = 6,
				SessionTypeFindFriend = 9,
				SessionTypeSolo,
				SessionTypeInviteOnly,
				SessionTypeJoinCrew
			};

			static int get_rp(int level);
			static bool is_friend(Player player);
			static bool in_crew(Player player);
			static bool is_freemode_script_host(Player Player);
			static int get_rockstar_id(Player Player);
			static void set_skill_stat(std::string Skill, int Level);
			static int get_skill_stat(std::string Skill);
			static void change_session(gtao::session_types type);
			static void set_rank(int Rank);
			static std::string get_current_character(bool NumberOnly = false);
			static int get_amount_killed();
			static int get_amount_died();

			// Trigger Script Event
			class tse
			{
			public:
				static void start_event(eRemoteEvents event_id, Player TargetPlayer, int property_teleport_index = -1);
			};
		};

		// Math
		class math
		{
		public:
			static float MSToKMH(float MS);
			static float MSToMPH(float MS);
			static float KMHToMS(float MS);
			static float MPHToMS(float MS);
		};
		
		// Vector
		class vector
		{
		public:
			static Vector3 multiply(Vector3 vector, float inc);
			static Vector3 add_two(Vector3 vectorA, Vector3 vectorB);
			static float get_distance_between_two(Vector3 a, Vector3 b);
			static Vector3 get_cam_direction();
		};

		// Weapon
		class weapon
		{
		public:
			static void give_all(Ped Ped);
			static void max_upgrade_all(Ped Ped);
		};

		// Entity
		class entity
		{
		public:
			static bool remove_attached_entity(Ped Ped, Hash Model);
			static void apply_force(Entity Entity, float x, float y, float z);
			static bool is_in_interior(Entity Entity);
			static void request_network_control(Entity Entity);
			static void attach_to_entity(Entity Entity, Hash Object);
		};

		// Teleport
		class teleport
		{
		public:
			static bool to_blip(Blip blip_sprite, int blip_color = -1);
			static bool to_objective();
			static void to_coords(Entity e, Vector3 coords, bool AutoCorrectGroundHeight, bool IgnoreCurrentPedVehicle);
		};

		// Ped
		class ped
		{
		public:
			static void play_animation(Ped TargetPed, char* AnimationName, char* AnimationID, bool Controllable);
			static void change_player_model(Hash Ped);
		};
	}
}

namespace Cheat
{
	namespace Game
	{
		extern std::string InGameHelpTextMessage;
		void ShowTeleportLocationsMenu(std::vector<game_data::TeleportLocation> category);
	}
}

// Macros
#define PlayersSessionForLoop for(int i = 0; i < 32; i++)
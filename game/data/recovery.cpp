#include "game_data.hpp"

namespace Cheat
{
	const std::map<std::string, int> game_data::recovery::IncreasedThrowables
	{
		{ "_SR_HIGHSCORE_1", 690 },
		{ "_SR_HIGHSCORE_2", 1860 },
		{ "_SR_HIGHSCORE_3", 2690 },
		{ "_SR_HIGHSCORE_4", 2660 },
		{ "_SR_HIGHSCORE_5", 2650 },
		{ "_SR_HIGHSCORE_6", 269 },
		{ "_SR_WEAPON_BIT_SET", 0xFFFFFFFF },
		{ "_SR_TIER_1_REWARD", true },
		{ "_SR_INCREASE_THROW_CAP", true },
		{ "_SR_TIER_3_REWARD", true }
	};
	const std::map<std::string, int> game_data::recovery::Tattoos
	{
		{ "_AWD_FM_DM_WINS", 50 },
		{ "_AWD_FM_TDM_MVP", 50 },
		{ "_AWD_FMBBETWIN", 50000 },
		{ "_AWD_FMWINEVERYGAMEMODE", 1 },
		{ "_AWD_FMRACEWORLDRECHOLDER", 1 },
		{ "_AWD_FMFULLYMODDEDCAR", 1 },
		{ "_AWD_FMMOSTKILLSSURVIVE", 1 },
		{ "_AWD_FMKILL3ANDWINGTARACE", 1 },
		{ "_AWD_FMKILLBOUNTY", 25 },
		{ "_AWD_FMREVENGEKILLSDM", 50 },
		{ "_AWD_FMKILLSTREAKSDM", 1 },
		{ "_AWD_HOLD_UP_SHOPS", 20 },
		{ "_AWD_LAPDANCES", 25 },
		{ "_AWD_SECURITY_CARS_ROBBED", 25 },
		{ "_AWD_RACES_WON", 50 },
		{ "_AWD_CAR_BOMBS_ENEMY_KILLS", 25 },
		{ "_PLAYER_HEADSHOTS", 500 },
		{ "_DB_PLAYER_KILLS", 1000 }
	};
}
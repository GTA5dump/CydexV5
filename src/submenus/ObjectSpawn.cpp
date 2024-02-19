#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../cheat/fibermain.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"
#include "../../game/data/game_data.hpp"

using namespace Cheat;
void GUI::Submenus::ObjectSpawn()
{
	GUI::Title("Object Spawn");
	for (auto& i : game_data::Objects)
	{
		if (GUI::Option(i, "Select to spawn"))
		{
			Hash Object = MISC::GET_HASH_KEY(helper::StringToChar(i));
			if (STREAMING::IS_MODEL_VALID(Object))
			{
				while (!STREAMING::HAS_MODEL_LOADED(Object)) { STREAMING::REQUEST_MODEL(Object); fibermain::pause(); }
				Vector3 NewObjectPosition = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0f, 5.0f, 0.0f);
				Ped NewObjectHandle = OBJECT::CREATE_OBJECT(Object, NewObjectPosition, true, false, false);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Object);
			}
		}
	}
}
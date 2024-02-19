#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
float SelfHealth = 200.f;
float SelfMaxHealth = 200.f;
int PlayerWantedLevelInteger = 0;
void GUI::Submenus::Self()
{
	GUI::Title("Local");
	GUI::MenuOption("Model", Submenus::SelfModel);
	GUI::MenuOption("Wardrobe", Submenus::Wardrobe);
	GUI::MenuOption("Animations", Submenus::Animations);
	GUI::MenuOption("Vision", Submenus::Vision);
	GUI::Break("Main", SELECTABLE_CENTER_TEXT);
	GUI::Toggle("Immortality", Features::GodmodeBool, "A.K.A God mode");
	GUI::Toggle("Never Wanted", Features::NeverWantedBool, "Never get a wanted level");
	GUI::Toggle("No Ragdoll", Features::NoRagdollAndSeatbeltBool, "Disables ragdoll on your character");
	if (GUI::StringVector("Invisibility", { "None", "Others", "Local", "Others + local" }, Features::SelfInvisiblityVectorPosition, "Others: you are invisible for other players\nLocal: you are invisible only to yourself", SELECTABLE_RETURN_VALUE_CHANGE))
	{
		// Reset at state change
		NETWORK::_NETWORK_SET_ENTITY_INVISIBLE_TO_NETWORK(PLAYER::PLAYER_PED_ID(), false);
		ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), true, false);
	}
	GUI::StringVector("Jump", { "Standard", "Super", "Beast" }, Features::JumpVectorPosition, "Makes your character jump higher");
	GUI::Break("Other", SELECTABLE_CENTER_TEXT);
	GUI::Toggle("Spin Bot", Features::Spin, "You'll be squirming around");
	GUI::Toggle("Seatbelt", Features::Seatbelt, "Unable to fall from from bike, also called Glued to seats.");
	GUI::Toggle("Tiny Player", Features::TinyPlayerBool, "Lowers your character's scaling");
	GUI::Toggle("Super Man", Features::SuperManBool, "Fly around like a superman!");
	GUI::Toggle("Explosive Melee", Features::ExplosiveMeleeBool, "Objects you hit with melee explode");
	if (GUI::Int("Wanted Level", PlayerWantedLevelInteger, 0, 5, 1, "Set Wanted Level", SELECTABLE_DISABLE_SAVE | SELECTABLE_RETURN_VALUE_CHANGE)) { Features::NeverWantedBool = false; PLAYER::SET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID(), PlayerWantedLevelInteger, false); PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_ID(), false); }
	if (GUI::Int("Opacity", Features::PlayerOpacityInt, 50, 250, 50, "", SELECTABLE_RETURN_VALUE_CHANGE | SELECTABLE_DISABLE_SAVE)) { ENTITY::SET_ENTITY_ALPHA(PLAYER::PLAYER_PED_ID(), (Features::PlayerOpacityInt), false); }
	if (GUI::StringVector("Sprint Speed", { "Standard", "Fast", "Super" }, Features::FastSuperRunPosition, "", SELECTABLE_RETURN_VALUE_CHANGE))
	{
		if (Features::FastSuperRunPosition == 0)
		{
			PLAYER::SET_RUN_SPRINT_MULTIPLIER_FOR_PLAYER(PLAYER::PLAYER_ID(), 1.f);
		}
	}
	if (GUI::Float("Health", SelfHealth, 1.f, ENTITY::GET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID()), 10.f, "", 0, SELECTABLE_RETURN_VALUE_CHANGE | SELECTABLE_DISABLE_SAVE))
	{
		ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), SelfHealth, 0);
	}
	if (GUI::Float("Max Health", SelfMaxHealth, 25.f, 1000.f, 25.f, "", 0, SELECTABLE_RETURN_VALUE_CHANGE | SELECTABLE_DISABLE_SAVE))
	{
		ENTITY::SET_ENTITY_MAX_HEALTH(PLAYER::PLAYER_PED_ID(), SelfMaxHealth);
	}
	if (GUI::Option("Max Armor", "You will get full armor")) { PED::ADD_ARMOUR_TO_PED(PLAYER::PLAYER_PED_ID(), 200); }
	if (GUI::Option("Suicide", "Rest in Peace.")) { PED::APPLY_DAMAGE_TO_PED(PLAYER::PLAYER_PED_ID(), 1000.f, false, false); }
	if (GUI::Option("Clean", "Clean some visual damages like blood")) { PED::CLEAR_PED_BLOOD_DAMAGE(PLAYER::PLAYER_PED_ID()); PED::RESET_PED_VISIBLE_DAMAGE(PLAYER::PLAYER_PED_ID()); Game::notification::Minimap((char*)"Player Cleaned"); }
}
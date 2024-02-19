#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::Weapon()
{
	GUI::Title("Weapon");
	GUI::MenuOption("Weapons", Submenus::Weapons);
	GUI::MenuOption("Visuals", Submenus::WeaponVisuals);
	GUI::MenuOption("Ammo Modification", Submenus::WeaponAmmo);
	GUI::MenuOption("Shoot Entities", Submenus::ShootEntities);
	GUI::Break("General", SELECTABLE_CENTER_TEXT);
	GUI::Toggle("No Reload", Features::NoWeaponReloadBool, "Disables the reload animation of all weapons");
	GUI::Toggle("Infinite Ammo", Features::InfiniteAmmoBool, "Never run out of ammo again");
	GUI::Toggle("Rapid Fire", Features::WeaponRapidFireBool, "Shoot very fast");
	if (GUI::Toggle("Bypass Sticky Bomb Limit", Features::StickyBombLimit, ""))
	{
		if (Features::StickyBombLimit)
			hooking::c4_spawn_limit->apply();
		else
			hooking::c4_spawn_limit->restore();
	}
	GUI::Toggle("One Shot One Kill", Features::OneHitKillBool, "");
	GUI::StringVector("Triggerbot", { "Off", "NPC", "Player", "NPC + Player" }, Features::TriggerbotVectorPosition, "");
	GUI::Break("Entity Control", SELECTABLE_CENTER_TEXT);
	GUI::Toggle("Gravity Gun", Features::GravityGunBool, "");
	GUI::Float("Entity Distance", Features::GravityGunEntityDistance, 5.f, 25.f, 1.f, "Changing distance is also possible with the scroll wheel", 0);
}
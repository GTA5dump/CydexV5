#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::WeaponAmmo()
{
	GUI::Title("Ammo Modification");
	GUI::StringVector("Impact", { "Off", "Fire", "Airstrike", "Teleport To", "Explosion", "Show Bullet Coord" }, Features::ImpactAmmoVectorPosition, "");
	GUI::StringVector("Custom", { "Off", "Valkyrie", "Rhino Tank", "RPG", "Firework" }, Features::CustomAmmoVectorPosition, "");
	GUI::StringVector("Particle", { "Off", "Cartoon" }, Features::ParticleAmmoVectorPosition, "");
	GUI::Break("Options", SELECTABLE_CENTER_TEXT);
	GUI::Toggle("Delete Gun", Features::DeleteGunBool, "Use SNS Pistol with this option");
	GUI::Toggle("Nerf Bullets", Features::NerfBulletsBool, "Significantly reduces bullet damage");
}
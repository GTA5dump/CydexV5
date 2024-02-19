#include "submenus.hpp"
#include "../../cheat/cheat.hpp"
#include "../../game/game.hpp"
#include "../../game/features.hpp"
#include "../../game/notification.hpp"
#include "../../gui/GUI.hpp"
#include "../../gui/gui_controls.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;
void GUI::Submenus::ProtectionScriptEvents()
{
	GUI::Title("Script Events Protection");
	GUI::Toggle("Block All", Features::BlockAllScriptEvents, "Blocks all script events send to you - join a session before enabling", SELECTABLE_DISABLE_SAVE);
	GUI::Break("Script Events", SELECTABLE_CENTER_TEXT);
	GUI::Toggle("Kicks", Features::ProtectionScriptEvents_Kicks, "");
	GUI::Toggle("CEO Ban", Features::ProtectionScriptEvents_CEOBan, "");
	GUI::Toggle("CEO Kick", Features::ProtectionScriptEvents_CEOKick, "");
	GUI::Toggle("CEO Money", Features::ProtectionScriptEvents_CEOMoney, "");
	GUI::Toggle("Property Teleport", Features::ProtectionScriptEvents_PropertyTeleport, "");
	GUI::Toggle("Cayo Perico Teleport", Features::ProtectionScriptEvents_CayoPericoTeleport, "");
	GUI::Toggle("Force Into Mission", Features::ProtectionScriptEvents_ForceIntoMission, "");
	GUI::Toggle("Bounty", Features::ProtectionScriptEvents_Bounty, "");
	GUI::Toggle("Clear Wanted Level", Features::ProtectionScriptEvents_ClearWantedlevel, "");
	GUI::Toggle("Game Banner", Features::ProtectionScriptEvents_GameBanner, "");
	GUI::Toggle("Crash", Features::ProtectionScriptEvents_Crash, "");
	GUI::Toggle("Personal Vehicle Destroyed", Features::ProtectionScriptEvents_PersonalVehicleDestroyed, "");
	GUI::Toggle("Remote Off Radar", Features::ProtectionScriptEvents_RemoteOffradar, "");
	GUI::Toggle("Send To Cutscene", Features::ProtectionScriptEvents_SendToCutscene, "");
	GUI::Toggle("Send To Location", Features::ProtectionScriptEvents_SendToLocation, "");
	GUI::Toggle("Sound Spam", Features::ProtectionScriptEvents_SoundSpam, "");
	GUI::Toggle("Spectate", Features::ProtectionScriptEvents_Spectate, "");
	GUI::Toggle("SMS", Features::ProtectionScriptEvents_SMS, "");
	GUI::Toggle("TSE Start", Features::ProtectionScriptEvents_TSEStart, "");
	GUI::Toggle("Notification", Features::ProtectionScriptEvents_Notification, "");
	GUI::Toggle("Interior Control", Features::ProtectionScriptEvents_InteriorControl, "");
	GUI::Toggle("Interior Kick", Features::ProtectionScriptEvents_InteriorKick, "");
	GUI::Toggle("Transaction Error", Features::ProtectionScriptEvents_TransactionError, "");
	GUI::Toggle("CEO Raid", Features::ProtectionScriptEvents_CEORaid, "");
	GUI::Toggle("Vehicle Kick", Features::ProtectionScriptEvents_VehicleKick, "");
	GUI::Toggle("Start Activity", Features::ProtectionScriptEvents_StartActivity, "");
	GUI::Toggle("Give Collectible", Features::ProtectionScriptEvents_GiveCollectible, "");
	GUI::Toggle("Warehouse Teleport", Features::ProtectionScriptEvents_WarehouseTeleport, "");
}
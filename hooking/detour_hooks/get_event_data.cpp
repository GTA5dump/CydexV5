#include "../../cheat/fibermain.hpp"
#include "../../game/notification.hpp"
#include "../../hooking/native/natives.hpp"
#include "../../game/features.hpp"

using namespace Cheat;

GetEventData hooking::GetEventDataOriginal = nullptr;
bool GetEventDataHooked(std::int32_t eventGroup, std::int32_t eventIndex, std::int64_t* args, std::uint32_t argCount)
{
	auto result = static_cast<decltype(&GetEventDataHooked)>(hooking::GetEventDataOriginal)(eventGroup, eventIndex, args, argCount);
	if (result)
	{
		bool BlockScriptEvent = false;
		std::string ScriptEventIDType;
		const auto EventHash = static_cast<eRemoteEvents>(args[0]);

		// Check whether the incoming event needs to be blocked
		if (Features::BlockAllScriptEvents)
		{
			BlockScriptEvent = true;
		}
		else
		{
			switch (EventHash)
			{
			case eRemoteEvents::KickToSP:
			case eRemoteEvents::KickToSP2:
			case eRemoteEvents::NetworkBail:
				if (Features::ProtectionScriptEvents_Kicks)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " session kick";
				}
				break;
			case eRemoteEvents::CeoMoney:
				if (Features::ProtectionScriptEvents_CEOMoney)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " giving CEO money";
				}
				break;
			case eRemoteEvents::PropertyTeleport:
				if (Features::ProtectionScriptEvents_PropertyTeleport)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " property teleport";
				}
				break;
			case eRemoteEvents::CayoPericoTeleport:
				if (Features::ProtectionScriptEvents_CayoPericoTeleport)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " cayo perico teleport";
				}
				break;
			case eRemoteEvents::ForceIntoMission:
			case eRemoteEvents::ForceIntoMission2:
				if (Features::ProtectionScriptEvents_ForceIntoMission)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " force into mission";
				}
				break;
			case eRemoteEvents::Bounty:
				if (Features::ProtectionScriptEvents_Bounty)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " setting bounty";
				}
				break;
			case eRemoteEvents::ClearWantedLevel:
				if (Features::ProtectionScriptEvents_ClearWantedlevel)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " clearing wanted level";
				}
				break;
			case eRemoteEvents::GameBanner:
				if (Features::ProtectionScriptEvents_GameBanner)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " showing a banner";
				}
				break;
			case eRemoteEvents::Crash:
			case eRemoteEvents::Crash2:
			case eRemoteEvents::Crash3:
				if (Features::ProtectionScriptEvents_Crash)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " game crash";

					if (EventHash == eRemoteEvents::Crash3 && args[3] <= 32)
						ScriptEventIDType = " MC teleport";		
				}
				break;
			case eRemoteEvents::Crash4:
				if (Features::ProtectionScriptEvents_Crash && (isnan(*(float*)&args[3]) || isnan(*(float*)&args[4])))
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " game crash";
				}
				break;
			case eRemoteEvents::PersonalVehicleDestroyed:
			case eRemoteEvents::PersonalVehicleDestroyed2:
				if (Features::ProtectionScriptEvents_PersonalVehicleDestroyed)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " showing personal vehicle destroyed msg";
				}
				break;
			case eRemoteEvents::RemoteOffradar:
				if (Features::ProtectionScriptEvents_RemoteOffradar)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " activating off the radar";
				}
				break;
			case eRemoteEvents::SendToCutscene:
				if (Features::ProtectionScriptEvents_SendToCutscene)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " sending you to a cutscene";
				}
				break;
			case eRemoteEvents::SendToLocation:
				if (Features::ProtectionScriptEvents_SendToLocation)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " sending you to a location";
				}
				break;
			case eRemoteEvents::SoundSpam:
				if (Features::ProtectionScriptEvents_SoundSpam)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " sound spam";
				}
				break;
			case eRemoteEvents::Spectate:
				if (Features::ProtectionScriptEvents_Spectate)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " spectating";
				}
				break;
			case eRemoteEvents::CEOBan:
				if (Features::ProtectionScriptEvents_CEOBan)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " CEO ban you";
				}
				break;
			case eRemoteEvents::CEOKick:
				if (Features::ProtectionScriptEvents_CEOKick)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " CEO kick you";
				}
				break;
			case eRemoteEvents::GiveCollectible:
				if (Features::ProtectionScriptEvents_GiveCollectible)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " giving you a collectible";
				}
				break;
			case eRemoteEvents::WarehouseTeleport:
				if (Features::ProtectionScriptEvents_WarehouseTeleport)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " warehouse teleport";
				}
				break;
			case eRemoteEvents::TransactionError:
				if (Features::ProtectionScriptEvents_TransactionError)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " showing a transaction error";
				}
				break;
			case eRemoteEvents::VehicleKick:
				if (Features::ProtectionScriptEvents_VehicleKick)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " kicking you from your vehicle";
				}
				break;
			case eRemoteEvents::StartActivity:
				if (Features::ProtectionScriptEvents_StartActivity)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " starting an activity";
				}
				break;
			case eRemoteEvents::InteriorKick:
				if (Features::ProtectionScriptEvents_InteriorKick)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " kicking you from interior";
				}
				break;
			case eRemoteEvents::InteriorControl:
				if (Features::ProtectionScriptEvents_InteriorControl)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " interior control";
				}
				break;
			case eRemoteEvents::SMS:
				if (Features::ProtectionScriptEvents_SMS)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " sending you a SMS";
				}
				break;
			case eRemoteEvents::Notification:
				if (Features::ProtectionScriptEvents_Notification)
				{
					switch (static_cast<eRemoteEvents>(args[2]))
					{
					case eRemoteEvents::NotificationBank:
					case eRemoteEvents::NotificationMoneyRemoved:
					case eRemoteEvents::NotificationMoneyStolen:
						BlockScriptEvent = true;
						ScriptEventIDType = " sending you a notification";
					}			
				}
				break;
			case eRemoteEvents::CEORaid:
				if (Features::ProtectionScriptEvents_CEORaid)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " triggering CEO raid";
				}
				break;
			case eRemoteEvents::TSEStart:
				if (Features::ProtectionScriptEvents_TSEStart)
				{
					BlockScriptEvent = true;
					ScriptEventIDType = " TSE start";
				}
				break;
			}
		}

		// Do the actual event block and show a notification
		if (BlockScriptEvent)
		{
			if (!Features::BlockAllScriptEvents)
			{
				std::string MessageString = "ID: " + std::to_string(args[0]);
				if (!ScriptEventIDType.empty())
				{
					MessageString.append("~n~Block reason: attempted" + ScriptEventIDType);
				}
				Game::notification::MinimapAdvanced(MessageString.data(), (char*)"Textures", (char*)"AdvancedNotificationImage", false, 4, (char*)"Script Events Protection", (char*)"", 0.8f, (char*)"");
			}
			return false;
		}
	}
	return result;
}
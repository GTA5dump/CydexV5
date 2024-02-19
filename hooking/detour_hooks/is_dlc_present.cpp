#include "../../cheat/fibermain.hpp"
#include "../../cheat/cheat.hpp"
#include "../../hooking/native/natives.hpp"

using namespace Cheat;

IsDLCPresent hooking::IsDLCPresentOriginal = nullptr;
bool IsDLCPresentHooked(std::uint32_t DLCHash)
{
	static uint64_t	Last = 0;
	uint64_t Current = MISC::GET_FRAME_COUNT();

	if (core::c_running && Last != Current)
	{
		Last = Current;
		if (fibermain::handle == nullptr)
			fibermain::handle = ConvertThreadToFiber(nullptr);

		static HANDLE scriptFiber;
		if (timeGetTime() > fibermain::wake_time)
		{
			if (scriptFiber != nullptr)
				SwitchToFiber(scriptFiber);
			else
				scriptFiber = CreateFiber(NULL, fibermain::func_, nullptr);
		}
	}
	return hooking::IsDLCPresentOriginal(DLCHash);
}
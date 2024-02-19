#pragma once
#include "common.hpp"

namespace Cheat
{
	class fibermain
	{
	public:
		static HANDLE handle;
		static void func_(LPVOID lpParameter);
		static void pause(bool show_msg = true, int duration = 0);
		static DWORD wake_time;
	};
}
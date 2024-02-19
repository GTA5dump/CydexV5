#pragma once
#include <iostream>
#include <vector>

namespace Cheat
{
	namespace Features
	{
		class theme_loader
		{
		public:
			static void Create(std::string Name);
			static bool Load(std::string Name);
			static bool Remove(std::string Name);
			static const std::string version;
			static std::string Current;
		};
	}
}
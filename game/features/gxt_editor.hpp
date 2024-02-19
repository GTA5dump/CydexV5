#pragma once
#include "../../cheat/common.hpp"

namespace Cheat
{
	namespace Features
	{
		class gxt_editor
		{
		public:
			static bool get_string(std::string gxt_string, const char*& modified_string);
			static void load_custom_file();
			static void create_custom(std::string gxt_string, std::string modified_string);
			static bool remove_custom(std::string gxt_string);

			static std::map<std::string, std::string> custom_strings;
		private:
			static const std::map<std::string, std::string> default_strings;
		};
	}
}
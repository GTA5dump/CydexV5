#include <filesystem>
#include "log.hpp"
#include "cheat.hpp"
#include "version.hpp"
#include "file_system.hpp"

using namespace Cheat;

//#define LOG_NO_CONSOLE // Uncomment to disable console - not advised

namespace Cheat
{
    std::shared_ptr<spdlog::logger> log::logger;
    void log::init()
    {
#ifndef LOG_NO_CONSOLE
#define _CRT_SECURE_NO_WARNINGS
        AllocConsole();
        freopen("CONIN$", "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        SetConsoleTitleA("Cydex 5.0 - Console");
        SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS);
#undef _CRT_SECURE_NO_WARNINGS
#endif

        try
        {
            // Create combined logger
            std::vector<spdlog::sink_ptr> sinks;
#ifndef LOG_NO_CONSOLE
            sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
#endif
            sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(file_system::paths::CheatLogFile));
            logger = std::make_shared<spdlog::logger>("logger", begin(sinks), end(sinks));
            
            // Set log level (log everything)
            logger->set_level(spdlog::level::trace);

            // Set log pattern
            logger->set_pattern("[%H:%M:%S.%f][%^%l|%s:%#%$] %v");

            // Flush immediately when log is of type debug or higher
            logger->flush_on(spdlog::level::debug);
        }
        catch (const spdlog::spdlog_ex& err)
        {
            std::ostringstream init_fail;
            init_fail << "Logger init failed: " << err.what() << std::endl;

#ifndef LOG_NO_CONSOLE
            std::cout << init_fail.str();    
#else
            MessageBoxA(NULL, init_fail.str().c_str(), "Cydex", MB_OK);
#endif
        }

#ifndef LOG_NO_CONSOLE
        std::cout << "Injecting Cydex, your current version of Cydex is " << build_info::VersionString << std::endl;
#endif

        // Cleanup main log files
        for (const auto& file : std::filesystem::directory_iterator(file_system::paths::LogDir))
        {
            struct stat result;
            if (stat(file.path().string().c_str(), &result) == 0)
            {
                auto CurrentEpoch = duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                auto FileEpoch = result.st_mtime; // Using st_mtime (last modified) instead of st_ctime (creation date) as the later changes when the file is recreated during a copy action

                // Delete files older than 24 hours
                if (CurrentEpoch - FileEpoch > 86400)
                {
                    std::remove(file.path().string().c_str());
                    LOG_DEBUG("Removed outdated logfile: {0}", file.path().filename().string().c_str());
                }
            }
        }
    }
}
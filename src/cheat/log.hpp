#pragma once
#include <string>
#define FMT_USE_NONTYPE_TEMPLATE_ARGS 0 // https://github.com/fmtlib/fmt/issues/2536
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/spdlog.h>

namespace Cheat
{
    class log
    {
    private:
        static std::shared_ptr<spdlog::logger> logger;
    public:
        static void init();
        inline static std::shared_ptr<spdlog::logger> & GetLogger() { return logger; }
    };
}

#define LOG_INFO(...)   SPDLOG_LOGGER_INFO(log::GetLogger(), __VA_ARGS__)
#define LOG_DEBUG(...)   SPDLOG_LOGGER_DEBUG(log::GetLogger(), __VA_ARGS__)
#define LOG_WARN(...)   SPDLOG_LOGGER_WARN(log::GetLogger(), __VA_ARGS__)
#define LOG_ERROR(...)   SPDLOG_LOGGER_ERROR(log::GetLogger(), __VA_ARGS__)
#define LOG_FATAL(...)  SPDLOG_LOGGER_CRITICAL(log::GetLogger(), __VA_ARGS__)
#include <spdlog/spdlog.h>

#include "common/assert.h"
#include "common/logging/backend_spdlog.h"
#include "common/logging/formatter.h"
#include "common/string_util.h"

static spdlog::level::level_enum GetLevel(Log::Level log_level) {
    switch (log_level) {
    case Log::Level::Trace:
        return spdlog::level::trace;
    case Log::Level::Debug:
        return spdlog::level::debug;
    case Log::Level::Info:
        return spdlog::level::info;
    case Log::Level::Warning:
        return spdlog::level::warn;
    case Log::Level::Error:
        return spdlog::level::err;
    case Log::Level::Critical:
        return spdlog::level::critical;
    default:
        UNREACHABLE();
        break;
    }
    return spdlog::level::off;
}

namespace Log {

SpdLogBackend& SpdLogBackend::instance() {
    static SpdLogBackend instance;
    return instance;
}

SpdLogBackend::SpdLogBackend() {
    // setup the custom citra formatter
    spdlog::set_formatter(std::make_shared<Formatter>());

    // Define the sinks to be passed to the loggers
    // true means truncate file
    auto file_sink = std::make_shared<spdlog::sinks::simple_file_sink_mt>("citra_log.txt", true);
#ifdef _WIN32
    auto color_sink = std::make_shared<spdlog::sinks::wincolor_stderr_sink_mt>();
#else
    auto stderr_sink = spdlog::sinks::stderr_sink_mt::instance();
    auto color_sink = std::make_shared<spdlog::sinks::ansicolor_sink>(stderr_sink);
#endif
    sinks.push_back(file_sink);
    sinks.push_back(color_sink);
}

SpdLogBackend::~SpdLogBackend() {
    spdlog::drop_all();
}

std::shared_ptr<spdlog::logger> SpdLogBackend::GetLogger(const std::string& class_name) {
    auto it = loggers_map.find(class_name);
    if (it != loggers_map.end()) {
        return it->second;
    }

    it = loggers_map.emplace_hint(it, class_name,
                                  spdlog::create(class_name, sinks.begin(), sinks.end()));
    return it->second;
}

void SpdLogImpl(Logger* logger, Level log_level, const char* format, fmt::ArgList& args) {
    logger->log(GetLevel(log_level), format, args);
}

Logger* GetLogger(const char* class_name) {
    // This instance is global and will never be destroyed, so it's safe to return just the raw
    // pointer, because the logger will always be kept alive by the map in the backend.
    return SpdLogBackend::instance().GetLogger(class_name).get();
}

}; // namespace Log

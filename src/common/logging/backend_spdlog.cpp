#include "common/assert.h"
#include "common/logging/backend_spdlog.h"
#include "common/logging/formatter.h"
#include "common/string_util.h"

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

const std::shared_ptr<spdlog::logger> SpdLogBackend::GetLogger(u32 logger) const {
    return loggers[logger];
}

u32 SpdLogBackend::RegisterLogger(const char* class_name) {
    loggers.push_back(std::make_shared<spdlog::logger>(class_name, sinks.begin(), sinks.end()));
    return loggers.size() - 1;
}

spdlog::level_t GetLevel(Level log_level) {
    switch (log_level) {
    case Level::Trace:
        return spdlog::level::trace;
    case Level::Debug:
        return spdlog::level::debug;
    case Level::Info:
        return spdlog::level::info;
    case Level::Warning:
        return spdlog::level::warn;
    case Level::Error:
        return spdlog::level::err;
    case Level::Critical:
        return spdlog::level::critical;
    default:
        UNREACHABLE();
        break;
    }
    return spdlog::level::off;
}

template <typename Arg1, typename... Args>
void SpdLogMessage(u32 logger, Level log_level, const char* filename, unsigned int line_nr,
                   const char* function, const char* format, const Arg1& arg, const Args&... args) {
    auto log = SpdLogBackend::instance().GetLogger(logger);
    fmt::MemoryWriter formatting_buffer;
    formatting_buffer << Common::TrimSourcePath(filename) << ':' << function << ':' << line_nr
                      << ": " << format;
    log->log(GetLevel(log_level), formatting_buffer.c_str(), arg, args...);
}

template <typename T>
void SpdLogMessage(u32 logger, Level log_level, const char* filename, unsigned int line_nr,
                   const char* function, const T& msg) {
    auto log = SpdLogBackend::instance().GetLogger(logger);
    fmt::MemoryWriter formatting_buffer;
    formatting_buffer << Common::TrimSourcePath(filename) << ':' << function << ':' << line_nr
                      << ": " << msg;
    log->log(GetLevel(log_level), formatting_buffer.c_str());
}

u32 RegisterLogger(const char* class_name) {
    return SpdLogBackend::instance().RegisterLogger(class_name);
}
}; // namespace Log

#include <chrono>
#include <string>
#include "common/assert.h"
#include "common/logging/formatter.h"

static const char* GetLevelName(spdlog::level_t log_level) {
    switch (log_level) {
    case spdlog::level::trace:
        return "Trace";
    case spdlog::level::debug:
        return "Debug";
    case spdlog::level::info:
        return "Info";
    case spdlog::level::warn:
        return "Warning";
    case spdlog::level::err:
        return "Error";
    case spdlog::level::critical:
        return "Critical";
    default:
        UNREACHABLE();
        return "UNREACHABLE";
    }
}

namespace Log {

void Formatter::format(spdlog::details::log_msg& msg) {
    using std::chrono::steady_clock;
    using std::chrono::duration_cast;

    static steady_clock::time_point time_origin = steady_clock::now();
    auto timestamp = duration_cast<std::chrono::microseconds>(steady_clock::now() - time_origin);

    unsigned int time_seconds = static_cast<unsigned int>(timestamp.count() / 1000000);
    unsigned int time_fractional = static_cast<unsigned int>(timestamp.count() % 1000000);

    msg.formatted << '[' << fmt::pad(time_seconds, 4, ' ') << '.'
                  << fmt::pad(time_fractional, 6, '0') << "] ";
    msg.formatted << *msg.logger_name << " <" << GetLevelName(msg.level) << "> ";

    msg.formatted << fmt::StringRef(msg.raw.data(), msg.raw.size());
    msg.formatted.write(spdlog::details::os::eol, spdlog::details::os::eol_size);
}

} // namespace Log
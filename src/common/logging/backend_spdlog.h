#include <memory>
#include <vector>
#include <spdlog/spdlog.h>
#include "common/logging/log.h"

namespace Log {

class SpdLogBackend {
public:
    static SpdLogBackend& instance();

    SpdLogBackend(SpdLogBackend const&) = delete;
    SpdLogBackend& operator=(SpdLogBackend const&) = delete;

    u32 RegisterLogger(const char* class_name);

    const std::shared_ptr<spdlog::logger>& GetLogger(u32 logger) const;

private:
    SpdLogBackend();

    ~SpdLogBackend();

    std::vector<std::shared_ptr<spdlog::logger>> loggers;
    std::vector<spdlog::sink_ptr> sinks;
};
} // namespace Log
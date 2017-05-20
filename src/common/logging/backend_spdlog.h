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

    std::shared_ptr<spdlog::logger> GetLogger(const std::string& class_name);

private:
    SpdLogBackend();

    ~SpdLogBackend();

    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> loggers_map;
    std::vector<spdlog::sink_ptr> sinks;
};
} // namespace Log
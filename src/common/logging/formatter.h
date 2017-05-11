#include <spdlog/spdlog.h>

namespace Log {

class Formatter : public spdlog::formatter {

public:
    explicit Formatter();
    Formatter(const Formatter&) = delete;
    Formatter& operator=(const Formatter&) = delete;
    void format(spdlog::details::log_msg& msg) override;
};

} // namespace Log
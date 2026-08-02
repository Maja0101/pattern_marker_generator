#include "logger_setup.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace pmg::logging {

    bool initialize() {
        auto console = spdlog::stdout_color_mt("console");

        console->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%8l%$] [%s:%#] [%!] %v");

        spdlog::set_default_logger(console);

        return true;
    }
    
} // namespace pmg::logging
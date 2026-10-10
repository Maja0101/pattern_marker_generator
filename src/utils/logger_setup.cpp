#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "utils/logger_setup.hpp"

namespace pmg::logging {

bool initialize() {
    auto console = spdlog::stdout_color_mt("console");
    auto basicConsole = spdlog::stdout_color_mt("basicConsole");

    console->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%8l%$] [%g:%#] [%!] %v");
    basicConsole->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%8l%$] %v");

    spdlog::set_default_logger(console);

    return true;
}
    
} // namespace pmg::logging
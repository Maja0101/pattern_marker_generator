#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "logger_setup.hpp"

#include <spdlog/spdlog.h>

int main()
{
    pmg::logging::initialize();

    SPDLOG_INFO("Starting Pattern Marker Generator...");

    return 0;
}



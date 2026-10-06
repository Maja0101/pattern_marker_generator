#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG

#include "utils/logger_setup.hpp"
#include "utils/timer.hpp"

#include <spdlog/spdlog.h>

int main()
{
    pmg::logging::initialize();

    SPDLOG_INFO("Starting Pattern Marker Generator...");

    {
        pmg::Timer timer;

        for (int i = 0; i < 1000; ++i) {
            SPDLOG_ERROR("!!! {} !!!", i);
        }
    }

    return 0;
}



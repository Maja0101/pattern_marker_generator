#pragma once

#include <chrono>
#include <source_location>

#include <spdlog/spdlog.h>

namespace pmg {

struct Timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> duration;
    std::source_location location;

    Timer(std::source_location location = std::source_location::current());
    ~Timer();

    private:
        std::shared_ptr<spdlog::logger> m_logger;
};
    
} // namespace pmg
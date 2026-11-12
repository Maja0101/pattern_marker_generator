#pragma once

#include <chrono>
#include <source_location>

#include <spdlog/spdlog.h>

namespace pmg::timing {

struct Timer
{
    explicit Timer(std::source_location location = std::source_location::current());
    ~Timer();
    
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    std::chrono::duration<float> duration;
    std::source_location location;
    std::shared_ptr<spdlog::logger> m_logger;
};
    
} // namespace pmg::timing
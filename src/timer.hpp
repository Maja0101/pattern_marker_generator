#pragma once

#include <chrono>

namespace pmg {

struct Timer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<float> duration;

    Timer();
    ~Timer();
};
    
} // namespace pmg
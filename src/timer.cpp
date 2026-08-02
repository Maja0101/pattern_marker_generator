#include "timer.hpp"

#include <spdlog/spdlog.h>

namespace pmg {

Timer::Timer() {
    start = std::chrono::high_resolution_clock::now();
}

Timer::~Timer() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    float ms = duration.count() * 1000.0f;

    SPDLOG_INFO("Execution time: {:.6f} ms", ms);
}

} // namespace pmg
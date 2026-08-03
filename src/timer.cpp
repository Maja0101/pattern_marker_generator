#include "timer.hpp"

#include <spdlog/spdlog.h>

namespace pmg {

Timer::Timer(std::source_location location) :
    m_logger(spdlog::get("basicConsole")),
    location(location),
    start(std::chrono::high_resolution_clock::now()){}

Timer::~Timer() {
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    float ms = duration.count() * 1000.0f;

    SPDLOG_LOGGER_INFO(m_logger, "[{}:{}] [{}] Execution time: {:.6f} ms", location.file_name(), location.line(), location.function_name(), ms);
}

} // namespace pmg
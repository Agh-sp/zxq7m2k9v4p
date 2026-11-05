#include "aether/core/Log.h"

namespace aether {

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");

    s_CoreLogger = spdlog::stdout_color_mt("AETHER");
    s_CoreLogger->set_level(spdlog::level::trace);

    s_ClientLogger = spdlog::stdout_color_mt("APP");
    s_ClientLogger->set_level(spdlog::level::trace);

    AE_CORE_INFO("Logging initialized");
}

void Log::Shutdown() {
    s_CoreLogger.reset();
    s_ClientLogger.reset();
    spdlog::shutdown();
}

std::shared_ptr<spdlog::logger>& Log::Core()   { return s_CoreLogger; }
std::shared_ptr<spdlog::logger>& Log::Client() { return s_ClientLogger; }

} // namespace aether

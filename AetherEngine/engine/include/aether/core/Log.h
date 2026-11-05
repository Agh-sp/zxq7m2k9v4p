#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>

namespace aether {

class Log {
public:
    static void Init();
    static void Shutdown();

    static std::shared_ptr<spdlog::logger>& Core();
    static std::shared_ptr<spdlog::logger>& Client();

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace aether

#ifdef _MSC_VER
#define AE_DEBUGBREAK() __debugbreak()
#else
#define AE_DEBUGBREAK() __builtin_trap()
#endif

#define AE_CORE_TRACE(...)    ::aether::Log::Core()->trace(__VA_ARGS__)
#define AE_CORE_INFO(...)     ::aether::Log::Core()->info(__VA_ARGS__)
#define AE_CORE_WARN(...)     ::aether::Log::Core()->warn(__VA_ARGS__)
#define AE_CORE_ERROR(...)    ::aether::Log::Core()->error(__VA_ARGS__)
#define AE_CORE_CRITICAL(...) ::aether::Log::Core()->critical(__VA_ARGS__)

#define AE_TRACE(...)    ::aether::Log::Client()->trace(__VA_ARGS__)
#define AE_INFO(...)     ::aether::Log::Client()->info(__VA_ARGS__)
#define AE_WARN(...)     ::aether::Log::Client()->warn(__VA_ARGS__)
#define AE_ERROR(...)    ::aether::Log::Client()->error(__VA_ARGS__)
#define AE_CRITICAL(...) ::aether::Log::Client()->critical(__VA_ARGS__)

#define AE_CORE_ASSERT(cond, ...) \
    do { \
        if (!(cond)) { \
            ::aether::Log::Core()->error("Assertion failed: " #cond " - " __VA_ARGS__); \
            AE_DEBUGBREAK(); \
        } \
    } while (0)

#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Mantra {

std::shared_ptr<spdlog::logger> Log::mEngineLogger = nullptr;
std::shared_ptr<spdlog::logger> Log::mClientLogger = nullptr;

void Log::Init() {
    spdlog::set_pattern("%^[%T] %n: %v%$");
    Log::mEngineLogger = spdlog::stdout_color_mt("MANTRA");
    mEngineLogger->set_level(spdlog::level::trace);

    Log::mClientLogger = spdlog::stdout_color_mt("APP");
    mClientLogger->set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Log::GetEngineLogger() {
    return mEngineLogger;
}

std::shared_ptr<spdlog::logger>& Log::GetClientLogger() {
    return mClientLogger;
}

}  // namespace Mantra
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Mantra {

std::shared_ptr<spdlog::logger> Log::mEngineLogger = nullptr;
std::shared_ptr<spdlog::logger> Log::mClientLogger = nullptr;

void Log::Init(int level) {
    spdlog::level::level_enum logLevel = static_cast<spdlog::level::level_enum>(level);

    spdlog::set_pattern("%^[%T; %n] %s:%# - %v%$");

    Log::mEngineLogger = spdlog::stdout_color_mt("MANTRA");
    mEngineLogger->set_level(logLevel);

    Log::mClientLogger = spdlog::stdout_color_mt("APP");
    mClientLogger->set_level(logLevel);
}

std::shared_ptr<spdlog::logger>& Log::GetEngineLogger() {
    return mEngineLogger;
}

std::shared_ptr<spdlog::logger>& Log::GetClientLogger() {
    return mClientLogger;
}

}  // namespace Mantra
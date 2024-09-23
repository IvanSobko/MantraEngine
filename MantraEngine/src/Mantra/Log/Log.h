#pragma once

#include "Mantra/Core.h"

#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

// Engine log macros
#define ME_CORE_TRACE(...) ::Mantra::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define ME_CORE_INFO(...)  ::Mantra::Log::GetEngineLogger()->info(__VA_ARGS__)
#define ME_CORE_WARN(...)  ::Mantra::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define ME_CORE_ERROR(...) ::Mantra::Log::GetEngineLogger()->error(__VA_ARGS__)
#define ME_CORE_FATAL(...) ::Mantra::Log::GetEngineLogger()->fatal(__VA_ARGS__)

// Client log macros
#define ME_TRACE(...) ::Mantra::Log::GetClientLogger()->trace(__VA_ARGS__)
#define ME_INFO(...)  ::Mantra::Log::GetClientLogger()->info(__VA_ARGS__)
#define ME_WARN(...)  ::Mantra::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ME_ERROR(...) ::Mantra::Log::GetClientLogger()->error(__VA_ARGS__)
#define ME_FATAL(...) ::Mantra::Log::GetClientLogger()->fatal(__VA_ARGS__)

namespace Mantra {

class MANTRA_API Log
{
public:
    static void Init();

    static std::shared_ptr<spdlog::logger>& GetEngineLogger();
    static std::shared_ptr<spdlog::logger>& GetClientLogger();

private:
    static std::shared_ptr<spdlog::logger> mEngineLogger;
    static std::shared_ptr<spdlog::logger> mClientLogger;
};

}  // namespace Mantra

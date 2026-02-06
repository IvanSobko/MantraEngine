#pragma once

#include "Mantra/Core.h"

#include "spdlog/spdlog.h"

#define LOG_LOC ::spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}

// Engine log macros
#define ME_CORE_TRACE(...) ::Mantra::Log::GetEngineLogger().get()->log(LOG_LOC, spdlog::level::trace, __VA_ARGS__);
#define ME_CORE_INFO(...)  ::Mantra::Log::GetEngineLogger().get()->log(LOG_LOC, spdlog::level::info, __VA_ARGS__);
#define ME_CORE_WARN(...)  ::Mantra::Log::GetEngineLogger().get()->log(LOG_LOC, spdlog::level::warn, __VA_ARGS__);
#define ME_CORE_ERROR(...) ::Mantra::Log::GetEngineLogger().get()->log(LOG_LOC, spdlog::level::err, __VA_ARGS__);
#define ME_CORE_FATAL(...) ::Mantra::Log::GetEngineLogger().get()->log(LOG_LOC, spdlog::level::critical, __VA_ARGS__);

// Client log macros
#define ME_TRACE(...) ::Mantra::Log::GetClientLogger().get()->log(LOG_LOC, spdlog::level::trace, __VA_ARGS__);
#define ME_INFO(...)  ::Mantra::Log::GetClientLogger().get()->log(LOG_LOC, spdlog::level::info, __VA_ARGS__);
#define ME_WARN(...)  ::Mantra::Log::GetClientLogger().get()->log(LOG_LOC, spdlog::level::warn, __VA_ARGS__);
#define ME_ERROR(...) ::Mantra::Log::GetClientLogger().get()->log(LOG_LOC, spdlog::level::err, __VA_ARGS__);
#define ME_FATAL(...) ::Mantra::Log::GetClientLogger().get()->log(LOG_LOC, spdlog::level::critical, __VA_ARGS__);

namespace Mantra {

class MANTRA_API Log
{
public:
    static void Init(int level = spdlog::level::trace);

    static std::shared_ptr<spdlog::logger>& GetEngineLogger();
    static std::shared_ptr<spdlog::logger>& GetClientLogger();

private:
    static std::shared_ptr<spdlog::logger> mEngineLogger;
    static std::shared_ptr<spdlog::logger> mClientLogger;
};

}  // namespace Mantra

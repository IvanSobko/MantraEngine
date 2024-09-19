// #include "Core.h"

// #include "spdlog/fmt/ostr.h"
// #include "spdlog/spdlog.h"

namespace Mantra {

// class Mantra_API Log
// {
// public:
//     static void Init();

//     inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return mEngineLogger; }
//     inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return mClientLogger; }

// private:
//     static std::shared_ptr<spdlog::logger> mEngineLogger;
//     static std::shared_ptr<spdlog::logger> mClientLogger;
// };

}  // namespace Mantra

// // Core log macros
// #define HZ_CORE_TRACE(...) ::Hazel::Log::GetEngineLogger()->trace(__VA_ARGS__)
// #define HZ_CORE_INFO(...)  ::Hazel::Log::GetEngineLogger()->info(__VA_ARGS__)
// #define HZ_CORE_WARN(...)  ::Hazel::Log::GetEngineLogger()->warn(__VA_ARGS__)
// #define HZ_CORE_ERROR(...) ::Hazel::Log::GetEngineLogger()->error(__VA_ARGS__)
// #define HZ_CORE_FATAL(...) ::Hazel::Log::GetEngineLogger()->fatal(__VA_ARGS__)

// // Client log macros
// #define HZ_TRACE(...) ::Hazel::Log::GetClientLogger()->trace(__VA_ARGS__)
// #define HZ_INFO(...)  ::Hazel::Log::GetClientLogger()->info(__VA_ARGS__)
// #define HZ_WARN(...)  ::Hazel::Log::GetClientLogger()->warn(__VA_ARGS__)
// #define HZ_ERROR(...) ::Hazel::Log::GetClientLogger()->error(__VA_ARGS__)
// #define HZ_FATAL(...) ::Hazel::Log::GetClientLogger()->fatal(__VA_ARGS__)
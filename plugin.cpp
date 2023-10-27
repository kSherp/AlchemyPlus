#include <spdlog/sinks/basic_file_sink.h>
#include <stddef.h>

namespace logger = SKSE::log;

void SetupLog() {
    auto logsFolder = SKSE::log::log_directory();
    if (!logsFolder) {
        SKSE::stl::report_and_fail("SKSE log_directory not provided, logs disabled.");
        return;
    }
    auto pluginName = SKSE::PluginDeclaration::GetSingleton()->GetName();
    auto logFilePath = *logsFolder / std::format("{}.log", pluginName);
    auto fileLoggerPtr = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logFilePath.string(), true);
    auto loggerPtr = std::make_shared<spdlog::logger>("log", std::move(fileLoggerPtr));
    spdlog::set_default_logger(std::move(loggerPtr));
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::info);
}

void PotionCreated() { 
    logger::trace("Potion created."); 
}

void InitializeHooking() {
    logger::trace("Initializing trampoline...");
    auto& trampoline = SKSE::GetTrampoline();
    trampoline.create(64);
    trampoline.write_call<6>(REL::ID(50449).address() + 0x3ef, PotionCreated);
    logger::trace("Trampoline initialized.");
}


SKSEPluginLoad(const SKSE::LoadInterface* skse) {
    SKSE::Init(skse);
    SetupLog();
    InitializeHooking();
    return true;
}

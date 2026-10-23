#include "main.hpp"

#include "autohooks/shared/hooks.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "logger.hpp"
#include "modInfo.hpp"
#include "scotland2/shared/modloader.h"

void RegisterDemoTabs();

MOD_EXPORT_FUNC void setup(CModInfo& info) {
    Paper::Logger::RegisterFileContextId(MOD_ID);
    info = modInfo.to_c();
    Logger.info("Setup done");
}

MOD_EXPORT_FUNC void load() {
    il2cpp_functions::Init();
}

MOD_EXPORT_FUNC void late_load() {
    Logger.info("===== CategoryTabs {} starting =====", VERSION);
    RegisterDemoTabs();
    auto lateHookCount = LATE_HOOK_COUNT;
    if (lateHookCount > 0) {
        Logger.info("Installing {} late hooks", lateHookCount);
        INSTALL_LATE_HOOKS();
        Logger.info("Late hooks installed");
    }
}

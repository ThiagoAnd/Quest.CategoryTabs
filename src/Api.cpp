#include "CategoryTabsApi.hpp"
#include "Registry.hpp"
#include "main.hpp"

MOD_EXPORT_FUNC uint32_t CategoryTabs_ApiVersion() {
    return CategoryTabsApi::Version;
}

MOD_EXPORT_FUNC bool CategoryTabs_Register(CategoryTabsTab const* tab) {
    return tab && Registry::Add(*tab);
}

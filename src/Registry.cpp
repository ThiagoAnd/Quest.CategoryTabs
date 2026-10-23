#include "Registry.hpp"

#include "logger.hpp"

namespace {
    std::deque<Registry::Tab> tabs;
}

bool Registry::Add(CategoryTabsTab const& tab) {
    if (!tab.id || !tab.id[0] || !tab.icon || tab.iconSize == 0 || !tab.onSelected) {
        Logger.error("Rejected a tab with missing fields");
        return false;
    }
    for (auto const& existing : tabs) {
        if (existing.id == tab.id) {
            Logger.warn("Tab {} is already registered", tab.id);
            return false;
        }
    }
    Tab added;
    added.id = tab.id;
    added.tooltip = tab.tooltip ? tab.tooltip : tab.id;
    added.icon.assign(tab.icon, tab.icon + tab.iconSize);
    added.onSelected = tab.onSelected;
    added.onDeselected = tab.onDeselected;
    tabs.push_back(std::move(added));
    Logger.info("Registered tab {}", tab.id);
    return true;
}

Registry::Tab* Registry::FindByValue(int value) {
    for (auto& tab : tabs) {
        if (tab.value == value) {
            return &tab;
        }
    }
    return nullptr;
}

std::deque<Registry::Tab>& Registry::All() {
    return tabs;
}

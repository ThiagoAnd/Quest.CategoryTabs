#pragma once

#include <deque>
#include <string>
#include <vector>

#include "CategoryTabsApi.hpp"

namespace Registry {
    struct Tab {
        std::string id;
        std::string tooltip;
        std::vector<uint8_t> icon;
        CategoryTabsSelected onSelected = nullptr;
        CategoryTabsDeselected onDeselected = nullptr;
        int value = -1;
    };

    bool Add(CategoryTabsTab const& tab);
    std::deque<Tab>& All();
    Tab* FindByValue(int value);
}

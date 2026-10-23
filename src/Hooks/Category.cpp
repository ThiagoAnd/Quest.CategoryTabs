#include "autohooks/shared/hooks.hpp"
#include "logger.hpp"

#include <algorithm>

#include "GlobalNamespace/AnnotatedBeatmapLevelCollectionsViewController.hpp"
#include "GlobalNamespace/LevelFilteringNavigationController.hpp"
#include "GlobalNamespace/SelectLevelCategoryViewController.hpp"
#include "Registry.hpp"
#include "UnityEngine/GameObject.hpp"
#include "bsml/shared/BSML-Lite.hpp"

using namespace GlobalNamespace;

namespace {
    using Category = SelectLevelCategoryViewController::LevelCategory;
    using Info = SelectLevelCategoryViewController::LevelCategoryInfo;

    Registry::Tab* active = nullptr;

    int HighestValue(ArrayW<Info*> infos) {
        int highest = 0;
        for (auto info : infos) {
            if (info) {
                highest = std::max(highest, static_cast<int>(info->levelCategory.value__));
            }
        }
        for (auto const& tab : Registry::All()) {
            highest = std::max(highest, tab.value);
        }
        return highest;
    }

    bool HasInfo(ArrayW<Info*> infos, int value) {
        for (auto info : infos) {
            if (info && info->levelCategory.value__ == value) {
                return true;
            }
        }
        return false;
    }

    bool HasCategory(ArrayW<Category> categories, int value) {
        for (auto category : categories) {
            if (category.value__ == value) {
                return true;
            }
        }
        return false;
    }

    UnityEngine::Sprite* LoadIcon(Registry::Tab const& tab) {
        ArrayW<uint8_t> bytes(tab.icon.size());
        std::copy(tab.icon.begin(), tab.icon.end(), bytes.begin());
        return BSML::Lite::ArrayToSprite(bytes);
    }

    void AddInfo(SelectLevelCategoryViewController* self) {
        auto infos = self->_allLevelCategoryInfos;
        if (!infos) {
            return;
        }
        for (auto& tab : Registry::All()) {
            if (tab.value < 0) {
                tab.value = HighestValue(infos) + 1;
                Logger.info("Tab {} uses category {}", tab.id, tab.value);
            }
            if (HasInfo(infos, tab.value)) {
                continue;
            }
            auto info = Info::New_ctor();
            info->levelCategory = tab.value;
            info->localizedKey = tab.tooltip;
            info->categoryIcon = LoadIcon(tab);
            ArrayW<Info*> extended(infos.size() + 1);
            infos.copy_to(extended, 0);
            extended[infos.size()] = info;
            infos = extended;
        }
        self->_allLevelCategoryInfos = infos;
    }

    ArrayW<Category> AddCategories(ArrayW<Category> categories) {
        for (auto const& tab : Registry::All()) {
            if (tab.value < 0 || HasCategory(categories, tab.value)) {
                continue;
            }
            ArrayW<Category> extended(categories.size() + 1);
            categories.copy_to(extended, 0);
            extended[categories.size()] = tab.value;
            categories = extended;
        }
        return categories;
    }

    void Deselect() {
        if (!active) {
            return;
        }
        auto tab = active;
        active = nullptr;
        if (!tab->onDeselected) {
            return;
        }
        try {
            tab->onDeselected();
        } catch (std::exception const& e) {
            ReportError("deselect of " + tab->id, e);
        }
    }
}

MAKE_LATE_HOOK_MATCH(
    SelectLevelCategoryViewController_Setup,
    &SelectLevelCategoryViewController::Setup,
    void,
    SelectLevelCategoryViewController* self,
    Category selected,
    ArrayW<Category> enabled
) {
    try {
        AddInfo(self);
        enabled = AddCategories(enabled);
    } catch (std::exception const& e) {
        ReportError("could not add the categories", e);
    }
    SelectLevelCategoryViewController_Setup(self, selected, enabled);
}

MAKE_LATE_HOOK_MATCH(
    LevelFilteringNavigationController_UpdateSecondChildControllerContent,
    &LevelFilteringNavigationController::UpdateSecondChildControllerContent,
    void,
    LevelFilteringNavigationController* self,
    Category category
) {
    auto tab = Registry::FindByValue(category.value__);
    if (active && active != tab) {
        Deselect();
    }
    if (tab) {
        active = tab;
        try {
            tab->onSelected(self);
        } catch (std::exception const& e) {
            ReportError("selection of " + tab->id, e);
        }
        if (self->_annotatedBeatmapLevelCollectionsViewController) {
            self->_annotatedBeatmapLevelCollectionsViewController->gameObject->SetActive(false);
        }
        return;
    }
    LevelFilteringNavigationController_UpdateSecondChildControllerContent(self, category);
    if (self->_annotatedBeatmapLevelCollectionsViewController) {
        self->_annotatedBeatmapLevelCollectionsViewController->gameObject->SetActive(category.value__ <= 2);
    }
}

# CategoryTabs

A Beat Saber (Quest) mod that other mods use to add tabs to the solo category bar. It hands out a free category number, builds the tab with the icon and tooltip the mod gives it, and pages the bar (Back, Next, `n/total`) when the tabs do not fit.

It does nothing on its own. Install it only if another mod asks for it.

## For mod authors

Copy `shared/CategoryTabsApi.hpp` into your mod and register the tab from `late_load`:

```cpp
CategoryTabsTab tab{
    "MyMod",
    "My Tooltip",
    icon.data(),
    icon.size(),
    [](void* navigation) { Show(static_cast<GlobalNamespace::LevelFilteringNavigationController*>(navigation)); },
    [] { Hide(); },
};
if (!CategoryTabsApi::Register(tab)) {
    Logger.warn("CategoryTabs is not installed");
}
```

- `icon` is a PNG file in memory.
- `onSelected` runs when the player picks your tab. Replace the second view controller of the navigation controller with your screen.
- `onDeselected` runs when the player leaves your tab for another one. It can be null.
- `Register` returns false when CategoryTabs is not loaded or the arguments are invalid, so your mod can fall back without crashing.

## Logs

`/sdcard/ModData/com.beatgames.beatsaber/logs2/CategoryTabs.log`. Errors start with `CATEGORYTABS ERROR`. Please send that file when reporting a problem.

## License

Copyright (C) 2026 ThiagoAnd

GPL-3.0, see `LICENSE`.

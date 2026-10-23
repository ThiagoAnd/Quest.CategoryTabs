#include "DemoIcons.hpp"
#include "Registry.hpp"

namespace {
    void Select(void*) {}
    void Deselect() {}

    template <size_t N>
    void Add(char const* id, std::array<uint8_t, N> const& icon) {
        CategoryTabsTab tab{id, id, icon.data(), icon.size(), Select, Deselect};
        Registry::Add(tab);
    }
}

void RegisterDemoTabs() {
    Add("Star", DemoIcons::Star);
    Add("Heart", DemoIcons::Heart);
    Add("Bolt", DemoIcons::Bolt);
    Add("Note", DemoIcons::Note);
    Add("Ring", DemoIcons::Ring);
    Add("Diamond", DemoIcons::Diamond);
    Add("Triangle", DemoIcons::Triangle);
    Add("Cross", DemoIcons::Cross);
    Add("Square", DemoIcons::Square);
}

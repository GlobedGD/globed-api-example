#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <dankmeme.globed2/soft-link/API.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool a) {
        if (!PlayLayer::init(level, a))
            return false;

        /// Add a keybind listener to invalidate icons and update them.
        this->addEventListener(
            KeybindSettingPressedEventV3(Mod::get(), "invalidate-kb"),
            [this](Keybind const& keybind, bool down, bool repeat, double time) {
                if (down) cycle();
            }
        );

        return true;
    }

    void cycle() {
        // Change the used cube icon to the next one
        auto& rsv = GameManager::get()->m_playerFrame;
        rsv = rsv.value() + 1;

        // Tell other clients to fetch our icons again
        globed::api::net::invalidateIcons();
        // Update our icons locally (e.g. in the progress bar icons)
        globed::api::game::updateLocalIcons();
    }
};

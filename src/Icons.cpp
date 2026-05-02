#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

#include <globed/soft-link/API.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
    bool init(GJGameLevel* level, bool a, bool b) {
        if (!PlayLayer::init(level, a, b))
            return false;

        this->addEventListener(
            KeybindSettingPressedEventV3(Mod::get(), "invalidate-kb"),
            [this](Keybind const& keybind, bool down, bool repeat, double time) {
                if (down) cycle();
            }
        );

        return true;
    }

    void cycle() {
        auto& rsv = GameManager::get()->m_playerFrame;
        rsv = rsv.value() + 1;
        globed::api::net::invalidateIcons();
        globed::api::game::updateLocalIcons();
    }
};

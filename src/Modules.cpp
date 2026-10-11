#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <globed/soft-link/Module.hpp>
#include <globed/core/Event.hpp>

using namespace geode::prelude;

struct MyModule : globed::SoftModule<MyModule> {
    static const inline globed::ModuleMetadata metadata {
        .id = "dankmeme.globedapi",
        .name = "Globed API Test Mod",
        .author = "dankmeme",
    };

    static constexpr inline auto AUTO_ENABLE = globed::AutoEnableMode::Level;

    void onJoinLevel(globed::GlobedGJBGL* gjbgl, GJGameLevel* level, bool editor) override {
        log::info("Joined level {}", level->m_levelID);
    }

    void onLeaveLevel(globed::GlobedGJBGL* gjbgl, bool editor) override {
        log::info("Left level");
    }
};

// Small proof-of-concept modify that shows how hook claiming works.
// The log inside destroyPlayer will be called when the player dies, but ONLY when the module is active,
// which is when you are connected to the server and in a level.
class $modify(PlayLayer) {
    static void onModify(auto& self) {
        MyModule::get().claimHooks(self);
    }

    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        PlayLayer::destroyPlayer(player, obj);

        log::info("destroyPlayed called!");
    }
};

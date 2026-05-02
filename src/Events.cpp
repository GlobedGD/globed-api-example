#include <Geode/Geode.hpp>
#include <globed/soft-link/API.hpp>
#include <globed/core/Event.hpp>

using namespace geode::prelude;

/// An event that has a message string and simply shows a notification with the message
struct NotifyEvent : globed::ServerEvent<NotifyEvent, globed::EventServer::Central> {
    static constexpr auto Id = "notify"_spr;

    std::string message;
    NotifyEvent(std::string message) : message(std::move(message)) {}

    static Result<NotifyEvent> decode(std::span<const uint8_t> data) {
        std::string msg{(const char*)data.data(), data.size()};
        return Ok(NotifyEvent{std::move(msg)});
    }

    std::vector<uint8_t> encode() const {
        return {message.begin(), message.end()};
    }
};

$on_mod(Loaded) {
    // listen to notifications
    NotifyEvent::listen([](const NotifyEvent& event) {
        Notification::create(event.message)->show();
    }).leak();

    // send a notification to everybody when the keybind is pressed
    // note that this will only work in a room, because unscoped events (events without targets) are disallowed in the global room
    KeybindSettingPressedEventV3(Mod::get(), "send-event-all-kb").listen([](Keybind const& keybind, bool down, bool repeat, double time) {
        if (!down) return;

        // specify in options that we wanna receive this event back, so we get the notification too
        globed::EventOptions opts{};
        opts.sendBack = true;
        NotifyEvent{"hello everybody!"}.send(opts);
    }).leak();

    // send a notification to the room owner when the other keybind is pressed
    KeybindSettingPressedEventV3(Mod::get(), "send-event-one-kb").listen([](Keybind const& keybind, bool down, bool repeat, double time) {
        if (!down) return;

        // specify in options that this event is for the room owner
        globed::EventOptions opts{};
        opts.targetPlayers.push_back(globed::api::room::getOwner());
        NotifyEvent{"hello buddy!"}.send(opts);
    }).leak();
}

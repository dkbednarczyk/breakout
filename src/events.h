#pragma once

#include <eventbus.h>

namespace Event {
struct Ball_HitPlayer {};
struct Ball_HitBlock {
    size_t row;
    size_t col;
};
struct Ball_HitWall {};
struct Ball_Missed {};

struct Game_Reset {};
struct Game_Paused {};
struct Game_Resumed {};
}  // namespace Event

using GameEventBus =
    EventBus<Event::Ball_HitPlayer, Event::Ball_HitBlock, Event::Ball_HitWall,
             Event::Ball_Missed, Event::Game_Reset, Event::Game_Paused,
             Event::Game_Resumed>;
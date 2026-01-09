#include "player.h"

Player::Player(GameEventBus* bus) : bus(bus) { Reset(); }

void Player::Update(float deltaTime) {
    // handle player input
    if (IsKeyDown(KEY_LEFT)) {
        MoveLeft(deltaTime);
    }

    if (IsKeyDown(KEY_RIGHT)) {
        MoveRight(deltaTime);
    }

    // clamp player position to window borders
    if (rect.x <= 0) {
        rect.x = 0;
    }
    if (rect.x + rect.width >= SCREEN_WIDTH) {
        rect.x = SCREEN_WIDTH - rect.width;
    }
}

void Player::Draw() { DrawRectangleRec(rect, WHITE); }

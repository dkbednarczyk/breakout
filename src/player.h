#pragma once

#include <raylib.h>

#include "constants.h"
#include "events.h"

class Player {
   private:
    GameEventBus* bus;

    Rectangle rect;
    float speed;

   public:
    Player(GameEventBus* bus);

    void Reset() {
        rect = {.x = (SCREEN_WIDTH - 100.0f) / 2.0f,
                .y = SCREEN_HEIGHT - 50.0f,
                .width = 100.0f,
                .height = 20.0f};
        speed = 500.0f;
    };

    void Update(float deltaTime);
    void Draw();

    void MoveLeft(float deltaTime) { rect.x -= speed * deltaTime; }
    void MoveRight(float deltaTime) { rect.x += speed * deltaTime; }

    Rectangle getRect() const { return rect; }
};

#include "ball.h"

#include <algorithm>

#include "events.h"

Ball::Ball(GameEventBus* bus) : bus(bus) { Reset(); }

void Ball::Update(float deltaTime, Layout& levelLayout, Player& player) {
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;

    if (missed()) {
        bus->Publish(Event::Ball_Missed{});
        return;
    }

    for (int r = 0; r < levelLayout.size(); r++) {
        for (int c = 0; c < levelLayout[r].size(); c++) {
            Block& block = levelLayout[r][c];

            if (block.active &&
                CheckCollisionCircleRec(position, radius, block.rect)) {
                bus->Publish(Event::Ball_HitBlock{r, c});
                goto broke_block;
            }
        }
    }

broke_block:

    if (CheckCollisionCircleRec(position, radius, player.getRect())) {
        bus->Publish(Event::Ball_HitPlayer{});
    }

    if (hitWall()) {
        bus->Publish(Event::Ball_HitWall{});
    }
}

void Ball::HandleHitWall() {
    if (position.x + radius >= SCREEN_WIDTH || position.x - radius <= 0) {
        velocity.x *= -1;
    }

    if (position.y - radius <= 0) {
        velocity.y *= -1;
    }
}

void Ball::Draw() { DrawCircleV(position, radius, RED); }

void Ball::HandleCollisionWithPlayer(const Player& player) {
    Rectangle playerRect = player.getRect();

    HandleCollisionWithRect(playerRect);

    // Adjust X velocity based on where the ball hit the player paddle
    float hitPos = (position.x - playerRect.x) /
                   playerRect.width;        // 0.0 (left) to 1.0 (right)
    velocity.x = (hitPos - 0.5f) * 300.0f;  // Scale to a reasonable speed
}

void Ball::HandleCollisionWithRect(const Rectangle& rect) {
    // Resolve collision by choosing the axis with the smallest overlap.
    // This avoids flipping both axes (or the wrong axis) when the ball hits a
    // corner.
    const float ballLeft = position.x - radius;
    const float ballRight = position.x + radius;
    const float ballTop = position.y - radius;
    const float ballBottom = position.y + radius;

    const float rectRight = rect.x + rect.width;
    const float rectBottom = rect.y + rect.height;

    const float overlapX = std::min(ballRight - rect.x, rectRight - ballLeft);
    const float overlapY = std::min(ballBottom - rect.y, rectBottom - ballTop);

    if (overlapX < overlapY) {  // Horizontal collision
        bool leftOfMidpoint = position.x < rect.x + (rect.width * 0.5f);

        if (leftOfMidpoint) {
            position.x -= overlapX;
        } else {
            position.x += overlapX;
        }

        velocity.x *= -1;
    } else {  // Vertical collision
        bool aboveMidpoint = position.y < rect.y + (rect.height * 0.5f);

        if (aboveMidpoint) {
            position.y -= overlapY;
        } else {
            position.y += overlapY;
        }

        velocity.y *= -1;
    }
}

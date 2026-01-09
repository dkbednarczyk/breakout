#pragma once

#include "constants.h"
#include "events.h"
#include "level.h"
#include "player.h"
#include <raylib.h>

class Ball {
private:
  GameEventBus *bus;

  Vector2 position;
  Vector2 velocity;
  float radius;

  bool hitWall() {
    return position.x + radius >= SCREEN_WIDTH || position.x - radius <= 0 ||
           position.y - radius <= 0;
  }
  bool missed() { return position.y + radius >= SCREEN_HEIGHT; }

public:
  Ball(GameEventBus *bus);

  void Reset() {
    position = {SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f};
    velocity = {(float)GetRandomValue(-150, 150), -500.0f};
    radius = 10.0f;
  }

  Vector2 GetPosition() const { return position; }
  float GetRadius() const { return radius; }

  void Update(float deltaTime, Layout &levelLayout, Player &player);
  void Draw();

  void HandleHitWall();
  void HandleCollisionWithPlayer(const Player &player);
  void HandleCollisionWithRect(const Rectangle &rect);
};

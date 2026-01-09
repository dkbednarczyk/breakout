#pragma once

#include "ball.h"
#include "events.h"
#include "level.h"
#include "player.h"
#include <raylib.h>

enum class GameState {
  WaitingForServe,
  Playing,
  Paused,
  Quitting,
  GameOverLoss,
  GameOverWin,
};

enum class SoundID {
  BlockBreak,
  HitPaddle,
  HitWall,
};

class Game {
private:
  GameEventBus bus;

  // Resettable
  Level level;
  Player player;
  Ball ball;

  GameState previousState;
  GameState state;

  // Constant or otherwise changed in game loop
  int lives = 3;
  std::unordered_map<SoundID, Sound> sounds;

  void LoadSounds() {
    auto loadSoundResource = [this](SoundID id, const char* path) {
      Sound sound = LoadSound(path);
      sounds[id] = sound;
    };

    loadSoundResource(SoundID::BlockBreak, "resources/block_break.wav");
    loadSoundResource(SoundID::HitPaddle, "resources/hit_paddle.wav");
    loadSoundResource(SoundID::HitWall, "resources/hit_wall.wav");
  }

  void HandlePauseMenu();

  void InitializeBallEvents();
  void InitializeGameState();

public:
  Game();
  ~Game() {
    for (auto &[_, sound] : sounds) {
      UnloadSound(sound);
    }
  }

  void TogglePause();

  void Run();
  void Update(float deltaTime);
  void Draw();
};

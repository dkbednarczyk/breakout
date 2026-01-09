#include "game.h"

#include <raygui.h>

#include <format>
#include <string>

#include "constants.h"

/* Constructor and destructor */
Game::Game() : bus(), level(5, 7), player(&bus), ball(&bus) {
    // Load sounds
    LoadSounds();

    InitializeBallEvents();
    InitializeGameState();

    previousState = GameState::WaitingForServe;
    state = GameState::WaitingForServe;

    bus.InstantPublish(Event::Game_Reset{});
}

void Game::InitializeBallEvents() {
    bus.Subscribe<Event::Ball_HitWall>([this](const auto& data) {
        PlaySound(sounds.at(SoundID::HitWall));

        ball.HandleHitWall();
    });

    bus.Subscribe<Event::Ball_HitBlock>([this](const auto& data) {
        PlaySound(sounds.at(SoundID::BlockBreak));

        Block& blockRef = level.GetLayout()[data.row][data.col];
        level.DeactivateBlock(blockRef);

        ball.HandleCollisionWithRect(blockRef.rect);
    });

    bus.Subscribe<Event::Ball_HitPlayer>([this](const auto& data) {
        PlaySound(sounds.at(SoundID::HitPaddle));

        ball.HandleCollisionWithPlayer(player);
    });

    bus.Subscribe<Event::Ball_Missed>([this](const auto& data) {
        lives--;
        if (lives == 0) {
            state = GameState::GameOverLoss;
        } else {
            // After a loss (but still have lives), wait for player input to
            // serve again.
            state = GameState::WaitingForServe;
        }

        ball.Reset();
    });
}

void Game::InitializeGameState() {
    bus.Subscribe<Event::Game_Reset>([this](const auto& data) {
        // Reset positions and wait for player to serve.
        level = Level(level.GetRowCount(), level.GetColumnCount());
        lives = 3;

        ball.Reset();
        player.Reset();

        bus.Clear();
        state = GameState::WaitingForServe;
    });

    bus.Subscribe<Event::Game_Paused>([this](const auto& data) {
        previousState = state;
        state = GameState::Paused;
    });

    bus.Subscribe<Event::Game_Resumed>(
        [this](const auto& data) { state = previousState; });
}

void Game::TogglePause() {
    if (state != GameState::Paused) {
        bus.InstantPublish(Event::Game_Paused{});
    } else {
        bus.InstantPublish(Event::Game_Resumed{});
    }
}

void Game::HandlePauseMenu() {
    constexpr Rectangle menuRect = {.x = SCREEN_WIDTH / 4.0f,
                                    .y = SCREEN_HEIGHT / 3.0f,
                                    .width = SCREEN_WIDTH / 2.0f,
                                    .height = SCREEN_HEIGHT / 3.0f};
    const int padding = 10;

    if (GuiWindowBox(menuRect, "Paused")) {
        TogglePause();
        return;
    }

    // Draw quit button
    const int buttonWidth = 180;
    const int buttonHeight = 40;

    const Rectangle resetButtonRect = {
        .x = menuRect.x + (menuRect.width - buttonWidth) / 2,
        .y = menuRect.y + buttonHeight,
        .width = buttonWidth,
        .height = buttonHeight};

    if (GuiButton(resetButtonRect, "Reset Level (R)") || IsKeyPressed(KEY_R)) {
        bus.InstantPublish(Event::Game_Reset{});

        return;
    }

    const Rectangle newButtonRect = {
        .x = resetButtonRect.x,
        .y = resetButtonRect.y + buttonHeight + padding,
        .width = buttonWidth,
        .height = buttonHeight};

    if (GuiButton(newButtonRect, "New Level (N)") || IsKeyPressed(KEY_N)) {
        level = Level{};

        bus.InstantPublish(Event::Game_Reset{});

        return;
    }

    const Rectangle quitButtonRect = {
        .x = newButtonRect.x,
        .y = newButtonRect.y + buttonHeight + padding,
        .width = buttonWidth,
        .height = buttonHeight};

    if (GuiButton(quitButtonRect, "Quit Game (Q)") || IsKeyPressed(KEY_Q)) {
        state = GameState::Quitting;
        return;
    }
}

/* Main public game loop functions */
void Game::Run() {
    // game loop
    while (!WindowShouldClose()) {
        if (state == GameState::Quitting) {
            break;
        }

        // Allow pause toggling even on game-over screens (and waiting state).
        if (IsKeyPressed(KEY_ESCAPE)) {
            TogglePause();
        }

        BeginDrawing();
        Draw();

        switch (state) {
            case GameState::Paused:
                HandlePauseMenu();
                break;
            default:
                Update(GetFrameTime());
                break;
        }

        EndDrawing();
    }
}

void Game::Draw() {
    // Draw game objects
    ClearBackground(BLACK);
    player.Draw();
    ball.Draw();

    for (auto& row : level.GetLayout()) {
        for (auto& block : row) {
            if (block.active) {
                DrawRectangleRec(block.rect, block.color);
            }
        }
    }

    switch (state) {
        case GameState::WaitingForServe: {
            const char* serveText = "Press any key to serve";
            int textWidth = MeasureText(serveText, 20);

            DrawText(serveText, (SCREEN_WIDTH - textWidth) / 2,
                     SCREEN_HEIGHT / 2, 20, YELLOW);
        } break;
        case GameState::GameOverLoss:
        case GameState::GameOverWin: {
            const char* gameOverText = (state == GameState::GameOverLoss)
                                           ? "You Lost! Press R to Restart"
                                           : "You Won! Press R to Restart";
            int textWidth = MeasureText(gameOverText, 20);

            DrawText(gameOverText, (SCREEN_WIDTH - textWidth) / 2,
                     SCREEN_HEIGHT / 2, 20, YELLOW);
        } break;
        default:
            break;
    }

    std::string livesText = std::format("Lives: {}", lives);
    DrawText(livesText.c_str(), 5, (SCREEN_HEIGHT - 25), 20, GREEN);
}

void Game::Update(float deltaTime) {
    switch (state) {
        case GameState::WaitingForServe: {
            // Allow player movement while waiting.
            player.Update(deltaTime);

            // Any key press besides the pause key should start ball movement.
            // (ESC is handled in the main loop and should not start the serve.)
            int key = GetKeyPressed();
            if (key != 0 && key != KEY_ESCAPE) {
                state = GameState::Playing;
            }

            bus.ProcessEvents();
        } break;
        case GameState::Playing: {
            // Check win condition
            if (level.IsEmpty()) {
                state = GameState::GameOverWin;
                break;
            }

            // Update game objects
            player.Update(deltaTime);
            ball.Update(deltaTime, level.GetLayout(), player);

            bus.ProcessEvents();
        } break;
        case GameState::GameOverLoss:
        case GameState::GameOverWin: {
            // Handle restart
            if (IsKeyPressed(KEY_R)) {
                bus.InstantPublish(Event::Game_Reset{});
            }
        } break;
        default:
            break;
    }
}

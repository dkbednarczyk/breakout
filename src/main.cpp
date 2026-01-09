#include <raylib.h>

#include "constants.h"
#include "game.h"

#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout");
    InitAudioDevice();

    SetTargetFPS(144);
    SetExitKey(KEY_NULL);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    Game game;
    game.Run();

    CloseAudioDevice();
    CloseWindow();
    return 0;
}

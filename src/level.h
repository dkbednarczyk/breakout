#pragma once

#include <raylib.h>

#include <vector>

constexpr int MAX_ROWS = 9;
constexpr int MAX_COLUMNS = 9;

constexpr Color BLOCK_COLORS[10] = {RED,    ORANGE, YELLOW,  GREEN, BLUE,
                                    PURPLE, PINK,   SKYBLUE, LIME,  GOLD};

constexpr float BLOCK_HEIGHT = 20.0f;

struct Block {
    Rectangle rect;
    Color color;
    bool active;
};

using Layout = std::vector<std::vector<Block>>;

class Level {
   private:
    int rows;
    int columns;
    int activeBlockCount;
    Layout layout;

   public:
    Level();
    Level(int rows, int columns);

    int GetRowCount() const { return rows; }
    int GetColumnCount() const { return columns; }
    Layout& GetLayout() { return layout; }

    bool IsEmpty() { return activeBlockCount == 0; };

    void DeactivateBlock(Block& block);
};
